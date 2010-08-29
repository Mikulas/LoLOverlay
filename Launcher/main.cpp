#include <iostream>
#include <windows.h>
#include <TCHAR.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "..\Input-Hook\Input-Hook.h"
#include "..\Direct3D-Hook\Direct3D-Hook.h"
#include "..\Overlay\Overlay.h"

using namespace std;

/*void mouseLeftClick() {
	INPUT Input = {0};
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));

	// left up
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}*/

int _tmain(int argc, TCHAR* argv[])
{
	if(argc < 2)
	{
		std::cout<< "Usage: Launcher filename" << std::endl << "Press enter to exit..." << std::endl;
		std::cin.ignore();
		return 1;
	}


	/* write to registry */
	HKEY regKey = 0;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Direct3D-Hook", 0, 0, 0, KEY_READ | KEY_WRITE, 0, &regKey, 0))
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
	/* end write to registry */


	/* hooks installation */
	std::cout << "Hooking Direct3D " << std::endl;
	InstallDirect3DHook();

	std::cout << "Hooking Input " << std::endl << std::endl;
	std::cout << "\toverlay is running ";
	LRESULT res = InstallInputHook();
	if (res == 1) {
		std::cout << "Hooking keyboard failed." << std::endl;
	} else if (res == 2) {
		std::cout << "Hooking mouse failed." << std::endl;
	}
	std::cin.ignore();
	/* end hooks installation */
	
	// NOW IT'S NOT POSSIBLE TO GET HERE DUE TO INFINITE LOOP IN INSTALLINPUTHOOK

	/* hooks removal */
	RemoveInputHook();
	RemoveDirect3DHook();
	/* endhooks removal */
	

	return 0;
}
