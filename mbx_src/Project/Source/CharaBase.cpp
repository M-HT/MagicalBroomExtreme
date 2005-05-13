//======================================================================================
//======================================================================================
//
//
//			キャラクター基底
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "CharaBase.h"
#include "List.h"
#include "Application.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CSPRITE::CSPRITE( eCharSort sort )
{
	bHit = false;
	bDeath = false;
	Sort = sort;
	Px = Py = 0;
	x1 = x2 = x3 = x4 = 0;
	y1 = y2 = y3 = y4 = 0;
	Angle = 0;
	Scale = 0;
	Speed = 0;
	Phase = 0;
	Time = 0;
	Count = 0;
	Power = 0;
	Life = 0;
	HitCount = 0;
	lpHitPt = NULL;

	// 処理リストに追加
	SpriteList.InsertEnd( this );
}

//======================================================================================
//
//	デストラクタ
//
//======================================================================================
CSPRITE::~CSPRITE()
{
	SpriteList.Erase( this );
}

//======================================================================================
//
//	あたり判定データ生成
//
//======================================================================================
void CSPRITE::SetHitCount( long num )
{
	if ( lpHitPt != NULL ) delete [] lpHitPt;
	// あたり判定用データ
	HitCount = num;
	lpHitPt = new POINT2D [ num ];
	memset( lpHitPt, 0x00, sizeof(POINT2D) * num );
}

//======================================================================================
//
//	適当なスプライト検索
//
//======================================================================================
void CSPRITE::Reference( float *x, float *y, eCharSort sort )
{
	LPCSPRITE pChar = CApplication::GetPlayer();

	LPCSPRITE p = (LPCSPRITE)SpriteList.Top();
	while ( p != NULL )
	{
		if ( p->Sort == sort )
		{
			if ( p->bHit )
			{
				pChar = p;
				break;
			}
		}
		p = (LPCSPRITE)p->Next;
	}

	*x = pChar->Px;
	*y = pChar->Py;
}

//======================================================================================
//
//	一番近いスプライト検索
//
//======================================================================================
void CSPRITE::ReferenceNear( float *x, float *y, eCharSort sort )
{
	LPCSPRITE near_p = CApplication::GetPlayer();
	long MinLen = 0x0FFFFFFF;

	long px = (long)Px;
	long py = (long)Py;

	LPCSPRITE p = (LPCSPRITE)SpriteList.Top();
	while ( p != NULL )
	{
		if ( p->Sort == sort )
		{
			if ( p->bHit )
			{
				long cx = (long)p->Px;
				long cy = (long)p->Py;
				long len = (px-cx)*(px-cx) + (py-cy)*(py-cy);
				if ( len < MinLen )
				{
					near_p = p;
					MinLen = len;
				}
			}
		}
		p = (LPCSPRITE)p->Next;
	}

	*x = near_p->Px;
	*y = near_p->Py;
}
