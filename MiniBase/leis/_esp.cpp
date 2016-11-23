#include "_esp.h"

cEsp g_Esp;

void cEsp::DrawLine( int x1 , int y1 , int x2 , int y2 , int r , int g , int b )
{
	glPushMatrix();
	glLoadIdentity();
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	glColor4ub( r , g , b , 255 );
	glLineWidth( 1.0 );
	glBegin( GL_LINES );
	glVertex2i( x1 , y1 );
	glVertex2i( x2 , y2 );
	glEnd();
	glColor3f( 1.0f , 1.0f , 1.0f );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glPopMatrix();
}

void cEsp::DrawVectorLine( float *flSrc , float *flDestination , int lw , int r , int g , int b )
{
	vec3_t vScreenSrc , vScreenDest;

	if ( CalcScreen( flSrc , vScreenSrc ) && CalcScreen( flDestination , vScreenDest ) )
	{
		DrawLine( (int)vScreenSrc[0] , (int)vScreenSrc[1] , (int)vScreenDest[0] , (int)vScreenDest[1] , r , g , b );
	}
}

void cEsp::Draw3DBox( cl_entity_t *pEnt , Vector origin , int r , int g , int b )
{
	vec3_t vF , vR , vU;
	g_Engine.pfnAngleVectors( Vector( 0.0f , pEnt->angles[1] , 0.0f ) , vF , vR , vU );
	
	float flForward = pEnt->curstate.maxs.y + 15.0f;
	float flBack = pEnt->curstate.mins.y - 5.0f;
	float flRight = pEnt->curstate.maxs.x + 5.0f;
	float flLeft = pEnt->curstate.mins.x - 5.0f;

	vec3_t vUFLeft = origin + vU*pEnt->curstate.maxs.z + vF*flForward + vR*flLeft; // vUFLeft = Top left front
	vec3_t vUFRight = origin + vU*pEnt->curstate.maxs.z + vF*flForward + vR*flRight; // vUFRight = Top right front
	vec3_t vUBLeft = origin + vU*pEnt->curstate.maxs.z + vF*flBack + vR*flLeft; // vUBLeft = Top left back
	vec3_t vUBRight = origin + vU*pEnt->curstate.maxs.z + vF*flBack + vR*flRight; // vUBRight = Top right back
	vec3_t vBFLeft = origin + vU*pEnt->curstate.mins.z + vF*flForward + vR*flLeft; // vBFLeft = Bottom left front
	vec3_t vBFRight = origin + vU*pEnt->curstate.mins.z + vF*flForward + vR*flRight; // vBFRight = Bottom right front
	vec3_t vBBLeft = origin + vU*pEnt->curstate.mins.z + vF*flBack + vR*flLeft; // vBBLeft = Bottom left back
	vec3_t vBBRight = origin + vU*pEnt->curstate.mins.z + vF*flBack + vR*flRight; // vBBRight = Bottom right back

																				  //Top Box
	DrawVectorLine( vUBLeft , vUBRight , 1 , r , g , b ); // Back left -> Back right
	DrawVectorLine( vUBRight , vUFRight , 1 , r , g , b ); // Back right -> Front right
	DrawVectorLine( vUFRight , vUFLeft , 1 , r , g , b ); // Front right -> Front left
	DrawVectorLine( vUFLeft , vUBLeft , 1 , r , g , b ); // Front left -> Back right
														 //Mid Box
	DrawVectorLine( vUBLeft , vBBLeft , 1 , r , g , b ); // Top left -> Bottom left
	DrawVectorLine( vUBRight , vBBRight , 1 , r , g , b ); // Top right -> Bottom right
	DrawVectorLine( vUFRight , vBFRight , 1 , r , g , b ); // Top right -> Bottom right
	DrawVectorLine( vUFLeft , vBFLeft , 1 , r , g , b ); // Top left -> Bottom left
														 //Bottom Box
	DrawVectorLine( vBBLeft , vBBRight , 1 , r , g , b ); // Back left -> Back right
	DrawVectorLine( vBBRight , vBFRight , 1 , r , g , b ); // Back right -> Front right
	DrawVectorLine( vBFRight , vBFLeft , 1 , r , g , b ); // Front right -> Front left
	DrawVectorLine( vBFLeft , vBBLeft , 1 , r , g , b ); // Front left -> Back right
}

