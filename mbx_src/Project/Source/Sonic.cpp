/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			衝撃波
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Sonic.h"
#include "Application.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CSONIC::CSONIC( float x, float y, long size, long time, bool rot, long no, D3DCOLOR color ) : CSPRITE( SORT_NULL )
{
	Px = x;
	Py = y;
	EndSize = size;
	EndTime = time;
	Color = color & 0x00FFFFFF;
	Alpha = 0;
	if ( rot )
	{
		yaw = RAND_PAI;
		pitch = RAND_PAI;
		roll = RAND_PAI;
	}
	else
	{
		yaw = pitch = roll = 0.0f;
	}
	No = no;
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CSONIC::Action( void )
{
	Alpha = LunaMath::FlatSpeed( 255L, 0L, EndTime, Time );
	Scale = LunaMath::FlatSpeed( 0L, EndSize, EndTime, Time );
	if ( ++Time > EndTime ) bDeath = true;

	if ( No == 0 )
	{
		CApplication::AddEffect( this );
	}
	else
	{
		RECT src = { 576, 630, 704, 758 };
		RECT dest = { (long)Px-Scale, (long)Py-Scale, (long)Px+Scale, (long)Py+Scale };
		Sprite[5][1]->DrawRotate( &dest, Color|(Alpha<<24), yaw, pitch, roll, &src );
	}

	return bDeath;
}

//======================================================================================
//
//	描画
//
//======================================================================================
void CSONIC::Draw( void )
{
	LunaEffect::SonicRotate( (long)Px, (long)Py, Scale, 0.5f, yaw, pitch, roll, Color|(Alpha<<24) );
}