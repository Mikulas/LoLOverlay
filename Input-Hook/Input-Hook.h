#pragma once

#ifdef INPUTHOOK_EXPORTS
	#define INPUTHOOK_API __declspec(dllexport)
#else
	#define INPUTHOOK_API __declspec(dllimport)
#endif

INPUTHOOK_API LRESULT InstallInputHook();
INPUTHOOK_API void RemoveInputHook();
