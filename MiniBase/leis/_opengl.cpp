#include "../main.h"

#pragma comment( lib, "OpenGL32.lib" )

typedef void ( APIENTRY *glBegin_t )( GLenum );
typedef void ( APIENTRY *glClear_t )( GLbitfield );
typedef void ( APIENTRY *glVertex3fv_t )( const GLfloat * );
typedef void ( APIENTRY *glVertex3f_t )( float x , float y , float z );
typedef void ( APIENTRY *glEnd_t )( void );

glBegin_t pglBegin = NULL;
glClear_t pglClear = NULL;
glVertex3fv_t pglVertex3fv = NULL;
glVertex3f_t pglVertex3f = NULL;
glEnd_t pglEnd = NULL;

#define LINE_WIDTH	1.0f
#define POLY_MAX	1024

bool bSmoke = false;
bool noSky = false;

bool bPolygon = false;
int polyCoords = 0;
float polyArray[POLY_MAX][3];

void APIENTRY Hooked_glBegin( GLenum mode )
{
	if ( !FirstFrame )
		return;

	__asm pushad
	__asm pushfd

	if ( bSnapshot || bScreenshot )
		goto end;

	cl_entity_t* ent = g_pStudio->GetCurrentEntity();

	bool ent_map = ( ent && ent->model && native_strstr( ent->model->name , ".bsp" ) );
	bool ent_mdl = ( ent && ent->model && native_strstr( ent->model->name , ".mdl" ) );

	bool gl_mdl = ( mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN );

	if ( ( cvar.rem_RemoveSky || ( cvar.wh_Wireframe && cvar.wh_Wireframe != 3 && cvar.wh_Wireframe <= 5 ) ) && mode == GL_QUADS )
	{
		if ( ent_map )
		{
			noSky = true;
		}
	}
	else
		noSky = false;

	if ( cvar.rem_RemoveSmoke && mode == GL_QUADS )
	{
		if ( ent && ent->model && native_strstr( ent->model->name , "black_smoke" ) )
		{
			GLfloat smokecol[4];
			glGetFloatv( GL_CURRENT_COLOR , smokecol );
			if ( ( smokecol[0] == smokecol[1] ) && ( smokecol[0] == smokecol[2] ) && ( smokecol[0] != 0.0 ) && ( smokecol[0] != 1.0 ) )
			{
				bSmoke = true;
			}
		}
	}
	else
	{
		bSmoke = false;
	}

	if ( cvar.wh_Wireframe == 1 && mode == GL_POLYGON && mode != GL_TRIANGLE_FAN && mode != GL_QUADS && mode != GL_TRIANGLES && mode != GL_LINES && mode != GL_POINTS && mode != GL_TRIANGLE_STRIP && mode != GL_QUAD_STRIP && mode != GL_LINE_STRIP )
	{
		glClearColor( (GLclampf)cvar.wh_WireframeR / 255.0f , (GLclampf)cvar.wh_WireframeG / 255.0f , (GLclampf)cvar.wh_WireframeB / 255.0f , 1.0f );

		glLineWidth( LINE_WIDTH );
		glColor3ub( 0 , 0 , 0 );
		glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
	}
	else if ( cvar.wh_Wireframe == 2 && mode == GL_POLYGON && mode != GL_TRIANGLE_FAN && mode != GL_QUADS && mode != GL_TRIANGLES && mode != GL_LINES && mode != GL_POINTS && mode != GL_TRIANGLE_STRIP && mode != GL_QUAD_STRIP && mode != GL_LINE_STRIP )
	{
		if ( ent_map )
			glDisable( GL_TEXTURE_2D );

		glClearColor( 0 , 0 , 0 , 255 );

		glLineWidth( LINE_WIDTH );
		glColor3ub( cvar.wh_WireframeR , cvar.wh_WireframeG , cvar.wh_WireframeB );

		glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
	}
	else if ( cvar.wh_Wireframe == 3 && mode != GL_POLYGON && gl_mdl )
	{
		if ( ent_mdl )
		{
			if ( cvar.rem_RemoveSky )
				glClearColor( 0.0 , 0.0 , 0.0 , 0.0 );

			glLineWidth( LINE_WIDTH );
			glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
		}
	}
	else if ( cvar.wh_Wireframe == 4 && ( gl_mdl || mode == GL_POLYGON ) )
	{
		glClearColor( (GLclampf)cvar.wh_WireframeR / 255.0f , (GLclampf)cvar.wh_WireframeG / 255.0f , (GLclampf)cvar.wh_WireframeB / 255.0f , 1.0f );

		glLineWidth( LINE_WIDTH );
		glColor3ub( 0 , 0 , 0 );
		glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
	}
	else if ( cvar.wh_Wireframe == 5 && ( gl_mdl || mode == GL_POLYGON ) )
	{
		if ( ent_map )
			glDisable( GL_TEXTURE_2D );

		glClearColor( 0.0 , 0.0 , 0.0 , 0.0 );
		glLineWidth( LINE_WIDTH );

		if ( ent != g_Engine.GetViewModel() )
			glColor3ub( cvar.wh_WireframeR , cvar.wh_WireframeG , cvar.wh_WireframeB );

		glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
	}
	else if ( cvar.wh_Wireframe >= 6 )
	{
		if ( cvar.wh_Wireframe == 6 && mode == GL_POLYGON && !gl_mdl )
		{
			glClearColor( 0.0 , 0.0 , 0.0 , 0.0 );
			bPolygon = true;
		}
		else if ( cvar.wh_Wireframe == 7 && mode != GL_POLYGON && gl_mdl )
		{
			bPolygon = true;	
		}
		else if ( cvar.wh_Wireframe >= 8 && ( mode == GL_POLYGON || gl_mdl ) )
		{
			glClearColor( 0.0 , 0.0 , 0.0 , 0.0 );
			bPolygon = true;
		}
		else
		{
			bPolygon = false;
		}
	}
	else if ( cvar.wh_Wireframe )
	{
		glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
	}

	polyCoords = 0;

	if ( cvar.wh_Wallmode == 1 ) // White wall
	{
		if ( mode != GL_TRIANGLES && mode != GL_TRIANGLE_STRIP  && mode != GL_TRIANGLE_FAN  && mode != GL_QUADS )
		{
			glTexEnvi( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_DECAL );
		}
	}
	else if ( cvar.wh_Wallmode == 2 ) // Night Mode
	{
		if ( mode != GL_TRIANGLES && mode != GL_TRIANGLE_STRIP && mode != GL_TRIANGLE_FAN && mode != GL_QUADS )
		{
			glTexEnvi( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_BLEND );
		}
	}
	else if ( cvar.wh_Wallmode == 3 && ent_map ) // FullBright
	{
		if ( mode != GL_TRIANGLES && mode != GL_TRIANGLE_STRIP && mode != GL_TRIANGLE_FAN && mode != GL_QUADS )
		{
			glTexEnvi( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_DECAL );
			glDisable( GL_TEXTURE_2D );
		}
	}
	else if ( cvar.wh_Wallmode >= 4 ) // Trans Model
	{
		if ( mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE , GL_ONE );
		}
	}

	if ( cvar.wh_DrawPlayer && !cvar.wh_DrawWeapon )
	{
		if ( ent && ent->model )
		{
			if ( ent->index == g_Local.iIndex )
				goto end;

			if ( ent->player && native_strstr( ent->model->name , "player" ) )
				glDepthRange( 0 , 0.5f );
			else
				glDepthRange( 0.5f , 1 );
		}
	}

	if ( cvar.wh_DrawWeapon && !cvar.wh_DrawPlayer )
	{
		if ( ent && ent->model )
		{
			if ( ent->index == g_Local.iIndex )
				goto end;

			if ( !ent->player && native_strstr( ent->model->name , "w_" ) )
				glDepthRange( 0 , 0.5f );
			else
				glDepthRange( 0.5f , 1 );
		}
	}

	if ( cvar.wh_DrawPlayer && cvar.wh_DrawWeapon )
	{
		if ( ent && ent->model )
		{
			if ( ent->index == g_Local.iIndex )
				goto end;

			if ( native_strstr( ent->model->name , "w_" ) || native_strstr( ent->model->name , "player" ) )
				glDepthRange( 0 , 0.5f );
			else
				glDepthRange( 0.5f , 1 );
		}
	}

