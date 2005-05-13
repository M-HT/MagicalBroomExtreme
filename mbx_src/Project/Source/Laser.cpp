//======================================================================================
//======================================================================================
//
//
//			ライフ破片
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Laser.h"
#include "Application.h"
#include "EnemyFx.h"
#include "Player.h"
#include "Sound.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CLASER::CLASER( float x, float y, float w, long angle ) : CSPRITE( SORT_PLAYERLASER )
{
	SetHitCount( LASER_DIV );
	bHit = true;
	Speed = 15;

	CSound::Play( SE_LASER );

	long shot, bit;
	CApplication::GetPlayer()->GetShotLevel( &shot, &bit );
	Power = ( (shot+1) + (bit+1) ) * 2;

	// レーザー初期化
	for ( long i = 0; i < LASER_DIV; i++ )
	{
		Lx[i] = x;
		Ly[i] = y;
		Lw[i] = w;
		Langle[i] = angle;
		Lcolor[i] = D3DCOLOR_ARGB(255,255,255,255);
	}
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CLASER::Action( void )
{
	//===============================================
	// 古い座標コピー
	for ( long i = LASER_DIV-2; i >= 0; i-- )
	{
		Lx[i+1]		= Lx[i];
		Ly[i+1]		= Ly[i];
		Langle[i+1]	= Langle[i];
	}

	switch ( Phase )
	{
	case 0:
		{
			//===============================================
			// 座標計算

			// 適当な敵検索
			ReferenceNear( &x4, &y4, SORT_ENEMY );

			// 目標との角度算出
			long a = LunaMath::Atan( (long)(x4-Lx[0]), (long)(y4-Ly[0]) );

			// 現在の角度との相対角度算出
			a = Langle[0] - a;
			a &= 4095;

			// 角度に応じた処理
			Langle[0] += ((a > 2048)?(+80):(-80));
			Langle[0] &= 4095;

			Lx[0] += LunaMath::Cos( (long)Langle[0], (long)Speed );
			Ly[0] += LunaMath::Sin( (long)Langle[0], (long)Speed );

			if ( ++Time > 75 ) Phase++;
		}
		break;
	case 1:
		{
			Lx[0] += LunaMath::Cos( (long)Langle[0], (long)Speed );
			Ly[0] += LunaMath::Sin( (long)Langle[0], (long)Speed );
			if ( ++Time > 150 ) bDeath = true;
		}
		break;
	}

	// 描画
	RECT uv = { 704, 0, 704+32, 360 };
	Sprite[6][1]->DrawLaser( Lx, Ly, Lw, Langle, Lcolor, &uv );

	// 当たり判定
	for ( long i = 0; i < LASER_DIV; i++ )
	{
		lpHitPt[i].x = Lx[i];
		lpHitPt[i].y = Ly[i];
	}

	return bDeath;
}
