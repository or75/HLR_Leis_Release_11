#include "_cvar.h"

cCvar cvar;

void cCvar::InitPatch()
{
	IniPatch = (PCHAR)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , MAX_PATH );
	IniValue = (PCHAR)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , INT_DIGITS );
	IniSection = (PCHAR)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , INT_DIGITS );

	native_strcpy( IniPatch , BaseDir );
	native_strcat( IniPatch , CFG_LEIS_CONFIG );

	bInitPatch = true;
}

void cCvar::LoadCvarFromIni()
{
	if ( !bInitPatch )
		InitPatch();

	for ( int i = 1; i <= 30; i++ )
	{
		wpn = GetSectionFromIndex( i );
		LoadCvarWeapon();
	}

	aim_DrawAimspot = ReadInt( CFG_AIMBOT , CFG_AIM_DRAWAIMSPOT , 0 , IniPatch );
	aim_DrawFov = ReadInt( CFG_AIMBOT , CFG_AIM_DRAW_FOV , 0 , IniPatch );

	esp_Style = ReadInt( CFG_ESP , CFG_ESP_STYLE , 0 , IniPatch );
	esp_Size = ReadInt( CFG_ESP , CFG_ESP_SIZE , 10 , IniPatch );
	esp_Visible = ReadInt( CFG_ESP , CFG_ESP_VISIBLE , 2 , IniPatch );
	esp_Name = ReadInt( CFG_ESP , CFG_ESP_NAME , 0 , IniPatch );
	esp_Weapon = ReadInt( CFG_ESP , CFG_ESP_WEAPON , 0 , IniPatch );
	esp_3Dbox = ReadInt( CFG_ESP , CFG_ESP_3DBOX , 0 , IniPatch );
	esp_Sound = ReadInt( CFG_ESP , CFG_ESP_SOUND , 0 , IniPatch );
	esp_SoundType = ReadInt( CFG_ESP , CFG_ESP_SOUND_TYPE , 1 , IniPatch );
	esp_Sequence = ReadInt( CFG_ESP , CFG_ESP_SEQUENCE , 0 , IniPatch );
	esp_GaitSequence = ReadInt( CFG_ESP , CFG_ESP_G_SEQUENCE , 0 , IniPatch );
	esp_Barrel = ReadInt( CFG_ESP , CFG_ESP_BAREL , 0 , IniPatch );
	esp_Spiked = ReadInt( CFG_ESP , CFG_ESP_SPIKED , 0 , IniPatch );
	esp_WorldWeapons = ReadInt( CFG_ESP , CFG_ESP_WORLD_WPN , 0 , IniPatch );
	esp_WorldSprite = ReadInt( CFG_ESP , CFG_ESP_WORLD_SPR , 0 , IniPatch );
	esp_DrawFont = ReadInt( CFG_ESP , CFG_ESP_DRAW_FONT , 0 , IniPatch );
	esp_MultiColor = ReadInt( CFG_ESP , CFG_ESP_MULTICOLOR , 1 , IniPatch );

	wh_DrawPlayer = ReadInt( CFG_WALLHACK , CFG_WH_DRAW_PLAYER , 0 , IniPatch );
	wh_DrawWeapon = ReadInt( CFG_WALLHACK , CFG_WH_DRAW_WEAPON , 0 , IniPatch );
	wh_ColorPlayer = ReadInt( CFG_WALLHACK , CFG_WH_COLOR_PLAYER , 0 , IniPatch );
	wh_ColorWeapon = ReadInt( CFG_WALLHACK , CFG_WH_COLOR_WEAPON , 0 , IniPatch );
	wh_ColorGrenade = ReadInt( CFG_WALLHACK , CFG_WH_COLOR_GRENADE , 0 , IniPatch );
	wh_TransEntity = ReadInt( CFG_WALLHACK , CFG_WH_TRAN_ENTITY , 255 , IniPatch );
	wh_Wallmode = ReadInt( CFG_WALLHACK , CFG_WH_WALL_MODE , 0 , IniPatch );
	wh_Wireframe = ReadInt( CFG_WALLHACK , CFG_WH_WIREFRAME , 0 , IniPatch );
	wh_WireframeR = ReadInt( CFG_WALLHACK , CFG_WH_WIREFRAME_R , 0 , IniPatch );
	wh_WireframeG = ReadInt( CFG_WALLHACK , CFG_WH_WIREFRAME_G , 200 , IniPatch );
	wh_WireframeB = ReadInt( CFG_WALLHACK , CFG_WH_WIREFRAME_B , 0 , IniPatch );
	wh_LocalGlow = ReadInt( CFG_WALLHACK , CFG_WH_LOCAL_GLOW , 0 , IniPatch );
	wh_LocalGlowR = ReadInt( CFG_WALLHACK , CFG_WH_LOCAL_GLOW_R , 0 , IniPatch );
	wh_LocalGlowG = ReadInt( CFG_WALLHACK , CFG_WH_LOCAL_GLOW_G , 200 , IniPatch );
	wh_LocalGlowB = ReadInt( CFG_WALLHACK , CFG_WH_LOCAL_GLOW_B , 0 , IniPatch );

	rad_Active = ReadInt( CFG_RADAR , CFG_RADAR_ACTIVE , 1 , IniPatch );
	rad_Size = ReadInt( CFG_RADAR , CFG_RADAR_SIZE , 130 , IniPatch );
	rad_Zoom = ReadInt( CFG_RADAR , CFG_RADAR_ZOOM , 3000 , IniPatch );
	rad_Type = ReadInt( CFG_RADAR , CFG_RADAR_TYPE , 0 , IniPatch );
	rad_Visible = ReadInt( CFG_RADAR , CFG_RADAR_VISIBLE , 2 , IniPatch );

	rem_RemoveSky = ReadInt( CFG_REMOVAL , CFG_REMOVE_SKY , 0 , IniPatch );
	rem_RemoveSmoke = ReadInt( CFG_REMOVAL , CFG_REMOVE_SMOKE , 0 , IniPatch );
	rem_RemoveFlash = ReadInt( CFG_REMOVAL , CFG_REMOVE_FLASH , 200 , IniPatch );
	rem_RemoveSprite = ReadInt( CFG_REMOVAL , CFG_REMOVE_SPRITE , 0 , IniPatch );
	rem_RemoveDrawWeapon = ReadInt( CFG_REMOVAL , CFG_REMOVE_DRAW_WEAPON , 0 , IniPatch );
	rem_FlashMessage = ReadInt( CFG_REMOVAL , CFG_REMOVE_FLASH_MSG , 0 , IniPatch );
	rem_AmxxMessage = ReadInt( CFG_REMOVAL , CFG_REMOVE_AMXX_MSG , 0 , IniPatch );

	knf_Active = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_ACTIVE , 0 , IniPatch );
	knf_Aim = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_AIM , 0 , IniPatch );
	knf_AimFov = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_AIM_FOV , 180 , IniPatch );
	knf_Team = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_TEAM , 0 , IniPatch );
	knf_Attack = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_ATTACK , 2 , IniPatch );
	knf_DistAttack = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_DST_ATTACK , 72 , IniPatch );
	knf_DistAttack2 = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_DST_ATTACK2 , 64 , IniPatch );

	misc_Bhop = ReadInt( CFG_MISC , CFG_MISC_BHOP , 0 , IniPatch );
	misc_ChaseCam = ReadInt( CFG_MISC , CFG_MISC_CHASE_CAM , 0 , IniPatch );
	misc_FreeLook = ReadInt( CFG_MISC , CFG_MISC_FREELOOK , 0 , IniPatch );
	misc_FreeLookSpeed = ReadInt( CFG_MISC , CFG_MISC_FREELOOK_SPEED , 700 , IniPatch );
	misc_AntiScreen = ReadInt( CFG_MISC , CFG_MISC_ANTI_SCREEN , 1 , IniPatch );
	misc_GranadeTrail = ReadInt( CFG_MISC , CFG_MISC_GRANADE_TRAIL , 0 , IniPatch );
	misc_FastRun = ReadInt( CFG_MISC , CFG_MISC_FASTRUN , 0 , IniPatch );
	misc_MenuColor = ReadInt( CFG_MISC , CFG_MISC_MENU_COLOR , 0 , IniPatch );

	key_ToggleMenu = ReadString( CFG_KEY , CFG_TOGGLE_MENU , "INS" , IniPatch );
	key_TogglePanic = ReadString( CFG_KEY , CFG_TOGGLE_PANIC , "DEL" , IniPatch );
	key_NavEnter = ReadString( CFG_KEY , CFG_NAV_ENTER , "ENTER" , IniPatch );
	key_NavLeave = ReadString( CFG_KEY , CFG_NAV_LEAVE , "BACKSPACE" , IniPatch );
	key_NavUp = ReadString( CFG_KEY , CFG_NAV_UP , "UPARROW" , IniPatch );
	key_NavDown = ReadString( CFG_KEY , CFG_NAV_DOWN , "DOWNARROW" , IniPatch );
	key_NavLeft = ReadString( CFG_KEY , CFG_NAV_LEFT , "LEFTARROW" , IniPatch );
	key_NavRight = ReadString( CFG_KEY , CFG_NAV_RIGHT , "RIGHTARROW" , IniPatch );
}

