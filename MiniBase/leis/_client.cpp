#include "_client.h"

cLeis g_Leis;
cEntity g_Entity;
cSound g_Sound;
noflash_s noflash;

bool bSnapshot = false;
bool bScreenshot = false;

Snapshot_t Snapshot_s = nullptr;
Screenshot_t Screenshot_s = nullptr;

#pragma warning(disable:4244)

void cEntity::AddEntity( char* name , int iImportant , Vector vOrigin , BYTE type )
{
	if ( EntityIndex < MAX_ENTITY )
	{
		native_strcpy( entity[EntityIndex].name , name );

		entity[EntityIndex].iImportant = iImportant;
		entity[EntityIndex].vOrigin = vOrigin;
		entity[EntityIndex].type = type;

		EntityIndex++;
	}
	else
		ClearEntity();
}

void cEntity::ClearEntity()
{
	for ( int i = 0; i < EntityIndex; i++ )
	{
		native_memset( &entity[i].name[0] , 64 , 0 );

		entity[i].iImportant = 0;
		entity[i].vOrigin = Vector( 0 , 0 , 0 );
		entity[i].type = 0;
	}

	EntityIndex = 0;
}

void cSound::AddSound( DWORD dwTime , Vector vOrigin )
{
	if ( SoundIndex < MAX_SOUNDS )
	{
		sound[SoundIndex].dwTime = dwTime;
		sound[SoundIndex].vOrigin = vOrigin;

		SoundIndex++;
	}
	else
		ClearSound();
}

void cSound::ClearSound()
{
	for ( int i = 0; i < SoundIndex; i++ )
	{
		sound[i].dwTime = 0;
		sound[i].vOrigin = Vector( 0 , 0 , 0 );
	}

	SoundIndex = 0;
}

void cSound::DrawSoundEsp()
{
	for ( int i = 0; i < g_Sound.SoundIndex; i++ )
	{
		if ( g_Sound.sound[i].dwTime + 800 <= GetTickCount() )
		{
			g_Sound.sound[i].dwTime = 0;
			g_Sound.sound[i].vOrigin = Vector( 0 , 0 , 0 );
		}
		else
		{
			float SoundScreen[3] = { 0 };

			if ( CalcScreen( g_Sound.sound[i].vOrigin , SoundScreen ) )
			{
				if ( cvar.esp_SoundType <= 1 )
					Verdana1.Print( SoundScreen[0] , SoundScreen[1] , 255 , 0 , 255 , 255 , FL_CENTER , LEIS_SOUND_CHAR );
				else
					g_Drawing.DrawBox( SoundScreen[0] - 10 , SoundScreen[1] - 10 , 10 , 10 , 1 , 255 , 255 , 255 , 255 );
			}
		}
	}
}

void cLeis::AntiSnapshot()
{
	if ( cvar.misc_AntiScreen )
		bSnapshot = true;
	else
		Snapshot_s();
}

void cLeis::AntiScreenshot()
{
	if ( cvar.misc_AntiScreen )
		bScreenshot = true;
	else
		Screenshot_s();
}

void cLeis::AntiScreen()
{
	if ( bSnapshot || bScreenshot )
	{
		ScreenTimer--;

		if ( cvar.rad_Active )
			g_Engine.pfnClientCmd( LEIS_DRAW_RADAR );

		if ( ScreenTimer <= 0 )
		{
			if ( bSnapshot )
			{
				Snapshot_s();
				bSnapshot = false;
			}
			else if ( bScreenshot )
			{
				Screenshot_s();
				bScreenshot = false;
			}

			ScreenTimer = 5;

			if ( cvar.rad_Active )
				g_Engine.pfnClientCmd( LEIS_HIDE_RADAR );
		}
	}
}

