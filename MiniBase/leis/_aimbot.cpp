#include "_aimbot.h"

cAimbot g_Aimbot;

#pragma warning(disable:4244)

bool cAimbot::bVisible( int iIndex )
{
	if ( cvar.weapon[cvar.wpn].aim_Aimthrough == 0 && !g_Player[iIndex].bVisible )
		return false;

	return true;
}

bool cAimbot::bCheckTeam( int iIndex )
{
	if ( cvar.weapon[cvar.wpn].aim_Deathmatch == 0 && g_Player[iIndex].iTeam == g_Local.iTeam )
		return false;

	return true;
}

void cAimbot::UpdateTargerId()
{
	float max_player_dist = 9999999.9f;
	float max_screen_dist = 9999999.9f;

	float center_screen[2];

	center_screen[0] = g_Screen.iWidth / 2;
	center_screen[1] = g_Screen.iHeight / 2;

	for ( int i = 1; i <= 32; i++ )
	{
		if ( !bCheckTeam( i ) || !bVisible( i ) || !g_Player[i].bUpdated || !g_Player[i].bAlive || g_Local.iIndex == i )
			continue;

		if ( cvar.weapon[cvar.wpn].aim_Distance <= 0 )
		{
			float SCR_Entity[2] = { 0 };
			float SCR_Aim[2] = { 0 };

			if ( g_Player[i].Entity )
			{
				if ( CalcScreen( g_Player[i].Entity->origin , SCR_Entity ) && 
					 CalcScreen( g_Player[i].vAimOrigin , SCR_Aim ) )
				{
					float screen_dist = CalcDistPlayerScreen( center_screen , SCR_Aim );

					if ( screen_dist < max_screen_dist )
					{
						max_screen_dist = screen_dist;
						iTargetID = i;
						iFovTargetID = i;
					}
				}
			}
		}
		else if ( cvar.weapon[cvar.wpn].aim_Distance >= 1 )
		{
			if ( g_Player[i].fDistance < max_player_dist )
			{
				max_player_dist = g_Player[i].fDistance;
				iTargetID = i;
				iFovTargetID = i;
			}
		}
	}
}

void cAimbot::UpdateAimInfo( int iIndex )
{	
	if ( cvar.weapon[cvar.wpn].aim_Mode <= 0 ) g_Player[iIndex].vAimOrigin = g_Aimbot.vCalcOriginOffset( iIndex );
	if ( cvar.weapon[cvar.wpn].aim_Mode == 1 ) g_Player[iIndex].vAimOrigin = g_Aimbot.vCalcBoneOffset( iIndex );
	if ( cvar.weapon[cvar.wpn].aim_Mode > 1 ) g_Player[iIndex].vAimOrigin = g_Aimbot.vCalcHitboxOffset( iIndex );

	float SCR_Entity[2] = { 0 };
	float SCR_Aim[2] = { 0 };

	if ( g_Player[iIndex].Entity )
	{
		g_Player[iIndex].fDistance = VectorDistance( g_Local.vViewOrg , g_Player[iIndex].Entity->origin );
		
		if ( CalcScreen( g_Player[iIndex].Entity->origin , SCR_Entity ) && 
			 CalcScreen( g_Player[iIndex].vAimOrigin , SCR_Aim ) )
		{
			int	iFovX = ( g_Screen.iWidth / 2 ) - (int)SCR_Aim[0];
			int	iFovY = ( g_Screen.iHeight / 2 ) - (int)SCR_Aim[1];

			int iFov = 0;

			if ( cvar.weapon[cvar.wpn].aim_FovType == 1 )
				iFov = cvar.weapon[cvar.wpn].aim_Fov * g_Player[iIndex].fDistance / cvar.weapon[cvar.wpn].aim_FovDist;
			else if ( cvar.weapon[cvar.wpn].aim_FovType > 1 )
				iFov = cvar.weapon[cvar.wpn].aim_Fov * cvar.weapon[cvar.wpn].aim_FovDist / g_Player[iIndex].fDistance;
			else
				iFov = cvar.weapon[cvar.wpn].aim_Fov;

			if ( iFovX < iFov && iFovX > -iFov && iFovY < iFov && iFovY > -iFov )
				g_Player[iIndex].isFov = true;
			else
				g_Player[iIndex].isFov = false;
		}
		else
			g_Player[iIndex].isFov = false;
	}
}

