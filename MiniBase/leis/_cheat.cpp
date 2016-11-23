#include "_cheat.h"

cCheat g_Cheat;

#pragma warning(disable:4244)

void cCheat::KnifeBotFunction( usercmd_s *cmd )
{
	float distance = 0;

	if ( g_Local.iWeaponID == WEAPONLIST_KNIFE && g_Local.bAlive )
	{
		for ( int i = 1; i<33; i++ )
		{
			if ( !g_Player[i].bUpdated )continue;
			if ( !g_Player[i].bAlive )continue;

			if ( cvar.knf_Team == 0 && g_Player[i].iTeam == g_Local.iTeam ) { continue; }

			switch ( cvar.knf_Attack )
			{
				case 1:
					distance = (float)cvar.knf_DistAttack;
					break;
				case 2:
					distance = (float)cvar.knf_DistAttack2;
					break;
			}

			if ( g_Player[i].Entity )
			{
				Vector vAttack = g_Player[i].Entity->origin - g_Local.vViewOrg;

				if ( VectorDistance( g_Local.Entity->origin , g_Player[i].Entity->origin ) <= distance )
				{
					float fov = AngleBetvenVectors( g_Local.vForward , vAttack );

					if ( (int)fov <= cvar.knf_AimFov )
					{
						if ( cvar.knf_Aim )
						{
							float attackva[3] = { };
							VectorAngles( vAttack , attackva );

							cmd->viewangles[0] = attackva[0];
							cmd->viewangles[1] = attackva[1];
						}

						switch ( cvar.knf_Attack )
						{
							case 1:
								cmd->buttons |= IN_ATTACK;
								break;
								return;
							case 2:
								cmd->buttons |= IN_ATTACK2;
								break;
								return;
						}
					}
				}
			}
		}
	}
}

void cCheat::FastRunFunction( usercmd_s *cmd )
{
	if ( g_Local.flXYspeed && g_Local.flZspeed == 0 && g_Local.iFlags&FL_ONGROUND && g_Local.iWeaponID == WEAPONLIST_KNIFE )
	{
		if ( ( cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVELEFT ) || ( cmd->buttons&IN_BACK && cmd->buttons&IN_MOVERIGHT ) )
		{
			if ( bFastRun ) { bFastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove -= 89.6f; }
			else { bFastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove += 89.6f; }
		}
		else if ( ( cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVERIGHT ) || ( cmd->buttons&IN_BACK && cmd->buttons&IN_MOVELEFT ) )
		{
			if ( bFastRun ) { bFastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove += 89.6f; }
			else { bFastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove -= 89.6f; }
		}
		else if ( cmd->buttons&IN_FORWARD || cmd->buttons&IN_BACK )
		{
			if ( bFastRun ) { bFastRun = false; cmd->sidemove -= 126.6f; }
			else { bFastRun = true;  cmd->sidemove += 126.6f; }
		}
		else if ( cmd->buttons&IN_MOVELEFT || cmd->buttons&IN_MOVERIGHT )
		{
			if ( bFastRun ) { bFastRun = false; cmd->forwardmove -= 126.6f; }
			else { bFastRun = true;  cmd->forwardmove += 126.6f; }
		}
	}
}

void cCheat::BhopFunction( usercmd_s *cmd )
{
	if ( cmd->buttons & IN_JUMP )
	{
		iBhopCount++;

		bool bGound = g_Local.iMoveType == FL_ONGROUND;

		if ( ( ( iBhopCount % 2 ) == 0 ) || bGound )
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}
	else iBhopCount = 0;
}

void cCheat::AimBotFunction( usercmd_s *cmd )
{
	Vector viewangles = cmd->viewangles;
	float forwardmove = cmd->forwardmove;
	float sidemove = cmd->sidemove;
	float upmove = cmd->upmove;

	g_Aimbot.CL_CreateMove( cmd );

	if ( cvar.weapon[cvar.wpn].aim_Silent >= 2 )
	{
		if ( cmd->buttons & IN_ATTACK )
		{
			iPacketCounter++;

			if ( iPacketCounter > 1 )
			{
				cmd->viewangles = viewangles;
				cmd->forwardmove = forwardmove;
				cmd->sidemove = sidemove;
				cmd->upmove = upmove;
			}
		}
		else
		{
			iPacketCounter = 0;

			cmd->viewangles = viewangles;
			cmd->forwardmove = forwardmove;
			cmd->sidemove = sidemove;
			cmd->upmove = upmove;
		}
	}
}

void cCheat::FreeLookFunction( float frametime , usercmd_s *cmd )
{
	if ( !bFreeLook )
	{
		g_Engine.GetViewAngles( g_Local.FreeLookAngles );

		g_Local.FreeLookOffset[0] = g_Local.Entity->origin.x;
		g_Local.FreeLookOffset[1] = g_Local.Entity->origin.y;
		g_Local.FreeLookOffset[2] = g_Local.Entity->origin.z + 80.0f;

		VectorCopy( g_Local.FreeLookAngles , cmd->viewangles );

		bFreeLook = true;
	}

	Vector curMove( 0.0f , 0.0f , 0.0f );

	Vector vr = g_Local.vRight;
	Vector vf = g_Local.vForward;

	if ( cmd->buttons&IN_MOVELEFT )curMove = curMove + vr*-1.0f;
	if ( cmd->buttons&IN_MOVERIGHT )curMove = curMove + vr*1.0f;
	if ( cmd->buttons&IN_FORWARD )curMove = curMove + vf*1.0f;
	if ( cmd->buttons&IN_BACK )curMove = curMove + vf*-1.0f;

	curMove = curMove*( cvar.misc_FreeLookSpeed * frametime );

	g_Local.FreeLookOffset[0] += curMove.x;
	g_Local.FreeLookOffset[1] += curMove.y;
	g_Local.FreeLookOffset[2] += curMove.z;

	cmd->buttons = 0;
	cmd->sidemove = 0;
	cmd->forwardmove = 0;
}

void cCheat::GranadeTrailFunction( usercmd_s *cmd )
{
	if ( ( g_Local.iWeaponID == WEAPONLIST_HEGRENADE || g_Local.iWeaponID == WEAPONLIST_SMOKEGRENADE || g_Local.iWeaponID == WEAPONLIST_FLASHBANG ) && cmd->buttons&IN_ATTACK2 )
	{
		float screenvec[2];
		float grenorigin[3];

		TraceGrenade( grenorigin );

		if ( CalcScreen( grenorigin , screenvec ) )
			g_Engine.pfnFillRGBA( screenvec[0] , screenvec[1] , 2 , 2 , 255 , 0 , 0 , 255 );
	}
}