void cLeis::InitHack()
{
	if ( offset.HLType != RENDERTYPE_HARDWARE )
	{
		offset.Error( ERR_MODE );
	}

	HookOpenGL();

	Verdana1.InitText( CFG_VERDANA , 12 , 0 );
	Verdana2.InitText( CFG_VERDANA , 14 , 0 );
	ArialBlack.InitText( CFG_ARIAL_BLACK , 28 , 0 );
	
	dwMsgMs = 0;
	bShowMsg = true;
	g_Local.Entity = nullptr;
	g_Local.bAlive = false;
	ScreenTimer = 5;
	g_Radar.bRadarHide = true;

	pcmd_t pSnapshot = CommandByName( CFG_SNAPSHOT );
	pcmd_t pScreenshot = CommandByName( CFG_SCREENSHOT );

	Snapshot_s = (Snapshot_t)pSnapshot->function;
	Screenshot_s = (Snapshot_t)pScreenshot->function;

	pSnapshot->function = (xcommand_t)AntiSnapshot;
	pScreenshot->function = (xcommand_t)AntiScreenshot;
		
	cvar.LoadCvarFromIni();
}

void cLeis::HUD_Redraw()
{
	g_Players.UpdatePlayerInfo();

	//Verdana2.Print( g_Screen.iWidth / 2 , 10 , 255 , 0 , 0 , 255 , FL_CENTER , native_itoa( g_Local.iFOV ) );

	if ( !bSnapshot && !bScreenshot )
	{
		if ( bShowMsg )
		{
			dwMsgMs = GetTickCount();
			bShowMsg = false;
		}
		if ( dwMsgMs + 5000 > GetTickCount() )
		{
			ArialBlack.Print( g_Screen.iWidth / 2 , g_Screen.iHeight / 2 - 30 , 255 , 255 , 255 , 255 , FL_CENTER , CFG_LEIS_MESSAGE );
			ArialBlack.Print( g_Screen.iWidth / 2 , g_Screen.iHeight / 2 , 255 , 255 , 255 , 255 , FL_CENTER , CFG_LEIS_AUTHOR );
			ArialBlack.Print( g_Screen.iWidth / 2 , g_Screen.iHeight / 2 + 30 , 255 , 255 , 255 , 255 , FL_CENTER , CFG_LEIS_WEBSITE );
		}
		else
		{
			Verdana2.Print( g_Screen.iWidth / 2 , 5 , 255 , 255 , 255 , 255 , FL_CENTER , CFG_LEIS_MESSAGE );
		}

		g_Menu.DrawMenu( g_Screen.iWidth - 301 , ( g_Local.bAlive ? 3 : 110 ) );
	}

	AntiScreen();
}

void cLeis::HUD_Key_Event( int keynum )
{
	g_Menu.KeyEvent( keynum );
}

void cLeis::HUD_PlayerMove( struct playermove_s *ppmove )
{
	g_Local.iFlags = ppmove->flags;
	g_Local.iMoveType = ppmove->movetype;
	g_Local.iWaterLevel = ppmove->waterlevel;

	g_Local.flZspeed = ( ppmove->velocity[2] != 0 ) ? -ppmove->velocity[2] : 0;
	g_Local.flXYspeed = mysqrt( POW( ppmove->velocity[0] ) + POW( ppmove->velocity[1] ) );
	g_Local.flFallSpeed = ppmove->flFallVelocity;

	float fYaw = float( ppmove->angles[1] * ( M_PI / 180.0 ) );

	g_Local.fsin_yaw = mysin( fYaw );
	g_Local.fminus_cos_yaw = -mycos( fYaw );

	g_Local.vVelocity = ppmove->velocity;
}