void cEsp::DrawNoFlash()
{
	if ( cvar.rem_RemoveFlash > 0 )
	{
		g_Engine.pfnGetScreenFade( &noflash.pScreenFade );

		if ( noflash.pScreenFade.fadeEnd > g_Engine.GetClientTime() )
		{
			noflash.FadeEnd = noflash.pScreenFade.fadeEnd;

			if ( cvar.rem_RemoveFlash >= 255 )
				noflash.pScreenFade.fadeEnd = 255.0f;
			else if ( noflash.pScreenFade.fadealpha > cvar.rem_RemoveFlash )
				noflash.pScreenFade.fadealpha = cvar.rem_RemoveFlash;
		}

		if ( noflash.FadeEnd > g_Engine.GetClientTime() )
			noflash.Flashed = ( noflash.FadeEnd - g_Engine.GetClientTime() ) * noflash.pScreenFade.fadeSpeed;
		else
			noflash.Flashed = 0.0f;

		if ( noflash.Flashed > 255.0f ) noflash.Flashed = 255.0f;
		if ( noflash.Flashed < 0.0f ) noflash.Flashed = 0.0f;

		float Percentage = ( noflash.Flashed / 255.0f ) * 100.0f;

		g_Engine.pfnSetScreenFade( &noflash.pScreenFade );

		if ( Percentage > 0 && cvar.rem_FlashMessage )
		{
			byte r , g , b;
			float alpha = Percentage / 100.f * 255.f;

			if ( Percentage >= 90.0 )
			{
				r = 255 , g = 0 , b = 0;
			}
			else if ( Percentage >= 70.0 && Percentage < 90 )
			{
				r = 255 , g = 255 , b = 0;
			}
			else if ( Percentage < 70.0 )
			{
				r = 0 , g = 255 , b = 0;
			}

			int flash_w = 100;
			int flash_h = 8;
			int flash_box_y = g_Screen.iHeight - 50;
			int flash_box_x = g_Screen.iWidth / 2;

			g_Drawing.DrawBox( flash_box_x - flash_w - 1 , flash_box_y - flash_h - 1 , flash_w * 2 + 2 , flash_h * 2 + 2 , 1 , 0 , 0 , 0 , (int)alpha );
			g_Drawing.FillArea( flash_box_x - flash_w , flash_box_y - flash_h , flash_w * 2 , flash_h * 2 , r , g , b , (int)alpha );

			char FlashMessage[20];
			char* value = native_itoa( (int)Percentage );

			native_strcpy( FlashMessage , ESP_YOR_ARE );
			native_strcat( FlashMessage , value );
			native_strcat( FlashMessage , ESP_FLAHED );

			Verdana2.Print( g_Screen.iWidth / 2 , flash_box_y - ( Verdana2.fheight / 2 ) + 1 , 0 , 0 , 0 , 255 , FL_CENTER , FlashMessage );
		}
	}
}

