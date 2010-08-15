#pragma once

#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"
#include <time.h>

class LeagueOverlay {
public:
	LeagueOverlay(IDirect3DDevice9* device) {
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
                     "Verdana", //pFacename,
                     &this->font); //ppFont

		this->fontColor = D3DCOLOR_ARGB(200, 200, 200, 200);
		
		this->fontRect.right = 100;
		this->fontRect.left = 100;
		this->fontRect.bottom = 40;
		this->fontRect.top = 100;

	}

	void render() {
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		
		char output[256] = "T:";
		strcat(output, asctime(timeinfo));
		
		this->font->DrawTextA(NULL, output, -1, &this->fontRect, DT_SINGLELINE|DT_NOCLIP, this->fontColor);
	}
private:
	ID3DXFont* font;
	D3DCOLOR fontColor;
	RECT fontRect;

	IDirect3DDevice9* device;
};
