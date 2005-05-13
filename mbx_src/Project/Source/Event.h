//======================================================================================
//======================================================================================
//
//
//			ƒCƒxƒ“ƒg
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
// STRUCT
//======================================================================================
typedef struct EVENTDATA
{
	short Time;
	short Type;
	short Sx;
	short Sy;
	short Temp[2];
	long Life;
}
EVENTDATA, *LPEVENTDATA;


//======================================================================================
// CLASS
//======================================================================================
typedef class CEVENT : public CSPRITE
{
	enum { ENEMY_EVENT_MAX = 256 };

protected:
	static long Attribute;
	static long Counter;
	static long EventNo;
	static long AddCounter;
	static EVENTDATA EventData[ENEMY_EVENT_MAX];

	EVENTDATA EvData;

	static void (CEVENT::*EventTbl[20])( void );

	void Event000( void );
	void Event001( void );
	void Event002( void );
	void Event003( void );
	void Event004( void );
	void Event005( void );
	void Event006( void );
	void Event007( void );
	void Event008( void );
	void Event009( void );
	void Event010( void );
	void Event011( void );
	void Event012( void );
	void Event013( void );
	void Event014( void );
	void Event015( void );
	void Event016( void );
	void Event017( void );
	void Event018( void );
	void Event019( void );
	void Event020( void );

public:
	static void Init( void );
	static void Drive( void );
	static void SetCounter( bool flag ) { AddCounter = ((flag)?(1):(0)); }

	// function
	CEVENT( LPEVENTDATA p );
	virtual ~CEVENT(){}
	virtual bool Action( void );
}
CEVENT, *LPCEVENT;