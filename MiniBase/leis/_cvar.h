#pragma once

#include "../main.h"

class cCvar
{
public:

	PCHAR IniPatch;
	PCHAR IniValue;
	PCHAR IniSection;

	bool bInitPatch;

	byte wpn;

	cCvar()
	{
		IniPatch = nullptr;
		IniValue = nullptr;
		IniSection = nullptr;
		bInitPatch = false;
		wpn = 6;
	}

	struct weapon_buffer_s
	{
		int aim_Active;
		int aim_Deathmatch;
		int aim_Aimthrough;
		int aim_Silent;
		int aim_Smooth;
		int aim_Fov;
		int aim_FovType;
		int aim_FovDist;
		int aim_NoTwitching;
		int aim_Mode;
		int aim_RecoilX;
		int aim_RecoilY;
		int aim_Origin;
		int aim_Bone;
		int aim_Hitbox;
		int aim_Time;
		int aim_Delay;
		int aim_ShotDelay;
		int aim_KillDelay;
		int aim_Prediction;
		int aim_Distance;
		int aim_Crosshair;
	} weapon[20];
	
	// 0 - other , 1 - glock , 2 - usp , 3 - deagle , 4 - ak47 , 5 - m4a1
	// 6 - famas , 7 - galil , 8 - sg550 , 9 - aug , 10 - m249 , 11 - mp5 , 12 - p90
	// 13 - mac10 , 14 - tmp , 15 - ump45 , 16 - awp , 17 - scout , 18 - g3sg1 , 19 - sg552

	int aim_DrawAimspot;
	int aim_DrawFov;
	
	int aim_SaveSettings;
	int aim_LoadSettings;

	int esp_Style;
	int esp_Size;
	int esp_Visible;
	int esp_Name;
	int esp_Weapon;
	int esp_3Dbox;
	int esp_Sound;
	int esp_SoundType;
	int esp_Sequence;
	int esp_GaitSequence;
	int esp_Barrel;
	int esp_Spiked;
	int esp_WorldWeapons;
	int esp_WorldSprite;
	int esp_DrawFont;
	int esp_MultiColor;

	int wh_DrawPlayer;
	int wh_DrawWeapon;
	int wh_ColorPlayer;
	int wh_ColorWeapon;
	int wh_ColorGrenade;
	int wh_TransEntity;
	int wh_Wallmode;
	int wh_Wireframe;
	int wh_WireframeR;
	int wh_WireframeG;
	int wh_WireframeB;
	int wh_LocalGlow;
	int wh_LocalGlowR;
	int wh_LocalGlowG;
	int wh_LocalGlowB;

	int rad_Active;
	int rad_Size;
	int rad_Zoom;
	int rad_Type;
	int rad_Visible;

	int rem_RemoveSky;
	int rem_RemoveSmoke;
	int rem_RemoveFlash;
	int rem_RemoveSprite;
	int rem_RemoveDrawWeapon;
	int rem_FlashMessage;
	int rem_AmxxMessage;

	int knf_Active;
	int knf_Aim;
	int knf_AimFov;
	int knf_Team;
	int knf_Attack;
	int knf_DistAttack;
	int knf_DistAttack2;

	int misc_Bhop;
	int misc_ChaseCam;
	int misc_FreeLook;
	int misc_FreeLookSpeed;
	int misc_AntiScreen;
	int misc_GranadeTrail;
	int misc_FastRun;
	int misc_MenuColor;

	int key_ToggleMenu;
	int key_TogglePanic;
	int key_NavEnter;
	int key_NavLeave;
	int key_NavUp;
	int key_NavDown;
	int key_NavLeft;
	int key_NavRight;
	
	void InitPatch();

	void LoadCvarFromIni();

	byte GetSectionFromIndex( int iIndex );

	void LoadCvarWeapon();
	void SaveCvarWeapon();
	
	void DisableAll();

	int ReadInt( char* szSection , char* szKey , int iDefaultValue , char* Patch );
	void SaveInt( char* szSection , char* szKey , int iValue , char* Patch );

	int ReadString( char* szSection , char* szKey , char* cDefaultValue , char* Patch );
};

extern cCvar cvar;