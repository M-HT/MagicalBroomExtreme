//======================================================================================
//======================================================================================
//
//
//			キャラクタ基底
//
//
//======================================================================================
//======================================================================================

#pragma once

//======================================================================================
// INCLUDE
//======================================================================================
#include "Base.h"
#include "Luna.h"


//======================================================================================
// キャラ種類
//======================================================================================
enum eCharSort
{
	SORT_NULL,
	SORT_EVENT,
	SORT_PLAYER,
	SORT_PLAYERSHOT,
	SORT_PLAYERLASER,
	SORT_ENEMY,
	SORT_ENEMYSHOT,
	SORT_COIN,
	SORT_ITEM,
	sizeof_eSort
};


//======================================================================================
// CLASS
//======================================================================================
typedef class CSPRITE : public CListBase
{
public:
	bool bHit;
	bool bDeath;					// 死亡フラグ
	eCharSort Sort;					// 種類

	long HitCount;					// 当たり判定数
	POINT2D *lpHitPt;				// 当たり判定位置

	float Px, Py;					// 位置情報
	float x1, x2, x3, x4;			// 汎用
	float y1, y2, y3, y4;			// 汎用
	float Speed;					// 速度
	long Angle;						// 角度
	long Scale;						// 拡大率

	long Life;						// ライフ
	long Power;						// パワー

	long Phase;						// フェーズ管理用
	long Time, Count;				// 時間管理用

	// function
	CSPRITE( eCharSort sort );
	virtual ~CSPRITE();
	virtual bool Action( void ) = 0;
	virtual void Draw( void ){}
	virtual void Damage( long pow ){}

	virtual void SetDeath( void ) { bDeath = true; Phase = 0; }
	virtual void SetHitCount( long num );
	virtual void Reference( float *x, float *y, eCharSort sort );
	virtual void ReferenceNear( float *x, float *y, eCharSort sort );
}
CSPRITE, *LPCSPRITE;