#include "_menu.h"

cMenu g_Menu;

cMenu::cMenu()
{
	hHeap = HeapCreate( 0 , INT_DIGITS , 0 );
	pValue = (PCHAR)HeapAlloc( hHeap , HEAP_ZERO_MEMORY , INT_DIGITS );

	Visible = false;
	SubMenu = false;

	M_EntryPos = 0;
	M_EntrySize = 0;

	S_EntryPos = 0;
	S_EntrySize = 0;

	dwLastSaveTime = 0;
	dwLastLoadTime = 0;

	ClearMenu();
	
	DisableReload = false;
}

void cMenu::InitializeMenu()
{
	ClearMenu();
	
	AddMenu( CFG_AIMBOT_SETTINGS );
	AddMenu( CFG_KNIFEBOT_SETTINGS );
	AddMenu( CFG_ESP_SETTINGS );
	AddMenu( CFG_WALLHACK_SETTINGS );
	AddMenu( CFG_RADAR_SETTINGS );
	AddMenu( CFG_REMOVAL_SETTINGS );
	AddMenu( CFG_MISC_SETTINGS );

	if ( M_EntryPos == 0 )
	{
		AddSubMenu( CFG_AIM_ACTIVE , &cvar.weapon[cvar.wpn].aim_Active , 0 , 1 , 1 );
		AddSubMenu( CFG_AIM_DEATHMATCH , &cvar.weapon[cvar.wpn].aim_Deathmatch , 0 , 1 , 1 );
		AddSubMenu( CFG_AIM_THROUGH , &cvar.weapon[cvar.wpn].aim_Aimthrough , 0 , 1 , 1 );
		AddSubMenu( CFG_AIM_SILENT , &cvar.weapon[cvar.wpn].aim_Silent , 0 , 2 , 1 );
		AddSubMenu( CFG_AIM_SMOOTH , &cvar.weapon[cvar.wpn].aim_Smooth , 0 , 10 , 1 );
		AddSubMenu( CFG_AIM_FOV , &cvar.weapon[cvar.wpn].aim_Fov , 1 , 180 , 1 );
		AddSubMenu( CFG_AIM_FOV_TYPE , &cvar.weapon[cvar.wpn].aim_FovType , 0 , 2 , 1 );
		AddSubMenu( CFG_AIM_FOV_DIST , &cvar.weapon[cvar.wpn].aim_FovDist , 5 , 7000 , 5 );
		AddSubMenu( CFG_AIM_NOTWITCHING , &cvar.weapon[cvar.wpn].aim_NoTwitching , 0 , 1 , 1 );
		AddSubMenu( CFG_AIM_MODE , &cvar.weapon[cvar.wpn].aim_Mode , 0 , 2 , 1 );
		AddSubMenu( CFG_AIM_RECOIL_X , &cvar.weapon[cvar.wpn].aim_RecoilX , 0 , 100 , 1 );
		AddSubMenu( CFG_AIM_RECOIL_Y , &cvar.weapon[cvar.wpn].aim_RecoilY , 0 , 100 , 1 );
		AddSubMenu( CFG_AIM_ORIGIN , &cvar.weapon[cvar.wpn].aim_Origin , 0 , 22 , 1 );
		AddSubMenu( CFG_AIM_BONE , &cvar.weapon[cvar.wpn].aim_Bone , 1 , 53 , 1 );
		AddSubMenu( CFG_AIM_HITBOX , &cvar.weapon[cvar.wpn].aim_Hitbox , 1 , 20 , 1 );
		AddSubMenu( CFG_AIM_TIME , &cvar.weapon[cvar.wpn].aim_Time , 0 , 5000 , 10 );
		AddSubMenu( CFG_AIM_DELAY , &cvar.weapon[cvar.wpn].aim_Delay , 0 , 3000 , 10 );
		AddSubMenu( CFG_AIM_SHOT_DELAY , &cvar.weapon[cvar.wpn].aim_ShotDelay , 0 , 500 , 1 );
		AddSubMenu( CFG_AIM_KILL_DELAY , &cvar.weapon[cvar.wpn].aim_KillDelay , 0 , 10000 , 10 );
		AddSubMenu( CFG_AIM_PREDICTION , &cvar.weapon[cvar.wpn].aim_Prediction , -1000 , 1000 , 1 );
		AddSubMenu( CFG_AIM_DISTANCE , &cvar.weapon[cvar.wpn].aim_Distance , 0 , 1 , 1 );

		AddSubMenu( CFG_AIM_DRAWAIMSPOT , &cvar.aim_DrawAimspot , 0 , 1 , 1 );
		AddSubMenu( CFG_AIM_DRAW_FOV , &cvar.aim_DrawFov , 0 , 1 , 1 );

		if ( g_Local.iWeaponID == WEAPONLIST_AWP )
		{
			AddSubMenu( CFG_AIM_CROSSHAIR , &cvar.weapon[cvar.wpn].aim_Crosshair , 0 , 1 , 1 );
		}

		AddSubMenu( CFG_AIM_SAVESETTINGS , &cvar.aim_SaveSettings , 0 , 1 , 1 );
		AddSubMenu( CFG_AIM_LOADSETTINGS , &cvar.aim_LoadSettings , 0 , 1 , 1 );
	}
	else if ( M_EntryPos == 1 )
	{
		AddSubMenu( CFG_KNIFEBOT_ACTIVE , &cvar.knf_Active , 0 , 1 , 1 );
		AddSubMenu( CFG_KNIFEBOT_AIM , &cvar.knf_Aim , 0 , 1 , 1 );
		AddSubMenu( CFG_KNIFEBOT_AIM_FOV , &cvar.knf_AimFov , 90 , 360 , 1 );
		AddSubMenu( CFG_KNIFEBOT_TEAM , &cvar.knf_Team , 0 , 1 , 1 );
		AddSubMenu( CFG_KNIFEBOT_ATTACK , &cvar.knf_Attack , 1 , 2 , 1 );
		AddSubMenu( CFG_KNIFEBOT_DST_ATTACK , &cvar.knf_DistAttack , 1 , 100 , 1 );
		AddSubMenu( CFG_KNIFEBOT_DST_ATTACK2 , &cvar.knf_DistAttack2 , 1 , 100 , 1 );
	}
	else if ( M_EntryPos == 2 )
	{
		AddSubMenu( CFG_ESP_STYLE , &cvar.esp_Style , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_SIZE , &cvar.esp_Size , 1 , 20 , 1 );
		AddSubMenu( CFG_ESP_VISIBLE , &cvar.esp_Visible , 0 , 2 , 1 );
		AddSubMenu( CFG_ESP_NAME , &cvar.esp_Name , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_WEAPON , &cvar.esp_Weapon , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_3DBOX , &cvar.esp_3Dbox , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_SOUND , &cvar.esp_Sound , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_SOUND_TYPE , &cvar.esp_SoundType , 1 , 2 , 1 );
		AddSubMenu( CFG_ESP_SEQUENCE , &cvar.esp_Sequence , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_G_SEQUENCE , &cvar.esp_GaitSequence , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_BAREL , &cvar.esp_Barrel , 0 , 5000 , 100 );
		AddSubMenu( CFG_ESP_SPIKED , &cvar.esp_Spiked , 0 , 5000 , 100 );
		AddSubMenu( CFG_ESP_WORLD_WPN , &cvar.esp_WorldWeapons , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_WORLD_SPR , &cvar.esp_WorldSprite , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_DRAW_FONT , &cvar.esp_DrawFont , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_MULTICOLOR , &cvar.esp_MultiColor , 0 , 1 , 1 );
	}
	else if ( M_EntryPos == 3 )
	{
		AddSubMenu( CFG_WH_DRAW_PLAYER , &cvar.wh_DrawPlayer , 0 , 1 , 1 );
		AddSubMenu( CFG_WH_DRAW_WEAPON , &cvar.wh_DrawWeapon , 0 , 1 , 1 );
		AddSubMenu( CFG_WH_COLOR_PLAYER , &cvar.wh_ColorPlayer , 0 , 1 , 1 );
		AddSubMenu( CFG_WH_COLOR_WEAPON , &cvar.wh_ColorWeapon , 0 , 1 , 1 );
		AddSubMenu( CFG_WH_COLOR_GRENADE , &cvar.wh_ColorGrenade , 0 , 1 , 1 );
		AddSubMenu( CFG_WH_TRAN_ENTITY , &cvar.wh_TransEntity , 0 , 255 , 1 );
		AddSubMenu( CFG_WH_WALL_MODE , &cvar.wh_Wallmode , 0 , 4 , 1 );
		AddSubMenu( CFG_WH_WIREFRAME , &cvar.wh_Wireframe , 0 , 8 , 1 );
		AddSubMenu( CFG_WH_WIREFRAME_R , &cvar.wh_WireframeR , 0 , 255 , 1 );
		AddSubMenu( CFG_WH_WIREFRAME_G , &cvar.wh_WireframeG , 0 , 255 , 1 );
		AddSubMenu( CFG_WH_WIREFRAME_B , &cvar.wh_WireframeB , 0 , 255 , 1 );
		AddSubMenu( CFG_WH_LOCAL_GLOW , &cvar.wh_LocalGlow , 0 , 30 , 1 );
		AddSubMenu( CFG_WH_LOCAL_GLOW_R , &cvar.wh_LocalGlowR , 0 , 255 , 1 );
		AddSubMenu( CFG_WH_LOCAL_GLOW_G , &cvar.wh_LocalGlowG , 0 , 255 , 1 );
		AddSubMenu( CFG_WH_LOCAL_GLOW_B , &cvar.wh_LocalGlowB , 0 , 255 , 1 );
	}
	else if ( M_EntryPos == 4 )
	{
		AddSubMenu( CFG_RADAR_ACTIVE , &cvar.rad_Active , 0 , 1 , 1 );
		AddSubMenu( CFG_RADAR_SIZE , &cvar.rad_Size , 0 , 500 , 1 );
		AddSubMenu( CFG_RADAR_ZOOM , &cvar.rad_Zoom , 100 , 5000 , 100 );
		AddSubMenu( CFG_RADAR_TYPE , &cvar.rad_Type , 0 , 2 , 1 );
		AddSubMenu( CFG_RADAR_VISIBLE , &cvar.rad_Visible , 0 , 2 , 1 );
	}
	else if ( M_EntryPos == 5 )
	{
		AddSubMenu( CFG_REMOVE_SKY , &cvar.rem_RemoveSky , 0 , 1 , 1 );
		AddSubMenu( CFG_REMOVE_SMOKE , &cvar.rem_RemoveSmoke , 0 , 1 , 1 );
		AddSubMenu( CFG_REMOVE_FLASH , &cvar.rem_RemoveFlash , 0 , 255 , 1 );
		AddSubMenu( CFG_REMOVE_SPRITE , &cvar.rem_RemoveSprite , 0 , 1 , 1 );
		AddSubMenu( CFG_REMOVE_DRAW_WEAPON , &cvar.rem_RemoveDrawWeapon , 0 , 1 , 1 );
		AddSubMenu( CFG_REMOVE_FLASH_MSG , &cvar.rem_FlashMessage , 0 , 1 , 1 );
		AddSubMenu( CFG_REMOVE_AMXX_MSG , &cvar.rem_AmxxMessage , 0 , 1 , 1 );
	}
	else if ( M_EntryPos == 6 )
	{
		AddSubMenu( CFG_MISC_BHOP , &cvar.misc_Bhop , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_CHASE_CAM , &cvar.misc_ChaseCam , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_FREELOOK , &cvar.misc_FreeLook , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_FREELOOK_SPEED , &cvar.misc_FreeLookSpeed , 100 , 2000 , 50 );
		AddSubMenu( CFG_MISC_ANTI_SCREEN , &cvar.misc_AntiScreen , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_GRANADE_TRAIL , &cvar.misc_GranadeTrail , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_FASTRUN , &cvar.misc_FastRun , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_MENU_COLOR , &cvar.misc_MenuColor , 0 , 1 , 1 );
	}
}

