#include "utils.h"

char native_itoa_buffer[INT_DIGITS + 2];

PUserMsg UserMsgByName( char* szMsgName )
{
	PUserMsg Ptr = nullptr;
	Ptr = pUserMsgBase;

	while ( Ptr->next )
	{
		if ( !native_strcmp( Ptr->name , szMsgName ) )
			return Ptr;

		Ptr = Ptr->next;
	}

	Ptr->pfn = 0;
	return Ptr;
}

PEngineMsg EngineMsgByName( char* szMsgName )
{
	PEngineMsg Ptr = nullptr;
	Ptr = pEngineMsgBase;

	while ( native_strcmp( Ptr->name , OFF_SVC_MESSAGES_P ) )
	{
		if ( !native_strcmp( Ptr->name , szMsgName ) )
			return Ptr;

		Ptr++;
	}

	Ptr->pfn = 0;
	return Ptr;
}

pcmd_t CommandByName( char* szName )
{
	pcmd_t pCmd = nullptr;
	pCmd = g_Engine.pfnGetCmdList();

	while ( pCmd )
	{
		if ( !native_strcmp( pCmd->name , szName ) )
			return pCmd;

		pCmd = pCmd->next;
	}

	return 0;
}

pfnUserMsgHook HookUserMsg( char *szMsgName , pfnUserMsgHook pfn )
{
	PUserMsg Ptr = nullptr;
	pfnUserMsgHook Original = nullptr;

	Ptr = UserMsgByName( szMsgName );

	if ( Ptr->pfn != 0 )
	{
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
	}
	else
	{
		MessageBoxA( 0 , szMsgName , 0 , MB_OK | MB_ICONERROR );
	}

	return Original;
}

pfnEngineMessage HookEngineMsg( char *szMsgName , pfnEngineMessage pfn )
{
	PEngineMsg Ptr = nullptr;
	pfnEngineMessage Original = nullptr;

	Ptr = EngineMsgByName( szMsgName );

	if ( Ptr->pfn != 0 )
	{
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
	}
	else
	{
		MessageBoxA( 0 , szMsgName , 0 , MB_OK | MB_ICONERROR );
	}

	return Original;
}

char* native_strcpy( char *dest , const char *src )
{
	return lstrcpyA( dest , src );
}

char* native_strncpy( char *dest , const char *src , int n )
{
	return lstrcpynA( dest , src , n );
}

char* native_strcat( char *dest , const char *src )
{
	return lstrcatA( dest , src );
}

char* native_strstr( char *in , char *str )
{
	char c;
	size_t len;
	c = *str++;
	if ( !c )
		return (char *)in;
	len = native_strlen( str );
	do
	{
		char sc;

		do
		{
			sc = *in++;
			if ( !sc )
				return (char *)0;
		} while ( sc != c );
	} while ( native_strncmp( in , str , len ) != 0 );
	return (char *)( in - 1 );
}

int native_strcmp( char *pStr1 , char *pStr2 )
{
	return lstrcmpA( pStr1 , pStr2 );
}

int native_strlen( char *pStr )
{
	return lstrlenA( pStr );
}

int native_strncmp( char const* _Str1 , char const* _Str2 , size_t MaxCount )
{
	if ( !MaxCount ) return 0;
	while ( --MaxCount && *_Str1 && *_Str1 == *_Str2 )
	{
		_Str1++;
		_Str2++;
	}
	return *(unsigned char *)_Str1 - *(unsigned char *)_Str2;
}

void native_memcpy( void * dst , const void * src , size_t count )
{
	_asm
	{
		mov	edi , [dst]
		mov	esi , [src]
		mov	ecx , [count]
		rep	movsb
	}
}

void native_memset( void *szBuffer , DWORD dwLen , DWORD dwSym )
{
	_asm
	{
		pushad
		mov edi , [szBuffer]
		mov ecx , [dwLen]
		mov eax , [dwSym]
		rep stosb
		popad
	}
}

PCHAR native_itoa( int i )
{
	char *p = native_itoa_buffer + INT_DIGITS + 1;
	if ( i >= 0 )
	{
		do
		{
			*--p = '0' + ( i % 10 );
			i /= 10;
		} while ( i != 0 );
		return p;
	}
	else
	{
		do
		{
			*--p = '0' - ( i % 10 );
			i /= 10;
		} while ( i != 0 );
		*--p = '-';
	}
	return p;
}