byte cCvar::GetSectionFromIndex( int iIndex )
{
	if ( iIndex == WEAPONLIST_GLOCK18 )
	{
		native_strcpy( IniSection , "glock" );
		return 1;
	}
	else if ( iIndex == WEAPONLIST_USP )
	{
		native_strcpy( IniSection , "usp" );
		return 2;
	}
	else if ( iIndex == WEAPONLIST_DEAGLE )
	{
		native_strcpy( IniSection , "deagle" );
		return 3;
	}
	else if ( iIndex == WEAPONLIST_AK47 )
	{
		native_strcpy( IniSection , "ak47" );
		return 4;
	}
	else if ( iIndex == WEAPONLIST_M4A1 )
	{
		native_strcpy( IniSection , "m4a1" );
		return 5;
	}
	else if ( iIndex == WEAPONLIST_FAMAS )
	{
		native_strcpy( IniSection , "famas" );
		return 6;
	}
	else if ( iIndex == WEAPONLIST_GALIL )
	{
		native_strcpy( IniSection , "galil" );
		return 7;
	}
	else if ( iIndex == WEAPONLIST_SG550 )
	{
		native_strcpy( IniSection , "sg550" );
		return 8;
	}
	else if ( iIndex == WEAPONLIST_AUG )
	{
		native_strcpy( IniSection , "aug" );
		return 9;
	}
	else if ( iIndex == WEAPONLIST_M249 )
	{
		native_strcpy( IniSection , "m249" );
		return 10;
	}
	else if ( iIndex == WEAPONLIST_MP5 )
	{
		native_strcpy( IniSection , "mp5" );
		return 11;
	}
	else if ( iIndex == WEAPONLIST_P90 )
	{
		native_strcpy( IniSection , "p90" );
		return 12;
	}
	else if ( iIndex == WEAPONLIST_MAC10 )
	{
		native_strcpy( IniSection , "mac10" );
		return 13;
	}
	else if ( iIndex == WEAPONLIST_TMP )
	{
		native_strcpy( IniSection , "tmp" );
		return 14;
	}
	else if ( iIndex == WEAPONLIST_UMP45 )
	{
		native_strcpy( IniSection , "ump45" );
		return 15;
	}
	else if ( iIndex == WEAPONLIST_AWP )
	{
		native_strcpy( IniSection , "awp" );
		return 16;
	}
	else if ( iIndex == WEAPONLIST_SCOUT )
	{
		native_strcpy( IniSection , "scout" );
		return 17;
	}
	else if ( iIndex == WEAPONLIST_G3SG1 )
	{
		native_strcpy( IniSection , "g3sg1" );
		return 18;
	}
	else if ( iIndex == WEAPONLIST_SG552 )
	{
		native_strcpy( IniSection , "sg552" );
		return 19;
	}
	else
	{
		if ( g_Local.iWeaponID != WEAPONLIST_GLOCK18 && g_Local.iWeaponID != WEAPONLIST_USP &&
			 g_Local.iWeaponID != WEAPONLIST_DEAGLE && g_Local.iWeaponID != WEAPONLIST_AK47 &&
			 g_Local.iWeaponID != WEAPONLIST_M4A1 && g_Local.iWeaponID != WEAPONLIST_AWP &&
			 g_Local.iWeaponID != WEAPONLIST_KNIFE )
		{
			native_strcpy( IniSection , CFG_AIMBOT );
			return 0;
		}
	}

	native_strcpy( IniSection , CFG_AIMBOT );
	return 0;
}

