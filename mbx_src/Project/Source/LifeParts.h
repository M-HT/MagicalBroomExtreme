/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			���C�t�j��
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
typedef class CLIFEPARTS : public CSPRITE
{
protected:
	long Alpha;
	long Color;
	float yaw, pitch, roll;

public:

	// function
	CLIFEPARTS( float x, float y, D3DCOLOR color );
	virtual ~CLIFEPARTS(){}
	virtual bool Action( void );

	virtual void Draw( void );
}
CLIFEPARTS, *LPCLIFEPARTS;