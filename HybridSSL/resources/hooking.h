#pragma once

#include "../Third Party/MinHook/MinHook.h"
#include "../Third Party/Kiero/kiero.h"

static bool Hook(void* Dest, void* detour, void* og, int type)
{
	switch (type)
	{
	case 0:
		break;
	case 1:
		MH_CreateHook(Dest, detour, (void**)og);
		if (!MH_EnableHook(Dest) != MH_OK) return false;
		break;
	case 2:
		/* kiero::Status::Enum a = kiero::bind(Dest, (void**)og, detour);
		if (a != kiero::Status::Success)
		{
			if (a == kiero::Status::NotInitializedError)
			{
				
			}
			return false;
		} */
		break;
	}
}