#include "client.h"

bool FirstFrame = false;
net_status_t g_nStatus;

void HookUserMessages()
{
	pUserMsgBase = (PUserMsg)offset.FindUserMsgBase();

	pResetHUD = HookUserMsg( RESET_HUD_MSG , ResetHUD );
	pSetFOV = HookUserMsg( SET_FOV_MSG , SetFOV );
	pTeamInfo = HookUserMsg( TEAM_INFO_MSG , TeamInfo );
	pCurWeapon = HookUserMsg( CUR_WEAPON_MSG , CurWeapon );
	pDeathMsg = HookUserMsg( DEATH_MSG_MSG , DeathMsg );
}

void HookEngineMessages()
{
	pEngineMsgBase = (PEngineMsg)offset.FindSVCMessages();

	pSVC_sound = HookEngineMsg( SVC_SOUND_MSG , SVC_Sound );
	pSVC_SpawnStaticSound = HookEngineMsg( SVC_SPAWNSTATIC_SOUND_MSG , SVC_SpawnStaticSound );
}

void InitHack()
{
	g_Leis.InitHack();
}

void HUD_Frame( double time )
{
	if ( !FirstFrame )
	{
		g_Screen.iSize = sizeof( SCREENINFO );

		offset.HLType = g_Studio.IsHardware() + 1;

		HookUserMessages();
		HookEngineMessages();

		InitHack();
		
		FirstFrame = true;
	}
	
	g_Engine.pfnGetScreenInfo( &g_Screen );
	g_Engine.pNetAPI->Status( &( g_nStatus ) );

	g_Client.HUD_Frame( time );
}

void HUD_Redraw( float time , int intermission )
{
	g_Client.HUD_Redraw( time , intermission );

	if ( g_nStatus.connected )
		g_Leis.HUD_Redraw();
}

int HUD_Key_Event( int down , int keynum , const char *pszCurrentBinding )
{
	if ( g_nStatus.connected )
	{
		if ( down )
		{
			g_Leis.HUD_Key_Event( keynum );

			if ( g_Menu.Visible || g_Menu.ExitMenu )
			{
				if ( keynum == cvar.key_NavEnter )
					return 0;
				else if ( keynum == cvar.key_TogglePanic )
					return 0;
				else if ( keynum == cvar.key_NavLeave )
					return 0;
				else if ( keynum == cvar.key_NavUp )
					return 0;
				else if ( keynum == cvar.key_NavDown )
					return 0;
				else if ( keynum == cvar.key_NavLeft )
					return 0;
				else if ( keynum == cvar.key_NavRight )
					return 0;
			}
		}
	}

	return g_Client.HUD_Key_Event( down , keynum , pszCurrentBinding );
}

void HUD_PlayerMove( struct playermove_s *ppmove , int server )
{
	g_Client.HUD_PlayerMove( ppmove , server );

	if ( g_nStatus.connected )
		g_Leis.HUD_PlayerMove( ppmove );
}

void V_CalcRefdef( struct ref_params_s *pparams )
{
	if ( g_nStatus.connected )
		g_Leis.V_CalcRefdef( pparams );
	else
		g_Client.V_CalcRefdef( pparams );
}

void StudioEntityLight( struct alight_s *plight )
{
	if ( g_nStatus.connected )
		g_Leis.StudioEntityLight();

	g_Studio.StudioEntityLight( plight );
}

int HUD_AddEntity( int type , struct cl_entity_s *ent , const char *modelname )
{
	if ( g_nStatus.connected )
		return g_Leis.HUD_AddEntity( type , ent , modelname );
	else
		return g_Client.HUD_AddEntity( type , ent , modelname );
}

void CL_CreateMove( float frametime , usercmd_s *cmd , int active )
{
	g_Client.CL_CreateMove( frametime , cmd , active );

	if ( g_nStatus.connected )
		g_Leis.CL_CreateMove( frametime , cmd );
}

int pfnDrawUnicodeCharacter( int x , int y , short number , int r , int g , int b , unsigned long hfont )
{
	if ( g_nStatus.connected )
	{
		if ( cvar.rem_AmxxMessage && !bSnapshot && !bScreenshot )
			return 1;
	}

	return g_Engine.pfnDrawUnicodeCharacter( x , y , number , r , g , b , hfont );
}

int CL_IsThirdPerson()
{
	if ( g_nStatus.connected )
	{
		if ( cvar.misc_ChaseCam || cvar.misc_FreeLook )
			return 1;
	}

	return g_Client.CL_IsThirdPerson();
}

void HUD_PostRunCmd( struct local_state_s *from , struct local_state_s *to , struct usercmd_s *cmd , int runfuncs , double time , unsigned int random_seed )
{
	g_Client.HUD_PostRunCmd( from , to , cmd , runfuncs , time , random_seed );

	if ( g_nStatus.connected )
		g_Leis.HUD_PostRunCmd( to );
}

void HookFunction()
{
	g_pClient->HUD_Frame = HUD_Frame;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->HUD_Key_Event = HUD_Key_Event;
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->V_CalcRefdef = V_CalcRefdef;
	g_pStudio->StudioEntityLight = StudioEntityLight;
	g_pClient->HUD_AddEntity = HUD_AddEntity;
	g_pClient->CL_CreateMove = CL_CreateMove;
	g_pEngine->pfnDrawUnicodeCharacter = pfnDrawUnicodeCharacter;
	g_pClient->CL_IsThirdPerson = CL_IsThirdPerson;
	g_pClient->HUD_PostRunCmd = HUD_PostRunCmd;
}