void cAimbot::UpdatePlayerInfo()
{
	iPreTargetID = iTargetID;

	if ( cvar.weapon[cvar.wpn].aim_KillDelay <= 0 || ( ( dwAimKillTime + cvar.weapon[cvar.wpn].aim_KillDelay ) <= GetTickCount() && !bAimEnable ) )
	{
		bAimEnable = true;
		dwAimKillTime = 0;
	}

	if ( iTargetID != 0 && bAimEnable )
	{
		if ( !g_Player[iTargetID].bAlive && bAimEnable && cvar.weapon[cvar.wpn].aim_KillDelay )
		{
			if ( !dwAimKillTime )
			{
				dwAimKillTime = GetTickCount();
				bAimEnable = false;
			}
		}

		UpdateTargerId();
	}
	else
	{
		UpdateTargerId();
	}
}

void cAimbot::DrawFov( int iIndex )
{
	if ( cvar.aim_DrawAimspot || cvar.aim_DrawFov )
	{
		float SCR_Entity[2] = { 0 };
		float SCR_Aim[2] = { 0 };

		if ( bCheckTeam( iIndex ) )
		{
			if ( g_Player[iIndex].Entity )
			{
				if ( CalcScreen( g_Player[iIndex].Entity->origin , SCR_Entity ) && 
					 CalcScreen( g_Player[iIndex].vAimOrigin , SCR_Aim ) )
				{
					if ( cvar.aim_DrawAimspot )
						g_Engine.pfnFillRGBA( (int)SCR_Aim[0] , (int)SCR_Aim[1] , 2 , 2 , 255 , 255 , 255 , 255 );

					if ( cvar.aim_DrawFov && cvar.weapon[cvar.wpn].aim_FovType >= 1 )
					{
						int iFov = 0;

						if ( cvar.weapon[cvar.wpn].aim_FovType == 1 )
							iFov = cvar.weapon[cvar.wpn].aim_Fov * g_Player[iIndex].fDistance / cvar.weapon[cvar.wpn].aim_FovDist;
						else if ( cvar.weapon[cvar.wpn].aim_FovType > 1 )
							iFov = cvar.weapon[cvar.wpn].aim_Fov * cvar.weapon[cvar.wpn].aim_FovDist / g_Player[iIndex].fDistance;

						int	iFovX = (int)SCR_Aim[0] - iFov;
						int	iFovY = (int)SCR_Aim[1] - iFov;

						int r , g , b;

						if ( iFovTargetID == iIndex && bVisible( iFovTargetID ) )
						{
							r = 255 , g = 255 , b = 0;
						}
						else
						{
							r = 255 , g = 10 , b = 255;
						}

						g_Drawing.DrawBox( iFovX , iFovY , iFov * 2 , iFov * 2 , 1 , r , g , b , 255 );
					}
				}
			}
		}
	}
}

void cAimbot::DrawFovCrosshair()
{
	if ( g_Local.iWeaponID == WEAPONLIST_AWP && g_Local.iFOV == 90 && cvar.weapon[cvar.wpn].aim_Crosshair )
	{
		g_Drawing.DrawBox( g_Screen.iWidth / 2 - 1 , g_Screen.iHeight / 2 - 1 , 2 , 2 , 2 , 255 , 0 , 0 , 255 );
	}

	if ( cvar.weapon[cvar.wpn].aim_Active && cvar.aim_DrawFov && cvar.weapon[cvar.wpn].aim_FovType <= 0 )
	{
		int iFovX = ( g_Screen.iWidth / 2 ) - cvar.weapon[cvar.wpn].aim_Fov;
		int iFovY = ( g_Screen.iHeight / 2 ) - cvar.weapon[cvar.wpn].aim_Fov;

		g_Drawing.DrawBox( iFovX , iFovY , cvar.weapon[cvar.wpn].aim_Fov * 2 , cvar.weapon[cvar.wpn].aim_Fov * 2 , 1 , 0 , 0 , 0 , 255 );
	}
}

