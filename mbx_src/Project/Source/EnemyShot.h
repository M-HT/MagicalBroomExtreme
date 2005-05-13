//======================================================================================
//======================================================================================
//
//
//			“GƒVƒ‡ƒbƒg
//
//
//======================================================================================
//======================================================================================

#pragma once

//======================================================================================
// INCLUDE
//======================================================================================
#include "Base.h"
#include "CharaBase.h"

//======================================================================================
// CLASS
//======================================================================================
typedef class CENEMYSHOT : public CSPRITE
{
protected:
	static long ShotCounter;
	long No;
	D3DCOLOR Color;
	LPCSPRITE lpParent;

public:
	// function
	CENEMYSHOT( float x, float y, float speed, long angle, long no, LPCSPRITE p = NULL );
	virtual ~CENEMYSHOT();	
	virtual bool Action( void );

	virtual void Damage( long pow );

	static void Create( float x, float y, float speed, long angle, long no, LPCSPRITE p = NULL );
}
CENEMYSHOT, *LPCENEMYSHOT;