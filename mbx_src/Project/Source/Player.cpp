//======================================================================================
//======================================================================================
//
//
//			プレイヤー
//
//
//======================================================================================
//======================================================================================


//======================================================================================
// INCLUDE
//======================================================================================
#include "Application.h"
#include "Sound.h"
#include "Player.h"
#include "Io.h"
#include "Tail.h"
#include "Shot.h"
#include "Bit.h"
#include "Life.h"
#include "Smoke.h"
#include "Sonic.h"
#include "Bonus.h"
#include "Fairy.h"
#include "Bomb.h"
#include "Laser.h"
#include "LaserFx.h"
#include "Voice.h"


//======================================================================================
// DEFINE
//======================================================================================
#define CHARGE_TIME_1	50
#define CHARGE_TIME_2	100
#define CHARGE_TIME_3	150


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CPLAYER::CPLAYER( void ) : CSPRITE( SORT_PLAYER )
{
	SetHitCount( 5 );
	const float spd[] = { 4.0f, 4.5f, 5.0f };
	Speed = spd[CApplication::GetLevel()];
	Life			= 10;
	AnimeNo			= 0;
	AnimeCount		= 0;
	ShotLevel		= 0;
	BitLevel		= 0;
	ShotCount		= 0;
	ShotAngle		= 16;
	HitTimer		= 0;
	FairyNum		= 0;
	NoDamageTimer	= 0;
	Charge			= 0;

	CVoice::Play( VOICE_START );

	new CLIFE( this );
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CPLAYER::Action( void )
{
	D3DCOLOR Color = D3DCOLOR_XRGB(255,255,255);
	const long AnimeData[] = { 0, 1, 2, 1 };

	//==================================================================
	// 死亡時
	//==================================================================
	if ( bDeath )
	{
		y1 += 2;
		if ( y1 < 500 )
		{
			if ( y1 > 450 )
			{
				y1 = 500;
				CApplication::SetScene( SCENE_GAMEOVER );
			}
			else
			{
				if ( ++HitTimer % 2 ) Color = D3DCOLOR_XRGB(0,0,0);
				//---------------------------------------------
				// 描画用
				//---------------------------------------------
				long x = RAND( -2, 2 );
				long y = RAND( -2, 2 );
				RECT dest = { (long)Px+x, (long)y1+y, (long)Px+x+90, (long)y1+y+55 };
				RECT src = { AnimeData[AnimeNo]*90, 300, AnimeData[AnimeNo]*90+90, 355 };
				Sprite[5][0]->Draw( &dest, Color, &src );
			}
		}
	}
	//==================================================================
	// 通常時
	//==================================================================
	else
	{
		static unsigned long ChargeTime = 0;

		switch ( Phase )
		{
		// ----------------------------------------------- 華麗？に登場
		case 0:
			{
				Px = LunaMath::Neville( -100.0f, 320.0f, 100.0f, F(Time)/100.0f );
				Py = LunaMath::Neville(   50.0f, 240.0f, 200.0f, F(Time)/100.0f );

				if ( ++Time == 100 )
				{
					CApplication::SetAddRate( 3 );
					bHit = true;
					bControl = true;
					Phase++;
				}
			}
			break;
		// ----------------------------------------------- 通常
		case 1:
			{
				if ( HitTimer < 270 )
				{
					//=================================================
					// 自機移動処理
					//=================================================
					static const float rt2 = 1.0f / sqrtf(2.0f);
					unsigned long flag = 0;
					float mx = 0, my = 0;

					// キー入力に応じてフラグ立て
					if ( IO::GetKey( KEYSTATE_HOLD, KEY_UP    ) ) flag |= 0x01;		// 上
					if ( IO::GetKey( KEYSTATE_HOLD, KEY_DOWN  ) ) flag |= 0x02;		// 下
					if ( IO::GetKey( KEYSTATE_HOLD, KEY_LEFT  ) ) flag |= 0x04;		// 左
					if ( IO::GetKey( KEYSTATE_HOLD, KEY_RIGHT ) ) flag |= 0x08;		// 右

					// フラグに応じて移動させる
					if ( flag & 0x01 ) my = -Speed;
					if ( flag & 0x02 ) my =  Speed;
					if ( flag & 0x04 ) mx = -Speed;
					if ( flag & 0x08 ) mx =  Speed;

					// 斜めの時は補正する
					if ( ( flag & ( 0x01 | 0x02 ) ) && ( flag & ( 0x04 | 0x08 ) ) )
					{
						mx *= rt2;
						my *= rt2;
					}

					// 移動
					Px += mx;
					Py += my;

					// 画面外に出ないようにする
					if ( Px <      0 ) Px = 0;
					if ( Px > 640-90 ) Px = 640-90;
					if ( Py <      0 ) Py = 0;
					if ( Py > 400-55 ) Py = 400-55;

					if ( bControl )
					{
						//=================================================
						// ボム
						//=================================================
						if ( IO::GetKey( KEYSTATE_PUSH, KEY_02 ) )
						{
							if ( CApplication::BootBomb( true ) )
							{
								Charge = 0;
								// ボム発動
								new CBOMB();
								// 発動から一定時間無敵
								NoDamageTimer = 300;
								bHit = false;
							}
						}
						//=================================================
						// 溜め
						//=================================================
						if ( IO::GetKey( KEYSTATE_HOLD, KEY_01 ) )
						{
							// ためサウンド
							if ( (++ChargeTime%25) == 0 )
							{
								CSound::Play( SE_CHARGE );
							}
							// ためレベル
							long lv = 0;
							if ( Charge > CHARGE_TIME_3 ) lv = 3;
							ef ( Charge > CHARGE_TIME_2 ) lv = 2;
							ef ( Charge > CHARGE_TIME_1 ) lv = 1;
							// ゲージ一定値以上でためエフェクト
							if ( Charge > 10 )
							{
								new CLASERFX( lv );
								new CLASERFX( lv );
								new CLASERFX( lv );
							}
							// 最大値
							if ( ++Charge > 400 ) Charge = 400;
						}
						else
						{
							// ボタン１を離した時
							if ( IO::GetKey( KEYSTATE_PULL, KEY_01 ) )
							{
								// ためレベル３
								if ( Charge > CHARGE_TIME_3 )
								{
									for ( long i = 1; i <= 12; i++ )
									{
										long angle = LunaMath::FlatSpeed( 1024L, -1024L, 9, i );
										new CLASER( Px+90, Py+30, 5, angle );
									}
								}
								// ためレベル２
								ef ( Charge > CHARGE_TIME_2 )
								{
									for ( long i = 1; i <= 6; i++ )
									{
										long angle = LunaMath::FlatSpeed( 512L, -512L, 5, i );
										new CLASER( Px+90, Py+30, 5, angle );
									}
								}
								// ためレベル１
								ef ( Charge > CHARGE_TIME_1 )
								{
									for ( long i = 1; i <= 3; i++ )
									{
										long angle = LunaMath::FlatSpeed( 256L, -256L, 3, i );
										new CLASER( Px+90, Py+30, 5, angle );
									}
								}
								// 通常ショット
								else
								{
									new CBIT( Px+15, Py+40, BitLevel );
									ShotCount = 3;
								}
							}
							else
							{
								// ためゲージ現象
								if ( --Charge < 0 ) Charge = 0;
							}

							//=================================================
							// ショット
							//=================================================
							if ( IO::GetKey( KEYSTATE_HOLD, KEY_03 ) )
							{
								new CBIT( Px+15, Py+40, BitLevel );
								ShotCount = 3;
							}

							static unsigned long PrevCnt = Luna::GetCounter();
							unsigned long NowCnt = Luna::GetCounter();
							if ( NowCnt - PrevCnt > 5 )
							{
								if ( ShotCount-- > 0 )
								{
									ShotAngle += 32;
									if ( ShotAngle == 256+16 ) ShotAngle = -ShotAngle;
									switch ( ShotLevel )
									{
									case 2:
										new CSHOT( Px+90, Py+30, +ShotAngle );
										new CSHOT( Px+90, Py+30, -ShotAngle );
									case 1:
										new CSHOT( Px+90, Py+30, (+ShotAngle)/2 );
										new CSHOT( Px+90, Py+30, (-ShotAngle)/2 );
									case 0:
										new CSHOT( Px+90, Py+30, 0 );
									}
									new CSMOKE( Px+90, Py+30, 2048, 0,  6, 10, 2 );
									new CSMOKE( Px+90, Py+30, 2048+RAND(-128,128), 2, 10, 60, 1 );
									PrevCnt = NowCnt;
								}
							}
						}
					}

					//=================================================
					// あたり判定
					//=================================================
					lpHitPt[0].x = (Px+38);	lpHitPt[0].y = (Py+30);
					lpHitPt[1].x = (Px+48);	lpHitPt[1].y = (Py+30);
					lpHitPt[2].x = (Px+48);	lpHitPt[2].y = (Py+40);
					lpHitPt[3].x = (Px+38);	lpHitPt[3].y = (Py+40);
					lpHitPt[4] = lpHitPt[0];
				}

				//=================================================
				// レート調整＆ダメージ判定
				//=================================================
				if ( ShotCount > 0 )
				{
					const long add[] = { 1, 3, 5 };
					CApplication::SetAddRate( add[CApplication::GetLevel()] );
					CApplication::SetAddForce( 1 );
				}
				else
				{
					const long add[] = { 2, 5, 8 };
					CApplication::SetAddRate( add[CApplication::GetLevel()] );
					CApplication::SetAddForce( 2 );
				}

				//=================================================
				// ダメージ無視タイマー
				//=================================================
				if ( NoDamageTimer > 0 )
				{
					NoDamageTimer--;	
					if ( NoDamageTimer == 0 )
					{
						bHit = true;
					}
					ef ( (NoDamageTimer % 3) == 0)
					{
						Color = D3DCOLOR_XRGB(0,0,0);
					}
				}

				//=================================================
				// ダメージ中タイマー
				//=================================================
				if ( HitTimer > 0 )
				{
					HitTimer--;
					CApplication::SetAddRate( -1 );
				}

				if ( ++Time == 50*30 )
				{
					Time = 0;
					new CBONUS( BONUS_NODAMAGE );
				}
			}
			break;
		}

		// 箒の尻尾
		static long cnt = 0;
		if ( (++cnt % 3) == 0 ) new CTAIL( Px, Py );

		//---------------------------------------------
		// 描画用
		//---------------------------------------------
		Angle += 128;
		long y = LunaMath::Sin( Angle, 2 );
		RECT dest = { (long)Px, (long)Py+y, (long)Px+90, (long)Py+y+55 };
		RECT src = { AnimeData[AnimeNo]*90, 300, AnimeData[AnimeNo]*90+90, 355 };
		Sprite[5][0]->Draw( &dest, Color, &src );
	}
	//---------------------------------------------
	// アニメ用
	//---------------------------------------------
	if ( ++AnimeCount == 3 )
	{
		AnimeCount = 0;
		if ( ++AnimeNo == 4 )
		{
			AnimeNo = 0;
		}
	}

	// 溜めゲージ用
	CApplication::AddEffect( this );

	return false;
}

//======================================================================================
//
//	溜めゲージ描画
//
//======================================================================================
void CPLAYER::Draw( void )
{
	if ( bDeath || (Charge > 500) ) return;

	D3DCOLOR c1;
	D3DCOLOR c2;
	RECT dest;
	long lv, time, x, y;

	lv = Charge;
	x = (long)(Px + 45);
	y = (long)(Py + 50);

	if ( lv > CHARGE_TIME_3 )
	{
		time = 100;
		lv = 100;
		c1 = D3DCOLOR_XRGB( 255, 32, 32);
		c2 = D3DCOLOR_XRGB( 255, 32, 32 );
	}
	ef ( lv > CHARGE_TIME_2 )
	{
		time = CHARGE_TIME_3 - CHARGE_TIME_2;
		lv -= CHARGE_TIME_2;
		c1 = D3DCOLOR_XRGB( 128, 255, 32 );
		c2 = D3DCOLOR_XRGB( 255, 32, 32 );
	}
	ef ( lv > CHARGE_TIME_1 )
	{
		time = CHARGE_TIME_2 - CHARGE_TIME_1;
		lv -= CHARGE_TIME_1;
		c1 = D3DCOLOR_XRGB( 32, 128, 255 );
		c2 = D3DCOLOR_XRGB( 128, 255, 32 );
	}
	ef ( lv > 0 )
	{
		time = CHARGE_TIME_1;
		c1 = D3DCOLOR_XRGB( 255, 255, 255 );
		c2 = D3DCOLOR_XRGB( 32, 128, 255 );
	}
	else
	{
		return;
	}

	// 背景
	SetRect( &dest, x-30, y, x+30, y+5 );
	LunaEffect::FillSquare( &dest, D3DCOLOR_XRGB(255,255,255) );

	lv = LunaMath::FlatSpeed( x-29L, x+29L, time, lv );

	SetRect( &dest, x-29, y+1, lv, y+4 );
	LunaEffect::FillSquare( &dest, c2 );

	SetRect( &dest, lv, y+1, x+29, y+4 );
	LunaEffect::FillSquare( &dest, c1 );
}

//======================================================================================
//
//	ダメージ
//
//======================================================================================
void CPLAYER::Damage( long pow )
{
	float x = Px + 45;
	float y = Py + 30;

	if ( RAND(0,1) )	CVoice::Play( VOICE_DAMAGE01 );
	else				CVoice::Play( VOICE_DAMAGE02 );

	new CSONIC( x, y, 300, 50, false, 1, D3DCOLOR_XRGB(255,255,255) );
	new CSONIC( x, y, 500, 30, true, 1, D3DCOLOR_XRGB(255,255,255) );
	bHit = false;
	Time = 0;
	Charge = 0;
	HitTimer = 300;
	NoDamageTimer =150;
	if ( --Life == 0 )
	{
		for ( long i = 0; i < 32; i++ )
		{
			new CSMOKE(
					x+RAND(-25,25), y+RAND(-25,25),
					RAND(0,4095), RAND(0,15)/10.0f, RAND(10,20), 75, 1 );
			new CSMOKE(
					x+RAND(-25,25), y+RAND(-25,25),
					RAND(0,4095), RAND(0,15)/10.0f, RAND(10,20), 50, 0 );
			new CSMOKE(
					x+RAND(-25,25), y+RAND(-25,25),
					RAND(0,4095), RAND(0,15)/10.0f, RAND(10,20), 50, 2 );
		}
		CApplication::SetAddRate( 0 );
		y1 = Py;
		bDeath = true;
		CSound::Play( SE_BOMB02 );
		CVoice::Play( VOICE_GAMEOVER );
	}
	else
	{
		CSound::Play( SE_PLAYERHIT );
	}

	// 画面振動
	SetQuake( RAND(0,1) );
}

//======================================================================================
//
//	スピードアップ
//
//======================================================================================
void CPLAYER::SpeedUp( void )
{
	if ( Speed > 8.0f )
	{
		CVoice::Play( VOICE_BONUS );
		Speed = 8.0f;
		CApplication::AddGold( 10000 );
		CApplication::AddScore( 10000 );
	}
	else
	{
		CVoice::Play( VOICE_SPEEDUP );
		Speed += 0.5f;
	}
}

//======================================================================================
//
//	ショットレベルアップ
//
//======================================================================================
void CPLAYER::ShotLevelUp( void )
{
	if ( ShotLevel == 2 )
	{
		CVoice::Play( VOICE_BONUS );
		CApplication::AddGold( 10000 );
		CApplication::AddScore( 10000 );
	}
	else
	{
		CVoice::Play( VOICE_POWERUP );
		ShotLevel++;
	}

}

//======================================================================================
//
//	ビットレベルアップ
//
//======================================================================================
void CPLAYER::BitLevelUp( void )
{
	if ( BitLevel == 3 )
	{
		CVoice::Play( VOICE_BONUS );
		CApplication::AddGold( 10000 );
		CApplication::AddScore( 10000 );
	}
	else
	{
		CVoice::Play( VOICE_BITUP );
		BitLevel++;
	}
}

//======================================================================================
//
//	ライフアップ
//
//======================================================================================
void CPLAYER::LifeUp( void )
{
	if ( Life == 10 )
	{
		CVoice::Play( VOICE_BONUS );
		CApplication::AddGold( 10000 );
		CApplication::AddScore( 10000 );
	}
	else
	{
		CVoice::Play( VOICE_LIFEUP );
		Life++;
	}
}

//======================================================================================
//
//	妖精GET
//
//======================================================================================
void CPLAYER::FairyUp( void )
{
	if ( FairyNum == 3 )
	{
		CVoice::Play( VOICE_BONUS );
		CApplication::AddGold( 10000 );
		CApplication::AddScore( 10000 );
	}
	else
	{
		new CFAIRY();
		CVoice::Play( VOICE_FAIRY );
		FairyNum++;
	}
}