void cAimbot::SmoothAimAngles( float *MyViewAngles , float *AimAngles , float *OutAngles , float Smoothing )
{
	VectorSubtract( AimAngles , MyViewAngles , OutAngles );

	if ( OutAngles[1]>180.0f )
		OutAngles[1] -= 360.0f;

	if ( OutAngles[1]<-180.0f )
		OutAngles[1] += 360.0f;

	OutAngles[0] = OutAngles[0] / Smoothing + MyViewAngles[0];
	OutAngles[1] = OutAngles[1] / Smoothing + MyViewAngles[1];

	if ( OutAngles[1] > 360.0f )
		OutAngles[1] -= 360.0f;

	if ( OutAngles[1] < 0.0f )
		OutAngles[1] += 360.0f;
}

void cAimbot::ApplySilentAngles( float *angles , usercmd_s *cmd )
{
	Vector viewforward , viewright , viewup , aimforward , aimright , aimup , vTemp;
	float newforward , newright , newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;
	if ( g_Local.Entity->curstate.movetype == MOVETYPE_WALK ) { g_Engine.pfnAngleVectors( Vector( 0.0f , cmd->viewangles.y , 0.0f ) , viewforward , viewright , viewup ); }
	else { g_Engine.pfnAngleVectors( cmd->viewangles , viewforward , viewright , viewup ); }
	if ( g_Local.Entity->curstate.movetype == MOVETYPE_WALK && !( g_Local.iMoveType == 5 ) )
	{
		cmd->viewangles.x = angles[0];
		cmd->viewangles.y = angles[1];
	}
	if ( g_Local.Entity->curstate.movetype == MOVETYPE_WALK ) { g_Engine.pfnAngleVectors( Vector( 0.0f , cmd->viewangles.y , 0.0f ) , aimforward , aimright , aimup ); }
	else { g_Engine.pfnAngleVectors( cmd->viewangles , aimforward , aimright , aimup ); }
	newforward = DotProduct( forward * viewforward.Normalize() , aimforward ) + DotProduct( right * viewright.Normalize() , aimforward ) + DotProduct( up * viewup.Normalize() , aimforward );
	newright = DotProduct( forward * viewforward.Normalize() , aimright ) + DotProduct( right * viewright.Normalize() , aimright ) + DotProduct( up * viewup.Normalize() , aimright );
	newup = DotProduct( forward * viewforward.Normalize() , aimup ) + DotProduct( right * viewright.Normalize() , aimup ) + DotProduct( up * viewup.Normalize() , aimup );
	if ( angles[0] > 81 ) { cmd->forwardmove = -newforward; }
	else { cmd->forwardmove = newforward; }
	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void cAimbot::GetBoneOrigin( cl_entity_s *Entity )
{
	if ( Entity && Entity->player && Entity->index != -1 && Entity->index != g_Local.iIndex && g_Players.isValidEntity( Entity ) )
	{
		typedef float TransformMatrix[MAXSTUDIOBONES][3][4];

		model_t* pModel = g_Studio.SetupPlayerModel( Entity->index );
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata( pModel );
		TransformMatrix* pbonetransform = (TransformMatrix*)g_Studio.StudioGetBoneTransform();

		iMaxBones = pStudioHeader->numbones;

		for ( int i = 0; i < pStudioHeader->numbones; i++ )
		{
			g_Player[Entity->index].vBone[i][0] = ( *pbonetransform )[i][0][3];
			g_Player[Entity->index].vBone[i][1] = ( *pbonetransform )[i][1][3];
			g_Player[Entity->index].vBone[i][2] = ( *pbonetransform )[i][2][3];

			g_Player[Entity->index].vBone[i] = g_Player[Entity->index].vBone[i] + ( g_Player[Entity->index].vVelocity * g_Player[Entity->index].fFrametime ) * cvar.weapon[cvar.wpn].aim_Prediction;
		}
	}
}

void cAimbot::GetHitboxOrigin( cl_entity_s *Entity )
{
	if ( Entity && Entity->player && Entity->index != -1 && Entity->index != g_Local.iIndex && g_Players.isValidEntity( Entity ) )
	{
		Vector vBBMin , vBBMax;
		typedef float BoneMatrix_t[MAXSTUDIOBONES][3][4];

		model_t* pModel = g_Studio.SetupPlayerModel( Entity->index );
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata( pModel );
		BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();
		mstudiobbox_t* pHitbox = (mstudiobbox_t*)( (byte*)pStudioHeader + pStudioHeader->hitboxindex );

		iMaxHitbox = pStudioHeader->numhitboxes;

		for ( int i = 0; i < pStudioHeader->numhitboxes; i++ )
		{
			VectorTransform( pHitbox[i].bbmin , ( *pBoneMatrix )[pHitbox[i].bone] , vBBMin );
			VectorTransform( pHitbox[i].bbmax , ( *pBoneMatrix )[pHitbox[i].bone] , vBBMax );

			g_Player[Entity->index].vHitbox[i] = ( vBBMax + vBBMin ) * 0.5f;
			g_Player[Entity->index].vHitbox[i] = g_Player[Entity->index].vHitbox[i] + ( g_Player[Entity->index].vVelocity * g_Player[Entity->index].fFrametime ) * cvar.weapon[cvar.wpn].aim_Prediction;
		}
	}
}

Vector cAimbot::vCalcOriginOffset( int iIndex )
{
	cl_entity_s *ent = g_Engine.GetEntityByIndex( iIndex );
	Vector vAngles , vF , vR , vU , vOut;
	vAngles = Vector( 0.0f , ent->angles[1] , 0.0f );
	g_Engine.pfnAngleVectors( vAngles , vF , vR , vU );

	if ( ent->curstate.gaitsequence == 2 || ent->curstate.gaitsequence == 5 ) // duck
	{
		vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_Origin;
	}
	else if ( ent->curstate.gaitsequence == 6 )									// stand
	{
		vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_Origin;
	}
	else
	{
		vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_Origin;
	}

	return ( vOut + ( g_Player[iIndex].vVelocity * g_Player[iIndex].fFrametime ) * cvar.weapon[cvar.wpn].aim_Prediction );
}

Vector cAimbot::vCalcBoneOffset( int iIndex )
{
	Vector vAngles , vF , vR , vU , vOut;
	vAngles = Vector( 0.0f , g_Player[iIndex].vAngle[1] , 0.0f );
	g_Engine.pfnAngleVectors( vAngles , vF , vR , vU );
	vOut = g_Player[iIndex].vBone[cvar.weapon[cvar.wpn].aim_Bone] + vF + vR + vU;
	return vOut;
}

Vector cAimbot::vCalcHitboxOffset( int iIndex )
{
	Vector vAngles , vF , vR , vU , vOut;
	vAngles = Vector( 0.0f , g_Player[iIndex].vAngle[1] , 0.0f );
	g_Engine.pfnAngleVectors( vAngles , vF , vR , vU );
	vOut = g_Player[iIndex].vHitbox[cvar.weapon[cvar.wpn].aim_Hitbox] + vF + vR + vU;
	return vOut;
}

void cAimbot::CL_CreateMove( usercmd_s *cmd )
{
	float fAimAngles[3] = { 0 };
	float fMyCmdAngles[3] = { 0 };
	float fMyViewAngles[3] = { 0 };
	float fSmoothAngles[3] = { 0 };

	VectorCopy( cmd->viewangles , fMyCmdAngles );

	g_Engine.GetViewAngles( fMyViewAngles );

	VectorAngles( g_Player[iTargetID].vAimOrigin - g_Local.vViewOrg , fAimAngles );

	if ( cmd->buttons & IN_ATTACK && !g_Local.bBadWeapon && bAimEnable )
	{
		float fRecoilX = g_Local.vPunchangle[1] * ( cvar.weapon[cvar.wpn].aim_RecoilX * 0.02 );
		float fRecoilY = g_Local.vPunchangle[0] * ( cvar.weapon[cvar.wpn].aim_RecoilY * 0.02 );

		if( cvar.weapon[cvar.wpn].aim_RecoilX )
			fAimAngles[1] -= fRecoilX;

		if ( cvar.weapon[cvar.wpn].aim_RecoilY )
			fAimAngles[0] -= fRecoilY;

		if ( cvar.weapon[cvar.wpn].aim_Smooth > 0 )
		{
			float* fMyView = cvar.weapon[cvar.wpn].aim_Silent <= 0 ? fMyViewAngles : fMyCmdAngles;
			SmoothAimAngles( fMyView , fAimAngles , fSmoothAngles , cvar.weapon[cvar.wpn].aim_Smooth );
		}
		else
		{
			VectorCopy( fAimAngles , fSmoothAngles );
		}

		if ( g_Player[iTargetID].isFov )
			g_Player[iTargetID].bEnableAim = true;
		else
			g_Player[iTargetID].bEnableAim = false;

		if ( g_Player[iTargetID].bEnableAim )
		{
			if ( !g_Player[iTargetID].dwStartTime )
			{
				g_Player[iTargetID].dwStartTime = GetTickCount();
			}

			DWORD dwNow = GetTickCount();

			if ( ( cvar.weapon[cvar.wpn].aim_Time <= 0 || ( g_Player[iTargetID].dwStartTime + cvar.weapon[cvar.wpn].aim_Time + cvar.weapon[cvar.wpn].aim_Delay ) >= dwNow ) &&
				 ( cvar.weapon[cvar.wpn].aim_Delay <= 0 || ( g_Player[iTargetID].dwStartTime + cvar.weapon[cvar.wpn].aim_Delay ) <= dwNow ) )
			{
				g_Player[iTargetID].bAttackTime = true;
			}
			else
			{
				g_Player[iTargetID].bAttackTime = false;
			}

			if ( ( cvar.weapon[cvar.wpn].aim_ShotDelay <= 0 || ( g_Player[iTargetID].dwStartTime + cvar.weapon[cvar.wpn].aim_ShotDelay + cvar.weapon[cvar.wpn].aim_Delay ) <= dwNow ) )
			{
				g_Player[iTargetID].bShotDelay = true;
			}
			else
			{
				g_Player[iTargetID].bShotDelay = false;
			}

			if ( iPreTargetID && !g_Player[iPreTargetID].bAlive )
			{
				if ( cvar.weapon[cvar.wpn].aim_NoTwitching && !bTwitching )
				{
					bTwitching = true;
				}
			}

			if ( !bTwitching && g_Player[iTargetID].bAlive && !g_Local.iInReload )
			{
				if ( g_Player[iTargetID].bAttackTime )
				{
					if ( cvar.weapon[cvar.wpn].aim_Silent <= 0 )
						g_Engine.SetViewAngles( fSmoothAngles );
					else if ( cvar.weapon[cvar.wpn].aim_Silent )
						ApplySilentAngles( fSmoothAngles , cmd );
				}

				if ( g_Player[iTargetID].bShotDelay )
					cmd->buttons |= IN_ATTACK;
				else
					cmd->buttons &= ~IN_ATTACK;
			}
		}
	}
	else
	{
		g_Player[iTargetID].dwStartTime = 0;
		g_Player[iTargetID].bEnableAim = false;
		g_Player[iTargetID].bAttackTime = false;

		bTwitching = false;
		iTargetID = 0;
		iPreTargetID = 0;
	}
}