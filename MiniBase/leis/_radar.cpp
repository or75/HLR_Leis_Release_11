#include "_radar.h"

cRadar g_Radar;

void cRadar::DrawGuiRadar()
{
	if ( cvar.rad_Active && bRadarHide )
	{
		g_Engine.pfnClientCmd( "hideradar" );
		bRadarHide = false;
	}
	else if ( !cvar.rad_Active && !bRadarHide )
	{
		g_Engine.pfnClientCmd( "drawradar" );
		bRadarHide = true;
	}

	if ( !cvar.rad_Active ) return;

	radar_x = 3;
	radar_y = ( g_Local.bAlive ? 3 : 110 );

	g_Drawing.FillArea( radar_x + 1 , radar_y + 1 , cvar.rad_Size - 2 , cvar.rad_Size - 2 , 20 , 20 , 20 , 170 );

	g_Drawing.FillArea( radar_x + 1 , cvar.rad_Size / 2 + radar_y , cvar.rad_Size - 2 , 1 , 100 , 100 , 100 , 120 );
	g_Drawing.FillArea( cvar.rad_Size / 2 + radar_x , radar_y + 1 , 1 , cvar.rad_Size - 2 , 100 , 100 , 100 , 120 );

	g_Drawing.DrawBox( radar_x , radar_y , cvar.rad_Size , cvar.rad_Size , 1 , 0 , 0 , 0 , 255 );
}

void cRadar::DrawRadarPlayer( int iIndex )
{
	byte r , g , b;

	int screenx = 0;
	int screeny = 0;

	if ( g_Player[iIndex].iTeam == 1 )
	{
		if ( g_Player[iIndex].bVisible && cvar.rad_Visible == 1 )
		{
			r = 0; g = 255; b = 0;
		}
		else if ( g_Player[iIndex].bVisible && g_Local.iTeam != g_Player[iIndex].iTeam && cvar.rad_Visible == 2 )
		{
			r = 0; g = 255; b = 0;
		}
		else
		{
			r = 255; g = 64; b = 64;
		}
	}
	else if ( g_Player[iIndex].iTeam == 2 )
	{
		if ( g_Player[iIndex].bVisible && cvar.rad_Visible == 1 )
		{
			r = 0; g = 255; b = 0;
		}
		else if ( g_Player[iIndex].bVisible && g_Local.iTeam != g_Player[iIndex].iTeam && cvar.rad_Visible == 2 )
		{
			r = 0; g = 255; b = 0;
		}
		else
		{
			r = 0; g = 164; b = 255;
		}
	}
	else
	{
		return;
	}

	int y = ( g_Local.bAlive ? 3 : 110 );

	CalcRadarPoint( g_Player[iIndex].Entity->origin , screenx , screeny );

	g_Drawing.FillArea( screenx , screeny , 3 , 3 , r , g , b , 255 );
}

void cRadar::DrawRadarSound()
{
	int screenx = 0;
	int screeny = 0;

	for ( int i = 0; i < g_Sound.SoundIndex; i++ )
	{
		if ( g_Sound.sound[i].dwTime + 800 <= GetTickCount() )
		{
			g_Sound.sound[i].dwTime = 0;
			g_Sound.sound[i].vOrigin = Vector( 0 , 0 , 0 );
		}
		else
		{
			CalcRadarPoint( g_Sound.sound[i].vOrigin , screenx , screeny );

			g_Drawing.FillArea( screenx , screeny , 3 , 3 , 255 , 255 , 255 , 255 );
		}
	}
}

void cRadar::RadarRange( float* x , float* y , float range )
{
	if ( myfabs( ( *x ) ) > range || myfabs( ( *y ) ) > range )
	{
		if ( ( *y ) > ( *x ) )
		{
			if ( ( *y ) > -( *x ) ) { ( *x ) = range*( *x ) / ( *y ); ( *y ) = range; }
			else { ( *y ) = -range*( *y ) / ( *x ); ( *x ) = -range; }
		}
		else
		{
			if ( ( *y ) > -( *x ) ) { ( *y ) = range*( *y ) / ( *x ); ( *x ) = range; }
			else { ( *x ) = -range*( *x ) / ( *y ); ( *y ) = -range; }
		}
	}
}

void cRadar::CalcRadarPoint( float* origin , int& screenx , int& screeny )
{
	float dx = origin[0] - g_Local.vViewOrg[0];
	float dy = origin[1] - g_Local.vViewOrg[1];

	float x = dy*g_Local.fminus_cos_yaw + dx*g_Local.fsin_yaw;
	float y = dx*g_Local.fminus_cos_yaw - dy*g_Local.fsin_yaw;

	float range = (float)cvar.rad_Zoom;

	RadarRange( &x , &y , range );

	int x_max = cvar.rad_Size + radar_x - 4;
	int y_max = cvar.rad_Size + radar_y - 4;

	int r_siz = cvar.rad_Size;

	screenx = ( r_siz / 2 + int( x / (float)range*float( r_siz ) ) + 2 );
	screeny = ( r_siz / 2 + int( y / (float)range*float( r_siz ) ) + 2 );

	if ( screenx > x_max )
		screenx = x_max;
	if ( screenx < radar_x )
		screenx = radar_x + 1;
	if ( screeny > y_max )
		screeny = y_max;
	if ( screeny < radar_y )
		screeny = radar_y + 1;
}