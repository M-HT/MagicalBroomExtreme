/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			煙
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Smoke.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CSMOKE::CSMOKE( float x, float y, long angle, float speed, long size, long time, long no ) : CSPRITE( SORT_NULL )
{
	Px = x;
	Py = y;
	x1 = F(LunaMath::Cos( angle )) / 65536.0f * speed;
	y1 = F(LunaMath::Sin( angle )) / 65536.0f * speed;
	No = no;
	Scale = size;
	Angle = RAND( 0, 4095 );
	EndTime = time;
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CSMOKE::Action( void )
{
	long w = LunaMath::FlatSpeed( Scale, Scale<<1, EndTime, Time );
	long a = LunaMath::FlatSpeed( 255L, 0L, EndTime, Time );

	if ( ++Time > EndTime ) return true;

	Px += x1;
	Py += y1;

	RECT src, dest = { (long)Px-w, (long)Py-w, (long)Px+w, (long)Py+w };

	switch ( No  )
	{
	case 0:
		SetRect( &src, 705, 881, 767, 943 );
		Sprite[5][1]->DrawRotate( &dest, D3DCOLOR_ARGB(a,255,255,255), ANGLE2PAI(Angle), &src );
		break;
	case 1:
		SetRect( &src, 705, 881, 767, 943 );
		Sprite[5][0]->DrawRotate( &dest, D3DCOLOR_ARGB(a,255,255,255), ANGLE2PAI(Angle), &src );
		break;
	case 2:
		SetRect( &src, 705, 753, 767, 815 );
		Sprite[5][1]->DrawRotate( &dest, D3DCOLOR_ARGB(a,255,255,255), ANGLE2PAI(Angle), &src );
		break;
	}

	return false;
}
