#pragma once

#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>

class LeagueOverlay {
public:
	LeagueOverlay(IDirect3DDevice9* device);

	void CALLBACK render();

	static void CALLBACK setMouse(int x, int y);

private:
	ID3DXFont* font;
	D3DCOLOR fontColor;
	RECT fontRect;

	ID3DXSprite* g_pSprite; // The sprite instance
	IDirect3DTexture9* g_pTexture; // Instance to hold the texture.
	D3DXVECTOR3 g_position; // Position of the sprite.

	IDirect3DDevice9* device;
	
	static int mouse_x;
	static int mouse_y;
};
