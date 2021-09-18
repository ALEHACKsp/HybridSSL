#pragma once

#include "ue4.h"
#include "../Third Party/xorstr.hpp"

inline auto GetFName(const wchar_t* String)
{
	
	UObject* KismetStringLibrary = FindObject<UObject*>(xor("KismetStringLibrary /Script/Engine.Default__KismetStringLibrary"));

	static auto fn = FindObject<UFunction*>(xor("Function /Script/Engine.KismetStringLibrary.Conv_StringToName"));

	UKismetStringLibrary_Conv_StringToName_Params params;
	params.inString = String;

	ProcessEventf(KismetStringLibrary, fn, &params);

	return params.ReturnValue; 

}

auto ImportPngAsTexture2D(const wchar_t* FileFullPath)
{
	// struct UTexture2D* ImportFileAsTexture2D(struct UObject* WorldContextObject, struct FString Filename); // Function Engine.KismetRenderingLibrary.ImportFileAsTexture2D // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable)

	UObject* KismetRenderingLibrary = FindObject<UObject*>(xor("KismetRenderingLibrary /Script/Engine.Default__KismetRenderingLibrary"));

	static auto fn = FindObject<UFunction*>(xor("Function /Script/Engine.KismetRenderingLibrary.ImportFileAsTexture2D"));

	UKismetRenderingLibrary_ImportFileAsTexture2D_Params params;
	params.WorldContextObject = GWorld;
	params.Filename = FileFullPath;

	ProcessEventf(KismetRenderingLibrary, fn, &params);

	return params.ReturnValue; 
}

static void SetBodyCustomTextureFromPng(const wchar_t* PngFileFullPath, bool bIsHead = false)
{
/*	const auto SetTextureParameterValue = FindObject<UFunction*>(xor ("Function /Script/Engine.MaterialInstanceDynamic.SetTextureParameterValue"));

	UMaterialInstanceDynamic_SetTextureParameterValue_Params params;
	params.ParameterName = GetFName(L"Diffuse");
	params.Value = ImportPngAsTexture2D(PngFileFullPath);

	for (auto i = 0; i < 6; i++)
	{
		auto MaterialInstanceDynamic = FindObject<UObject*>(xor("MaterialInstanceDynamic /Game/Athena/Apollo/Maps/Apollo_Terrain.Apollo_Terrain.PersistentLevel.PlayerPawn_Athena_C_"), false, false, i);

		if (!MaterialInstanceDynamic)
		{
			MaterialInstanceDynamic = FindObject<UObject*>(xor("MaterialInstanceDynamic /Game/Maps/Frontend.Frontend.PersistentLevel.PlayerPawn_Athena_C_"), false, false, i);
		}

		if (MaterialInstanceDynamic)
		{
			auto toFind = xor("CharacterPartSkelMesh_Body");

			if (bIsHead)
			{
				toFind = xor("CharacterPartSkelMesh_Head");
			}

			if (MaterialInstanceDynamic->GetFullName().find(toFind) != std::wstring::npos)
			{
				ProcessEventf(MaterialInstanceDynamic, SetTextureParameterValue, &params);
				return;
			}
		}
		else break;
	} */
}