void cCvar::LoadCvarWeapon()
{
	weapon[wpn].aim_Active = ReadInt( IniSection , CFG_AIM_ACTIVE , 0 , IniPatch );
	weapon[wpn].aim_Deathmatch = ReadInt( IniSection , CFG_AIM_DEATHMATCH , 0 , IniPatch );
	weapon[wpn].aim_Aimthrough = ReadInt( IniSection , CFG_AIM_THROUGH , 0 , IniPatch );
	weapon[wpn].aim_Silent = ReadInt( IniSection , CFG_AIM_SILENT , 0 , IniPatch );
	weapon[wpn].aim_Smooth = ReadInt( IniSection , CFG_AIM_SMOOTH , 0 , IniPatch );
	weapon[wpn].aim_Fov = ReadInt( IniSection , CFG_AIM_FOV , 10 , IniPatch );
	weapon[wpn].aim_FovType = ReadInt( IniSection , CFG_AIM_FOV_TYPE , 0 , IniPatch );
	weapon[wpn].aim_FovDist = ReadInt( IniSection , CFG_AIM_FOV_DIST , 500 , IniPatch );
	weapon[wpn].aim_NoTwitching = ReadInt( IniSection , CFG_AIM_NOTWITCHING , 0 , IniPatch );
	weapon[wpn].aim_Mode = ReadInt( IniSection , CFG_AIM_MODE , 2 , IniPatch );
	weapon[wpn].aim_RecoilX = ReadInt( IniSection , CFG_AIM_RECOIL_X , 0 , IniPatch );
	weapon[wpn].aim_RecoilY = ReadInt( IniSection , CFG_AIM_RECOIL_Y , 0 , IniPatch );
	weapon[wpn].aim_Origin = ReadInt( IniSection , CFG_AIM_ORIGIN , 21 , IniPatch );
	weapon[wpn].aim_Bone = ReadInt( IniSection , CFG_AIM_BONE , 8 , IniPatch );
	weapon[wpn].aim_Hitbox = ReadInt( IniSection , CFG_AIM_HITBOX , 11 , IniPatch );
	weapon[wpn].aim_Time = ReadInt( IniSection , CFG_AIM_TIME , 300 , IniPatch );
	weapon[wpn].aim_Delay = ReadInt( IniSection , CFG_AIM_DELAY , 0 , IniPatch );
	weapon[wpn].aim_ShotDelay = ReadInt( IniSection , CFG_AIM_SHOT_DELAY , 45 , IniPatch );
	weapon[wpn].aim_KillDelay = ReadInt( IniSection , CFG_AIM_KILL_DELAY , 0 , IniPatch );
	weapon[wpn].aim_Prediction = ReadInt( IniSection , CFG_AIM_PREDICTION , 0 , IniPatch );
	weapon[wpn].aim_Distance = ReadInt( IniSection , CFG_AIM_DISTANCE , 0 , IniPatch );
	
	if ( g_Local.iWeaponID == WEAPONLIST_AWP )
	{
		weapon[wpn].aim_Crosshair = ReadInt( IniSection , CFG_AIM_CROSSHAIR , 0 , IniPatch );
	}
}

