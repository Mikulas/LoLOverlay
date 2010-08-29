#pragma once

#include "d3d9.h"
#include "d3dx9.h"

#ifdef OVERLAY_EXPORTS
	#define OVERLAY_API __declspec(dllexport)
#else
	#define OVERLAY_API __declspec(dllimport)
#endif


OVERLAY_API struct Mouse {
	char* x;
	char* y;
};

class OVERLAY_API Overlay {
public:
	Overlay();
	Overlay(IDirect3DDevice9* device);
	void setDevice(IDirect3DDevice9* device);
	void render();
	
	static Mouse getMouse();
	static void setMouse(int x, int y);

private:
	static Mouse mouse;

	IDirect3DDevice9* device;

	ID3DXFont* font;
	D3DCOLOR fontColor;
	RECT fontRect;

	ID3DXSprite* g_pSprite; // The sprite instance
	IDirect3DTexture9* g_pTexture; // Instance to hold the texture.
	D3DXVECTOR3 g_position; // Position of the sprite.
};
