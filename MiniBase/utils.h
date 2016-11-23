#pragma once

#include "main.h"

#define HW_DLL "hw.dll"
#define SW_DLL "sw.dll"

#define CLIENT_DLL "client.dll"
#define GAMEUI_DLL "GameUI.dll"

#define ERROR_FIND "Find Cl/En/St Modules Error"
#define OFF_ERROR "Error Find"

#define OFF_CLIENT_PATTERN "ScreenFade"

#define OFF_ENGINE_PATTERN	"\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF"
#define OFF_ENGINE_MASK "xx????x????xx????xx????x????xx????xx????x????xx????xx????x????xx????xx????x????xx????xx????x????xx????xx????x????xx????xx????"

#define OFF_SVC_MSG_PATTERN "\xBF\xFF\xFF\xFF\xFF\x8B\x04\xB5\xFF\xFF\xFF\xFF\x85\xC0\x74\xFF\x81\xFF\xFF\xFF\xFF\xFF\x7F\x04\x8B\x0F\xEB\x05"
#define OFF_SVC_MSG_MASK "x????xxx????xxx?xx????xxxxxx"

#define OFF_MSG_USER_MASK1 "xxx????x"
#define OFF_MSG_USER_MASK2 "???xxxx????x"

#define OFF_USER_MSG_EROR1 "Couldn't find UserMsgBase #1 pointer."
#define OFF_USER_MSG_EROR2 "Couldn't find UserMsgBase #2 pointer."

#define OFF_MSG_READ_CORD "MSG_ReadCoord"
#define OFF_MSG_STR_READING "MSG_StartBitReading"
#define OFF_MSG_END_READING "MSG_EndBitReading"
#define OFF_ENGINE_MSG_BASE "EngineMsgBase"
#define OFF_SVC_MESSAGES_P "End of List"

#define RESET_HUD_MSG "ResetHUD"
#define SET_FOV_MSG "SetFOV"
#define TEAM_INFO_MSG "TeamInfo"
#define CUR_WEAPON_MSG "CurWeapon"
#define DEATH_MSG_MSG "DeathMsg"
#define SVC_SOUND_MSG "svc_sound"
#define SVC_SPAWNSTATIC_SOUND_MSG "svc_spawnstaticsound"

#define TERRORIST_UMSG "TERRORIST"
#define CT_UMSG "CT"

#define INT_DIGITS 19

extern char native_itoa_buffer[INT_DIGITS + 2];

PUserMsg UserMsgByName( char* szMsgName );
PEngineMsg EngineMsgByName( char* szMsgName );
pcmd_t CommandByName( char* szName );

pfnUserMsgHook HookUserMsg( char *szMsgName , pfnUserMsgHook pfn );
pfnEngineMessage HookEngineMsg( char *szMsgName , pfnEngineMessage pfn );

char* native_strcpy( char *dest , const char *src );
char* native_strncpy( char *dest , const char *src , int n );
char* native_strcat( char *dest , const char *src );
char* native_strstr( char *in , char *str );

int native_strcmp( char *pStr1 , char *pStr2 );
int native_strlen( char *pStr );
int native_strncmp( char const* _Str1 , char const* _Str2 , size_t MaxCount );

void native_memcpy( void * dst , const void * src , size_t count );
void native_memset( void *szBuffer , DWORD dwLen , DWORD dwSym );

PCHAR native_itoa( int i );