void cMenu::DrawMenu( int x , int y )
{
	if ( !Visible )
	{
		dwLastSaveTime = 0;
		dwLastLoadTime = 0;

		cvar.aim_SaveSettings = 0;
		cvar.aim_LoadSettings = 0;
		
		native_memset( subm_entry , sizeof( subm_entry ) , 0 );
		native_memset( menu_entry , sizeof( menu_entry ) , 0 );
		
		return;
	}

	InitializeMenu();

	int menu_head_width = SUBM_ENTRY_WIDTH + MENU_ENTRY_WIDTH + SUBM_PADDING_LEFT;

	g_Drawing.DrawBox( x , y , menu_head_width , ENTRY_HEIGHT , 1 , 20 , 20 , 20 , 255 );
	g_Drawing.FillArea( x + 1 , y + 1 , menu_head_width - 1 , ENTRY_HEIGHT - 1 , 20 , 20 , 20 , 170 );

	if ( cvar.aim_SaveSettings && !dwLastSaveTime )
	{
		dwLastSaveTime = GetTickCount() + 1500;
		cvar.SaveCvarWeapon();
	}
	else if ( dwLastSaveTime > GetTickCount() && !dwLastLoadTime )
	{
		Verdana1.Print( x + ( menu_head_width / 2 ) , y , 0 , 255 , 0 , 255 , FL_CENTER , CFG_LEIS_SAVED );
	}
	else
	{
		dwLastSaveTime = 0;
		cvar.aim_SaveSettings = 0;
	}

	if ( cvar.aim_LoadSettings && !dwLastLoadTime )
	{
		dwLastLoadTime = GetTickCount() + 1500;
		cvar.LoadCvarWeapon();
	}
	else if ( dwLastLoadTime > GetTickCount() && !dwLastSaveTime )
	{
		Verdana1.Print( x + ( menu_head_width / 2 ) , y , 0 , 255 , 0 , 255 , FL_CENTER , CFG_LEIS_LOADED );
	}
	else
	{
		dwLastLoadTime = 0;
		cvar.aim_LoadSettings = 0;
	}

	if ( !dwLastSaveTime && !dwLastLoadTime )
	{
		Verdana1.Print( x + ( menu_head_width / 2 ) , y , 70 , 120 , 250 , 255 , FL_CENTER , CFG_LEIS_MESSAGE );
	}
	
	y += ENTRY_HEIGHT + MENU_PADDING_TOP;

	g_Drawing.DrawBox( x , y , MENU_ENTRY_WIDTH , ENTRY_HEIGHT * M_EntrySize + 2 , 1 , 20 , 20 , 20 , 255 );
	g_Drawing.FillArea( x + 1 , y + 1 , MENU_ENTRY_WIDTH - 1 , ENTRY_HEIGHT * M_EntrySize , 20 , 20 , 20 , 170 );


	for ( int i = 0; i < M_EntrySize; i++ )
	{
		if ( i == M_EntryPos )
		{
			if ( !SubMenu )
				g_Drawing.FillArea( x + 1 , y + 1 + ENTRY_HEIGHT * i , MENU_ENTRY_WIDTH - 2 , ENTRY_HEIGHT , 100 , 100 , 100 , 120 );
		}

		for ( byte a = 0; a < sizeof( m_entry::name ); a++ )
			menu_entry[i].name[a] ^= MENU_ENTRY_KEY;

		Verdana1.Print( x + TEXT_PADDING_LEFT , y + ENTRY_HEIGHT * i , 70 , 120 , 250 , 255 , 0 , menu_entry[i].name );

		for ( byte a = 0; a < sizeof( m_entry::name ); a++ )
			menu_entry[i].name[a] ^= MENU_ENTRY_KEY;
	}

	int subm_x = x + MENU_ENTRY_WIDTH + SUBM_PADDING_LEFT;

	g_Drawing.DrawBox( subm_x , y , SUBM_ENTRY_WIDTH , ENTRY_HEIGHT * S_EntrySize + 2 , 1 , 20 , 20 , 20 , 255 );
	g_Drawing.FillArea( subm_x + 1 , y + 1 , SUBM_ENTRY_WIDTH - 1 , ENTRY_HEIGHT * S_EntrySize , 20 , 20 , 20 , 170 );

	if ( S_EntryPos > ( S_EntrySize - 1 ) )
		S_EntryPos = ( S_EntrySize - 1 );

	for ( int i = 0; i < S_EntrySize; i++ )
	{
		char* on = "on";
		char* off = "off";

		BYTE r1 , g1 , b1;
		BYTE r2 , g2 , b2;
		BYTE r3 , g3 , b3;

		if ( cvar.misc_MenuColor )
		{
			r1 = 0; g1 = 255; b1 = 0;
			r2 = 255; g2 = 0; b2 = 0;
			r3 = 255; g3 = 255; b3 = 0;
		}
		else
		{
			r1 = 70; g1 = 120; b1 = 250;
			r2 = 70; g2 = 120; b2 = 250;
			r3 = 70; g3 = 120; b3 = 250;
		}

		if ( i == S_EntryPos )
		{
			if ( SubMenu )
				g_Drawing.FillArea( subm_x + 1 , y + 1 + ENTRY_HEIGHT * i , SUBM_ENTRY_WIDTH - 2 , ENTRY_HEIGHT , 100 , 100 , 100 , 120 );
		}

		for ( byte a = 0; a < 32; a++ )
			subm_entry[i].name[a] ^= SUBM_ENTRY_KEY;

		if ( subm_entry[i].min == 0 && subm_entry[i].max == 1 )
		{
			Verdana1.Print( subm_x + TEXT_PADDING_LEFT , y + ENTRY_HEIGHT * i , 70 , 120 , 250 , 255 , 0 , subm_entry[i].name );

			if ( *subm_entry[i].value == 0 )
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r2 , g2 , b2 , 255 , 0 , off );
			else
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r1 , g1 , b1 , 255 , 0 , on );
		}
		else
		{
			Verdana1.Print( subm_x + TEXT_PADDING_LEFT , y + ENTRY_HEIGHT * i , 70 , 120 , 250 , 255 , 0 , subm_entry[i].name );

			if ( *subm_entry[i].value == 0 )
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r2 , g2 , b2 , 255 , 0 , off );
			else
			{
				pValue = native_itoa( *subm_entry[i].value );
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r3 , g3 , b3 , 255 , 0 , pValue );
			}
		}

		for ( byte a = 0; a < 32; a++ )
			subm_entry[i].name[a] ^= SUBM_ENTRY_KEY;
	}
}

