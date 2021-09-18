#include <Windows.h>

#include "util.h"
#include "Third Party/MinHook/MinHook.h"
#include "resources/console.h"
#include "resources/skins.h"
#include "settings.h"
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
    MH_CreateHook((void*)CurlEasyOptP, changeeasyopt, (void**)&curl_easy_setopt_original);
    MH_CreateHook((void*)ProcessEventP, PEHook, (void**)&ProcessEvent);
    MH_EnableHook((void*)ProcessEventP);
    MH_EnableHook((void*)CurlEasyOptP);
#ifdef EXPERIMENTAL
    bool init_hook = false; // imgui
    do
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent);
            init_hook = true;
        }
    } while (!init_hook);
#endif
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

