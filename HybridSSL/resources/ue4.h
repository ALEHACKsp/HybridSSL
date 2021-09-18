#pragma once

#include <Windows.h>
#include <cwchar>
#include <ctype.h>
#include <locale>
#include <algorithm>

#include "../util.h"
#include "finder.h"

//FString
template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;

		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline bool IsValid(int i)
	{
		return i < Num();
	}

private:
	T* Data;

	int Count;
	int Max;
};

struct FString : private TArray<wchar_t>
{
	FString()
	{
		Data = nullptr;

		Max = Count = 0;
	}

	FString(const wchar_t* Value)
	{
		Max = Count = *Value ? std::wcslen(Value) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(Value);
		}
	};

	inline bool IsValid()
	{
		return Data != nullptr;
	}

	inline wchar_t* c_str()
	{
		return Data;
	}

	const std::string ToString()
	{

		auto length = wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

void (*FNameToString)(void*, FString*);

//UObject
struct UObject
{
	void** VTable;         // 0x0000
	int         ObjectFlags;    // 64 = 0x0008 | 32 = 0x0004
	int         InternalIndex;  // 64 = 0x000C | 32 = 0x0008
	void* ClassPrivate;   // 64 = 0x0010 | 32 = 0x000C (Missing: UClass)
	void* Name;     // 64 = 0x0018 | 32 = 0x0010
	UObject* OuterPrivate;   // 64 = 0x0020 | 32 = 0x0018
	std::string GetName()
	{
		FString res;
		FNameToString(&Name, &res);
		return res.ToString();
	}

	std::string GetFullName()
	{
		std::string temp;

		for (auto outer = OuterPrivate; outer; outer = outer->OuterPrivate)
		{
			temp = outer->GetName() + "." + temp;
		}

		temp = reinterpret_cast<UObject*>(ClassPrivate)->GetName() + " " + temp + this->GetName();
		return temp;
	}
};

//console params
struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
{
	UObject* WorldContextObject;
	FString Command;
	UObject* SpecificPlayer;
};

typedef void(__fastcall* sExecuteConsoleCommand)
(
	UObject* WorldContextObject
	, FString Command
	, UObject* SpecificPlayer
	);

//Free UE4
typedef void(__fastcall* sFreeMemory)
(
	void* Buffer
	);

//GetObjectName
typedef FString(__fastcall* sGetObjectName)
(
	UObject* Object
	);

//processevent
typedef void* (__fastcall* sProcessEvent)
(
	UObject* Object
	, UObject* Function
	, void* Params
	);


//GObjects
struct FUObjectItem
{
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

struct UFunction
{
	int32_t FunctionFlags;
	int16_t RepOffset;
	int8_t NumParms;
	char unknown1[1];
	int16_t ParmsSize;
	int16_t ReturnValueOffset;
	int16_t RPCId;
	int16_t RPCResponseId;
	class UProperty* FirstPropertyToInit;
	UFunction* EventGraphFunction;
	int32_t EventGraphCallOffset;
	void* Func;
};

struct PreFUObjectItem
{
	FUObjectItem* FUObject[10];
};

struct GObjects
{
	PreFUObjectItem* ObjectArray;
	BYTE unknown1[8];
	int32_t MaxElements;
	int32_t NumElements;

	void NumChunks(int* start, int* end) const
	{
		int cStart = 0, cEnd = 0;

		if (!cEnd)
		{
			while (true)
			{
				if (ObjectArray->FUObject[cStart] == nullptr)
				{
					cStart++;
				}
				else
				{
					break;
				}
			}

			cEnd = cStart;
			while (true)
			{
				if (ObjectArray->FUObject[cEnd] == nullptr)
				{
					break;
				}
				cEnd++;
			}
		}

		*start = cStart;
		*end = cEnd;
	}

	UObject* GetByIndex(int32_t index) const
	{
		int cStart = 0, cEnd = 0;
		int chunkIndex, chunkSize = 0xFFFF, chunkPos;
		FUObjectItem* Object;

		NumChunks(&cStart, &cEnd);

		chunkIndex = index / chunkSize;
		if (chunkSize * chunkIndex != 0 &&
			chunkSize * chunkIndex == index)
		{
			chunkIndex--;
		}

		chunkPos = cStart + chunkIndex;
		if (chunkPos < cEnd)
		{
			Object = ObjectArray->FUObject[chunkPos] + (index - chunkSize * chunkIndex);
			if (!Object) { return nullptr; }

			return Object->Object;
		}

		return nullptr;
	}
};



/*struct FProperty : FField
{
	int32_t ArrayDim;
	int32_t ElementSize;
	EPropertyFlags PropertyFlags;
	uint16_t RepIndex;
	TEnumAsByte<ELifetimeCondition> BlueprintReplicationCondition;
	int32_t Offset_Internal;
	FName RepNotifyFunc;
	FProperty* PropertyLinkNext;
	FProperty* NextRef;
	FProperty* DestructorLinkNext;
	FProperty* PostConstructLinkNext;
};

struct UField : UObject
{
	UField* Next;
	void* padding_01;
	void* padding_02;
};

struct UStruct : UField
{
	UStruct* SuperStruct;
	UField* Children;
	FField* ChildProperties;
	int32_t PropertiesSize;
	int32_t MinAlignment;
	TArray<uint8_t> Script;
	FProperty* PropertyLink;
	FProperty* RefLink;
	FProperty* DestructorLink;
	FProperty* PostConstructLink;
};

struct UFunction : UStruct
{
	int32_t FunctionFlags;
	int16_t RepOffset;
	int8_t NumParms;
	char unknown1[1];
	int16_t ParmsSize;
	int16_t ReturnValueOffset;
	int16_t RPCId;
	int16_t RPCResponseId;
	class UProperty* FirstPropertyToInit;
	UFunction* EventGraphFunction;
	int32_t EventGraphCallOffset;
	void* Func;
};*/

typedef void* (__fastcall* fProcessEvent)
(
	UObject* Object
	, UFunction* Function
	, void* Params
	);

static fProcessEvent ProcessEventf;

typedef void* (__fastcall* UProcessEvent)
(
	UObject* Object
	, UObject* Function
	, void* Params
	);

static UProcessEvent ProcessEvent;


static std::string GetObjectFirstName(UObject* Object) {

	FString res;
	FNameToString(&Object->Name, &res);
	return res.ToString();
}

static std::string GetObjectFullName(UObject* Object) {

	std::string temp;

	for (auto outer = Object->OuterPrivate; outer; outer = outer->OuterPrivate)
	{
		temp = GetObjectFirstName(outer) + "." + temp;
	}

	temp = GetObjectFirstName(reinterpret_cast<UObject*>(Object->ClassPrivate)) + " " + temp + GetObjectFirstName(Object);
	return temp;
}

// CONSOLE STUFFY

struct UClass;

struct UPackage;

struct UConsole : UObject {
	/* char pad_28[0x10]; // 0x28(0x10)
	struct ULocalPlayer* ConsoleTargetPlayer; // 0x38(0x08)
	struct UTexture2D* DefaultTexture_Black; // 0x40(0x08)
	struct UTexture2D* DefaultTexture_White; // 0x48(0x08)
	char pad_50[0x18]; // 0x50(0x18)
	struct TArray<struct FString> HistoryBuffer; // 0x68(0x10)
	char pad_78[0xb8]; // 0x78(0xb8) */
};



struct FStaticConstructObjectParameters
{
	/** The class of the object to create */
	const UClass* Class ={};

	/** The object to create this object within (the Outer property for the new object will be set to the value specified here). */
	UObject* Outer = {};

	/** The name to give the new object.If no value(NAME_None) is specified, the object will be given a unique name in the form of ClassName_#. */
	void* Name = {};

	/** The ObjectFlags to assign to the new object. some flags can affect the behavior of constructing the object. */
	int SetFlags = {};

	/** The InternalObjectFlags to assign to the new object. some flags can affect the behavior of constructing the object. */
	unsigned int InternalSetFlags = {};

	/** If true, copy transient from the class defaults instead of the pass in archetype ptr(often these are the same) */
	bool bCopyTransientsFromClassDefaults = false;

	/** If true, Template is guaranteed to be an archetype */
	bool bAssumeTemplateIsArchetype = false;

	/**
	 * If specified, the property values from this object will be copied to the new object, and the new object's ObjectArchetype value will be set to this object.
	 * If nullptr, the class default object is used instead.
	 */
	UObject* Template = nullptr;

	/** Contains the mappings of instanced objects and components to their templates */
	void* InstanceGraph = nullptr;

	/** Assign an external Package to the created object if non-null */
	UPackage* ExternalPackage = nullptr;

	FStaticConstructObjectParameters() {};
};

typedef UObject* (__fastcall* fStaticConstructObject_Internal)
(
	const FStaticConstructObjectParameters&
	);
static fStaticConstructObject_Internal StaticConstructObject_Internal;


struct UGameViewportClient {// : UScriptViewportClient {
	//char pad_38[0x8]; // 0x38(0x08)
	UConsole* ViewportConsole; // 0x40(0x08)
	/*struct TArray<struct FDebugDisplayProperty> DebugProperties; // 0x48(0x10)
	char pad_58[0x10]; // 0x58(0x10)
	int32_t MaxSplitscreenPlayers; // 0x68(0x04)
	char pad_6C[0xc]; // 0x6c(0x0c)
	struct UWorld* World; // 0x78(0x08)
	struct UGameInstance* GameInstance; // 0x80(0x08)
	char pad_88[0x2d8]; // 0x88(0x2d8)

	void SSSwapControllers(); // Function Engine.GameViewportClient.SSSwapControllers // (Exec|Native|Public) // @ game+0x11b53a0
	void ShowTitleSafeArea(); // Function Engine.GameViewportClient.ShowTitleSafeArea // (Exec|Native|Public) // @ game+0x527ed24
	void SetConsoleTarget(int32_t PlayerIndex); // Function Engine.GameViewportClient.SetConsoleTarget // (Exec|Native|Public) // @ game+0x64aaad8*/
};

struct UEngine : UObject {
	/*char pad_28[0x8]; // 0x28(0x08)
	struct UFont* TinyFont; // 0x30(0x08)
	struct FSoftObjectPath TinyFontName; // 0x38(0x18)
	struct UFont* SmallFont; // 0x50(0x08)
	struct FSoftObjectPath SmallFontName; // 0x58(0x18)
	struct UFont* MediumFont; // 0x70(0x08)
	struct FSoftObjectPath MediumFontName; // 0x78(0x18)
	struct UFont* LargeFont; // 0x90(0x08)
	struct FSoftObjectPath LargeFontName; // 0x98(0x18)
	struct UFont* SubtitleFont; // 0xb0(0x08)
	struct FSoftObjectPath SubtitleFontName; // 0xb8(0x18)
	struct TArray<struct UFont*> AdditionalFonts; // 0xd0(0x10)
	struct TArray<struct FString> AdditionalFontNames; // 0xe0(0x10) */
	UConsole* ConsoleClass; // 0xf0(0x08)
	//Class* ConsoleClass;
	//struct FSoftClassPath ConsoleClassName; // 0xf8(0x18)
	//UGameViewportClient* GameViewportClientClass; // 0x110(0x08)
	/*struct FSoftClassPath GameViewportClientClassName; // 0x118(0x18)
	struct ULocalPlayer* LocalPlayerClass; // 0x130(0x08)
	struct FSoftClassPath LocalPlayerClassName; // 0x138(0x18)
	struct AWorldSettings* WorldSettingsClass; // 0x150(0x08)
	struct FSoftClassPath WorldSettingsClassName; // 0x158(0x18)
	struct FSoftClassPath NavigationSystemClassName; // 0x170(0x18)
	struct UNavigationSystemBase* NavigationSystemClass; // 0x188(0x08)
	struct FSoftClassPath NavigationSystemConfigClassName; // 0x190(0x18)
	struct UNavigationSystemConfig* NavigationSystemConfigClass; // 0x1a8(0x08)
	struct FSoftClassPath AvoidanceManagerClassName; // 0x1b0(0x18)
	struct UAvoidanceManager* AvoidanceManagerClass; // 0x1c8(0x08)
	struct FSoftClassPath AIControllerClassName; // 0x1d0(0x18)
	struct UPhysicsCollisionHandler* PhysicsCollisionHandlerClass; // 0x1e8(0x08)
	struct FSoftClassPath PhysicsCollisionHandlerClassName; // 0x1f0(0x18)
	struct FSoftClassPath GameUserSettingsClassName; // 0x208(0x18)
	struct UGameUserSettings* GameUserSettingsClass; // 0x220(0x08)
	struct UGameUserSettings* GameUserSettings; // 0x228(0x08)
	struct ALevelScriptActor* LevelScriptActorClass; // 0x230(0x08)
	struct FSoftClassPath LevelScriptActorClassName; // 0x238(0x18)
	struct FSoftClassPath DefaultBlueprintBaseClassName; // 0x250(0x18)
	struct FSoftClassPath GameSingletonClassName; // 0x268(0x18)
	struct UObject* GameSingleton; // 0x280(0x08)
	struct FSoftClassPath AssetManagerClassName; // 0x288(0x18)
	struct UAssetManager* AssetManager; // 0x2a0(0x08)
	struct UTexture2D* DefaultTexture; // 0x2a8(0x08)
	struct FSoftObjectPath DefaultTextureName; // 0x2b0(0x18)
	struct UTexture* DefaultDiffuseTexture; // 0x2c8(0x08)
	struct FSoftObjectPath DefaultDiffuseTextureName; // 0x2d0(0x18)
	struct UTexture2D* DefaultBSPVertexTexture; // 0x2e8(0x08)
	struct FSoftObjectPath DefaultBSPVertexTextureName; // 0x2f0(0x18)
	struct UTexture2D* HighFrequencyNoiseTexture; // 0x308(0x08)
	struct FSoftObjectPath HighFrequencyNoiseTextureName; // 0x310(0x18)
	struct UTexture2D* DefaultBokehTexture; // 0x328(0x08)
	struct FSoftObjectPath DefaultBokehTextureName; // 0x330(0x18)
	struct UTexture2D* DefaultBloomKernelTexture; // 0x348(0x08)
	struct FSoftObjectPath DefaultBloomKernelTextureName; // 0x350(0x18)
	struct UMaterial* WireframeMaterial; // 0x368(0x08)
	struct FString WireframeMaterialName; // 0x370(0x10)
	struct UMaterial* DebugMeshMaterial; // 0x380(0x08)
	struct FSoftObjectPath DebugMeshMaterialName; // 0x388(0x18)
	struct UMaterial* EmissiveMeshMaterial; // 0x3a0(0x08)
	struct FSoftObjectPath EmissiveMeshMaterialName; // 0x3a8(0x18)
	struct UMaterial* LevelColorationLitMaterial; // 0x3c0(0x08)
	struct FString LevelColorationLitMaterialName; // 0x3c8(0x10)
	struct UMaterial* LevelColorationUnlitMaterial; // 0x3d8(0x08)
	struct FString LevelColorationUnlitMaterialName; // 0x3e0(0x10)
	struct UMaterial* LightingTexelDensityMaterial; // 0x3f0(0x08)
	struct FString LightingTexelDensityName; // 0x3f8(0x10)
	struct UMaterial* ShadedLevelColorationLitMaterial; // 0x408(0x08)
	struct FString ShadedLevelColorationLitMaterialName; // 0x410(0x10)
	struct UMaterial* ShadedLevelColorationUnlitMaterial; // 0x420(0x08)
	struct FString ShadedLevelColorationUnlitMaterialName; // 0x428(0x10)
	struct UMaterial* RemoveSurfaceMaterial; // 0x438(0x08)
	struct FSoftObjectPath RemoveSurfaceMaterialName; // 0x440(0x18)
	struct UMaterial* VertexColorMaterial; // 0x458(0x08)
	struct FString VertexColorMaterialName; // 0x460(0x10)
	struct UMaterial* VertexColorViewModeMaterial_ColorOnly; // 0x470(0x08)
	struct FString VertexColorViewModeMaterialName_ColorOnly; // 0x478(0x10)
	struct UMaterial* VertexColorViewModeMaterial_AlphaAsColor; // 0x488(0x08)
	struct FString VertexColorViewModeMaterialName_AlphaAsColor; // 0x490(0x10)
	struct UMaterial* VertexColorViewModeMaterial_RedOnly; // 0x4a0(0x08)
	struct FString VertexColorViewModeMaterialName_RedOnly; // 0x4a8(0x10)
	struct UMaterial* VertexColorViewModeMaterial_GreenOnly; // 0x4b8(0x08)
	struct FString VertexColorViewModeMaterialName_GreenOnly; // 0x4c0(0x10)
	struct UMaterial* VertexColorViewModeMaterial_BlueOnly; // 0x4d0(0x08)
	struct FString VertexColorViewModeMaterialName_BlueOnly; // 0x4d8(0x10)
	struct FSoftObjectPath DebugEditorMaterialName; // 0x4e8(0x18)
	struct UMaterial* ConstraintLimitMaterial; // 0x500(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialX; // 0x508(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialXAxis; // 0x510(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialY; // 0x518(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialYAxis; // 0x520(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialZ; // 0x528(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialZAxis; // 0x530(0x08)
	struct UMaterialInstanceDynamic* ConstraintLimitMaterialPrismatic; // 0x538(0x08)
	struct UMaterial* InvalidLightmapSettingsMaterial; // 0x540(0x08)
	struct FSoftObjectPath InvalidLightmapSettingsMaterialName; // 0x548(0x18)
	struct UMaterial* PreviewShadowsIndicatorMaterial; // 0x560(0x08)
	struct FSoftObjectPath PreviewShadowsIndicatorMaterialName; // 0x568(0x18)
	struct UMaterial* ArrowMaterial; // 0x580(0x08)
	struct UMaterialInstanceDynamic* ArrowMaterialYellow; // 0x588(0x08)
	struct FSoftObjectPath ArrowMaterialName; // 0x590(0x18)
	struct FLinearColor LightingOnlyBrightness; // 0x5a8(0x10)
	struct TArray<struct FLinearColor> ShaderComplexityColors; // 0x5b8(0x10)
	struct TArray<struct FLinearColor> QuadComplexityColors; // 0x5c8(0x10)
	struct TArray<struct FLinearColor> LightComplexityColors; // 0x5d8(0x10)
	struct TArray<struct FLinearColor> StationaryLightOverlapColors; // 0x5e8(0x10)
	struct TArray<struct FLinearColor> LODColorationColors; // 0x5f8(0x10)
	struct TArray<struct FLinearColor> HLODColorationColors; // 0x608(0x10)
	struct TArray<struct FLinearColor> StreamingAccuracyColors; // 0x618(0x10)
	float MaxPixelShaderAdditiveComplexityCount; // 0x628(0x04)
	float MaxES3PixelShaderAdditiveComplexityCount; // 0x62c(0x04)
	float MinLightMapDensity; // 0x630(0x04)
	float IdealLightMapDensity; // 0x634(0x04)
	float MaxLightMapDensity; // 0x638(0x04)
	char bRenderLightMapDensityGrayscale : 1; // 0x63c(0x01)
	char pad_63C_1 : 7; // 0x63c(0x01)
	char pad_63D[0x3]; // 0x63d(0x03)
	float RenderLightMapDensityGrayscaleScale; // 0x640(0x04)
	float RenderLightMapDensityColorScale; // 0x644(0x04)
	struct FLinearColor LightMapDensityVertexMappedColor; // 0x648(0x10)
	struct FLinearColor LightMapDensitySelectedColor; // 0x658(0x10)
	struct TArray<struct FStatColorMapping> StatColorMappings; // 0x668(0x10)
	struct UPhysicalMaterial* DefaultPhysMaterial; // 0x678(0x08)
	struct FSoftObjectPath DefaultPhysMaterialName; // 0x680(0x18)
	struct TArray<struct FGameNameRedirect> ActiveGameNameRedirects; // 0x698(0x10)
	struct TArray<struct FClassRedirect> ActiveClassRedirects; // 0x6a8(0x10)
	struct TArray<struct FPluginRedirect> ActivePluginRedirects; // 0x6b8(0x10)
	struct TArray<struct FStructRedirect> ActiveStructRedirects; // 0x6c8(0x10)
	struct UTexture2D* PreIntegratedSkinBRDFTexture; // 0x6d8(0x08)
	struct FSoftObjectPath PreIntegratedSkinBRDFTextureName; // 0x6e0(0x18)
	struct UTexture2D* BlueNoiseTexture; // 0x6f8(0x08)
	struct FSoftObjectPath BlueNoiseTextureName; // 0x700(0x18)
	struct UTexture2D* MiniFontTexture; // 0x718(0x08)
	struct FSoftObjectPath MiniFontTextureName; // 0x720(0x18)
	struct UTexture* WeightMapPlaceholderTexture; // 0x738(0x08)
	struct FSoftObjectPath WeightMapPlaceholderTextureName; // 0x740(0x18)
	struct UTexture2D* LightMapDensityTexture; // 0x758(0x08)
	struct FSoftObjectPath LightMapDensityTextureName; // 0x760(0x18)
	char pad_778[0x8]; // 0x778(0x08) */
	struct UGameViewportClient* GameViewport; // 0x780(0x08)
	/*struct TArray<struct FString> DeferredCommands; // 0x788(0x10)
	float NearClipPlane; // 0x798(0x04)
	char bSubtitlesEnabled : 1; // 0x79c(0x01)
	char bSubtitlesForcedOff : 1; // 0x79c(0x01)
	char pad_79C_2 : 6; // 0x79c(0x01)
	char pad_79D[0x3]; // 0x79d(0x03)
	int32_t MaximumLoopIterationCount; // 0x7a0(0x04)
	char bCanBlueprintsTickByDefault : 1; // 0x7a4(0x01)
	char bOptimizeAnimBlueprintMemberVariableAccess : 1; // 0x7a4(0x01)
	char bAllowMultiThreadedAnimationUpdate : 1; // 0x7a4(0x01)
	char bEnableEditorPSysRealtimeLOD : 1; // 0x7a4(0x01)
	char pad_7A4_4 : 1; // 0x7a4(0x01)
	char bSmoothFrameRate : 1; // 0x7a4(0x01)
	char bUseFixedFrameRate : 1; // 0x7a4(0x01)
	char pad_7A4_7 : 1; // 0x7a4(0x01)
	char pad_7A5[0x3]; // 0x7a5(0x03)
	float FixedFrameRate; // 0x7a8(0x04)
	struct FFloatRange SmoothedFrameRateRange; // 0x7ac(0x10)
	char pad_7BC[0x4]; // 0x7bc(0x04)
	struct UEngineCustomTimeStep* CustomTimeStep; // 0x7c0(0x08)
	char pad_7C8[0x20]; // 0x7c8(0x20)
	struct FSoftClassPath CustomTimeStepClassName; // 0x7e8(0x18)
	struct UTimecodeProvider* TimecodeProvider; // 0x800(0x08)
	char pad_808[0x20]; // 0x808(0x20)
	struct FSoftClassPath TimecodeProviderClassName; // 0x828(0x18)
	bool bGenerateDefaultTimecode; // 0x840(0x01)
	char pad_841[0x3]; // 0x841(0x03)
	struct FFrameRate GenerateDefaultTimecodeFrameRate; // 0x844(0x08)
	float GenerateDefaultTimecodeFrameDelay; // 0x84c(0x04)
	char bCheckForMultiplePawnsSpawnedInAFrame : 1; // 0x850(0x01)
	char pad_850_1 : 7; // 0x850(0x01)
	char pad_851[0x3]; // 0x851(0x03)
	int32_t NumPawnsAllowedToBeSpawnedInAFrame; // 0x854(0x04)
	char bShouldGenerateLowQualityLightmaps : 1; // 0x858(0x01)
	char pad_858_1 : 7; // 0x858(0x01)
	char pad_859[0x3]; // 0x859(0x03)
	struct FColor C_WorldBox; // 0x85c(0x04)
	struct FColor C_BrushWire; // 0x860(0x04)
	struct FColor C_AddWire; // 0x864(0x04)
	struct FColor C_SubtractWire; // 0x868(0x04)
	struct FColor C_SemiSolidWire; // 0x86c(0x04)
	struct FColor C_NonSolidWire; // 0x870(0x04)
	struct FColor C_WireBackground; // 0x874(0x04)
	struct FColor C_ScaleBoxHi; // 0x878(0x04)
	struct FColor C_VolumeCollision; // 0x87c(0x04)
	struct FColor C_BSPCollision; // 0x880(0x04)
	struct FColor C_OrthoBackground; // 0x884(0x04)
	struct FColor C_Volume; // 0x888(0x04)
	struct FColor C_BrushShape; // 0x88c(0x04)
	float StreamingDistanceFactor; // 0x890(0x04)
	char pad_894[0x4]; // 0x894(0x04)
	struct FDirectoryPath GameScreenshotSaveDirectory; // 0x898(0x10)
	bool UseStaticMeshMinLODPerQualityLevels; // 0x8a8(0x01)
	enum class ETransitionType TransitionType; // 0x8a9(0x01)
	char pad_8AA[0x6]; // 0x8aa(0x06)
	struct FString TransitionDescription; // 0x8b0(0x10)
	struct FString TransitionGameMode; // 0x8c0(0x10)
	char bAllowMatureLanguage : 1; // 0x8d0(0x01)
	char pad_8D0_1 : 7; // 0x8d0(0x01)
	char pad_8D1[0x3]; // 0x8d1(0x03)
	float CameraRotationThreshold; // 0x8d4(0x04)
	float CameraTranslationThreshold; // 0x8d8(0x04)
	float PrimitiveProbablyVisibleTime; // 0x8dc(0x04)
	float MaxOcclusionPixelsFraction; // 0x8e0(0x04)
	char bPauseOnLossOfFocus : 1; // 0x8e4(0x01)
	char pad_8E4_1 : 7; // 0x8e4(0x01)
	char pad_8E5[0x3]; // 0x8e5(0x03)
	int32_t MaxParticleResize; // 0x8e8(0x04)
	int32_t MaxParticleResizeWarn; // 0x8ec(0x04)
	struct TArray<struct FDropNoteInfo> PendingDroppedNotes; // 0x8f0(0x10)
	float NetClientTicksPerSecond; // 0x900(0x04)
	float DisplayGamma; // 0x904(0x04)
	float MinDesiredFrameRate; // 0x908(0x04)
	struct FLinearColor DefaultSelectedMaterialColor; // 0x90c(0x10)
	struct FLinearColor SelectedMaterialColor; // 0x91c(0x10)
	struct FLinearColor SelectionOutlineColor; // 0x92c(0x10)
	struct FLinearColor SubduedSelectionOutlineColor; // 0x93c(0x10)
	struct FLinearColor SelectedMaterialColorOverride; // 0x94c(0x10)
	bool bIsOverridingSelectedColor; // 0x95c(0x01)
	char pad_95D[0x3]; // 0x95d(0x03)
	char bEnableOnScreenDebugMessages : 1; // 0x960(0x01)
	char bEnableOnScreenDebugMessagesDisplay : 1; // 0x960(0x01)
	char bSuppressMapWarnings : 1; // 0x960(0x01)
	char bDisableAILogging : 1; // 0x960(0x01)
	char pad_960_4 : 4; // 0x960(0x01)
	char pad_961[0x3]; // 0x961(0x03)
	uint32_t bEnableVisualLogRecordingOnStart; // 0x964(0x04)
	int32_t ScreenSaverInhibitorSemaphore; // 0x968(0x04)
	char bLockReadOnlyLevels : 1; // 0x96c(0x01)
	char pad_96C_1 : 7; // 0x96c(0x01)
	char pad_96D[0x3]; // 0x96d(0x03)
	struct FString ParticleEventManagerClassPath; // 0x970(0x10)
	float SelectionHighlightIntensity; // 0x980(0x04)
	float BSPSelectionHighlightIntensity; // 0x984(0x04)
	float SelectionHighlightIntensityBillboards; // 0x988(0x04)
	char pad_98C[0x26c]; // 0x98c(0x26c)
	struct TArray<struct FNetDriverDefinition> NetDriverDefinitions; // 0xbf8(0x10)
	struct TArray<struct FString> ServerActors; // 0xc08(0x10)
	struct TArray<struct FString> RuntimeServerActors; // 0xc18(0x10)
	float NetErrorLogInterval; // 0xc28(0x04)
	char bStartedLoadMapMovie : 1; // 0xc2c(0x01)
	char pad_C2C_1 : 7; // 0xc2c(0x01)
	char pad_C2D[0x1b]; // 0xc2d(0x1b)
	int32_t NextWorldContextHandle; // 0xc48(0x04)
	char pad_C4C[0x194]; // 0xc4c(0x194) */
};

struct ULocalPlayer { // : UPlayer {
	char pad_48[0x28]; // 0x48(0x28)
	struct UGameViewportClient* ViewportClient; // 0x70(0x08)
	char pad_78[0x1c]; // 0x78(0x1c)
	//enum class EAspectRatioAxisConstraint AspectRatioAxisConstraint; // 0x94(0x01)
	char pad_95[0x3]; // 0x95(0x03)
	struct APlayerController* PendingLevelPlayerControllerClass; // 0x98(0x08)
	char bSentSplitJoin : 1; // 0xa0(0x01)
	char pad_A0_1 : 7; // 0xa0(0x01)
	char pad_A1[0x17]; // 0xa1(0x17)
	int32_t ControllerId; // 0xb8(0x04)
	char pad_BC[0x19c]; // 0xbc(0x19c)
};

typedef void* (__fastcall* fConsoleCommand) // UConsole
(
	const FString& Command
	);
static fConsoleCommand ExecuteConsoleCommand;

template <typename T>
static T FindObject(std::string name, bool ends_withb = false, bool to_lower = false, int toSkip = 0, GObjects* GObjects = GObjs)
{
	for (auto i = 0x0; i < GObjects->NumElements; ++i)
	{
		auto object = GObjects->GetByIndex(i);
		if (object == nullptr)
		{
			continue;
		}

		std::string objectFullName = object->GetFullName();

		if (to_lower)
		{
			std::transform(objectFullName.begin(), objectFullName.end(), objectFullName.begin(),
				[](const unsigned char c) { return std::tolower(c); });
		}

		if (!ends_withb)
		{
			if (starts_with(objectFullName, name))
			{
				if (toSkip > 0)
				{
					toSkip--;
				}
				else
				{
					return reinterpret_cast<T>(object);
				}
			}
		}
		else
		{
			if (ends_with(objectFullName, name))
			{
				return reinterpret_cast<T>(object);
			}
		}
	}
	return nullptr;
}

struct SpawnObject_Params
{
	UObject* ObjectClass;
	UObject* Outer;
};

struct UKismetRenderingLibrary_ImportFileAsTexture2D_Params
{
	UObject* WorldContextObject;
	FString Filename;
	UObject* ReturnValue;
};

/*struct UKismetStringLibrary_Conv_StringToName_Params
{
	FString inString;
	FName ReturnValue;
};

struct UMaterialInstanceDynamic_SetTextureParameterValue_Params
{
	FName ParameterName;
	UObject* Value;
}; */