/* encoding = CP932 */
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
#include "BossLaser.h"
#include "Application.h"
#include "Sound.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CBOSSLASER::CBOSSLASER( float x, float y, long angle ) : CSPRITE( SORT_ENEMYSHOT )
{
	SetHitCount( 5 );

	CSound::Play( SE_BOSS_LASER );

	const float spd[] = { 8, 9, 10 };

	Speed = spd[CApplication::GetLevel()];
	bHit = true;
	for ( long i = 0; i < LASER_DIV; i++ )
	{
		Lx[i] = x;
		Ly[i] = y;
		Lw[i] = 10;
		Langle[i] = angle;
		Lcolor[i] = D3DCOLOR_ARGB(255,255,255,255);
	}
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CBOSSLASER::Action( void )
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

			x4 = CApplication::GetPlayer()->Px + 43;
			y4 = CApplication::GetPlayer()->Py + 35;

			// 目標との角度算出
			long a = LunaMath::Atan( (long)(x4-Lx[0]), (long)(y4-Ly[0]) );

			// 現在の角度との相対角度算出
			a = Langle[0] - a;
			a &= 4095;

			// 角度に応じた処理
			if ( a > 2048 )	{ Langle[0] += 80; }
			else			{ Langle[0] -= 80; }
			Langle[0] &= 4095;

			Lx[0] += LunaMath::Cos( (long)Langle[0], (long)Speed );
			Ly[0] += LunaMath::Sin( (long)Langle[0], (long)Speed );

			if ( ++Time > 30 ) Phase++;
		}
		break;
	case 1:
		{
			Lx[0] += LunaMath::Cos( (long)Langle[0], (long)Speed );
			Ly[0] += LunaMath::Sin( (long)Langle[0], (long)Speed );
			Speed += 0.5f;
			if ( ++Time > 100 ) bDeath = true;
		}
		break;
	}

	// 当たり判定
	float xx, yy;
	xx = 0;
	yy = Lw[0];
	LunaMath::RotatePoint( &xx, &yy, Langle[0] );
	lpHitPt[0].x = Lx[0]-xx;	lpHitPt[0].y = Ly[0]-yy;
	lpHitPt[1].x = Lx[0]+xx;	lpHitPt[1].y = Ly[0]+yy;
	xx = 0;
	yy = Lw[1];
	LunaMath::RotatePoint( &xx, &yy, Langle[1] );
	lpHitPt[2].x = Lx[1]+xx;	lpHitPt[2].y = Ly[1]+yy;
	lpHitPt[3].x = Lx[1]-xx;	lpHitPt[3].y = Ly[1]-yy;

	lpHitPt[4] = lpHitPt[0];

	RECT uv[] = {
		{ 640, 0, 640+32, 360 },
		{ 672, 0, 672+32, 360 },
	};
	Sprite[6][1]->DrawLaser( Lx, Ly, Lw, Langle, Lcolor, &uv[Time&0x01] );

	return bDeath;
}
