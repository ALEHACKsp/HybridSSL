#pragma once

#include <string>
#include <fstream>

static std::string tolowers(std::string str)
{
	std::for_each(str.begin(), str.end(), [](char& c) {
		c = ::tolower(c);
		});
	return str;
}

static bool CheatScriptStuff(std::string cheatscript)
{
	std::string csl = tolowers(cheatscript);
	if (csl == "test")
	{
		printf("Test cheatscript worked!\n");
		return true;
	}

	// DUMP
	/* else if (csl == "dumpobjects")
	{
		std::ofstream objdump("DUMP/obj.txt");
		for(int i = 0; i < UObject::GetObjObjects().Num(); ++i)
		{
			auto Objects = UObject::GetObjObjects().GetByIndex(i);

			if (Objects != nullptr) objdump << Objects->GetFullName() << "\n";
		}
		printf("Done dumping objects!\n");
		objdump.close();
		return true;
	} */

	else if (csl == "dumpnames")
	{
		return true;
	}
	else if (csl == "dumpbps")
	{

		return true;
	}
	else if (csl == "dumpwids")
	{
		return true;
	}
	return false;
}