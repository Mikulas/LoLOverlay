#include "stdafx.h"
#include "Overlay.h"

#include <TCHAR.h>

#include "d3d9.h"
#include "d3dx9.h"


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

#pragma data_seg ()


OVERLAY_API Mouse Overlay::mouse = Mouse();

OVERLAY_API Overlay::Overlay() {
}

OVERLAY_API Overlay::Overlay(IDirect3DDevice9* id3ddevice) {
	setDevice(id3ddevice);
}

OVERLAY_API void Overlay::setDevice(IDirect3DDevice9* id3ddevice) {
	device = id3ddevice;

	D3DXCreateFont(device, //D3D Device
                    22, //Font height
                    0, //Font width
                    FW_NORMAL, //Font Weight
                    1, //MipLevels
                    false, //Italic
                    DEFAULT_CHARSET, //CharSet
                    OUT_DEFAULT_PRECIS, //OutputPrecision
                    ANTIALIASED_QUALITY, //Quality
                    DEFAULT_PITCH|FF_DONTCARE, //PitchAndFamily
                    "Verdana", //pFacename
					&font); //ppFont

	fontColor = D3DCOLOR_ARGB(200, 200, 200, 200);
		
	fontRect.right = 100;
	fontRect.left = 50;
	fontRect.bottom = 40;
	fontRect.top = 100;	

	char* texture_file;
	texture_file = "C:\\Users\\Rullaf\\Desktop\\Direct3D-Hook\\Texture.jpg";
	//texture_file = "C:\\Games\\League of Legends - backup\\game\\DATA\\Menu\\TeamFrame_I11.dds";
	//texture_file = "C:\\Users\\Rullaf\\Desktop\\lol\\satDishPnl.tga";
	
	D3DXCreateTextureFromFile(device, texture_file, &g_pTexture);
	g_position.x = 50;
	g_position.y = 200;
	g_position.z = 0;
	D3DXCreateSprite(device, &g_pSprite);

	Overlay::mouse.x = 0;
	Overlay::mouse.y = 0;
}

OVERLAY_API Mouse Overlay::getMouse() {
	Mouse mouse = Mouse();
	
	char value_x[2048];
	char value_y[2048];
	DWORD count = 2048;
	DWORD count2 = 2048;

	HKEY appKey = 0;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Input-Hook", 0, KEY_QUERY_VALUE, &appKey)) {
		if (ERROR_SUCCESS == RegQueryValueEx(appKey, "x", 0, 0, reinterpret_cast<BYTE*>(value_x), &count))
		{
			mouse.x = value_x;
		}

		if (ERROR_SUCCESS == RegQueryValueEx(appKey, "y", 0, 0, reinterpret_cast<BYTE*>(value_y), &count2))
		{
			mouse.y = value_y;
		}
		RegCloseKey(appKey);
	}
	return mouse;
}

OVERLAY_API void Overlay::render() {
	char output[256] = "W|";

	char c_mouse_x[30] = "";
	char c_mouse_y[30] = "";
	
	strcat(output, "x:");
	strcat(output, Overlay::getMouse().x);
	strcat(output, "|y:");
	strcat(output, Overlay::getMouse().y);

	device->BeginScene();
	g_pSprite->Begin(0);
	g_pSprite->Draw(g_pTexture, NULL, NULL, &g_position, 0xFFFFFF);
	g_pSprite->End();
	device->EndScene();
	

	font->DrawTextA(NULL, output, -1, &fontRect, DT_SINGLELINE|DT_NOCLIP, fontColor);
}

// called from different thread and for some reason no variable was able to get through this barrier to the Overlay::render()
OVERLAY_API void Overlay::setMouse(int x, int y) {	
	char c_mouse_x[2048];
	char c_mouse_y[2048];
	
	itoa(x, c_mouse_x, 10);
	itoa(y, c_mouse_y, 10);

	/* write to registry */
	HKEY regKey = 0;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Input-Hook", 0, 0, 0, KEY_READ | KEY_WRITE, 0, &regKey, 0))
	{
		OutputDebugString("Could not open or create registry key: HKEY_CURRENT_USER\\Software\\Input-Hook");
		return;
	}

	if(ERROR_SUCCESS != RegSetValueEx(regKey, "x", 0, REG_SZ, reinterpret_cast<BYTE*>(c_mouse_x), (_tcsclen(c_mouse_x) + 1) * sizeof(TCHAR)))
	{
		OutputDebugString("Could not update registry value");
		return;
	}

	if(ERROR_SUCCESS != RegSetValueEx(regKey, "y", 0, REG_SZ, reinterpret_cast<BYTE*>(c_mouse_y), (_tcsclen(c_mouse_y) + 1) * sizeof(TCHAR)))
	{
		OutputDebugString("Could not update registry value");
		return;
	}
	/* end write to registry */

}


#ifdef _MANAGED
#pragma managed(pop)
#endif
