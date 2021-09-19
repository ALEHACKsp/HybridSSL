#include <Windows.h>

#include "util.h"
#include "Third Party/MinHook/MinHook.h"
#include "resources/console.h"
#include "resources/skins.h"
#include "data.h"
#ifdef EXPERIMENTAL
#include "Third Party/Kiero/kiero.h"
#include "resources/imgui.h"
#endif

void Main()
{
    MH_Initialize();
    CreateConsole();
    Init();
    std::cout << "afceafe";
    //UnlockConsole();
    //ExecuteConsoleCommand(L"testaaaaaaaaaa");
#ifdef EXPERIMENTAL
    bool imgui_hook = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D11 /* Auto */) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent);
            imgui_hook = true;
        }
    } while (!imgui_hook);
#endif
    MH_CreateHook((void*)CurlEasyOptP, changeeasyopt, (void**)&curl_easy_setopt_original);
    MH_CreateHook((void*)ProcessEventP, PEHook, (void**)&ProcessEvent);
    MH_EnableHook((void*)ProcessEventP);
    MH_EnableHook((void*)CurlEasyOptP);
    //SpawnConsole();
    //SpawnObject((long long)GEngine->ConsoleClass, nullptr, (long long*)GEngine->GameViewport);
    //WriteMemory(0x6FB2098, (int)"Mogging in...");
    //std::cout << ReadMemory(0x6FB2098, sizeof("Logging In...") - 1) << std::endl;
    //CreateThread(0, 0, CreateUE4Console, 0, 0, 0); 
    //CreateUE4Console();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        Main();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}

