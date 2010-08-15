#include <iostream>
#include <windows.h>
#include <TCHAR.h>
#include "..\Direct3D-Hook\Direct3D-Hook.h"

int _tmain(int argc, TCHAR* argv[])
{
	if(argc < 2)
	{
		std::cout<< "Usage: Launcher filename" << std::endl << "Press enter to exit..." << std::endl;
		std::cin.ignore();
		return 1;
	}

	// Create/update the registry entry
	HKEY regKey = 0;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Direct3D-Hook", 0, 0, 0, KEY_READ | KEY_WRITE, 0, &regKey, 0))
	{
		std::cout << "Could not open or create registry key: HKEY_CURRENT_USER\\Software\\Direct3D-Hook" << std::endl << "Press enter to exit..." << std::endl;
		std::cin.ignore();
		return 2;
	}

	if(ERROR_SUCCESS != RegSetValueEx(regKey, 0, 0, REG_SZ, reinterpret_cast<BYTE*>(argv[1]), (_tcsclen(argv[1]) + 1) * sizeof(TCHAR)))
	{
		std::cout << "Could not update key value." << std::endl << "Press enter to exit..." << std::endl;
		std::cin.ignore();
		return 3;
	}

	std::cout<< "Hooking Direct3D...";
	InstallHook();
	
	std::cout<< "Done." << std::endl << "Press enter to unhook and exit." << std::endl;
	std::cin.ignore();
	
	RemoveHook();

	return 0;
}