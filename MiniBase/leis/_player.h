#pragma once

#include "../main.h"

struct local_s
{
	int iTeam;
	int iIndex;
	int iFOV;
	int iClip;

	bool bAlive;
	bool bBadWeapon;

	float flZspeed;
	float flXYspeed;
	float flFallSpeed;
	float fsin_yaw;
	float fminus_cos_yaw;

	float FreeLookOffset[3];
	float FreeLookAngles[3];

	float fFrametime;
	
	int iWeaponID;
	int iInReload;

	int iFlags;
	int iMoveType;
	int iWaterLevel;

	Vector vRight;
	Vector vForward;
	Vector vViewOrg;
	Vector vPunchangle;
	Vector vVelocity;

	cl_entity_s *Entity;
};

struct player_s
{
	int iTeam;

	bool bAlive;
	bool bUpdated;
	bool bVisible;

	Vector vAngle;
	Vector vAimOrigin;
	Vector vVelocity;

	Vector vBone[53];
	Vector vHitbox[21];

	bool isFov;
	bool bEnableAim;
	bool bAttackTime;
	bool bShotDelay;

	DWORD dwStartTime;

	float fDistance;
	float fFrametime;

	cl_entity_s *Entity;

	hud_player_info_t Info;
};

class cPlayers
{
public:

	bool IsAliveEntity( cl_entity_s *Entity );
	bool isValidEntity( cl_entity_s *Entity );

	void UpdateLocalEntity();
	void UpdatePlayerInfo();
};

extern local_s g_Local;
extern player_s g_Player[33];
extern cPlayers g_Players;