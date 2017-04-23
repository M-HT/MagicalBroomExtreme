/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			ウィンドウ
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
typedef class CWINDOW : public CSPRITE
{
protected:
	unsigned long ScoreAdd;
	unsigned long TempScore;
	unsigned long GoldAdd;
	unsigned long TempGold;
	long RateAdd;
	long ForceAdd;
	long RateCount;
	bool IsAddForce;

public:
	unsigned long Gold;
	unsigned long Score;
	long Rate;
	long Force;

	// function
	CWINDOW( void );
	virtual ~CWINDOW(){}
	virtual bool Action( void );

	virtual void SetScore( unsigned long score );
	virtual void SetGold( unsigned long gold );
	virtual void AddScore( unsigned long score );
	virtual void AddGold( unsigned long gold );
	virtual void SetAddRate( long add );
	virtual void SetAddForce( long add );
	virtual long GetRate( void );
	virtual bool BootBomb( bool flag );
}
CWINDOW, *LPCWINDOW;