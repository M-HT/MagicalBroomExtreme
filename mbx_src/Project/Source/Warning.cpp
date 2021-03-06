/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			警告
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Warning.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CWARNING::CWARNING( float x, float y ) : CSPRITE( SORT_NULL )
{
	Px = x;
	Py = y;
	Alpha = 0;
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CWARNING::Action( void )
{
	long sx = 32, sy = 32;

	switch ( Phase )
	{
	// -------------------------------------------------------- 縮小しながら参上
	case 0:
		Alpha = LunaMath::FlatSpeed(   0L, 255L, 25, Time );
		sx = LunaMath::FlatSpeed( 128L, 32L, 25, Time );
		sy = LunaMath::FlatSpeed( 128L, 32L, 25, Time );
		if ( ++Time == 25 )
		{
			Time = 0;
			Phase++;
		}
		break;
	// -------------------------------------------------------- 一服
	case 1:
		if ( ++Time == 50 )
		{
			Time = 0;
			Phase++;
		}
		break;
	// -------------------------------------------------------- 縦に伸びる
	case 2:
		sx = (long)LunaMath::Neville( 32.0f, 24.0f, 100.0f, F(Time)/25.0f );
		sy = (long)LunaMath::Neville( 32.0f, 40.0f,   1.0f, F(Time)/25.0f );
		if ( ++Time == 25 )
		{
			Time = 0;
			Phase++;
		}
		break;
	// -------------------------------------------------------- 横に縮まる
	case 3:
		sy = 1;
		sx = LunaMath::FlatSpeed( 100L, 0L, 10, Time );
		if ( ++Time == 10 )
		{
			Time = 0;
			return true;
		}
		break;
	}

	// 描画
	RECT dest = { (long)Px-sx, (long)Py-sy, (long)Px+sx, (long)Py+sy };
	RECT src = { 704, 816, 768, 880 };
	Sprite[9][0]->Draw( &dest, D3DCOLOR_ARGB(Alpha,255,255,255), &src );

	return false;
}