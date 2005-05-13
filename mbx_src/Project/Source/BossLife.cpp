//======================================================================================
//======================================================================================
//
//
//			タイトル
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "BossLife.h"
#include "Application.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CBOSSLIFE::CBOSSLIFE( long max, LPCSPRITE p ) : CSPRITE( SORT_NULL )
{
	Alpha = 0;
	MaxLife = max;
	lpParent = p;
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CBOSSLIFE::Action( void )
{
	if ( bDeath )
	{
		switch ( Phase )
		{
		case 0:
			Time = 0;
			Phase++;
			break;
		case 1:
			Alpha = LunaMath::FlatSpeed( 255L, 0L, 50, Time );
			if ( ++Time > 50 ) return true;
		}
	}
	else
	{
		if ( lpParent->bDeath ) bDeath = true;

		switch ( Phase )
		{
		case 0:
			Alpha = LunaMath::FlatSpeed( 0L, 255L, 50, Time );
			if ( ++Time > 50 )
			{
				Time = 0;
				Phase++;
			}
			break;
		}
	}

	// メーター
	CApplication::AddEffect( this );

	return false;
}

//======================================================================================
//
//	描画
//
//======================================================================================
void CBOSSLIFE::Draw( void )
{
	Life = lpParent->Life;
	if ( Life < 0 ) Life = 0;

	long w = LunaMath::FlatSpeed( 50L, 590L, MaxLife, Life );

	long r = LunaMath::FlatSpeed( 255L, 128L, MaxLife, Life );
	long b = LunaMath::FlatSpeed( 128L, 255L, MaxLife, Life );
	D3DCOLOR color = D3DCOLOR_ARGB(Alpha,r,128,b);

	RECT dest;
	SetRect( &dest, 48, 373, 592, 387 );
	LunaEffect::FillSquare( &dest, D3DCOLOR_ARGB(Alpha,255,255,255) );
	SetRect( &dest, 50, 375, w, 385 );
	LunaEffect::FillSquare( &dest, color );
}