end:

	__asm popfd
	__asm popad

	pglBegin( mode );
}

void APIENTRY Hooked_glClear( GLbitfield mask )
{
	if ( !FirstFrame )
		return;

	if ( mask == GL_DEPTH_BUFFER_BIT && cvar.wh_Wireframe || cvar.misc_FreeLook )
	{
		mask = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
	}

	if ( mask == GL_DEPTH_BUFFER_BIT && cvar.rem_RemoveSky && !cvar.wh_Wireframe )
	{
		mask = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
		glClearColor( 0.0 , 0.0 , 0.0 , 0.0 );
	}
	
	pglClear( mask );
}

void APIENTRY Hooked_glVertex3fv( GLfloat *v )
{
	if ( !FirstFrame )
		return;

	if ( v && bPolygon && polyCoords < POLY_MAX )
	{
		polyArray[polyCoords][0] = v[0];
		polyArray[polyCoords][1] = v[1];
		polyArray[polyCoords++][2] = v[2];
	}
	else polyCoords = 0;

	if ( bSmoke || noSky )
		return;

	pglVertex3fv( v );
}

void APIENTRY Hooked_glVertex3f( float x , float y , float z )
{
	if ( !FirstFrame )
		return;

	if ( x && y && z && bPolygon && polyCoords < POLY_MAX )
	{
		polyArray[polyCoords][0] = x;
		polyArray[polyCoords][1] = y;
		polyArray[polyCoords++][2] = z;
	}
	else polyCoords = 0;

	pglVertex3f( x , y , z );
}

