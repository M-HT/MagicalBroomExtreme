//======================================================================================
//======================================================================================
//
//
//			ボーナス
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Bonus.h"
#include "Application.h"
#include "Voice.h"


long CBONUS::BonusNum = 0;
//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CBONUS::CBONUS( eBonus no ) : CSPRITE( SORT_NULL )
{
	No = no;

	eVoiceType voice_type[] = {
		VOICE_RATEUP,
		VOICE_ENERGYMAX,
		VOICE_NODAMAGE,
		VOICE_NOMISS,
	};
	CVoice::Play( voice_type[no] );

	CApplication::AddGold( 500000 );

	Px = -65536;
	Py = -65536;
	Type = RAND( 0, 3 );

	for ( long i = 0; i < DIV_MAX; i++ )
	{
		Pos[i] = -65536;
	}

	float x[] = { 320, 260, 380, 380, 260 };
	float y[] = { 200, 160, 240, 160, 240 };

	x1 = x[BonusNum];
	y1 = y[BonusNum];

	++BonusNum %= 5;
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CBONUS::Action( void )
{
	const float start[] = { 0-64, 640+64, 0-32, 640+32 };
	const float center[] = { x1, x1, y1, y1 };
	const float ctrl[] = { +16, +16, +8, -8 };
	const float end[] = { 640+64, 0-64, 640+32, 0-32 };
	float *pos[] = { &Px, &Px, &Py, &Py };

	//---------------------------------------------------
	// 移動処理
	//---------------------------------------------------
	switch ( Phase )
	{
	case 0:
		*pos[Type] = F(LunaMath::Neville( start[Type], center[Type]+ctrl[Type], center[Type], F(Time)/40 ));
		if ( ++Time > 40 )
		{
			Time = 0;
			Phase++;
		}
		break;
	case 1:
		if ( ++Time == 40 )
		{
			Time = 0;
			Phase++;
		}
		break;
	case 2:
		*pos[Type] = F(LunaMath::Neville( center[Type], center[Type]-ctrl[Type], end[Type], F(Time)/40 ));
		if ( ++Time > 40 )
		{
			Time = 0;
			Phase++;
		}
		break;
	case 3:
		if ( ++Time == DIV_MAX )
		{
			return true;
		}
		break;
	}

	//---------------------------------------------------
	// 古い座標コピー
	//---------------------------------------------------
	for ( long i = DIV_MAX-1; i > 0; i-- )
	{
		Pos[i] = Pos[i-1];
	}
	Pos[0] = (long)(*pos[Type]);

	const long w[] = { 64/DIV_MAX, 64/DIV_MAX, 128/DIV_MAX, 128/DIV_MAX };
	const long temp[] = { -32+(long)y1, -32+(long)y1, -64+(long)x1, -64+(long)x1 };
	const long sy[] = { 126, 126+64, 126+128, 126+192 };

	//---------------------------------------------------
	// 描画処理
	//---------------------------------------------------
	switch ( Type )
	{
	case 0:
	case 1:
		for ( long i = 0; i < DIV_MAX; i++ )
		{
			RECT src = { 896, sy[No]+i*w[Type], 1024, sy[No]+i*w[Type]+w[Type] };
			RECT dest = { (long)Pos[i]-64, temp[Type]+i*w[Type], (long)Pos[i]+64, temp[Type]+i*w[Type]+w[Type] };
			Sprite[7][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
		}
		break;
	case 2:
	case 3:
		for ( long i = 0; i < DIV_MAX; i++ )
		{
			RECT src = { 896+i*w[Type], sy[No], 896+i*w[Type]+w[Type], sy[No]+64 };
			RECT dest = { temp[Type]+i*w[Type], (long)Pos[i]-32,  temp[Type]+i*w[Type]+w[Type], (long)Pos[i]+32 };
			Sprite[7][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
		}
		break;
	}

	return false;
}