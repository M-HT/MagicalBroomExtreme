/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			コンボ
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include <stdio.h>
#include "Combo.h"
#include "Application.h"


//======================================================================================
//
//	コンストラクタ
//
//======================================================================================
CCOMBO::CCOMBO( void ) : CSPRITE( SORT_NULL )
{
	MaxHit = 0;
	Alpha = 0;
	Combo = 0;
	ComboTimer = -1;
	ComboStr[0] = 0;
	ComboStr[1] = 0;
	ComboStr[2] = 0;
	ComboStr[3] = 0;
	for ( long i = 0; i < 4; i++ )
	{
		Time[i] = 0;
		Angle[i] = 0;
	}
}

//======================================================================================
//
//	処理
//
//======================================================================================
bool CCOMBO::Action( void )
{
	if ( ComboTimer == 0 )
	{
		if ( Combo > MaxHit ) MaxHit = Combo;
		Combo = 0;
		ComboTimer--;
	}
	ef ( ComboTimer > 0 )
	{
		// HIT数表示
		ComboTimer--;

		Alpha = 255;
		if ( ComboTimer < 30 )
		{
			Alpha = LunaMath::FlatSpeed( 255L, 0L, 30, ++Count );
		}
		bool bZero = true;
		for ( long i = 0; i < 4; i++ )
		{
			if ( bZero && (ComboStr[i] != 0) ) bZero = false;
			if ( bZero != true )
			{
				long dy = 50;
				long size = 0;

				switch ( Phase[i] )
				{
				case 0:
					{
						Angle[i] += 512;
						long scale = LunaMath::FlatSpeed( 10L, 0L, 60, Time[i] );
						size = LunaMath::FlatSpeed( 10L, 0L, 60, Time[i] );
						dy = LunaMath::Sin( Angle[i], scale ) + 50;
						if ( ++Time[i] > 60 )
						{
							Time[i] = 0;
							Phase[i]++;
						}
					}
					break;
				}

				long dx = 15+i*30;
				long sx = 723+ComboStr[i]*24;
				RECT dest = { dx-size, dy-15-size*2, dx+24+size, dy+15 };
				RECT src = { sx, 546, sx+24, 576 };
				Sprite[7][0]->Draw( &dest, D3DCOLOR_ARGB(Alpha,255,255,255), &src );
			}
		}

		// HIT表示
		if ( Combo == 1 )
		{
			RECT dest = { 140, 35, 184, 65 };
			RECT src = { 962, 546, 1008, 576 };
			Sprite[7][0]->Draw( &dest, D3DCOLOR_ARGB(Alpha,255,255,255), &src );
		}
		else
		{
			RECT dest = { 140, 35, 201, 65 };
			RECT src = { 962, 546, 1024, 576 };
			Sprite[7][0]->Draw( &dest, D3DCOLOR_ARGB(Alpha,255,255,255), &src );
		}

		// メーター
		CApplication::AddEffect( this );
	}

	return bDeath;
}

//======================================================================================
//
//	メーター描画
//
//======================================================================================
void CCOMBO::Draw( void )
{
	long cnt = 180-ComboTimer;

	long w = LunaMath::FlatSpeed( 200L, 15L, 180, cnt );
	long r = LunaMath::FlatSpeed( 128L, 255L, 180, cnt );
	long b = LunaMath::FlatSpeed( 255L, 128L, 180, cnt );
	D3DCOLOR color = D3DCOLOR_ARGB(Alpha,r,128,b);

	RECT dest;
	SetRect( &dest, 14, 67, 201, 76 );
	LunaEffect::FillSquare( &dest, D3DCOLOR_ARGB(Alpha,255,255,255) );
	SetRect( &dest, 15, 68, w, 75 );
	LunaEffect::FillSquare( &dest, color );
}

//======================================================================================
//
//	コンボ加算
//
//======================================================================================
void CCOMBO::Add( void )
{
	if ( Combo < 9999 )
	{
		Combo++;
		char buff[5] = "";
		sprintf( buff, "%04d", (unsigned int) Combo );
		for (  long i = 0; i < 4; i++ )
		{
			ComboStr[i] = (char)(buff[i] - '0');
			Time[i] = 0;
			Angle[i] = i*512;
			Phase[i] = 0;
		}
	}
	Count = 0;
	ComboTimer = 180;
}
