/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			敵ショット
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "EnemyShot.h"
#include "Application.h"
#include "Sound.h"


long CENEMYSHOT::ShotCounter;

//======================================================================================
//
//	弾管理
//
//		no について
//
//			0 : 通常弾（設定した方向へ設定した速度で移動。画面外で消滅。
//			1 : 誘導弾（自機に向かって進行、画面外で消滅。
//			2 : 大型弾（設定した方向へ設定した速度で移動。画面外で消滅。
//			3 : 大型誘導弾（時期に向かって進行、画面外で消滅
//			4 : レーザー集積
//
//======================================================================================
void CENEMYSHOT::Create( float x, float y, float speed, long angle, long no, LPCSPRITE p )
{
	// 制限つきの弾の時
	if ( no == 0 || no == 1 || no == 2 )
	{
		// 自機との距離が近すぎる場合は無効
		float px = (CApplication::GetPlayer()->Px+45) - x;
		float py = (CApplication::GetPlayer()->Py+25) - y;
		if ( ((px*px) + (py*py)) < 120*120 ) return;

		// 画面外からの攻撃は無効
		if ( x < 0 || y < 0 || x > 640 || y > 400 ) return;
	}


	// 画面上に出現できる弾の最大数
	long RateLevel = CApplication::GetRate() / 1000 / 3;
	long max = RateLevel + CApplication::GetLevel();
	max++;

	// 弾数制限
	if ( no == 0 || no == 1 )
	{
		if ( ShotCounter > max ) return;
	}

	// 弾生成
	new CENEMYSHOT( x, y, speed, angle, no, p );
}

//======================================================================================
//
//	コンストラクタ
//
//		no について
//
//			0 : 通常弾（設定した方向へ設定した速度で移動。画面外で消滅。
//			1 : 誘導弾（自機に向かって進行、画面外で消滅。
//			2 : 大型弾（設定した方向へ設定した速度で移動。画面外で消滅。
//			3 : 大型誘導弾（時期に向かって進行、画面外で消滅
//			4 : レーザー集積
//
//======================================================================================
CENEMYSHOT::CENEMYSHOT( float x, float y, float speed, long angle, long no, LPCSPRITE p ) : CSPRITE( SORT_ENEMYSHOT )
{
	No = no;
	Px = x;
	Py = y;
	lpParent = p;
	Speed = speed;
	Angle = angle;
	Color = D3DCOLOR_XRGB(255,255,255);
	if ( lpParent != NULL ) lpParent->AddRef();

	switch ( no )
	{
	//------------------------------------------------------------ 通常弾
	case 0:
		{
			bHit = true;
			SetHitCount( 5 );
			CSound::Play( SE_ENEMY_SHOT );
			ShotCounter++;
			x1 = LunaMath::CosF( ANGLE2PAI(angle) ) * speed;
			y1 = LunaMath::SinF( ANGLE2PAI(angle) ) * speed;
		}
		break;
	//------------------------------------------------------------ 誘導弾
	case 1:
		{
			bHit = true;
			SetHitCount( 5 );
			CSound::Play( SE_ENEMY_SHOT );
			ShotCounter++;
			x = CApplication::GetPlayer()->Px + 43;
			y = CApplication::GetPlayer()->Py + 35;
			x -= Px;
			y -= Py;
			float len = F(speed / sqrt( x*x + y*y ));
			x1 = x * len;
			y1 = y * len;
		}
		break;
	//------------------------------------------------------------ 大型弾
	case 2:
		{
			bHit = true;
			SetHitCount( 5 );
			CSound::Play( SE_ENEMY_SHOT );
			x1 = LunaMath::CosF( ANGLE2PAI(angle) ) * speed;
			y1 = LunaMath::SinF( ANGLE2PAI(angle) ) * speed;
		}
		break;
	//------------------------------------------------------------ 大型誘導弾
	case 3:
		{
			bHit = true;
			SetHitCount( 5 );
			CSound::Play( SE_ENEMY_SHOT );
			x = CApplication::GetPlayer()->Px + 43;
			y = CApplication::GetPlayer()->Py + 35;
			x -= Px;
			y -= Py;
			float len = F(speed / sqrt( x*x + y*y ));
			x1 = x * len;
			y1 = y * len;
		}
		break;
	//------------------------------------------------------------ レーザーためエフェクト
	case 4:
		{
			x2 = x;
			y2 = y;
			Angle = RAND(0,4095);
			Scale = RAND(0,200);
			x1 = x2 + F(LunaMath::Cos( Angle, Scale ));
			y1 = y2 + F(LunaMath::Sin( Angle, Scale ));
		}
		break;
	//------------------------------------------------------------ レーザー
	case 5:
		{
			bHit = true;
			SetHitCount( 5 );
			CSound::Play( SE_BIG_LASER );
		}
		break;
	}
}

