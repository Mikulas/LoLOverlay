// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "InputHook.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")	
// Shared instance for all processes.

HHOOK hookKeyboard = HHOOK();
HHOOK hookMouse = HHOOK();

#pragma data_seg ()

INPUTHOOK_API LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* keyboard = (KBDLLHOOKSTRUCT*)lParam;
	bool keydown = wParam == 0x100 ? true : false;
	
	
	/*if (nCode < 0)
		return (CallNextHookEx (hookKeyboard, nCode, wParam, lParam));
	if (nCode == HC_NOREMOVE)
		return (CallNextHookEx (hookKeyboard, nCode, wParam, lParam));
	*/
	
	/** @see http://delphi.about.com/od/objectpascalide/l/blvkc.htm */
	
	if (keyboard->vkCode == 0x57) { // W
		if (keydown) {
			Beep(0x400, 120);
		} else {
			Beep(0x200, 80);
		}
	}

	return CallNextHookEx(hookKeyboard, nCode, wParam, lParam);
}

INPUTHOOK_API LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// disabled in InstallInputHook@InputHook.cpp
	/*
	mouse_x = ((MOUSEHOOKSTRUCT*)lParam)->pt.x;
	mouse_y = ((MOUSEHOOKSTRUCT*)lParam)->pt.y;
	*/
	return CallNextHookEx(hookMouse, nCode, wParam, lParam);
}

INPUTHOOK_API void InstallInputHook()
{
	hookKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
	//hookMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, NULL);
	OutputDebugString("INPUTHOOK hook installed.\n" );
}

INPUTHOOK_API void RemoveInputHook()
{
    UnhookWindowsHookEx(hookKeyboard);
	UnhookWindowsHookEx(hookMouse);
	OutputDebugString("INPUTHOOK hook removed.\n");
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
