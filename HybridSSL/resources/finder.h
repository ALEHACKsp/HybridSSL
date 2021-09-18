#pragma once

#include <Windows.h>
#include <string>
#include <vector>

static unsigned __int64* sigscan(std::string pattern, int times = 0) //not by me
{
    uintptr_t MemoryBase = (uintptr_t)GetModuleHandleA(0);

    static auto patternToByte = [](const char* pattern)
    {
        auto       bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else { bytes.push_back(strtoul(current, &current, 16)); }
        }
        return bytes;
    };

    const auto dosHeader = (PIMAGE_DOS_HEADER)MemoryBase;
    const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)MemoryBase + dosHeader->e_lfanew);

    const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto       patternBytes = patternToByte(pattern.c_str());
    const auto scanBytes = reinterpret_cast<std::uint8_t*>(MemoryBase);

    const auto s = patternBytes.size();
    const auto d = patternBytes.data();

    size_t nFoundResults = 0;

    for (auto i = 0ul; i < sizeOfImage - s; ++i)
    {
        bool found = true;
        for (auto j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            if (times != 0)
            {
                if (nFoundResults < times)
                {
                    nFoundResults++;                                   // Skip Result To Get nSelectResultIndex.
                    found = false;                                     // Make sure we can loop again.
                }
                else
                {
                    return (unsigned __int64*)reinterpret_cast<uintptr_t>(&scanBytes[i]);  // Result By Index.
                }
            }
            else
            {
                return (unsigned __int64*)reinterpret_cast<uintptr_t>(&scanBytes[i]);      // Default/First Result.
            }
        }
    }
    return NULL;
}

static unsigned __int64* offsetscan(int offset)
{
    return reinterpret_cast<unsigned __int64*>(reinterpret_cast<unsigned __int64>(GetModuleHandle(0)) + offset);
}

void WriteMemory(int offset, __int64 value) {

    DWORD old, unused;
    unsigned __int64* address = offsetscan(offset);
    VirtualProtect(address, 1, PAGE_READWRITE, &old);
    WriteProcessMemory(GetModuleHandleW((LPCWSTR)0x0), address, &value, sizeof(value), nullptr);
    VirtualProtect(address, 1, old, &unused);
}

const char* ReadMemory(int offset, int length) {

    const char* res;
    DWORD old, unused;
    unsigned __int64* address = offsetscan(offset);
    VirtualProtect(address, 1, PAGE_READWRITE, &old);
    ReadProcessMemory(GetModuleHandleW((LPCWSTR)0x0), address, &res, length, 0);
    VirtualProtect(address, 1, old, &unused);

    return res;
}