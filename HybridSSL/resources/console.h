#pragma once

#include "../util.h"
#include "ue4.h"
#include "finder.h"
#include "functions.h"

static UConsole* ConstructConsole(UClass* ConsoleClass, UObject* outer)
{

    /*typedef UObject* (__fastcall* fStaticConstructObject_Internal)
    (
        const UClass* Class,
        UObject* InOuter,
        void* Name,
        int SetFlags,
        unsigned int InternalSetFlags,
        UObject* Template,
        bool  bCopyTransientsFromClassDefaults,
        void* InstanceGraph,
        bool  bAssumeTemplateIsArchetype//,
        //UPackage* ExternalPackage
        ); */
    FStaticConstructObjectParameters Params;
    Params.Class = ConsoleClass;
    Params.Outer = outer;
    Params.Name = nullptr;
    Params.SetFlags = 0;
    Params.InternalSetFlags = 0;
    Params.Template = nullptr;
    Params.bCopyTransientsFromClassDefaults = false;
    Params.InstanceGraph = nullptr;
    Params.bAssumeTemplateIsArchetype = false;
    Params.ExternalPackage = nullptr;
    //return reinterpret_cast<UConsole*>(StaticConstructObject_Internal(ConsoleClass, outer, nullptr, 0, 0, nullptr, false, nullptr, false));// , nullptr));
    return reinterpret_cast<UConsole*>(StaticConstructObject_Internal(Params));
}

/*static void CreateUE4ConsoleOld()
{
    std::cout << "Base Address: " << GetModuleHandleW(nullptr) << std::endl;
    auto SCOIAddr = offsetscan(0xD82E50);
    //auto SCOIAddr = sigscan("E8 ? ? ? ? 8A 5D 80"); // relative address
    StaticConstructObject_Internal = reinterpret_cast<fStaticConstructObject_Internal>(SCOIAddr);

    UEngine* GEngine;
    //auto GEngineAddr = sigscan("A0 60 05 4F 33 02 00 00"); // 0x98F5D40
    auto GEngineAddr = offsetscan(0x98F5D40);
    std::cout << "GEngineAddr: " << GEngineAddr << std::hex << std::endl;
    std::cout << "SCOI: " << StaticConstructObject_Internal << std::endl;
    std::cout << "SCOIAddr: " << SCOIAddr << std::hex << std::endl;
    GEngine = *reinterpret_cast<UEngine**>(GEngineAddr);
    if(GEngine == 0x0) // for some reason,if you inject on startup GEngine is nullptr.
    {
        while (GEngine == 0x0)
        {
            GEngine = *reinterpret_cast<UEngine**>(GEngineAddr);
        }
    }
    std::cout << "Test" << std::endl;
    std::cout << GEngine << std::endl;
    std::cout << GEngine->GameViewport << std::endl;
    std::cout << GEngine->GameViewport->ViewportConsole << std::endl;

    FStaticConstructObjectParameters Params;
    Params.Class = reinterpret_cast<UClass*>(GEngine->ConsoleClass);
    Params.Outer = reinterpret_cast<UObject*>(GEngine->GameViewport);
    Params.Name = nullptr;
    Params.SetFlags = 0;
    Params.InternalSetFlags = 0;
    Params.Template = nullptr;
    Params.bCopyTransientsFromClassDefaults = false;
    Params.InstanceGraph = nullptr;
    Params.bAssumeTemplateIsArchetype = false;
    std::cout << "Test";
    //ConstructConsole(reinterpret_cast<UClass*>(GEngine->ConsoleClass), reinterpret_cast<UObject*>(GEngine->GameViewport));
    GEngine->GameViewport->ViewportConsole = ConstructConsole(reinterpret_cast<UClass*>(GEngine->ConsoleClass), reinterpret_cast<UObject*>(GEngine->GameViewport));
    //GEngine->GameViewport->ViewportConsole = reinterpret_cast<UConsole*>(StaticConstructObject_Internal(Params));
    std::cout << "Crash" << std::endl;
    //GEngine->GameViewportClientClass->ViewportConsole = ConstructConsole(GEngine->ConsoleClass, reinterpret_cast<UObject*>(GEngine->GameViewportClientClass));
    //std::cout << GEngine->GameViewportClientClass->ViewportConsole << std::endl;

    /*

    UE4

    ViewportConsole = NewObject<UConsole>(this, GetOuterUEngine()->ConsoleClass);

    Basically its creating a object (our object creator is StaticConstructObject_Internal).
    UEngine is just GEngine for us.
    What's interesting is that it's not a pointer.

   
    //return NULL;
}

void CreateUE4ConsoleTest()
{
    std::cout << "test";
    auto SCOIAddr = offsetscan(0xD82E50);
    StaticConstructObject_Internal = reinterpret_cast<fStaticConstructObject_Internal>(SCOIAddr);
    std::cout << "test";
    auto GEngineAddr = offsetscan(0x98F5D40);
    GEngine = *reinterpret_cast<UEngine**>(GEngineAddr);
    std::cout << "test";
    void* ConsoleClass = *(void**)(GEngine + 0xf0);
    std::cout << "test";
    void* GameViewport = *(void**)(GEngine + 0x780);
    std::cout << "test";
    FStaticConstructObjectParameters Params;
    Params.Class = reinterpret_cast<UClass*>(ConsoleClass);
    Params.Outer = reinterpret_cast<UObject*>(GameViewport);
    Params.Name = nullptr;
    Params.SetFlags = 0;
    Params.InternalSetFlags = 0;
    Params.Template = nullptr;
    Params.bCopyTransientsFromClassDefaults = false;
    Params.InstanceGraph = nullptr;
    Params.bAssumeTemplateIsArchetype = false;
    std::cout << "test";
    void* console = StaticConstructObject_Internal(Params);

    std::cout << "test";

    GameViewport = console;
    std::cout << "test1";
} */