int cMenu::CharToKey( char* Key )
{
	if ( !native_strcmp( Key , "TAB" ) ) return K_TAB;
	if ( !native_strcmp( Key , "ENTER" ) ) return K_ENTER;
	if ( !native_strcmp( Key , "ESCAPE" ) ) return K_ESCAPE;
	if ( !native_strcmp( Key , "BACKSPACE" ) ) return K_BACKSPACE;
	if ( !native_strcmp( Key , "UPARROW" ) ) return K_UPARROW;
	if ( !native_strcmp( Key , "DOWNARROW" ) ) return K_DOWNARROW;
	if ( !native_strcmp( Key , "LEFTARROW" ) ) return K_LEFTARROW;
	if ( !native_strcmp( Key , "RIGHTARROW" ) ) return K_RIGHTARROW;

	if ( !native_strcmp( Key , "F1" ) ) return K_F1;
	if ( !native_strcmp( Key , "F2" ) ) return K_F2;
	if ( !native_strcmp( Key , "F3" ) ) return K_F3;
	if ( !native_strcmp( Key , "F4" ) ) return K_F4;
	if ( !native_strcmp( Key , "F5" ) ) return K_F5;
	if ( !native_strcmp( Key , "F6" ) ) return K_F6;
	if ( !native_strcmp( Key , "F7" ) ) return K_F7;
	if ( !native_strcmp( Key , "F8" ) ) return K_F8;
	if ( !native_strcmp( Key , "F9" ) ) return K_F9;
	if ( !native_strcmp( Key , "F10" ) ) return K_F10;
	if ( !native_strcmp( Key , "F11" ) ) return K_F11;
	if ( !native_strcmp( Key , "F12" ) ) return K_F12;

	if ( !native_strcmp( Key , "INS" ) ) return K_INS;
	if ( !native_strcmp( Key , "DEL" ) ) return K_DEL;
	if ( !native_strcmp( Key , "PGDN" ) ) return K_PGDN;
	if ( !native_strcmp( Key , "PGUP" ) ) return K_PGUP;
	if ( !native_strcmp( Key , "HOME" ) ) return K_HOME;
	if ( !native_strcmp( Key , "END" ) ) return K_END;

	if ( !native_strcmp( Key , "MWHEELDOWN" ) ) return K_MWHEELDOWN;
	if ( !native_strcmp( Key , "MWHEELUP" ) ) return K_MWHEELUP;

	if ( !native_strcmp( Key , "MOUSE1" ) ) return K_MOUSE1;
	if ( !native_strcmp( Key , "MOUSE2" ) ) return K_MOUSE2;

	return 0;
}

