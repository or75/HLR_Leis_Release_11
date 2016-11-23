#pragma once

#include "../main.h"

class cAimbot
{
public:

	cAimbot()
	{
		iNeed = 0;
		iTargetID = 0;
		iFovTargetID = 0;
		iPreTargetID = 0;
		bTwitching = false;
		bAimEnable = true;
		dwAimKillTime = 0;
	}

	int iNeed;
	int iTargetID;
	int iFovTargetID;
	int iPreTargetID;

	int iMaxBones;
	int iMaxHitbox;

	bool bTwitching;
	bool bAimEnable;

	DWORD dwAimKillTime;

	bool bVisible( int iIndex );
	bool bCheckTeam( int iIndex );

	void UpdateTargerId();
	void UpdateAimInfo( int iIndex );
	void UpdatePlayerInfo();

	void DrawFov( int iIndex );
	void DrawFovCrosshair();
	
	void SmoothAimAngles( float *MyViewAngles , float *AimAngles , float *OutAngles , float Smoothing );
	void ApplySilentAngles( float *angles , usercmd_s *cmd );

	void GetBoneOrigin( cl_entity_s *Entity );
	void GetHitboxOrigin( cl_entity_s *Entity );

	Vector vCalcOriginOffset( int iIndex );
	Vector vCalcBoneOffset( int iIndex );
	Vector vCalcHitboxOffset( int iIndex );

	void CL_CreateMove( usercmd_s *cmd );
};

extern cAimbot g_Aimbot;