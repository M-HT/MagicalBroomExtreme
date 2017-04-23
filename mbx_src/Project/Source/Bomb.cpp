/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			ボム
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Bomb.h"
#include "Application.h"
#include "Sonic.h"
#include "EnemyShot.h"
#include "Smoke.h"
#include "Sound.h"
#include "Voice.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CBOMB::CBOMB( void ) : CSPRITE( SORT_PLAYERSHOT )
{
	CVoice::Play( VOICE_BOMB );
	SetHitCount( 5 );
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CBOMB::Action( void )
{
	RECT dest, src;

	switch ( Phase )
	{
	case 0:		//---------------------------------------------------- 準備
		{
			CApplication::GetPlayer()->SetControl( false );
			for ( long i = 0; i < 64; i++ )
			{
				CENEMYSHOT::Create( F(RAND(0,640)), F(RAND(0,400)), 0, 0, 4 );
			}
			CSound::Play( SE_MAGIC );
			Phase++;
		}
		break;
	case 1:		//---------------------------------------------------- 魔方陣出現
		{
			CENEMYSHOT::Create( F(RAND(0,640)), F(RAND(0,400)), 0, 0, 4 );
			Angle += 32;
			Angle &= 4095;
			x1 = LunaMath::SubSpeed( 400.0f, 200.0f, 50, Time );
			x2 = LunaMath::SubSpeed( 000.0f, 200.0f, 50, Time );
			long alpha = LunaMath::FlatSpeed( 0L, 128L, 50, Time );
			SetRect( &src, 1024-256, 1024-256, 1024, 1024 );
			SetRect( &dest, (long)(320-x1), (long)(200-x1), (long)(320+x1), (long)(200+x1) );
			Sprite[7][1]->DrawRotate( &dest, D3DCOLOR_ARGB(alpha,255,255,255), +ANGLE2PAI(Angle), &src );
			SetRect( &dest, (long)(320-x2), (long)(200-x2), (long)(320+x2), (long)(200+x2) );
			Sprite[7][1]->DrawRotate( &dest, D3DCOLOR_ARGB(alpha,255,255,255), -ANGLE2PAI(Angle), &src );
			if ( ++Time > 40 )
			{
				CSound::Play( SE_EXPLOSION );
				for ( long i = 0; i < 4; i++ )
				{
					new CSONIC( 320, 200, 800, 60, false, 0, D3DCOLOR_XRGB(255,255,255) );
					new CSONIC( 320, 200, 500, 25, true, 1, D3DCOLOR_XRGB(RAND(128,255),RAND(128,255),RAND(128,255)) );
					new CSONIC( 320, 200, 700, 25, true, 1, D3DCOLOR_XRGB(RAND(128,255),RAND(128,255),RAND(128,255)) );
					new CSONIC( 320, 200, 900, 25, true, 1, D3DCOLOR_XRGB(RAND(128,255),RAND(128,255),RAND(128,255)) );
				}
				Power = 50;
				bHit = true;
				lpHitPt[0].x = 0;	lpHitPt[0].y = 0;
				lpHitPt[1].x = 640;	lpHitPt[1].y = 0;
				lpHitPt[2].x = 640;	lpHitPt[2].y = 400;
				lpHitPt[3].x = 0;	lpHitPt[3].y = 400;
				lpHitPt[4] = lpHitPt[0];
				Time = 0;
				Phase++;
			}
		}
		break;
	case 2:		//---------------------------------------------------- 回転
		{
			new CSMOKE( F(RAND(0,640)), F(RAND(0,400)), RAND(0,4095), RAND(0,15)/10.0f, RAND(50,80), 40, 2 );

			Angle += 32;
			Angle &= 4095;
			SetRect( &src, 1024-256, 1024-256, 1024, 1024 );
			SetRect( &dest, (long)(320-200), (long)(200-200), (long)(320+200), (long)(200+200) );
			Sprite[7][1]->DrawRotate( &dest, D3DCOLOR_ARGB(255,255,255,255), ANGLE2PAI(Angle), &src );
			if ( ++Time > 15 )
			{
				new CSMOKE( 320, 200, RAND(0,4095), RAND(0,15)/10.0f, 200, 80, 1 );
				new CSMOKE( 320, 200, RAND(0,4095), RAND(0,15)/10.0f, 200, 40, 0 );
				new CSMOKE( 320, 200, RAND(0,4095), RAND(0,15)/10.0f, 200, 40, 2 );
				Time = 0;
				Phase++;
			}
		}
		break;
	case 3:		//---------------------------------------------------- 回転
		{
			Angle += 32;
			Angle &= 4095;
			x1 = LunaMath::SubSpeed( 200.0f, 400.0f, 60, Time );
			x2 = LunaMath::SubSpeed( 200.0f, 000.0f, 60, Time );
			long alpha = LunaMath::FlatSpeed( 128L, 0L, 60, Time );
			SetRect( &src, 1024-256, 1024-256, 1024, 1024 );
			SetRect( &dest, (long)(320-x1), (long)(200-x1), (long)(320+x1), (long)(200+x1) );
			Sprite[7][1]->DrawRotate( &dest, D3DCOLOR_ARGB(alpha,255,255,255), +ANGLE2PAI(Angle), &src );
			SetRect( &dest, (long)(320-x2), (long)(200-x2), (long)(320+x2), (long)(200+x2) );
			Sprite[7][1]->DrawRotate( &dest, D3DCOLOR_ARGB(alpha,255,255,255), -ANGLE2PAI(Angle), &src );
			if ( ++Time > 60 )
			{
				bHit = false;
				Time = 0;
				Phase++;
			}
		}
		break;
	case 4:	//---------------------------------------------------- 後始末
		{
			CApplication::GetPlayer()->SetControl( true );
			CApplication::BootBomb( false );
			bDeath = true;
		}
		break;
	}

	return bDeath;
}
