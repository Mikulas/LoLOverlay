#pragma once

#include "stdafx.h"
#include "LeagueOverlay.h"


int LeagueOverlay::mouse_x = 0;
int LeagueOverlay::mouse_y = 0;

void CALLBACK LeagueOverlay::setMouse(int x, int y) {
	LeagueOverlay::mouse_x = x;
	LeagueOverlay::mouse_y = y;
}

LeagueOverlay::LeagueOverlay(IDirect3DDevice9* device) {
	
	this->device = device;
	
	D3DXCreateFont(this->device, //D3D Device
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
					&this->font); //ppFont

	this->fontColor = D3DCOLOR_ARGB(200, 200, 200, 200);
		
	this->fontRect.right = 100;
	this->fontRect.left = 50;
	this->fontRect.bottom = 40;
	this->fontRect.top = 100;	

	D3DXCreateTextureFromFile(device, "C:\\Users\\Rullaf\\Desktop\\Direct3D-Hook\\Texture.jpg", &this->g_pTexture);
	this->g_position.x = 50;
	this->g_position.y = 200;
	this->g_position.z = 0;
	D3DXCreateSprite(device, &this->g_pSprite);
}

void CALLBACK LeagueOverlay::render() {
	D3DDISPLAYMODE display_mode;
	device->GetDisplayMode(0, &display_mode);

	RAWINPUT input;
	GetRawInputDeviceInfoA(device, RID_INPUT, &input, 0);
	
	char output[256] = "W|";
	
	/*
	char display_width[256], display_height[256];
	itoa(display_mode.Width, display_width, 10);
	itoa(display_mode.Height, display_height, 10);
	strcat(output, display_width);
	strcat(output, "/");
	strcat(output, display_height);
	*/

	/*char c_mouse_x[30] = "0000000000";
	char c_mouse_y[30] = "0000000000";
	itoa(mouse_x, c_mouse_x, 10);
	itoa(mouse_y, c_mouse_y, 10);
	strcat(output, "x:");
	strcat(output, c_mouse_x);
	strcat(output, "|y:");
	strcat(output, c_mouse_y);
	*/
	//strcat(output, " [mouse ok]");


	device->BeginScene();
	this->g_pSprite->Begin(NULL);
	this->g_pSprite->Draw(g_pTexture, NULL, NULL, &g_position, 0xFFFFFF);
	this->g_pSprite->End();
	device->EndScene();

	this->font->DrawTextA(NULL, output, -1, &this->fontRect, DT_SINGLELINE|DT_NOCLIP, this->fontColor);
}