void cMenu::KeyEvent( int iKeynum )
{
	if ( iKeynum == cvar.key_ToggleMenu )
		Visible = !Visible;

	if ( iKeynum == cvar.key_TogglePanic && !DisableReload )
	{
		cvar.DisableAll();
		DisableReload = true;
		Visible = false;
	}
	else if ( iKeynum == cvar.key_TogglePanic && DisableReload )
	{
		cvar.LoadCvarFromIni();
		DisableReload = false;
	}

	ExitMenu = false;

	if ( !Visible )
		return;

	if ( iKeynum == cvar.key_NavEnter && !SubMenu )
	{
		S_EntryPos = 0;
		SubMenu = true;
	}
	else if ( iKeynum == cvar.key_NavLeave && !SubMenu )
	{
		Visible = false;
		ExitMenu = true;
	}
	else if ( iKeynum == cvar.key_NavLeave && SubMenu )
	{
		SubMenu = false;
	}
	else if ( iKeynum == cvar.key_NavUp && !SubMenu )
	{
		if ( M_EntryPos > 0 )
			M_EntryPos--;
		else
			M_EntryPos = M_EntrySize - 1;
	}
	else if ( iKeynum == cvar.key_NavDown && !SubMenu )
	{
		if ( M_EntryPos < ( M_EntrySize - 1 ) )
			M_EntryPos++;
		else
			M_EntryPos = 0;
	}
	else if ( iKeynum == cvar.key_NavUp && SubMenu )
	{
		if ( S_EntryPos > 0 )
			S_EntryPos--;
		else
			S_EntryPos = S_EntrySize - 1;
	}
	else if ( iKeynum == cvar.key_NavDown && SubMenu )
	{
		if ( S_EntryPos < ( S_EntrySize - 1 ) )
			S_EntryPos++;
		else
			S_EntryPos = 0;
	}
	else if ( iKeynum == cvar.key_NavLeft && SubMenu )
	{
		*subm_entry[S_EntryPos].value -= subm_entry[S_EntryPos].step;

		if ( *subm_entry[S_EntryPos].value < subm_entry[S_EntryPos].min )
			*subm_entry[S_EntryPos].value = subm_entry[S_EntryPos].min;
	}
	else if ( iKeynum == cvar.key_NavRight && SubMenu )
	{
		*subm_entry[S_EntryPos].value += subm_entry[S_EntryPos].step;

		if ( *subm_entry[S_EntryPos].value > subm_entry[S_EntryPos].max )
			*subm_entry[S_EntryPos].value = subm_entry[S_EntryPos].max;
	}
}

