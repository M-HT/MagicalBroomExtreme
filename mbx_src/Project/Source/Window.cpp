/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			ウィンドウ
//
//
//======================================================================================
//======================================================================================


//======================================================================================
// INCLUDE
//======================================================================================
#include "Window.h"
#include "Bonus.h"

#include <stdio.h>


//======================================================================================
// DEFINE
//======================================================================================
#define FORCE_MAX	9000
#define FORCE_BOMB	(FORCE_MAX/3)


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CWINDOW::CWINDOW( void ) : CSPRITE( SORT_NULL )
{
	Score = 0;
	Gold = 0;
	Rate = 0;
	ScoreAdd = 0;
	GoldAdd = 0;
	RateAdd = 0;
	ForceAdd = 0;
	TempScore = 0;
	TempGold = 0;

	RateCount = 3000;

	Force = 0;

	IsAddForce = true;
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CWINDOW::Action( void )
{
	RECT dest, src;
	char buff[11] = "";
	//------------------------------------------
	// 描画リストに追加
	//------------------------------------------

	// ウィンドウ
	SetRect( &dest, 0, 400, 50, 480 );
	SetRect( &src, 688, 944, 738, 1024 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	SetRect( &dest, 590, 400, 640, 480 );
	SetRect( &src, 718, 944, 768, 1024 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );

	for ( long i = 50; i < 590; i += 60 )
	{
		SetRect( &dest, i, 400, i+60, 480 );
		SetRect( &src, 698, 944, 758, 1024 );
		Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	}

	// GOLD
	SetRect( &dest, 10, 413, 51, 439 );
	SetRect( &src, 688, 605, 729, 631 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	SetRect( &dest, 60, 408, 217, 439 );
	SetRect( &src, 183, 440, 340, 471 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );

	// SCORE
	SetRect( &dest, 10, 447, 51, 473 );
	SetRect( &src, 728, 605, 769, 631 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	SetRect( &dest, 60, 442, 217, 473 );
	SetRect( &src, 339, 440, 496, 471 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );

	// RATE
	SetRect( &dest, 240, 413, 280, 439 );
	SetRect( &src, 649, 605, 689, 631 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	SetRect( &dest, 290, 408, 363, 439 );
	SetRect( &src, 495, 440, 568, 471 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );

	// FORCE
	SetRect( &dest, 240, 445, 302, 471 );
	SetRect( &src, 585, 605, 650, 631 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	SetRect( &dest, 314, 437, 577, 471 );
	SetRect( &src, 308, 479, 571, 513 );
	Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );

	//--------------------------------------------------
	// FORCE
	//--------------------------------------------------
	long f = LunaMath::FlatSpeed( 0L, 258, FORCE_MAX, Force );
	D3DCOLOR color = D3DCOLOR_XRGB(255,192,192);
	if ( Force == -1 )
	{
		f = 258;
		static long no = 0;
		D3DCOLOR c[] = { 0xFFC0FFFF, 0xFFFFC0FF };
		color = c[no];
		++no %= 2;
	}
	else
	{
		if ( IsAddForce ) Force += ForceAdd;
		if ( Force > FORCE_MAX )
		{
			Force = -1;
			new CBONUS( BONUS_ENERGYMAX );
		}
		ef ( Force > FORCE_BOMB )
		{
			color = D3DCOLOR_XRGB(192,255,192);
		}
	}

	SetRect( &dest, 316, 439, 316+f, 468 );
	SetRect( &src, 308, 512, 308+f, 541 );
	Sprite[9][0]->Draw( &dest, color, &src );

	//--------------------------------------------------
	// FORCE DATA
	//--------------------------------------------------

	if ( Force == -1 )
	{
		if ( --y4 < -32 ) y4 = 32;

		long sy1 = (long)0;
		long sy2 = (long)32;
		long dy1 = (long)y4;
		long dy2 = (long)y4+32;

		if ( dy1 < 0 ) { sy1 = -dy1; dy1 = 0; }
		if ( dy2 > 32 ) { sy2 = 32-(dy2-32); dy2 = 32; }

		SetRect( &dest, 350, 439+dy1, 350+187, 439+dy2 );
		SetRect( &src, 520, 541+sy1, 707, 541+sy2 );
		Sprite[9][1]->Draw( &dest, color, &src );
	}
	ef ( Force > FORCE_BOMB )
	{
		if ( --y4 < -32 ) y4 = 32;

		long sy1 = (long)0;
		long sy2 = (long)32;
		long dy1 = (long)y4;
		long dy2 = (long)y4+32;

		if ( dy1 < 0 ) { sy1 = -dy1; dy1 = 0; }
		if ( dy2 > 32 ) { sy2 = 32-(dy2-32); dy2 = 32; }

		SetRect( &dest, 350, 439+dy1, 350+187, 439+dy2 );
		SetRect( &src, 520, 573+sy1, 707, 573+sy2 );
		Sprite[9][1]->Draw( &dest, color, &src );
	}

	//--------------------------------------------------
	// GOLD DATA
	//--------------------------------------------------
	TempGold += GoldAdd;
	if ( TempGold > Gold )
	{
		Gold = TempGold;
		GoldAdd = 0;
	}
	// スコアを文字列に変換
	sprintf( buff, "%010d", TempGold );
	// 1桁ずつ表示
	for ( long i = 0; i < 10; i++ )
	{
		buff[i] -= '0';
		long yy = (buff[i]*24);
		SetRect( &dest, 63+i*15, 411, 64+15+i*15, 411+24 );
		SetRect( &src, 866, 142+yy, 882, 142+yy+24 );
		Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	}

	//--------------------------------------------------
	// SCORE DATA
	//--------------------------------------------------
	TempScore += ScoreAdd;
	if ( TempScore > Score )
	{
		Score = TempScore;
		ScoreAdd = 0;
	}
	// スコアを文字列に変換
	sprintf( buff, "%010d", TempScore );
	// 1桁ずつ表示
	for ( long i = 0; i < 10; i++ )
	{
		buff[i] -= '0';
		long yy = (buff[i]*24);
		SetRect( &dest, 63+i*15, 445, 64+15+i*15, 445+24 );
		SetRect( &src, 851, 142+yy, 867, 142+yy+24 );
		Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	}

	//--------------------------------------------------
	// RATE DATA
	//--------------------------------------------------
	if ( RateAdd < 0 )
	{
		Rate -= (Rate / 300);
		if ( Rate < 1000 )
		{
			Rate = 1000;
		}
	}
	else
	{
		Rate += RateAdd;
		if ( Rate > 99999 )
		{
			Rate = 99999;
		}
		if ( Rate >= RateCount )
		{
			RateCount += 3000;
			new CBONUS( BONUS_RATEUP );
		}
	}
	// レートを文字列に変換
	sprintf( buff, "%05d", Rate );
	// 1桁ずつ表示
	for ( long i = 0; i < 4; i++ )
	{
		long srcx[4] = { 293, 308, 329, 344 };
		buff[i] -= '0';
		long yy = (buff[i]*24);
		SetRect( &dest, srcx[i], 411, srcx[i]+15, 411+24 );
		SetRect( &src, 881, 142+yy, 896, 142+yy+24 );
		Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
	}

	return bDeath;
}

//======================================================================================
//
//	スコアセット
//
//======================================================================================
void CWINDOW::SetScore( unsigned long score )
{
	Score = score;
	TempScore = score;
	ScoreAdd = 0;
}

//======================================================================================
//
//	ゴールドセット
//
//======================================================================================
void CWINDOW::SetGold( unsigned long gold )
{
	Gold = gold;
	TempGold = gold;
	GoldAdd = 0;
}

//======================================================================================
//
//	スコア追加
//
//======================================================================================
void CWINDOW::AddScore( unsigned long score )
{
	Score += score;
	ScoreAdd = ( (Score - TempScore) / 100 );
}

//======================================================================================
//
//	ゴールド追加
//
//======================================================================================
void CWINDOW::AddGold( unsigned long gold )
{
	Gold += gold * Rate / 1000;
	GoldAdd = ( (Gold - TempGold) / 100 );
}

//======================================================================================
//
//	レートセット
//
//======================================================================================
void CWINDOW::SetAddRate( long add )
{
	RateAdd = add;
}

//======================================================================================
//
//	レート取得
//
//======================================================================================
long CWINDOW::GetRate( void )
{
	return Rate;
}

//======================================================================================
//
//	フォースセット
//
//======================================================================================
void CWINDOW::SetAddForce( long add )
{
	ForceAdd = add;
}

//======================================================================================
//
//	ボム起動
//
//======================================================================================
bool CWINDOW::BootBomb( bool flag )
{
	if ( flag )
	{
		if ( Force > FORCE_BOMB )
		{
			IsAddForce = false;
			Force -= FORCE_BOMB;
			return true;
		}
		ef ( Force == -1 )
		{
			IsAddForce = false;
			Force = FORCE_MAX - FORCE_BOMB;
			return true;
		}
	}
	else
	{
		IsAddForce = true;
	}

	return false;
}