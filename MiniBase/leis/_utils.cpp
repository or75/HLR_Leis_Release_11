#include "_utils.h"

#pragma warning (disable:4244)
#pragma warning (disable:4723)

bool UpdateOneTarget = true;

bool CalcScreen( float *pflOrigin , float *pflVecScreen )
{
	int iResult = g_Engine.pTriAPI->WorldToScreen( pflOrigin , pflVecScreen );

	if ( pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1 && !iResult )
	{
		pflVecScreen[0] = pflVecScreen[0] * ( g_Screen.iWidth / 2 ) + ( g_Screen.iWidth / 2 );
		pflVecScreen[1] = -pflVecScreen[1] * ( g_Screen.iHeight / 2 ) + ( g_Screen.iHeight / 2 );
		return true;
	}

	return false;
}

float CalcDistPlayerScreen( float fScreenCenter[2] , float fPlayerPosScreen[2] )
{
	return ( mysqrt( POW( fPlayerPosScreen[0] - fScreenCenter[0] ) + POW( fPlayerPosScreen[1] - fScreenCenter[1] ) ) );
}

bool PathFree( Vector vInput )
{
	pmtrace_t *sTrace = g_Engine.PM_TraceLine( g_Local.vViewOrg , vInput , 0 , 2 , -1 );
	return ( sTrace->fraction >= 1.0f );
}

bool CalcPlayerVisibility( int Index )
{
	byte HitBoxCheckVisible[8] = { 0,2,6,7,8,9,11,19 };

	if ( PathFree( g_Player[Index].vBone[17] ) )
		return true;

	for ( byte i = 0; i < sizeof( HitBoxCheckVisible ); i++ )
	{
		if ( PathFree( g_Player[Index].vHitbox[HitBoxCheckVisible[i]] ) )
			return true;
	}

	return false;
}

void VectorAngles( const float *forward , float *angles )
{
	float tmp , yaw , pitch;
	if ( forward[1] == 0 && forward[0] == 0 )
	{
		yaw = 0;
		if ( forward[2] > 0 )
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = ( myatan2( forward[1] , forward[0] ) * 180 / M_PI );
		if ( yaw < 0 )
			yaw += 360;
		tmp = mysqrt( forward[0] * forward[0] + forward[1] * forward[1] );
		pitch = ( myatan2( -forward[2] , tmp ) * 180 / M_PI );
		if ( pitch < 0 )
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
	while ( angles[0]<-89 ) { angles[0] += 180; angles[1] += 180; }
	while ( angles[0]>89 ) { angles[0] -= 180; angles[1] += 180; }
	while ( angles[1]<-180 ) { angles[1] += 360; }
	while ( angles[1]>180 ) { angles[1] -= 360; }
}

float AngleBetvenVectors( Vector a , Vector b )
{
	float l1 = 0.0f , l2 = 0.0f;
	float sc = a.x*b.x + a.y*b.y + a.z*b.z;
	return mycos( sc / ( l1*l2 ) )*( 180.0 / M_PI );
}

#define NUMBLOOPS 50.0f //cvar.test7
#define TIMEALIVE 3.00f //cvar.test8
#define OLDDAMPER 1.75f //cvar.test6
#define NEWDAMPER 0.75f //cvar.test5
#define SVGRAVITY 3.75f //cvar.test3
#define FLOORSTOP 0.20f //cvar.test4

void TraceGrenade( float *dest )
{
	float throwangles[3] , throwvector[3] , startpos[3] , endpos[3];
	float viewAngles[3];
	pmtrace_t pmtrace;

	float gravity = 800 / SVGRAVITY;

	g_Engine.GetViewAngles( viewAngles );
	throwangles[0] = viewAngles[0];
	throwangles[1] = viewAngles[1];
	throwangles[2] = viewAngles[2];

	if ( throwangles[0] < 0 )
		throwangles[0] = -10 + throwangles[0] * ( ( 90 - 10 ) / 90.0 );
	else
		throwangles[0] = -10 + throwangles[0] * ( ( 90 + 10 ) / 90.0 );

	float flVel = ( 90 - throwangles[0] ) * 4;
	if ( flVel > 500 )
		flVel = 500;

	g_Engine.pfnAngleVectors( throwangles , throwvector , NULL , NULL );

	startpos[0] = g_Local.vViewOrg[0] + throwvector[0] * 16;
	startpos[1] = g_Local.vViewOrg[1] + throwvector[1] * 16;
	startpos[2] = g_Local.vViewOrg[2] + throwvector[2] * 16;

	throwvector[0] = ( throwvector[0] * flVel ) + g_Local.vVelocity[0];
	throwvector[1] = ( throwvector[1] * flVel ) + g_Local.vVelocity[1];
	throwvector[2] = ( throwvector[2] * flVel ) + g_Local.vVelocity[2];

	int collisions = 0;
	float timelive;
	float step = ( TIMEALIVE / NUMBLOOPS );

	for ( timelive = 0; timelive<TIMEALIVE; timelive += step )
	{
		endpos[0] = startpos[0] + throwvector[0] * step;
		endpos[1] = startpos[1] + throwvector[1] * step;
		endpos[2] = startpos[2] + throwvector[2] * step; //move

		g_Engine.pEventAPI->EV_SetTraceHull( 2 );
		g_Engine.pEventAPI->EV_PlayerTrace( startpos , endpos , PM_STUDIO_BOX , -1 , &pmtrace );

		if ( pmtrace.ent != g_Local.iIndex && pmtrace.fraction < 1.0 ) //hits a wall
		{
			//hitpoint
			endpos[0] = startpos[0] + throwvector[0] * pmtrace.fraction * step;
			endpos[1] = startpos[1] + throwvector[1] * pmtrace.fraction * step;
			endpos[2] = startpos[2] + throwvector[2] * pmtrace.fraction * step;

			if ( pmtrace.plane.normal[2] > 0.9 && throwvector[2] <= 0 && throwvector[2] >= -gravity*FLOORSTOP )
			{
				dest[0] = endpos[0];
				dest[1] = endpos[1];
				dest[2] = endpos[2];
				return;
			}

			float proj = DotProduct( throwvector , pmtrace.plane.normal );

			throwvector[0] = ( throwvector[0] * OLDDAMPER - proj * 2 * pmtrace.plane.normal[0] ) * NEWDAMPER; //reflection off the wall
			throwvector[1] = ( throwvector[1] * OLDDAMPER - proj * 2 * pmtrace.plane.normal[1] ) * NEWDAMPER;
			throwvector[2] = ( throwvector[2] * OLDDAMPER - proj * 2 * pmtrace.plane.normal[2] ) * NEWDAMPER;

			collisions++;
			if ( collisions > 30 ) break;

			timelive -= ( step * ( 1 - pmtrace.fraction ) );
		}
		int beamindex = g_Engine.pEventAPI->EV_FindModelIndex( "sprites/laserbeam.spr" );
		g_Engine.pEfxAPI->R_BeamPoints( startpos , endpos , beamindex , 0.05f , 0.6f , 0 , 127 , 0 , 0 , 0 , 0 , 255 , 0 );

		startpos[0] = endpos[0];
		startpos[1] = endpos[1];
		startpos[2] = endpos[2];

		throwvector[2] -= gravity * pmtrace.fraction * step; //gravity
	}

	dest[0] = startpos[0];
	dest[1] = startpos[1];
	dest[2] = startpos[2];
}