void cEsp::DrawPlayerESP( int iIndex )
{
	float VecScreen[2] = { 0 };
	byte r = 255 , g = 255 , b = 255;

	int Cstrike_SequenceInfo[111] = {
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 1 , 2 , 0 , 1 , 2 , 0 , 1 , 2 , 0 ,
		1 , 2 , 0 , 1 , 1 , 2 , 0 , 1 , 1 , 2 ,
		0 , 1 , 2 , 0 , 1 , 2 , 0 , 1 , 2 , 0 ,
		1 , 2 , 0 , 1 , 2 , 0 , 1 , 2 , 0 , 1 ,
		2 , 0 , 1 , 2 , 0 , 0 , 0 , 4 , 0 , 4 ,
		0 , 5 , 0 , 5 , 0 , 0 , 1 , 1 , 2 , 0 ,
		1 , 1 , 2 , 0 , 6 , 0 , 6 , 0 , 1 , 2 ,
		0 , 1 , 2 , 7 , 4 , 7 , 4 , 7 , 6 , 7 ,
		6 , 7 , 1 , 2 , 7 , 1 , 2 , 8 , 8 , 0 ,
		0 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 ,
		3
	};

	int Cstrike_EGaitSequenceInfo[10] = {
		0 , 0 , 1 , 2 , 3 , 4 , 5 , 5 , 6 , 6
	};

	if ( g_Player[iIndex].iTeam == 1 )
	{
		if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 0 )
		{
			r = 255; g = 64; b = 64;
		}
		else if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 1 )
		{
			r = 0; g = 255; b = 0;
		}
		else if ( g_Player[iIndex].bVisible && g_Local.iTeam != g_Player[iIndex].iTeam && cvar.esp_Visible == 2 )
		{
			r = 0; g = 255; b = 0;
		}
		else
		{
			r = 255; g = 64; b = 64;
		}
	}
	else if ( g_Player[iIndex].iTeam == 2 )
	{
		if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 0 )
		{
			r = 0; g = 164; b = 255;
		}
		else if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 1 )
		{
			r = 0; g = 255; b = 0;
		}
		else if ( g_Player[iIndex].bVisible && g_Local.iTeam != g_Player[iIndex].iTeam && cvar.esp_Visible == 2 )
		{
			r = 0; g = 255; b = 0;
		}
		else
		{
			r = 0; g = 164; b = 255;
		}
	}
	else
	{
		return;
	}

	if ( CalcScreen( g_Player[iIndex].Entity->origin , VecScreen ) )
	{
		int esp_x = (int)VecScreen[0];
		int esp_y = (int)VecScreen[1];

		int esp_box_x = esp_x - cvar.esp_Size;
		int esp_box_y = esp_y - cvar.esp_Size;

		if ( cvar.esp_Style >= 1 )
		{
			int esp_size = cvar.esp_Size * 2;

			g_Drawing.DrawBox( esp_box_x , esp_box_y , esp_size , esp_size , 1 , r , g , b , 255 );
		}

		if ( cvar.esp_Name || cvar.esp_Weapon || cvar.esp_3Dbox || cvar.esp_Sequence || cvar.esp_GaitSequence )
		{
			int esp_name_y = esp_y - ( cvar.esp_Style ? cvar.esp_Size : 0 ) - Verdana1.fheight;
			int esp_weapon_y = esp_y + ( cvar.esp_Style ? cvar.esp_Size : 0 );
			int esp_sequence_y = cvar.esp_Weapon ? esp_weapon_y + ( cvar.esp_Name ? Verdana1.fheight : 0 ) : ( cvar.esp_Name ? esp_weapon_y : esp_name_y );
			int esp_g_squencse_y = cvar.esp_Sequence ? ( cvar.esp_Weapon ? esp_sequence_y + Verdana1.fheight : esp_weapon_y ) : ( cvar.esp_Weapon ? esp_sequence_y : ( cvar.esp_Name ? esp_weapon_y : esp_name_y ) );

			int n_esp_name_y = ( cvar.esp_Weapon ? esp_weapon_y : esp_name_y );
			int n_esp_weapon_y = ( cvar.esp_Name ? esp_weapon_y : esp_name_y );

			if ( cvar.esp_Name && cvar.esp_Weapon )
			{
				n_esp_name_y = esp_name_y;
				n_esp_weapon_y = esp_weapon_y;
			}

			if ( cvar.esp_Name )
			{
				if ( cvar.esp_DrawFont )
					Verdana1.Print( esp_x , n_esp_name_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Player[iIndex].Info.name );
				else
					Verdana1.PrintFont( esp_x , n_esp_name_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Player[iIndex].Info.name );
			}

			if ( cvar.esp_Weapon )
			{
				model_s *pModel = g_Studio.GetModelByIndex( g_Player[iIndex].Entity->curstate.weaponmodel );

				if ( pModel && pModel->name )
				{
					char WeaponName[64];

					int iLen = native_strlen( pModel->name + 9 ) - 3;
					native_strncpy( WeaponName , pModel->name + 9 , iLen );
					WeaponName[iLen] = '\0';

					if ( cvar.esp_DrawFont )
					{
						if ( cvar.esp_MultiColor )
							Verdana1.Print( esp_x , n_esp_weapon_y , 0 , 255 , 255 , 255 , FL_CENTER | FL_OUTLINE , WeaponName );
						else
							Verdana1.Print( esp_x , n_esp_weapon_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , WeaponName );
					}
					else
					{
						if ( cvar.esp_MultiColor )
							Verdana1.PrintFont( esp_x , n_esp_weapon_y , 0 , 255 , 255 , 255 , FL_CENTER | FL_OUTLINE , WeaponName );
						else
							Verdana1.PrintFont( esp_x , n_esp_weapon_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , WeaponName );
					}
				}
			}

			if ( cvar.esp_3Dbox )
			{
				Draw3DBox( g_Player[iIndex].Entity , g_Player[iIndex].Entity->origin , r , g , b );
			}

			if ( cvar.esp_Sequence )
			{
				char* Sequence;

				int atype = Cstrike_SequenceInfo[g_Player[iIndex].Entity->curstate.sequence];
				if ( atype == 0 ) { Sequence = ESP_IDLE; }
				else if ( atype == 1 ) { Sequence = ESP_SHOOTING; }
				else if ( atype == 2 ) { Sequence = ESP_RELOADING; }
				else if ( atype == 3 ) { Sequence = ESP_DEAD; }
				else if ( atype == 4 ) { Sequence = ESP_THROWING; }
				else if ( atype == 5 ) { Sequence = ESP_PLANTING; }
				else if ( atype == 6 ) { Sequence = ESP_SWINGING; }
				else if ( atype == 7 ) { Sequence = ESP_OPEN_SHIELD; }
				else if ( atype == 8 ) { Sequence = ESP_CLOS_SHIELD; }

				if ( cvar.esp_DrawFont )
				{
					if ( cvar.esp_MultiColor )
						Verdana1.Print( esp_x , esp_sequence_y , 255 , 255 , 0 , 255 , FL_CENTER | FL_OUTLINE , Sequence );
					else
						Verdana1.Print( esp_x , esp_sequence_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , Sequence );
				}
				else
				{
					if ( cvar.esp_MultiColor )
						Verdana1.PrintFont( esp_x , esp_sequence_y , 255 , 255 , 0 , 255 , FL_CENTER | FL_OUTLINE , Sequence );
					else
						Verdana1.PrintFont( esp_x , esp_sequence_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , Sequence );
				}
			}

			if ( cvar.esp_GaitSequence )
			{
				char* Sequence;
				int atype = Cstrike_EGaitSequenceInfo[g_Player[iIndex].Entity->curstate.gaitsequence];
				if ( atype == 0 ) { Sequence = ESP_STANDING; }
				else if ( atype == 1 ) { Sequence = ESP_CROUCHING; }
				else if ( atype == 2 ) { Sequence = ESP_WALKING; }
				else if ( atype == 3 ) { Sequence = ESP_RUNNING; }
				else if ( atype == 4 ) { Sequence = ESP_CROUCH_RUN; }
				else if ( atype == 5 ) { Sequence = ESP_JUMPING; }
				else if ( atype == 6 ) { Sequence = ESP_SWIMMING; }

				if ( cvar.esp_DrawFont )
				{
					if ( cvar.esp_MultiColor )
						Verdana1.Print( esp_x , esp_g_squencse_y , 255 , 0 , 255 , 255 , FL_CENTER | FL_OUTLINE , Sequence );
					else
						Verdana1.Print( esp_x , esp_g_squencse_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , Sequence );
				}
				else
				{
					if ( cvar.esp_MultiColor )
						Verdana1.PrintFont( esp_x , esp_g_squencse_y , 255 , 0 , 255 , 255 , FL_CENTER | FL_OUTLINE , Sequence );
					else
						Verdana1.PrintFont( esp_x , esp_g_squencse_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , Sequence );
				}
			}
		}
	}
}