void cLeis::V_CalcRefdef( struct ref_params_s *pparams )
{
	VectorCopy( pparams->punchangle , g_Local.vPunchangle );

	g_Client.V_CalcRefdef( pparams );

	if ( cvar.misc_ChaseCam && g_Local.bAlive && !cvar.misc_FreeLook )
	{
		Vector r , u , b , Offset;

		VectorMul( pparams->right , 0 , r );
		VectorMul( pparams->up , 10 , u );
		VectorMul( pparams->forward , -100 , b );

		Offset = Offset + r;
		Offset = Offset + u;
		Offset = Offset + b;

		pparams->vieworg[0] += Offset[0];
		pparams->vieworg[1] += Offset[1];
		pparams->vieworg[2] += Offset[2];
	}

	if ( cvar.misc_FreeLook && g_Local.bAlive )
	{
		VectorCopy( g_Local.FreeLookOffset , pparams->vieworg );
	}

	VectorCopy( pparams->forward , g_Local.vForward );
	VectorCopy( pparams->right , g_Local.vRight );
}

void cLeis::StudioEntityLight()
{
	cl_entity_s* Entity = g_Studio.GetCurrentEntity();
	
	if ( Entity && Entity->player && cvar.weapon[cvar.wpn].aim_Active )
	{
		g_Aimbot.GetBoneOrigin( Entity );
		g_Aimbot.GetHitboxOrigin( Entity );
	}
}

void cLeis::HUD_PostRunCmd( struct local_state_s *to )
{
	g_Local.iInReload = ( to->weapondata[g_Local.iWeaponID].m_fInReload > 0 || !g_Local.iClip );
}

int cLeis::HUD_AddEntity( int type , struct cl_entity_s *ent , const char *modelname )
{
	int ret = g_Client.HUD_AddEntity( type , ent , modelname );

	if ( bSnapshot || bScreenshot )
	{
		if ( cvar.wh_LocalGlow )
		{
			cl_entity_t *wpn = g_Engine.GetViewModel();

			wpn->curstate.rendercolor.r = 0;
			wpn->curstate.rendercolor.g = 0;
			wpn->curstate.rendercolor.b = 0;
		}

		return ret;
	}

	if ( native_strstr( ent->model->name , SPR_REMOVE_SMOKE ) && cvar.rem_RemoveSmoke )
		return ret;

	if ( native_strstr( ent->model->name , SPR_REMOVE_SPRITE ) && cvar.rem_RemoveSprite )
		return 0;

	if ( native_strstr( ent->model->name , SPR_REMOVE_DRAW_WEAPON ) && cvar.rem_RemoveDrawWeapon )
		return 0;

	g_Esp.HUD_AddEntity( type , ent );

	return ret;
}

void cLeis::CL_CreateMove( float frametime , usercmd_s *cmd )
{
	if ( g_Local.bAlive )
	{
		g_Local.bBadWeapon = ( g_Local.iWeaponID == WEAPONLIST_C4 ) || ( g_Local.iWeaponID == WEAPONLIST_FLASHBANG ) || ( g_Local.iWeaponID == WEAPONLIST_HEGRENADE ) || ( g_Local.iWeaponID == WEAPONLIST_KNIFE ) || ( g_Local.iWeaponID == WEAPONLIST_SMOKEGRENADE );
		g_Local.fFrametime = frametime;

		if ( !g_Local.bBadWeapon )
			cvar.wpn = cvar.GetSectionFromIndex( g_Local.iWeaponID );

		if ( cvar.knf_Active )
			g_Cheat.KnifeBotFunction( cmd );

		if ( cvar.misc_FastRun )
			g_Cheat.FastRunFunction( cmd );

		if ( cvar.misc_Bhop )
			g_Cheat.BhopFunction( cmd );

		if ( cvar.weapon[cvar.wpn].aim_Active )
			g_Cheat.AimBotFunction( cmd );

		if ( cvar.misc_FreeLook )
			g_Cheat.FreeLookFunction( frametime , cmd );
		else
			g_Cheat.bFreeLook = false;

		if ( cvar.misc_GranadeTrail && !bSnapshot && !bScreenshot )
			g_Cheat.GranadeTrailFunction( cmd );
	}
}