void cCvar::SaveCvarWeapon()
{
	SaveInt( IniSection , CFG_AIM_ACTIVE , weapon[wpn].aim_Active , IniPatch );
	SaveInt( IniSection , CFG_AIM_DEATHMATCH , weapon[wpn].aim_Deathmatch , IniPatch );
	SaveInt( IniSection , CFG_AIM_THROUGH , weapon[wpn].aim_Aimthrough , IniPatch );
	SaveInt( IniSection , CFG_AIM_SILENT , weapon[wpn].aim_Silent , IniPatch );
	SaveInt( IniSection , CFG_AIM_SMOOTH , weapon[wpn].aim_Smooth , IniPatch );
	SaveInt( IniSection , CFG_AIM_FOV , weapon[wpn].aim_Fov , IniPatch );
	SaveInt( IniSection , CFG_AIM_FOV_TYPE , weapon[wpn].aim_FovType , IniPatch );
	SaveInt( IniSection , CFG_AIM_FOV_DIST , weapon[wpn].aim_FovDist , IniPatch );
	SaveInt( IniSection , CFG_AIM_NOTWITCHING , weapon[wpn].aim_NoTwitching , IniPatch );
	SaveInt( IniSection , CFG_AIM_MODE , weapon[wpn].aim_Mode , IniPatch );
	SaveInt( IniSection , CFG_AIM_RECOIL_X , weapon[wpn].aim_RecoilX , IniPatch );
	SaveInt( IniSection , CFG_AIM_RECOIL_Y , weapon[wpn].aim_RecoilY , IniPatch );
	SaveInt( IniSection , CFG_AIM_ORIGIN , weapon[wpn].aim_Origin , IniPatch );
	SaveInt( IniSection , CFG_AIM_BONE , weapon[wpn].aim_Bone , IniPatch );
	SaveInt( IniSection , CFG_AIM_HITBOX , weapon[wpn].aim_Hitbox , IniPatch );
	SaveInt( IniSection , CFG_AIM_TIME , weapon[wpn].aim_Time , IniPatch );
	SaveInt( IniSection , CFG_AIM_DELAY , weapon[wpn].aim_Delay , IniPatch );
	SaveInt( IniSection , CFG_AIM_SHOT_DELAY , weapon[wpn].aim_ShotDelay , IniPatch );
	SaveInt( IniSection , CFG_AIM_KILL_DELAY , weapon[wpn].aim_KillDelay , IniPatch );
	SaveInt( IniSection , CFG_AIM_PREDICTION , weapon[wpn].aim_Prediction , IniPatch );
	SaveInt( IniSection , CFG_AIM_DISTANCE , weapon[wpn].aim_Distance , IniPatch );

	if ( g_Local.iWeaponID == WEAPONLIST_AWP )
	{
		SaveInt( IniSection , CFG_AIM_CROSSHAIR , weapon[wpn].aim_Crosshair , IniPatch );
	}
}

