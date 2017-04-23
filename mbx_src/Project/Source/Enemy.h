/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			“GƒLƒƒƒ‰
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
#include "Event.h"

//======================================================================================
// CLASS
//======================================================================================
typedef class CENEMY : public CSPRITE
{
protected:
	EVENTDATA EvData;
	long Attribute;
	D3DCOLOR Color;
	long AnimeCount;
	long AnimeNo;
	long LifeTimer;
	long MaxLife;
	long HitTimer;

	static bool (CENEMY::*ActionTbl[20])( void );
	static bool (CENEMY::*DeathTbl[20])( void );
	static void (CENEMY::*DamageTbl[20])( long pow );

	virtual void ActionSubZako( long no, bool shot );
	virtual bool DeathZako( long no );

	virtual bool Action000( void );
	virtual bool Death000( void );
	virtual void Damage000( long pow );
	virtual bool Action001( void );
	virtual bool Death001( void );
	virtual void Damage001( long pow );
	virtual bool Action002( void );
	virtual bool Death002( void );
	virtual void Damage002( long pow );
	virtual bool Action003( void );
	virtual bool Death003( void );
	virtual void Damage003( long pow );
	virtual bool Action004( void );
	virtual bool Death004( void );
	virtual void Damage004( long pow );
	virtual bool Action005( void );
	virtual bool Death005( void );
	virtual void Damage005( long pow );
	virtual bool Action006( void );
	virtual bool Death006( void );
	virtual void Damage006( long pow );
	virtual bool Action007( void );
	virtual bool Death007( void );
	virtual void Damage007( long pow );
	virtual bool Action008( void );
	virtual bool Death008( void );
	virtual void Damage008( long pow );
	virtual bool Action009( void );
	virtual bool Death009( void );
	virtual void Damage009( long pow );
	virtual bool Action010( void );
	virtual bool Death010( void );
	virtual void Damage010( long pow );
	virtual bool Action011( void );
	virtual bool Death011( void );
	virtual void Damage011( long pow );
	virtual bool Action012( void );
	virtual bool Death012( void );
	virtual void Damage012( long pow );
	virtual bool Action013( void );
	virtual bool Death013( void );
	virtual void Damage013( long pow );
	virtual bool Action014( void );
	virtual bool Death014( void );
	virtual void Damage014( long pow );
	virtual bool Action015( void );
	virtual bool Death015( void );
	virtual void Damage015( long pow );
	virtual bool Action016( void );
	virtual bool Death016( void );
	virtual void Damage016( long pow );
	virtual bool Action017( void );
	virtual bool Death017( void );
	virtual void Damage017( long pow );
	virtual bool Action018( void );
	virtual bool Death018( void );
	virtual void Damage018( long pow );
	virtual bool Action019( void );
	virtual bool Death019( void );
	virtual void Damage019( long pow );

public:

	// function
	CENEMY( LPEVENTDATA p, long att );
	virtual ~CENEMY(){}
	virtual bool Action( void );
	virtual void Damage( long pow );
	virtual void Draw( void );

	virtual bool Search( long att );
}
CENEMY, *LPCENEMY;