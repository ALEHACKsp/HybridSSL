// hooks.h - Contains the hooks of process event and cURL.
#pragma once


#include <Windows.h>
#include <string>
#include <regex>

#include "resources/ue4.h"
#include "resources/curldefs.h"
#include "resources/cheatscript.h"
#include "util.h"
#include "settings.h"

typedef void CURL;

unsigned __int64* CurlEasyOptP;
unsigned __int64* CurlSetOptP;

unsigned __int64* ProcessEventP;

auto (*curl_easy_setopt_original)(CURL* cURL, uintptr_t opt, ...)->CURLcode;

void Log(const char* msg, bool nl = true)
{
    std::cout << msg;
    if (nl) std::cout << "\n";
}

auto changeeasyopt(CURL* Curl, uintptr_t opt, va_list info)->CURLcode
{
    int OFF = 0;
    switch (opt)
    {
    case CURL_NOPROXY:
        return curl_easy_setopt_original(Curl, opt, "");
        break;

    case CURL_VERIFYPEER:
        return curl_easy_setopt_original(Curl, opt, OFF);
        break;
    case CURL_VERIFYHOST:
        return curl_easy_setopt_original(Curl, opt, OFF);
        break;
    case CURL_PINNEDPUBLICKEY:
        return CURLcode::CURLE_OK;
        break;
    case CURL_URL:
        std::string url = info;
        std::regex Host(xor ("(.*).ol.epicgames.com"));
        /*if (std::regex_search(info, std::regex(xor ("/fortnite/api/cloudstorage/system")))) {
            url = std::regex_replace(info, Host, FNhost);
            Log(xor ("Redirected Cloudstorage / System"));
        } */
        /* else */if (std::regex_search(info, std::regex(xor("/fortnite/api/v2/versioncheck/")))) {
            url = std::regex_replace(info, Host, FNhost);
            Log(xor ("Redirected Versioncheck"));
        }
        else if (std::regex_search(info, std::regex(xor("/fortnite/api/game/v2/profile")))) {
            url = std::regex_replace(info, Host, FNhost);
            Log(xor ("Redirected Profile"));
        }
        else if (std::regex_search(info, std::regex(xor("/content/api/pages/fortnite-game")))) {
            url = std::regex_replace(info, Host, FNhost);
            Log(xor ("Redirected fortnite-game"));
        }
        else if (std::regex_search(info, std::regex(xor("/affiliate/api/public/affiliates/slug")))) {
            url = std::regex_replace(info, Host, FNhost);
            Log(xor ("Redirected Affiliates"));
        }
        else if (std::regex_search(info, std::regex(xor("/socialban/api/public/v1")))) {
            url = std::regex_replace(info, Host, FNhost);
            Log(xor ("Redirected Socialban"));
        } 
        //Log(url.c_str());
        return curl_easy_setopt_original(Curl, opt, url.c_str());
        break;
    }
    return curl_easy_setopt_original(Curl, opt, info);
}

void WriteToLog(std::string msg)
{
    std::ofstream f;
    f.open("funcs.txt", std::ios::out | std::ios::app);
    f << msg << std::endl;
    f.close();
} 

void* PEHook(UObject* Object, UObject* Function, void* Params)
{
    if (Object && Function)
    {
        std::string decObject = GetObjectFullName(Object);
        std::string decFunc = GetObjectFullName(Function);
        //WriteToLog(std::string("Object: " + decObject + " Function: " + decObject)); // TAKES UP ALOT OF STORAGE
        if (decFunc.find(xor("SendClientHello")) != std::string::npos ||
            decFunc.find(xor("SendPacketToServer")) != std::string::npos ||
            decFunc.find(xor("SendPacketToClient")) != std::string::npos ||
            decFunc.find(xor("UACNetworkComponent")) != std::string::npos)
        {
            //std::cout << "Func: " << decFunc << " Index: " << Function->InternalIndex << " ClassPrivate: " << Function->ClassPrivate << " ObjectFlags: " << Function->ObjectFlags << " OuterPrivate: " << Function->OuterPrivate << " VTable: " << Function->VTable << std::endl;
            //std::cout << "Object: " << decObject << " Index: " << Object->InternalIndex << " ClassPrivate: " << Object->ClassPrivate << " ObjectFlags: " << Object->ObjectFlags << " OuterPrivate: " << Object->OuterPrivate << " VTable: " << Object->VTable << std::endl;
            //std::cout << "Params: " << Params << std::endl;         
            return NULL;
        }
        /*else if (decFunc.find(tolowers("cheatscript")))
        {
            // mostly neonite++
            FString ScriptNameF = static_cast<UKismetSystemLibrary_ExecuteConsoleCommand_Params*>(Params)->Command;

            if (ScriptNameF.IsValid())
            {
                std::string ScriptName = ScriptNameF.ToString();

                std::string arg;

                if (ScriptName.find(" ") != std::wstring::npos)
                {
                    arg = ScriptName.substr(ScriptName.find(" ") + 1);
                }
                // no more neonite++
                CheatScriptStuff(arg);
            }
        } */
        return ProcessEvent(Object, Function, Params); 
    }
}