/*void SendCommandToConsole() // Basically a copy of void APlayerController::SendToConsole(const FString& Command)
{


    Idea:
    GEngine-GameViewportClientClass->World->do stuff to get local playerr and just doi that code

    */
    /*
    if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
        {
            LocalPlayer->ViewportClient->ViewportConsole->ConsoleCommand(Command);
        }
    }

} */

void UnlockConsole()
{
    std::cout << "Class GameplayStatics: " << FindObject<UObject*>(xor ("Class /Script/Engine.GameplayStatics")) << std::endl;
    std::cout << "Default GameplayStatics: " << FindObject<UObject*>(xor ("GameplayStatics /Script/Engine.Default__GameplayStatics")) << std::endl;
    std::cout << "Function SpawnObject: " << FindObject<UObject*>(xor ("Function /Script/Engine.GameplayStatics.SpawnObject")) << std::endl;
    std::cout << "GEngine: " << GEngine << std::endl;
    std::cout << "GEngine ConsoleClass: " << GEngine->ConsoleClass << std::endl;
    std::cout << "UObject* GEngine GameViewport: " << reinterpret_cast<UObject*>(GEngine->GameViewport) << std::endl;

    // inline UObject* (*SpawnActor)(UObject* UWorld, UClass* Class, FTransform const* UserTransformPtr, const FActorSpawnParameters& SpawnParameters);

    /*std::cout << "H" << std::endl;
    auto GameplayStatics = FindObject<UObject*>(xor("GameplayStatics /Script/Engine.Default__GameplayStatics"));
    //auto GameplayStatics = FindObject<UObject*>(xor("Class /Script/Engine.GameplayStatics")); // Either one works.
    std::cout << "A" << std::endl;
    auto fn = FindObject<UObject*>(xor("Function /Script/Engine.GameplayStatics.SpawnObject"));

    SpawnObject_Params params;

    params.ObjectClass = GEngine->ConsoleClass;
    params.Outer = reinterpret_cast<UObject*>(GEngine->GameViewport);
    
    std::cout << GameplayStatics << std::endl;
    std::cout << fn << std::endl;
    std::cout << params.ObjectClass << std::endl;
    std::cout << params.Outer << std::endl;

    ProcessEvent(GameplayStatics, fn, &params);
    //GEngine->GameViewport->ViewportConsole = reinterpret_cast<UConsole*>(ProcessEvent(GameplayStatics, fn, &params)); */
}