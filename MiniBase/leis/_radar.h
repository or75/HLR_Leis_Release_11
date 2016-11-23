#pragma once

#include "../main.h"

class cRadar
{
public:

	bool bRadarHide;

	int radar_x;
	int radar_y;

	cRadar()
	{
		bRadarHide = true;
	}

	void DrawGuiRadar();
	void DrawRadarPlayer( int iIndex );
	void DrawRadarSound();
	void RadarRange( float* x , float* y , float range );
	void CalcRadarPoint( float* origin , int& screenx , int& screeny );
};

extern cRadar g_Radar;