/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			�r�b�g�c��
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
typedef class CBITBLUR : public CSPRITE
{
public:

	// function
	CBITBLUR( float x, float y );
	virtual ~CBITBLUR(){}
	virtual bool Action( void );
}
CBITBLUR, *LPCBITBLUR;