//======================================================================================
//======================================================================================
//
//
//			ÉvÉåÉCÉÑÅ[
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
typedef class CPLAYER : public CSPRITE
{
protected:
	bool bControl;

	long AnimeNo;
	long AnimeCount;
	float Speed;

	long ShotLevel;
	long BitLevel;
	long ShotCount;
	long ShotAngle;
	long FairyNum;

	long NoDamageTimer;
	long HitTimer;

	long Charge;

public:

	// function
	CPLAYER( void );
	virtual ~CPLAYER(){}
	virtual bool Action( void );
	virtual void Draw( void );
	virtual void Damage( long pow );

	virtual void SpeedUp( void );
	virtual void ShotLevelUp( void );
	virtual void BitLevelUp( void );
	virtual void LifeUp( void );
	virtual void FairyUp( void );
	virtual void SetHit( bool flag ) { bHit = flag; }
	virtual void SetControl( bool flag ) { bControl = flag; }
	virtual void GetShotLevel( long *shot, long *bit ) { *shot = ShotLevel; *bit = BitLevel; }
}
CPLAYER, *LPCPLAYER;