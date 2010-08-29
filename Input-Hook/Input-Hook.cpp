#include "stdafx.h"
#include "Input-Hook.h"
#include "..\Overlay\Overlay.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE hDLL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)  // When initializing....
	{
		hDLL = hModule;
		// We don't need thread notifications for what we're doing.  Thus, get
		// rid of them, thereby eliminating some of the overhead of this DLL
		DisableThreadLibraryCalls( hModule );
	}

	return TRUE;
}

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")		
// Shared instance for all processes.
HHOOK hookKeyboard = NULL;
HHOOK hookMouse = NULL;
#pragma data_seg ()

INPUTHOOK_API LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	OutputDebugString("\nLOWLEVELKEYBOARDPROC CALLED");

	KBDLLHOOKSTRUCT* keyboard = (KBDLLHOOKSTRUCT*)lParam;
	bool keydown = wParam == 0x100 ? true : false;
	
	
	/*if (nCode < 0)
		return (CallNextHookEx (hookKeyboard, nCode, wParam, lParam));
	if (nCode == HC_NOREMOVE)
		return (CallNextHookEx (hookKeyboard, nCode, wParam, lParam));
	*/
	
	/** @see http://delphi.about.com/od/objectpascalide/l/blvkc.htm */
	
	if (keyboard->vkCode == 0x42) { // B
		if (keydown) {
			Beep(0x300, 80);
		} else {
			Beep(0x200, 80);
		}
	}

	return CallNextHookEx(hookKeyboard, nCode, wParam, lParam);
}

INPUTHOOK_API LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {	
	if (nCode < HC_ACTION)
	{
		CallNextHookEx(hookMouse, nCode, wParam, lParam);
		return 0;
	}

	if (wParam == WM_LBUTTONDOWN || wParam == WM_NCLBUTTONDOWN) {
		Beep(0x200, 50);
		OutputDebugString("\nWM_LBUTTONDOWN_MouseHook");
	}

	if (wParam == WM_RBUTTONDOWN || wParam == WM_NCRBUTTONDOWN) {
		OutputDebugString("\nWM_RBUTTONDOWN_MouseHook");
	}

	if (wParam == WM_MOUSEMOVE) {
		OutputDebugString("\nWM_MOUSEMOVE_MouseHook");
	}

	if (wParam == WM_MOUSEWHEEL) {
		OutputDebugString("\nWM_MOUSEWHEEL_MouseHook");
	}

	
	Overlay::setMouse(((MOUSEHOOKSTRUCT*)lParam)->pt.x, ((MOUSEHOOKSTRUCT*)lParam)->pt.y);


	return CallNextHookEx(hookMouse, nCode, wParam, lParam);
}

INPUTHOOK_API LRESULT InstallInputHook()
{
	hookKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
	if (hookKeyboard == NULL) {
		OutputDebugString( "INPUTHOOK keyboard hook installation FAILED.\n" );
		return 1;
	} else {
		OutputDebugString( "INPUTHOOK keyboard hook installed.\n" );
	}
	
	hookMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, NULL);
	if (hookMouse == NULL) {
		OutputDebugString( "INPUTHOOK mouse hook installation FAILED.\n" );
		return 2;
	} else {
		OutputDebugString( "INPUTHOOK mouse hook installed.\n" );
	}

	
	BOOL bRet;
	MSG msg;
	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0 ) {
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	return 0;
}

INPUTHOOK_API void RemoveInputHook()
{
	UnhookWindowsHookEx(hookKeyboard);
	UnhookWindowsHookEx(hookMouse);
	OutputDebugString( "INPUTHOOK hook removed.\n" );
}


#ifdef _MANAGED
#pragma managed(pop)
#endif