void APIENTRY Hooked_glEnd( void )
{
	if ( !FirstFrame )
		return;

	pglEnd();

	if ( cvar.wh_Wireframe >= 6 && bPolygon )
	{
		float curcolor[4];
		glGetFloatv( GL_CURRENT_COLOR , curcolor );
		glDisable( GL_TEXTURE_2D );
		glEnable( GL_LINE_SMOOTH );
		glLineWidth( 1.0f );
		glColor4f( cvar.wh_WireframeR / 255.0f , cvar.wh_WireframeG / 255.0f , cvar.wh_WireframeB / 255.0f , 1.0f );
		pglBegin( GL_LINE_LOOP );
		for ( int i = 0; i<polyCoords; i++ )
			pglVertex3fv( polyArray[i] );
		pglEnd();
		glColor4f( curcolor[0] , curcolor[1] , curcolor[2] , 1.0f );
		glDisable( GL_LINE_SMOOTH );
		glEnable( GL_TEXTURE_2D );
	}

	bPolygon = false;
	polyCoords = 0;
	
	pglEnd();
}

void* DetourFunc( BYTE *src , const BYTE *dst , const int len )
{
	BYTE *jmp = (BYTE*)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , len + 5 );
	
	if ( src[0] != 0xE9 )
	{
		DWORD dwback;
		VirtualProtect( src , len , PAGE_READWRITE , &dwback );
		native_memcpy( jmp , src , len );
		jmp += len;
		jmp[0] = 0xE9;
		*(DWORD*)( jmp + 1 ) = (DWORD)( src + len - jmp ) - 5;
		src[0] = 0xE9;
		*(DWORD*)( src + 1 ) = (DWORD)( dst - src ) - 5;
		VirtualProtect( src , len , dwback , &dwback );
		return ( jmp - len );
	}

	return 0;
}

void HookOpenGL()
{
	HMODULE hmOpenGL = GetModuleHandleA( CFG_OPENGL32 );

	DWORD pdwglBegin = (DWORD)DetourFunc( (LPBYTE)GetProcAddress( hmOpenGL , CFG_GLBEGIN ) , (LPBYTE)&Hooked_glBegin , 6 );
	DWORD pdwglClear = (DWORD)DetourFunc( (LPBYTE)GetProcAddress( hmOpenGL , CFG_GLCLEAR ) , (LPBYTE)&Hooked_glClear , 7 );
	DWORD pdwglVertex3fv = (DWORD)DetourFunc( (LPBYTE)GetProcAddress( hmOpenGL , CFG_GLVERTEX3FV ) , (LPBYTE)&Hooked_glVertex3fv , 6 );
	DWORD pdwglVertex3f = (DWORD)DetourFunc( (LPBYTE)GetProcAddress( hmOpenGL , CFG_GLVERTEX3F ) , (LPBYTE)&Hooked_glVertex3f , 6 );
	DWORD pdwglEnd = (DWORD)DetourFunc( (LPBYTE)GetProcAddress( hmOpenGL , CFG_GLEND ) , (LPBYTE)&Hooked_glEnd , 6 );

	if ( pdwglBegin ) pglBegin = (glBegin_t)pdwglBegin;
	if ( pdwglClear ) pglClear = (glClear_t)pdwglClear;
	if ( pdwglVertex3fv ) pglVertex3fv = (glVertex3fv_t)pdwglVertex3fv;
	if ( pdwglVertex3f ) pglVertex3f = (glVertex3f_t)pdwglVertex3f;
	if ( pdwglEnd ) pglEnd = (glEnd_t)pdwglEnd;
}