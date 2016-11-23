#pragma once

#include "../main.h"

class cEsp
{
public:

	void DrawLine( int x1 , int y1 , int x2 , int y2 , int r , int g , int b );
	void DrawVectorLine( float *flSrc , float *flDestination , int lw , int r , int g , int b );
	void Draw3DBox( cl_entity_t *pEnt , Vector origin , int r , int g , int b );

	void DrawNoFlash();
	void DrawPlayerESP( int iIndex );
	void DrawAllEntityESP();

	void HUD_AddEntity( int type , struct cl_entity_s *ent );
};

extern cEsp g_Esp;