void cEsp::DrawAllEntityESP()
{
	if ( cvar.esp_WorldWeapons || cvar.esp_WorldSprite )
	{
		for ( int i = 0; i < g_Entity.EntityIndex; i++ )
		{
			float EntScreen[2] = { 0 };

			if ( CalcScreen( g_Entity.entity[i].vOrigin , EntScreen ) )
			{
				if ( g_Entity.entity[i].type == 1 )
				{
					byte r , g , b;

					if ( g_Entity.entity[i].iImportant == 0 )
					{
						r = 255 , g = 255 , b = 255;
					}
					else if ( g_Entity.entity[i].iImportant == 1 )
					{
						r = 255; g = 64; b = 64;
					}
					else if ( g_Entity.entity[i].iImportant == 2 )
					{
						r = 255 , g = 255 , b = 0;
					}
					else if ( g_Entity.entity[i].iImportant == 3 )
					{
						r = 0 , g = 255 , b = 0;
					}

					if ( !cvar.esp_MultiColor )
					{
						if ( PathFree( g_Entity.entity[i].vOrigin ) )
						{
							r = 0 , g = 255 , b = 0;
						}
						else
						{
							r = 255 , g = 0 , b = 0;
						}
					}

					if ( cvar.esp_DrawFont )
					{
						Verdana1.Print( (int)EntScreen[0] , (int)EntScreen[1] , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
					else
					{
						Verdana1.PrintFont( (int)EntScreen[0] , (int)EntScreen[1] , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
				}
				else if ( g_Entity.entity[i].type == 2 && cvar.esp_WorldSprite )
				{
					byte r , g;

					if ( !cvar.esp_MultiColor )
					{
						if ( PathFree( g_Entity.entity[i].vOrigin ) )
						{
							r = 0 , g = 255;
						}
						else
						{
							r = 255 , g = 0;
						}
					}
					else
					{
						r = 255 , g = 255;
					}

					if ( cvar.esp_DrawFont )
					{
						Verdana1.Print( (int)EntScreen[0] , (int)EntScreen[1] , r , g , 0 , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
					else
					{
						Verdana1.PrintFont( (int)EntScreen[0] , (int)EntScreen[1] , r , g , 0 , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
				}
			}
		}
		
		g_Entity.ClearEntity();
	}
}

void cEsp::HUD_AddEntity( int type , struct cl_entity_s *ent )
{
	if ( type == ET_NORMAL && ent->curstate.solid == SOLID_BSP && cvar.wh_TransEntity < 255 )
	{
		ent->curstate.renderamt = cvar.wh_TransEntity;
		ent->curstate.rendermode = 2;
	}

	cl_entity_t *wpn = g_Engine.GetViewModel();

	if ( cvar.wh_LocalGlow )
	{
		if ( wpn )
		{
			wpn->curstate.weaponanim = 0;
			wpn->curstate.renderamt = cvar.wh_LocalGlow;
			wpn->curstate.rendercolor.r = cvar.wh_LocalGlowR;
			wpn->curstate.rendercolor.g = cvar.wh_LocalGlowG;
			wpn->curstate.rendercolor.b = cvar.wh_LocalGlowB;

			wpn->curstate.renderfx = kRenderFxGlowShell;
		}
	}
	else
	{
		wpn->curstate.rendercolor.r = 0;
		wpn->curstate.rendercolor.g = 0;
		wpn->curstate.rendercolor.b = 0;
	}

	if ( ent && !ent->player && ent->model && ent->model->name && cvar.esp_WorldWeapons )
	{
		int iImportant = 0;

		if ( native_strstr( ent->model->name , "w_" ) )
		{
			char name[64];

			int iLen = native_strlen( ent->model->name + 7 ) - 3;
			native_strncpy( name , ent->model->name + 7 , iLen );
			name[iLen] = '\0';

			if ( native_strstr( ent->model->name , "hegrenade" ) ) { iImportant = 1; }
			else if ( native_strstr( ent->model->name , "flash" ) ) { iImportant = 2; }
			else if ( native_strstr( ent->model->name , "smoke" ) ) { iImportant = 3; }

			g_Entity.AddEntity( name , iImportant , ent->origin , 1 );
		}
	}

	if ( ent && !ent->player && ent->model && ent->model->name && cvar.esp_WorldSprite )
	{
		if ( !native_strstr( ent->model->name , "w_" ) && native_strstr( ent->model->name , ".spr" ) )
		{
			char name[64];

			int iLen = native_strlen( ent->model->name + 8 ) - 3;
			native_strncpy( name , ent->model->name + 8 , iLen );
			name[iLen] = '\0';

			g_Entity.AddEntity( name , 0 , ent->origin , 2 );
		}
	}

	bool bPlayer = ( ent && ent->index != g_Local.iIndex && ent->player && g_Player[ent->index].bUpdated );

	if ( bPlayer && cvar.esp_Barrel )
	{
		int beamindex = g_Engine.pEventAPI->EV_FindModelIndex( "sprites/laserbeam.spr" );

		vec3_t vecEnd , up , right , forward , EntViewOrg;
		VectorCopy( ent->origin , EntViewOrg );

		if ( ent->curstate.usehull == 0 )
			EntViewOrg[2] += 17;
		else if ( ent->curstate.usehull == 1 )
			EntViewOrg[2] += 12;

		g_Engine.pfnAngleVectors( ent->angles , forward , right , up );
		forward[2] = -forward[2];

		vec3_t vecBegin( EntViewOrg );

		BYTE r , b;

		if ( g_Player[ent->index].iTeam == 1 )
		{
			r = 255; b = 0;
		}
		else if ( g_Player[ent->index].iTeam == 2 )
		{
			r = 0; b = 255;
		}

		vecEnd = vecBegin + forward * (float)cvar.esp_Barrel;
		g_Engine.pEfxAPI->R_BeamPoints( vecBegin , vecEnd , beamindex , 0.001f , 0.9f , 0 , 32 , 2 , 0 , 0 , r , 0 , b );
	}

	if ( bPlayer && cvar.esp_Spiked )
	{
		int beamindex = g_Engine.pEventAPI->EV_FindModelIndex( "sprites/laserbeam.spr" );
		vec3_t vecEnd , up , right , forward , EntViewOrg;
		VectorCopy( ent->origin , EntViewOrg );

		g_Engine.pfnAngleVectors( ent->angles , forward , right , up );
		forward[2] = -forward[2];
		vec3_t vecBegin( EntViewOrg );
		vecEnd = vecBegin + forward * (float)cvar.esp_Spiked;
		vecBegin = vecBegin - forward * (float)cvar.esp_Spiked;
		g_Engine.pEfxAPI->R_BeamPoints( vecBegin , vecEnd , beamindex , 0.001f , 1.2f , 0 , 32 , 2 , 0 , 0 , 255 , 0 , 0 );
		vecBegin = EntViewOrg;
		vecEnd = vecBegin + right * (float)cvar.esp_Spiked;
		vecBegin = vecBegin - right * (float)cvar.esp_Spiked;
		g_Engine.pEfxAPI->R_BeamPoints( vecBegin , vecEnd , beamindex , 0.001f , 1.2f , 0 , 32 , 2 , 0 , 0 , 0 , 255 , 0 );
		vecBegin = EntViewOrg;
		vecEnd = vecBegin + up * (float)cvar.esp_Spiked;
		vecBegin = vecBegin - up * (float)cvar.esp_Spiked;
		g_Engine.pEfxAPI->R_BeamPoints( vecBegin , vecEnd , beamindex , 0.001f , 1.2f , 0 , 32 , 2 , 0 , 0 , 0 , 0 , 255 );
	}

	if ( bPlayer && cvar.wh_ColorPlayer )
	{
		BYTE r , g , b;

		if ( g_Player[ent->index].iTeam == 1 )
		{
			r = 255; g = 64; b = 64;
		}
		else if ( g_Player[ent->index].iTeam == 2 )
		{
			r = 0; g = 164; b = 255;
		}
		if ( g_Player[ent->index].bVisible )
		{
			r = 0; g = 255; b = 0;
		}

		ent->curstate.renderamt = 30;
		ent->curstate.rendercolor.r = r;
		ent->curstate.rendercolor.g = g;
		ent->curstate.rendercolor.b = b;
		ent->curstate.renderfx = kRenderFxGlowShell;
	}

	if ( ent && !ent->player && cvar.wh_ColorWeapon )
	{
		bool visible = ( PathFree( ent->origin ) );

		if ( native_strstr( ent->model->name , "w_" ) )
		{
			BYTE r , g , b;

			if ( visible )
			{
				r = 0; g = 255; b = 0;
			}
			else
			{
				r = 255; g = 255; b = 255;
			}

			ent->curstate.renderamt = 30;
			ent->curstate.rendercolor.r = r;
			ent->curstate.rendercolor.g = g;
			ent->curstate.rendercolor.b = b;
			ent->curstate.renderfx = kRenderFxGlowShell;
		}
	}

	if ( ent && !ent->player && cvar.wh_ColorGrenade && 
		 ( native_strstr( ent->model->name , "hegrenade" )  || 
		 native_strstr( ent->model->name , "flash" ) || 
		 native_strstr( ent->model->name , "smoke" ) ) )
	{
		BYTE r , g , b;

		if ( native_strstr( ent->model->name , "hegrenade" ) )
		{
			r = 255; g = 0; b = 0;
		}
		else if ( native_strstr( ent->model->name , "flash" ) )
		{
			r = 255; g = 255; b = 0;
		}
		else if ( native_strstr( ent->model->name , "smoke" ) )
		{
			r = 0; g = 255; b = 0;
		}

		ent->curstate.renderamt = 30;
		ent->curstate.rendercolor.r = r;
		ent->curstate.rendercolor.g = g;
		ent->curstate.rendercolor.b = b;
		ent->curstate.renderfx = kRenderFxGlowShell;
	}
}