void cMenu::AddMenu( char* name )
{
	native_memset( menu_entry[M_EntrySize].name , sizeof( m_entry::name ) , 0 );
	native_strcpy( menu_entry[M_EntrySize].name , name );

	for ( byte i = 0; i < 32; i++ )
		menu_entry[M_EntrySize].name[i] ^= MENU_ENTRY_KEY;

	M_EntrySize++;
}

void cMenu::AddSubMenu( char* name , int* value , int min , int max , int step )
{
	native_memset( subm_entry[S_EntrySize].name , sizeof( s_entry::name ) , 0 );
	native_strcpy( subm_entry[S_EntrySize].name , name );

	for ( byte i = 0; i < 32; i++ )
		subm_entry[S_EntrySize].name[i] ^= SUBM_ENTRY_KEY;

	subm_entry[S_EntrySize].value = value;
	subm_entry[S_EntrySize].min = min;
	subm_entry[S_EntrySize].max = max;
	subm_entry[S_EntrySize].step = step;

	S_EntrySize++;
}

void cMenu::ClearMenu()
{
	for ( byte i = 0; i < M_EntrySize; i++ )
	{
		native_memset( &menu_entry[i].name , sizeof( m_entry::name ) , 0 );
	}

	for ( byte i = 0; i < S_EntrySize; i++ )
	{
		native_memset( &subm_entry[i].name , sizeof( s_entry::name ) , 0 );

		subm_entry[i].value = 0;
		subm_entry[i].min = 0;
		subm_entry[i].max = 0;
		subm_entry[i].step = 0;
	}

	M_EntrySize = 0;
	S_EntrySize = 0;
}