void cCvar::DisableAll()
{
	weapon[wpn].aim_Active = 0;
	weapon[wpn].aim_RecoilX = 0;
	weapon[wpn].aim_RecoilY = 0;
	weapon[wpn].aim_Crosshair = 0;

	aim_DrawAimspot = 0;
	aim_DrawFov = 0;

	esp_Style = 0;
	esp_Name = 0;
	esp_Weapon = 0;
	esp_3Dbox = 0;
	esp_Sound = 0;
	esp_Sequence = 0;
	esp_GaitSequence = 0;
	esp_Barrel = 0;
	esp_Spiked = 0;
	esp_WorldWeapons = 0;
	esp_WorldSprite = 0;

	wh_DrawPlayer = 0;
	wh_DrawWeapon = 0;
	wh_ColorPlayer = 0;
	wh_ColorWeapon = 0;
	wh_ColorGrenade = 0;
	wh_TransEntity = 255;
	wh_Wallmode = 0;
	wh_Wireframe = 0;
	wh_LocalGlow = 0;

	rad_Active = 0;

	rem_RemoveSky = 0;
	rem_RemoveSmoke = 0;
	rem_RemoveFlash = 0;
	rem_RemoveSprite = 0;
	rem_RemoveDrawWeapon = 0;
	rem_FlashMessage = 0;
	rem_AmxxMessage = 0;

	knf_Active = 0;

	misc_Bhop = 0;
	misc_ChaseCam = 0;
	misc_FreeLook = 0;
	misc_GranadeTrail = 0;
	misc_FastRun = 0;
}

int cCvar::ReadInt( char* szSection , char* szKey , int iDefaultValue , char* Patch )
{
	return GetPrivateProfileIntA( szSection , szKey , iDefaultValue , Patch );
}

void cCvar::SaveInt( char* szSection , char* szKey , int iValue , char* Patch )
{
	IniValue = native_itoa( iValue );
	WritePrivateProfileStringA( szSection , szKey , IniValue , Patch );
}

int cCvar::ReadString( char* szSection , char* szKey , char* cDefaultValue , char* Patch )
{
	char cTempString[INT_DIGITS];
	GetPrivateProfileStringA( szSection , szKey , cDefaultValue , cTempString , 16 , Patch );
	return g_Menu.CharToKey( cTempString );
}