#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>

#include "Third Party/xorstr.hpp"
#include "resources/finder.h"

static bool starts_with(std::string const& str, std::string const& tofind)
{
    return str.rfind(tofind, 0) == 0;
}

static bool ends_with(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

#include "hooks.h"

bool WriteLogsToFile = false;

UEngine* GEngine;
GObjects* GObjs = nullptr;
UObject* GWorld;

static std::string tolower_s(std::string str)
{
    std::for_each(str.begin(), str.end(), [](char& c) {
        c = ::tolower(c);
        });
    return str;
}

static void CreateConsole()
{
    AllocConsole();

    SetConsoleTitleA(xor("PE Hook SSL Bypass by Milxnor | discord.gg/JqJDDBFUWn"));

    FILE* file;

    freopen_s(&file, WriteLogsToFile ? "logs.txt" : "CONOUT$", "w", stdout); // 1
    freopen_s(&file, WriteLogsToFile ? "logs.txt" : "CONOUT$", "w", stderr); // 2
    freopen_s(&file, WriteLogsToFile ? "logs.txt" : "CONIN$", "w", stdin); // 0
}

static void Init() // Init patterns etc.
{
    unsigned __int64* FNameToStringA = sigscan(xor ("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6 48 8B F2 44 39 71 04 0F 85 ? ? ? ? 8B 19 0F B7 FB E8 ? ? ? ? 8B CB 48 8D 54 24 ? 48 C1 E9 10 8D 1C 3F 48 03 5C C8 ? 48 8B CB F6 03 01 0F 85 ? ? ? ? E8 ? ? ? ?"));
    FNameToString = reinterpret_cast<decltype(FNameToString)>(FNameToStringA);
    CurlEasyOptP = sigscan(xor ("89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 48 83 EC 28 48 85 C9"));
    CurlSetOptP = sigscan(xor ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 33 ED 49 8B F0 48 8B D9"));
    ProcessEventP = sigscan(xor ("40 55 53 56 57 41 54 41 56 41 57 48 81 ec"));
    //unsigned __int64* GObjsA = offsetscan(0x9968C38);
    //GObjs = reinterpret_cast<decltype(GObjs)>(GObjsA);
    //unsigned __int64* ConsoleCommandP = sigscan(xor ("48 8B C4 48 89 58 10 48 89 68 18 48 89 70 20 57 48 83 EC 40 48 8D 79 68"));
    //ExecuteConsoleCommand = reinterpret_cast<decltype(ExecuteConsoleCommand)>(ConsoleCommandP);
    //unsigned __int64* SpawnObjectP = sigscan(xor ("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 F6 4D 8B E0 48 89 75 77"));
    //SpawnObject = reinterpret_cast<decltype(SpawnObject)>(SpawnObjectP);
    //auto GEngineAddr = offsetscan(0x98F5D40);
    GEngine = /**reinterpret_cast<UEngine**>(*/FindObject<UEngine*>(xor ("FortEngine /Engine/Transient.FortEngine_"));//);
    //GEngine = *reinterpret_cast<UEngine**>(GEngineAddr);
    /*LPVOID lpCurlSetopt = reinterpret_cast<LPVOID>(CurlSetOptP);
    LONG(*CurlSetoptCast)(LPVOID, INT, va_list) = NULL;
    CurlSetoptCast = reinterpret_cast<decltype(CurlSetoptCast)>(lpCurlSetopt); */
    //std::cout << GEngine << std::endl;

}
