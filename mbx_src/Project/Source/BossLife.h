/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			�^�C�g��
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
typedef class CBOSSLIFE : public CSPRITE
{
protected:
	long MaxLife;
	long Alpha;
	LPCSPRITE lpParent;

public:

	// function
	CBOSSLIFE( long max, LPCSPRITE p );
	virtual ~CBOSSLIFE(){}
	virtual bool Action( void );
	virtual void Draw( void );
}
CBOSSLIFE, *LPCBOSSLIFE;