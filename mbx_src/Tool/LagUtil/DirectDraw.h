//==========================================================================
//
//
//		DirectDraw ファイル関連
//
//
//==========================================================================

#pragma once


//==========================================================================
// INCLUDE
//==========================================================================
#include "Lag.h"


//==========================================================================
// DEFINE
//==========================================================================
#define SAFE_RELEASE( p ) if ( (p) != NULL ) { (p)->Release(); (p) = NULL; }


//==========================================================================
// ENUM
//==========================================================================
enum eFormat
{
	FMT_GRAPHIC,
	FMT_ALPHA,
};


//==========================================================================
// PROTOTYPE
//==========================================================================
bool InitDirectX( HWND hWnd );
void UninitDirectX( void );
bool Refresh( HWND hWnd, long dx, long dy );
bool LoadBMP( char *file, eFormat fmt );
void GetGraphicSize( long *w, long *h );
void GetColor( long *r, long *g, long *b, long x, long y );
void SetColorKey( long r, long g, long b );
void RestoreColorKey( void );
PIXELDATA32 *LockSurface( long *pitch );
void UnlockSurface( void );
void GraphciLeftRight( void );
void GraphciUpDown( void );
void GraphciReverse( void );
void BiLinearFilter( float sx, float sy );