//======================================================================================
//
//	デストラクタ
//
//======================================================================================
CENEMYSHOT::~CENEMYSHOT()
{
	switch ( No )
	{
	case 0:
	////////////////////////////////////////////////////////////// 通常弾
		ShotCounter--;
		break;
	////////////////////////////////////////////////////////////// 誘導弾
	case 1:
		ShotCounter--;
		break;
	}

	if ( lpParent != NULL ) lpParent->Release();
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CENEMYSHOT::Action( void )
{
	switch ( No )
	{
	////////////////////////////////////////////////////////////// 通常弾
	case 0:
	case 1:
		{
			Angle += 192;
			Px += x1;
			Py += y1;
			if ( Px < -30 || Px > 670 ) bDeath = true;
			if ( Py < -30 || Py > 430 ) bDeath = true;

			// 描画
			RECT src = { 736, 414, 736+33, 414+33 };
			RECT dest = { (long)Px-8, (long)Py-8, (long)Px+8, (long)Py+8 };
			Sprite[8][0]->DrawRotate( &dest, D3DCOLOR_XRGB(255,255,255), ANGLE2PAI(Angle), &src );

			// 当たり判定
			lpHitPt[0].x = -8;	lpHitPt[0].y = -8;
			lpHitPt[1].x = +8;	lpHitPt[1].y = -8;
			lpHitPt[2].x = +8;	lpHitPt[2].y = +8;
			lpHitPt[3].x = -8;	lpHitPt[3].y = +8;
			for ( long i = 0; i < 4; i++ )
			{
				LunaMath::RotatePoint( &lpHitPt[i].x, &lpHitPt[i].y, Angle );
				lpHitPt[i].x += (long)Px;
				lpHitPt[i].y += (long)Py;
			}
			lpHitPt[4] = lpHitPt[0];
		}
		break;
	//////////////////////////////////////////////////////////////
	case 2:
	case 3:
		{
			Angle += 192;
			Px += x1;
			Py += y1;
			if ( Px < -30 || Px > 670 ) bDeath = true;
			if ( Py < -30 || Py > 430 ) bDeath = true;

			// 描画
			RECT src = { 736, 414, 736+33, 414+33 };
			RECT dest = { (long)Px-15, (long)Py-8, (long)Px+15, (long)Py+8 };
			Sprite[8][0]->DrawRotate( &dest, D3DCOLOR_XRGB(255,255,255), ANGLE2PAI(Angle), &src );

			// 当たり判定
			lpHitPt[0].x = -15;	lpHitPt[0].y = -8;
			lpHitPt[1].x = +15;	lpHitPt[1].y = -8;
			lpHitPt[2].x = +15;	lpHitPt[2].y = +8;
			lpHitPt[3].x = -15;	lpHitPt[3].y = +8;
			for ( long i = 0; i < 4; i++ )
			{
				LunaMath::RotatePoint( &lpHitPt[i].x, &lpHitPt[i].y, Angle );
				lpHitPt[i].x += (long)Px;
				lpHitPt[i].y += (long)Py;
			}
			lpHitPt[4] = lpHitPt[0];
		}
		break;
	////////////////////////////////////////////////////////////// レーザー集積
	case 4:
		{
			x3 = LunaMath::FlatSpeed( 0.0f, 50.0f, 50, Time );
			y3 = LunaMath::FlatSpeed( 0.0f, 50.0f, 50, Time );
			Px = LunaMath::AddSpeed( x1, x2, 50, Time );
			Py = LunaMath::AddSpeed( y1, y2, 50, Time );
			if ( ++Time > 50 ) bDeath = true;

			// 描画
			RECT src = { 736, 478, 768, 510 };
			RECT dest = { (long)(Px-x3), (long)(Py-y3), (long)(Px+x3), (long)(Py+y3) };
			Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
		}
		break;
	////////////////////////////////////////////////////////////// レーザー
	case 5:
		{
			switch ( Phase )
			{
			case 0:
				{
					RECT dest, src = { 320, 556, 520, 656 };
					// 描画
					SetRect( &dest, (long)Px, (long)Py-50+RAND(-1,1), 640, (long)Py+50+RAND(-1,1) );
					Sprite[5][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
					SetRect( &dest, (long)Px, (long)Py-60+RAND(-1,1), 640, (long)Py+60+RAND(-1,1) );
					Sprite[5][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
					// 点滅
					if ( Time & 0x01 )
					{
						// 描画
						SetRect( &dest, (long)Px, (long)Py-50+RAND(-1,1), 640, (long)Py+50+RAND(-1,1) );
						Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
						SetRect( &dest, (long)Px, (long)Py-60+RAND(-1,1), 640, (long)Py+60+RAND(-1,1) );
						Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
					}
					// 当たり判定
					lpHitPt[0].x = Px+30;	lpHitPt[0].y = Py-50;
					lpHitPt[1].x = 640;		lpHitPt[1].y = Py-50;
					lpHitPt[2].x = 640;		lpHitPt[2].y = Py+50;
					lpHitPt[3].x = Px+30;	lpHitPt[3].y = Py+50;

					if ( ++Time > 150 )
					{
						Time = 0;
						Phase++;
					}
					lpHitPt[4] = lpHitPt[0];
				}
				break;
			case 1:
				{
					y1 = LunaMath::FlatSpeed( 50.0f, 0.0f, 100, Time );
					y2 = LunaMath::FlatSpeed( 60.0f, 0.0f, 100, Time );

					RECT dest, src = { 320, 556, 520, 656 };
					// 描画
					SetRect( &dest, (long)Px, (long)(Py-y1)+RAND(-1,1), 640, (long)(Py+y1)+RAND(-1,1) );
					Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
					SetRect( &dest, (long)Px, (long)(Py-y2)+RAND(-1,1), 640, (long)(Py+y2)+RAND(-1,1) );
					Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
					// 点滅
					if ( Time & 0x01 )
					{
						// 描画
						SetRect( &dest, (long)Px, (long)(Py-y1)+RAND(-1,1), 640, (long)(Py+y1)+RAND(-1,1) );
						Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
						SetRect( &dest, (long)Px, (long)(Py-y2)+RAND(-1,1), 640, (long)(Py+y2)+RAND(-1,1) );
						Sprite[8][1]->Draw( &dest, D3DCOLOR_ARGB(255,255,128,128), &src );
					}
					// 当たり判定
					lpHitPt[0].x = Px+30;	lpHitPt[0].y = (Py-y1);
					lpHitPt[1].x = 640;		lpHitPt[1].y = (Py-y1);
					lpHitPt[2].x = 640;		lpHitPt[2].y = (Py+y1);
					lpHitPt[3].x = Px+30;	lpHitPt[3].y = (Py+y1);
					if ( ++Time > 100 )
					{
						bDeath = true;
					}
					lpHitPt[4] = lpHitPt[0];
				}
				break;
			}
		}
		break;
	}

	//-----------------------------------------
	// 親が死んだら終わりですわ
	//-----------------------------------------
	if ( lpParent != NULL && lpParent->bDeath ) bDeath = true;

	return bDeath;
}

//======================================================================================
//
//	ダメージ
//
//======================================================================================
void CENEMYSHOT::Damage( long pow )
{
	switch ( No )
	{
	case 0:
	case 1:
	case 2:
	case 3:
		bHit = false;
		bDeath = true;
		break;
	}
}