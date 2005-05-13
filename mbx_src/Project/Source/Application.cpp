//======================================================================================
//======================================================================================
//
//
//			アプリケーション基底
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include <stdio.h>
#include "Application.h"
#include "Io.h"
#include "Event.h"
#include "CharaBase.h"
#include "Title.h"
#include "Window.h"
#include "Bonus.h"
#include "Bg.h"
#include "Snow.h"
#include "Scroll.h"
#include "Parts.h"
#include "Sound.h"
#include "Voice.h"


//======================================================================================
//
//	データメンバ
//
//======================================================================================
D3DCOLOR CApplication::FadeColor;
unsigned long CApplication::NoMissCounter;
unsigned long CApplication::EnemyDestroy;
LPCPLAYER CApplication::lpPlayer;
LPCWINDOW CApplication::lpWindow;
LPCCOMBO CApplication::lpCombo;
eScene CApplication::NowScene;
long CApplication::ScenePhase;
long CApplication::Difficulty;
long CApplication::Stage;

LPCSPRITE CApplication::EffectList[EFFECT_MAX];
unsigned long CApplication::EffectNum;

// スコアデータ
SCOREDATA CApplication::ScoreData[10] = 
{
	"RENGE",	1000000,
	"AYAME",	800000,
	"KIKYOU",	500000,
	"MADOKA",	300000,
	"KASUMI",	100000,
	"SAKURA",	80000,
	"YURI",		50000,
	"MOMIJI",	30000,
	"SUMIRE",	10000,
	"HANIWA",	1,
};

// 関数テーブル
void (*CApplication::SceneFunc[sizeof_eScene])( void ) =
{
	CApplication::SceneLogo,
	CApplication::SceneTitle,
	CApplication::SceneStg,
	CApplication::SceneRanking,
	CApplication::SceneOption,
	CApplication::SceneGameover,
	CApplication::SceneClear,
};

//======================================================================================
//
//	シーン設定
//
//======================================================================================
void CApplication::SetScene( eScene scene )
{
	NowScene = scene;
	ScenePhase = 0;
}

//======================================================================================
//
//	初期化
//
//======================================================================================
void CApplication::Initialize( void )
{
	EffectNum = 0;
	Difficulty = 1;

	// サウンド初期化
	CSound::Init();

	// ボイス初期化
	CVoice::Init();

	// スコアデータ読み込み
	LoadScore();

	// 最初のシーンを設定
	SetScene( SCENE_LOGO );
}

//======================================================================================
//
//	終了
//
//======================================================================================
void CApplication::UnInitialize( void )
{
	CVoice::Uninit();
	CSound::Uninit();
}

//======================================================================================
//
//	メイン
//
//======================================================================================
void CApplication::Main( void )
{
	//-------------------------------------------
	// リスト演算処理
	//-------------------------------------------
	// リストの先頭取得
	LPCSPRITE p = (LPCSPRITE)SpriteList.Top();
	// リストの終端まで･･･
	while ( p != NULL )
	{
		// あらかじめ次を取得
		LPCSPRITE _p = (LPCSPRITE)p->Next;
		// 処理
		if ( p->Action() ) p->Release();
		// 次
		p = _p;
	}

	//-------------------------------------------
	// 描画処理
	//-------------------------------------------
	if ( Luna3D::BeginScene() )
	{
		//-------------------------------------------
		// 各シーンごとの処理
		//-------------------------------------------
		SceneFunc[NowScene]();

		//-------------------------------------------
		// 画面描画
		//-------------------------------------------
		for ( long i = 0; i < 10; i++ )
		{
			Sprite[i][0]->Rendering();
			Sprite[i][1]->Rendering();
		}
		// エフェクトの描画
		ScreenEffect();

		//-------------------------------------------
		// 描画シーン終了
		//-------------------------------------------
		Luna3D::EndScene();
	}

	//-------------------------------------------
	// あたり判定処理
	//-------------------------------------------
	AllHitCheck();
/*
	//-------------------------------------------
	// デバッグ用データ表示
	//-------------------------------------------
	if ( Luna3D::BeginScene() )
	{
		// 当たり判定表示
		LunaEffect::Begin( BLEND_NORMAL );
		// リストの先頭取得
		LPCSPRITE p = (LPCSPRITE)SpriteList.Top();
		// リストの終端まで･･･
		while ( p != NULL )
		{
			if ( p->bHit )
			{
				// 処理
				long cnt = p->HitCount-1;
				for ( long i = 0; i < cnt; i++ )
				{
					LunaEffect::Line(
							p->lpHitPt[i+0].x, p->lpHitPt[i+0].y,
							p->lpHitPt[i+1].x, p->lpHitPt[i+1].y,
							D3DCOLOR_XRGB(255,255,0) );
				}
			}
			// 次
			p = (LPCSPRITE)p->Next;
		}
		LunaEffect::End();

		// 描画シーン終了
		Luna3D::EndScene();
	}
*/
}

//======================================================================================
//
//	ロゴ
//
//======================================================================================
void CApplication::SceneLogo( void )
{
	static long FadeLevel;

	// 背景
	Luna3D::ColorFill( D3DCOLOR_XRGB(255,255,255) );

	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 初期化
	case 0:
		{
			CSound::Play( SE_LOGO );
			FadeLevel = 255;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- フェードイン
	case 1:
		{
			// ロゴ
			RECT src = { 0, 411, 256, 488 };
			RECT dest = { 320-128, 240-40, 320+128, 240+37 }; 
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
			// 徐々にフェードイン
			SetFade( D3DCOLOR_ARGB(FadeLevel,0,0,0) );
			FadeLevel -= 2;
			if ( FadeLevel < 0 )
			{
				FadeLevel = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- ウェイト
	case 2:
		{
			// ロゴ
			RECT src = { 0, 411, 256, 488 };
			RECT dest = { 320-128, 240-40, 320+128, 240+37 };
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
			// タイマー代わり（1秒待機
			FadeLevel++;
			if ( FadeLevel > 50 )
			{
				FadeLevel = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- フェードアウト
	case 3:
		{
			// ロゴ
			RECT src = { 0, 411, 256, 488 };
			RECT dest = { 320-128, 240-40, 320+128, 240+37 };
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
			// 徐々にフェードアウト
			SetFade( D3DCOLOR_ARGB(FadeLevel,0,0,0) );
			FadeLevel += 5;
			if ( FadeLevel > 255 )
			{
				Luna3D::Clear();
				// タイトル画面へ
				SetScene( SCENE_TITLE );
			}
		}
		break;
	}
}

//======================================================================================
//
//	タイトル
//
//======================================================================================
void CApplication::SceneTitle( void )
{
	static LPCTITLE lpTitle[4];		// タイトル表示用

	static long Select;
	static long FadeLevel;

	RECT src, dest;


	//-----------------------------------------------------------
	// タイトル画面
	//-----------------------------------------------------------
	if ( ScenePhase > 0 )
	{
		// 背景
		SetRect( &dest, 0, 0, 640, 480 );
		D3DCOLOR color[] = {
			D3DCOLOR_XRGB(192,192,255),
			D3DCOLOR_XRGB(64,64,96),
			D3DCOLOR_XRGB(192,192,255),
			D3DCOLOR_XRGB(64,64,96),
		};
		LunaEffect::Begin( BLEND_NORMAL );
		LunaEffect::Gradation( &dest, color );
		LunaEffect::End();
		// タイトル
		SetRect( &src, 0, 0, 512, 140 );
		SetRect( &dest, 320-256, 50, 320+256, 190 );
		SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
		// タイトル選択
		for ( long i = 0; i < 4; i++ )
		{
			if ( lpTitle[i] != NULL )
			{
				lpTitle[i]->GetRect( &dest, &src );
				// 選択中なら明るくする
				if ( i == Select )
				{
					SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_ADD );
				}
				else
				{
					SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(32,64,64), BLEND_ADD );
				}
			}
		}
	}

	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 初期化
	case 0:
		{
			CSound::Play( BGM_TITLE );
			Luna3D::Clear();
			// タイトル用選択肢生成
			for ( long i = 0; i < 4; i++ )
			{
				lpTitle[i] = new CTITLE( F(320), F(280+(i*60)), i );
			}
			Select = 0;
			FadeLevel = 255;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- フェードイン
	case 1:
		{
			// 徐々にフェードイン
			SetFade( D3DCOLOR_ARGB(FadeLevel,0,0,0) );
			FadeLevel -= 5;
			if ( FadeLevel < 0 )
			{
				FadeLevel = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- 選択
	case 2:
		{
			// キー処理
			if ( IO::GetKey( KEYSTATE_PUSH, KEY_UP ) )		// 上
			{
				CSound::Play( SE_CURSOR );
				lpTitle[Select]->SetSelect( false, 0 );
				if ( --Select < 0 ) Select = 2;
				lpTitle[Select]->SetSelect( true, 1 );
			}
			if ( IO::GetKey( KEYSTATE_PUSH, KEY_DOWN ) )	// 下
			{
				CSound::Play( SE_CURSOR );
				lpTitle[Select]->SetSelect( false, 0 );
				if ( ++Select > 2 ) Select = 0;
				lpTitle[Select]->SetSelect( true, 0 );
			}
			if ( IO::GetKey( KEYSTATE_PUSH, KEY_01 ) )		// ボタン1
			{
				CSound::Play( SE_DECIDE );
				switch ( Select )
				{
				case 0: ScenePhase = 10; break;		// スタート
				case 1: ScenePhase = 20; break;		// スコアランキング
				case 2: ScenePhase = 30; break;		// 終了
				}
			}
		}
		break;
	//------------------------------------------------------------------------------- スタート
	case 10:
		{
			for ( long i = 0; i < 4; i++ )
			{
				lpTitle[i]->Release();
				lpTitle[i] = NULL;
			}
			Select = 1;
			ScenePhase++;
		}
		break;
	case 11:
		{
			// キー処理
			if ( IO::GetKey( KEYSTATE_PUSH, KEY_UP ) )		// 上
			{
				CSound::Play( SE_CURSOR );
				if ( --Select < 0 ) Select = 2;
			}
			if ( IO::GetKey( KEYSTATE_PUSH, KEY_DOWN ) )	// 下
			{
				CSound::Play( SE_CURSOR );
				if ( ++Select > 2 ) Select = 0;
			}
			if ( IO::GetKey( KEYSTATE_PUSH, KEY_01 ) )		// ボタン1
			{
				CSound::Play( SE_DECIDE );
				SetLevel( Select );
				ScenePhase++;
			}
			for ( long i = 0; i < 3; i++ )
			{
				long alpha = ((i==Select)?(255):(32));
				long py = 220 + (i*54);
				long sy = 140 + (i*32);
				RECT dest = { 320-96, py, 320+96, py+48 };
				RECT src = { 352, sy, 352+128, sy+32 };
				SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_ARGB(alpha,255,255,255), BLEND_ADD );
			}
		}
		break;
	case 12:
		{
			LSURFACE pSurf = LunaSurface::Create( 640, 480, FORMAT_BACKBUFFER );
			pSurf->CopyFromBackBuffer( 0, 0, 0, 0, 640, 480 );
			for ( long y = 0; y < 480; y += 80 )
			{
				for ( long x = 0; x < 640; x += 80 )
				{
					RECT rect = { x, y, x+80, y+80 };
					new CPARTS( F(x), F(y), &rect, pSurf );
				}
			}
			pSurf->Release();
			SetScene( SCENE_STG );
		}
		break;
	//------------------------------------------------------------------------------- スコア
	case 20:
		{
			LSURFACE pSurf = LunaSurface::Create( 640, 480, FORMAT_BACKBUFFER );
			pSurf->CopyFromBackBuffer( 0, 0, 0, 0, 640, 480 );
			for ( long y = 0; y < 480; y += 80 )
			{
				for ( long x = 0; x < 640; x += 80 )
				{
					RECT rect = { x, y, x+80, y+80 };
					new CPARTS( F(x), F(y), &rect, pSurf );
				}
			}
			pSurf->Release();
			for ( long i = 0; i < 4; i++ )
			{
				lpTitle[i]->Release();
				lpTitle[i] = NULL;
			}
			SetScene( SCENE_RANKING );
		}
		break;
	//------------------------------------------------------------------------------- 終了
	case 30:
		{
			// 徐々にフェードイン
			SetFade( D3DCOLOR_ARGB(FadeLevel,0,0,0) );
			FadeLevel += 5;
			if ( FadeLevel > 255 )
			{
				for ( long i = 0; i < 4; i++ )
				{
					lpTitle[i]->Release();
					lpTitle[i] = NULL;
				}
				// 終了
				Luna::Exit();
			}
		}
		break;
	}
}

//======================================================================================
//
//	シューティング
//
//======================================================================================
void CApplication::SceneStg( void )
{
	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 初期化
	case 0:
		{
			CSound::Play( BGM_STG );
			// イベント初期化
			CEVENT::Init();
			// ウィンドウ生成
			SetWindow( new CWINDOW() );
			// 自機生成
			SetPlayer( new CPLAYER() );
			// コンボ
			SetCombo( new CCOMBO() );
			// 星
			for ( long i = 0; i < 64; i++ )
			{
				new CBG( F(RAND(0,640)), F(RAND(0,400)), 736, 382, 768, 414, RAND(10,100)/500.0f );
			}
			// 雪
			for ( long i = 0; i < 96; i++ )
			{
				new CSNOW( F(RAND(0,640)), F(RAND(0,400)), F(RAND(50,100)/400.0f), -F(RAND(256,768)+2048)*PAI2/4096.0f );
			}
			// 月
			new CBG( 320, 100, 586, 360, 736, 510 );
			// 背景
			new CSCROLL( 320, 400-180, 0, 0, 640, 180, -3 );
			new CSCROLL( 320, 400-120, 0, 180, 640, 300, -5 );
			// 初期化
			NoMissCounter = EnemyDestroy = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- メイン
	case 1:
		{
			//------------------------------------------------------
			// 背景描画処理
			//------------------------------------------------------
			RECT dest;
			D3DCOLOR color[4];

			LunaEffect::Begin( BLEND_NORMAL );

			color[0] = D3DCOLOR_XRGB(4,4,64),
			color[1] = D3DCOLOR_XRGB(4,4,64),
			color[2] = D3DCOLOR_XRGB(16,16,128),
			color[3] = D3DCOLOR_XRGB(16,16,128),
			SetRect( &dest, 0, 0, 640, 250 );
			LunaEffect::Gradation( &dest, color );

			color[0] = D3DCOLOR_XRGB(16,16,128),
			color[1] = D3DCOLOR_XRGB(16,16,128),
			color[2] = D3DCOLOR_XRGB(64,64,255),
			color[3] = D3DCOLOR_XRGB(64,64,255),
			SetRect( &dest, 0, 250, 640, 400 );
			LunaEffect::Gradation( &dest, color );

			LunaEffect::End();
			//------------------------------------------------------
			// イベント処理
			//------------------------------------------------------
			CEVENT::Drive();
		}
		break;
	}
}

//======================================================================================
//
//	ランキング
//
//======================================================================================
void CApplication::SceneRanking( void )
{
	static long SceneTime = 0;
	static long Angle = 0;
	RECT src, dest;

	//------------------------------------------------------------------------------- 背景
	SetRect( &dest, 0, 0, 640, 480 );
	D3DCOLOR color[] = {
		D3DCOLOR_XRGB(64,64,32),
		D3DCOLOR_XRGB(64,64,32),
		D3DCOLOR_XRGB(192,192,96),
		D3DCOLOR_XRGB(192,192,96),
	};
	LunaEffect::Begin( BLEND_NORMAL );
	LunaEffect::Gradation( &dest, color );
	LunaEffect::End();

	//------------------------------------------------------------------------------- タイトル文字
	SetRect( &src, 256, 464, 512, 512 );
	SetRect( &dest, 320-256, 40-32, 320+256, 40+32 );
	SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_ADD );

	Angle += 96;
	long rot1 = Angle;

	//------------------------------------------------------------------------------- 各スコアデータ表示
	for ( long i = 0; i < 10; i++ )
	{
		long py = 85 + (i*38);
		char score[11] = "";
		char *name = ScoreData[i].Name;
		sprintf( score, "%010d", ScoreData[i].Score );

		long rot2 = rot1;

		//---------------------------------------------
		// 名前描画
		//---------------------------------------------
		for ( long j = 0; j < 8; j++ )
		{
			long c = name[j];
			long px = 80 + (j*18);
			long dy = py + LunaMath::Sin( rot2, 3 );

			SetRect( &dest, px, dy, px+16, dy+32 );

			if ( c != '\0' && c != ' ' )
			{
				switch ( c )
				{
				case '-':
					SetRect( &src, 224, 268, 224+16, 268+32 );
					break;
				case '^':
					SetRect( &src, 240, 268, 240+16, 268+32 );
					break;
				case '.':
					SetRect( &src, 256, 268, 256+16, 268+32 );
					break;
				default:
					if ( '0' <= c && c <= '9' )
					{
						c = (c - '0') * 16;
						SetRect( &src, 192+c, 140, 192+c+16, 172 );
					}
					ef ( 'A' <= c && c <= 'Z' )
					{
						long xx = ((c-'A')  & 7) * 16;
						long yy = ((c-'A') >> 3) * 32;
						SetRect( &src, 192+xx, 172+yy, 192+xx+16, 172+yy+32 );
					}
					break;
				}
				src.right++;
				src.bottom++;
				SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
			}
			rot2 += 256;
		}

		//---------------------------------------------
		// ￥マーク描画
		//---------------------------------------------
		long dy = py + LunaMath::Sin( rot2, 3 );
		SetRect( &dest, 360, dy, 360+16, dy+32 );
		SetRect( &src, 320, 268, 320+16, 268+32 );
		src.right++;
		src.bottom++;
		SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
		rot2 += 256;

		//---------------------------------------------
		// スコア描画
		//---------------------------------------------
		for ( long j = 0; j < 10; j++ )
		{
			long c = (score[j] - '0') * 16;
			long px = 380 + (j*18);
			long dy = py + LunaMath::Sin( rot2, 3 );

			SetRect( &dest, px, dy, px+16, dy+32 );
			SetRect( &src, 192+c, 140, 192+c+16, 172 );
			src.right++;
			src.bottom++;
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );

			rot2 += 256;
		}
		rot1 += 384;
	}

	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 破片終了までしばし待機
	case 0:
		if ( ++SceneTime > 100 )
		{
			SceneTime = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- キー入力で終了
	case 1:
		if ( IO::GetKey( KEYSTATE_PUSH, KEY_01 ) || IO::GetKey( KEYSTATE_PUSH, KEY_02 ) )
		{
			CSound::Play( SE_DECIDE );
			SceneTime = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- フェードアウト　
	case 2:
		{
			long fade = LunaMath::FlatSpeed( 0L, 255L, 50, SceneTime );
			SetFade( D3DCOLOR_ARGB(fade,0,0,0) );
			if ( ++SceneTime > 50 )
			{
				SceneTime = 0;
				SetScene( SCENE_TITLE );
			}
		}
		break;
	}
}

//======================================================================================
//
//	オプション
//
//======================================================================================
void CApplication::SceneOption( void )
{
	static long SceneTime;
	RECT src, dest;

	//------------------------------------------------------------------------------- 背景
	SetRect( &dest, 0, 0, 640, 480 );
	D3DCOLOR color[] = {
		D3DCOLOR_XRGB(64,32,64),
		D3DCOLOR_XRGB(192,96,192),
		D3DCOLOR_XRGB(64,32,64),
		D3DCOLOR_XRGB(192,96,192),
	};
	LunaEffect::Begin( BLEND_NORMAL );
	LunaEffect::Gradation( &dest, color );
	LunaEffect::End();

	//------------------------------------------------------------------------------- タイトル文字
	SetRect( &src, 256, 416, 512, 464 );
	SetRect( &dest, 320-256, 40-32, 320+256, 40+32 );
	SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_ADD );


	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 破片終了までしばし待機
	case 0:
		if ( ++SceneTime > 100 )
		{
			SceneTime = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- キー入力で終了
	case 1:
		if ( IO::GetKey( KEYSTATE_PUSH, KEY_02 ) )
		{
			SceneTime = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- フェードアウト　
	case 2:
		{
			long fade = LunaMath::FlatSpeed( 0L, 255L, 50, SceneTime );
			SetFade( D3DCOLOR_ARGB(fade,0,0,0) );
			if ( ++SceneTime > 50 )
			{
				SceneTime = 0;
				SetScene( SCENE_TITLE );
			}
		}
		break;
	}
}

//======================================================================================
//
//	ゲームオーバー
//
//======================================================================================
void CApplication::SceneGameover( void )
{
	static long Px, Py, Sx, Sy, Time;
	static unsigned long Gold;

	//------------------------------------------------------------------------------- STGパート背景
	if ( ScenePhase < 5 )
	{
		RECT dest;
		D3DCOLOR color[4];

		LunaEffect::Begin( BLEND_NORMAL );

		color[0] = D3DCOLOR_XRGB(4,4,64),
		color[1] = D3DCOLOR_XRGB(4,4,64),
		color[2] = D3DCOLOR_XRGB(16,16,128),
		color[3] = D3DCOLOR_XRGB(16,16,128),
		SetRect( &dest, 0, 0, 640, 250 );
		LunaEffect::Gradation( &dest, color );

		color[0] = D3DCOLOR_XRGB(16,16,128),
		color[1] = D3DCOLOR_XRGB(16,16,128),
		color[2] = D3DCOLOR_XRGB(64,64,255),
		color[3] = D3DCOLOR_XRGB(64,64,255),
		SetRect( &dest, 0, 250, 640, 400 );
		LunaEffect::Gradation( &dest, color );

		LunaEffect::End();
	}

	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 初期化
	case 0:
		{
			// シューティングパートでのデータ取得
			Gold = 0;
			GetData( NULL, &Gold, NULL );
			// 初期化
			Px = 320;
			Py = 160;
			Sx = 0;
			Sy = 0;
			Time = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- GAMEOVER 表示開始
	case 1:
		{
			Sx = (long)LunaMath::Neville( 1000.0f, 230.0f, 256.0f, F(Time)/25.0f );
			Sy = (long)LunaMath::Neville(    0.0f,  80.0f,  64.0f, F(Time)/25.0f );
			RECT dest = { Px-Sx, Py-Sy, Px+Sx, Py+Sy };
			RECT src = { 768, 446-64, 1024, 510-64 };
			Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
			if ( ++Time > 25 )
			{
				Time = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- GAMEOVER 表示
	case 2:
		{
			RECT dest = { Px-Sx, Py-Sy, Px+Sx, Py+Sy };
			RECT src = { 768, 446-64, 1024, 510-64 };
			Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
			if ( ++Time > 100 )
			{
				Time = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- GAMEOVER 表示終了
	case 3:
		{
			Sx = (long)LunaMath::Neville( 256.0f, 230.0f, 1000.0f, F(Time)/25.0f );
			Sy = (long)LunaMath::Neville(  64.0f,  80.0f,    0.0f, F(Time)/25.0f );
			RECT dest = { Px-Sx, Py-Sy, Px+Sx, Py+Sy };
			RECT src = { 768, 446-64, 1024, 510-64 };
			Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
			if ( ++Time > 25 )
			{
				Time = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- フェードアウト
	case 4:
		{
			long alpha = LunaMath::FlatSpeed( 0L, 255L, 200, Time );
			SetFade( D3DCOLOR_ARGB(alpha,0,0,0) );
			if ( ++Time > 200 )
			{
				Luna3D::Clear();
				// リスト開放
				SpriteReset();
				Time = 0;
				// ネームエントリーするかチェック
				if ( ScoreData[9].Score < Gold )
				{
					ScenePhase = 5;
				}
				else
				{
					ScenePhase = 6;
				}
			}
		}
		break;
	//------------------------------------------------------------------------------- ネームエントリー
	case 5:
		if ( NameEntry( Gold ) ) ScenePhase++;
		break;
	//------------------------------------------------------------------------------- メッセージ表示開始
	case 6:
		if ( Message() ) SetScene( SCENE_TITLE );
		break;
	}
}

//======================================================================================
//
//	クリアー
//
//======================================================================================
void CApplication::SceneClear( void )
{
	static long Px, Py, Sx, Sy, Time;
	static unsigned long Gold;
	static char Name[10] = "";

	//------------------------------------------------------------------------------- STGパート背景
	if ( ScenePhase < 5 )
	{
		RECT dest;
		D3DCOLOR color[4];

		LunaEffect::Begin( BLEND_NORMAL );

		color[0] = D3DCOLOR_XRGB(4,4,64),
		color[1] = D3DCOLOR_XRGB(4,4,64),
		color[2] = D3DCOLOR_XRGB(16,16,128),
		color[3] = D3DCOLOR_XRGB(16,16,128),
		SetRect( &dest, 0, 0, 640, 250 );
		LunaEffect::Gradation( &dest, color );

		color[0] = D3DCOLOR_XRGB(16,16,128),
		color[1] = D3DCOLOR_XRGB(16,16,128),
		color[2] = D3DCOLOR_XRGB(64,64,255),
		color[3] = D3DCOLOR_XRGB(64,64,255),
		SetRect( &dest, 0, 250, 640, 400 );
		LunaEffect::Gradation( &dest, color );

		LunaEffect::End();
	}

	switch ( ScenePhase )
	{
	//------------------------------------------------------------------------------- 初期化
	case 0:
		{
			// シューティングパートでのデータ取得
			Gold = 0;
			GetData( NULL, &Gold, NULL );
			// 初期化
			Px = 320;
			Py = 160;
			Sx = 0;
			Sy = 0;
			Time = 0;
			ScenePhase++;
		}
		break;
	//------------------------------------------------------------------------------- CLEAR 表示開始
	case 1:
		{
			Sx = (long)LunaMath::Neville( 1000.0f, 230.0f, 256.0f, F(Time)/25.0f );
			Sy = (long)LunaMath::Neville(    0.0f,  80.0f,  64.0f, F(Time)/25.0f );
			RECT dest = { Px-Sx, Py-Sy, Px+Sx, Py+Sy };
			RECT src = { 768, 446, 1024, 510 };
			Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
			if ( ++Time > 25 )
			{
				Time = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- CLEAR 表示
	case 2:
		{
			RECT dest = { Px-Sx, Py-Sy, Px+Sx, Py+Sy };
			RECT src = { 768, 446, 1024, 510 };
			Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
			if ( ++Time > 100 )
			{
				Time = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- CLEAR 表示終了
	case 3:
		{
			Sx = (long)LunaMath::Neville( 256.0f, 230.0f, 1000.0f, F(Time)/25.0f );
			Sy = (long)LunaMath::Neville(  64.0f,  80.0f,    0.0f, F(Time)/25.0f );
			RECT dest = { Px-Sx, Py-Sy, Px+Sx, Py+Sy };
			RECT src = { 768, 446, 1024, 510 };
			Sprite[9][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );
			if ( ++Time > 25 )
			{
				Time = 0;
				ScenePhase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- フェードアウト
	case 4:
		{
			long alpha = LunaMath::FlatSpeed( 0L, 255L, 200, Time );
			SetFade( D3DCOLOR_ARGB(alpha,0,0,0) );
			if ( ++Time > 200 )
			{
				Luna3D::Clear();
				// リスト開放
				SpriteReset();
				Time = 0;
				// ネームエントリーするかチェック
				if ( ScoreData[9].Score < Gold )
				{
					ScenePhase = 5;
				}
				else
				{
					ScenePhase = 6;
				}
			}
		}
		break;
	//------------------------------------------------------------------------------- ネームエントリー
	case 5:
		if ( NameEntry( Gold ) ) ScenePhase++;
		break;
	//------------------------------------------------------------------------------- メッセージ表示開始
	case 6:
		if ( Message() ) SetScene( SCENE_TITLE );
		break;
	}
}

//======================================================================================
//
//	ネームエントリー
//
//======================================================================================
bool CApplication::NameEntry( unsigned long gold )
{
	static long Phase = 0;
	static long Cursor, Select, KeyTime, Time, Fade;
	static char Name[12];
	char GoldStr[12] = "";
	RECT src, dest;

	// 位置テーブル
	const long px[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
	};
	// 位置テーブル
	const long py[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4,
	};
	// 文字変換テーブル
	const char StrTbl[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', '-', '^', '.', '\0', '\b', ' ',
	};

	// 背景
	SetRect( &dest, 0, 0, 640, 480 );
	D3DCOLOR color[] = {
		D3DCOLOR_XRGB(192,192,192),
		D3DCOLOR_XRGB(192,192,192),
		D3DCOLOR_XRGB(64,64,64),
		D3DCOLOR_XRGB(64,64,64),
	};
	LunaEffect::Begin( BLEND_NORMAL );
	LunaEffect::Gradation( &dest, color );
	LunaEffect::End();

	sprintf( GoldStr, "%010d", gold );

	switch ( Phase )
	{
	// ------------------------------------------------------------------------- 初期化
	case 0:
		memset( Name, 0x00, sizeof(char[12]) );
		Time = 0;
		Cursor = 0;
		Select = 10;
		KeyTime = 0;
		Luna3D::Clear();
		CSound::Play( BGM_TITLE );
		Phase++;
		break;
	// ------------------------------------------------------------------------- フェードイン
	case 1:
		Fade = LunaMath::FlatSpeed( 255L, 0L, 50, Time );
		SetFade( D3DCOLOR_ARGB(Fade,0,0,0) );
		if ( ++Time > 50 )
		{
			Time = 0;
			Phase++;
		}
		break;
	// ------------------------------------------------------------------------- 入力画面
	case 2:
		break;
	// ------------------------------------------------------------------------- フェードアウト
	case 3:
		Fade = LunaMath::FlatSpeed( 0L, 255L, 50, Time );
		SetFade( D3DCOLOR_ARGB(Fade,0,0,0) );
		if ( ++Time > 50 )
		{
			SetFade( 0 );
			Phase = 0;
			// 名前データコンバート
			for ( long i = 0; i < 8; i++ )
			{
				Name[i] = StrTbl[Name[i]];
			}
			if ( Name[0] == '\0' ) strcpy( Name, "AYAME" );
			// データ保存
			SaveScore( gold, Name );
			Luna3D::Clear();
			return true;
		}
		break;
	}

	// 最後の文字に選択権はなし
	if ( Cursor == 8 ) Select = 39;

	// ￥マーク描画
	SetRect( &dest, 210, 150, 210+16, 150+32 );
	SetRect( &src, 320, 268, 320+16, 268+32 );
	src.right++;
	src.bottom++;
	SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );

	// スコア描画
	for ( long j = 0; j < 10; j++ )
	{
		long c = (GoldStr[j] - '0') * 16;
		long dx = 230 + (j*18);
		long dy = 150;

		SetRect( &dest, dx, dy, dx+16, dy+32 );
		SetRect( &src, 192+c, 140, 192+c+16, 140+32 );
		src.right++;
		src.bottom++;
		SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_XRGB(255,255,255), BLEND_NORMAL );
	}

	// 名前描画
	for ( long i = 0; i < Cursor+1; i++ )
	{
		static long cnt = 0;
		long num = Name[i];
		D3DCOLOR c = D3DCOLOR_XRGB(255,255,255);

		if ( i == Cursor )
		{
			num = Select;
			c = D3DCOLOR_XRGB(0,255,255);
		}
		// 途中のスペースは描画しない
		if ( !(StrTbl[num] == ' ' && i != Cursor) )
		{
			long sx = 192 + (px[num] * 16);
			long sy = 140 + (py[num] * 32);
			long dx = 230 + (i*18);
			long dy = 230;
			SetRect( &dest, dx, dy, dx+16, dy+32 );
			SetRect( &src, sx, sy, sx+16, sy+32 );
			src.right++;
			src.bottom++;
			SubGraphic->BlitToTarget( &dest, &src, c, BLEND_NORMAL );
		}
	}

	if ( Phase < 3 )
	{
		// 下ボタン
		if ( IO::GetKey( KEYSTATE_PUSH, KEY_UP ) )
		{
			CSound::Play( SE_CURSOR );
			KeyTime = 0;
			if ( --Select < 0 ) Select = 41;
		}
		// 上ボタン
		if ( IO::GetKey( KEYSTATE_PUSH, KEY_DOWN ) )
		{
			CSound::Play( SE_CURSOR );
			KeyTime = 0;
			if ( ++Select > 41 ) Select = 0;
		}
		// 上・キーリピート
		if ( IO::GetKey( KEYSTATE_HOLD, KEY_UP ) )
		{
			// 1/2秒間押しっぱなしで
			if ( ++KeyTime > 25 )
			{
				// サクサク進める
				static long cnt = 0;
				if ( (++cnt%3) == 0 )
				{
					CSound::Play( SE_CURSOR );
					if ( --Select == -1 ) Select = 41;
				}
			}
		}
		// 下・キーリピート
		if ( IO::GetKey( KEYSTATE_HOLD, KEY_DOWN ) )
		{
			// 1/2秒間押しっぱなしで
			if ( ++KeyTime > 25 )
			{
				// サクサク進める
				static long cnt = 0;
				if ( (++cnt%3) == 0 )
				{
					CSound::Play( SE_CURSOR );
					if ( ++Select == 42 ) Select = 0;
				}
			}
		}
		if ( IO::GetKey( KEYSTATE_PUSH, KEY_02 ) )
		{
			CSound::Play( SE_CANCEL );
			if ( Cursor > 0 )
			{
				// バックスペース
				Name[Cursor] = '\0';
				Select = Name[Cursor-1];
				Cursor--;
			}
		}
		if ( IO::GetKey( KEYSTATE_PUSH, KEY_01 ) )
		{
			CSound::Play( SE_DECIDE );
			Name[Cursor] = (char)Select;
			// 終了
			if ( StrTbl[Select] == '\0' )
			{
				// 次
				Phase++;
			}
			// バックスペース
			ef ( StrTbl[Select] == '\b' )
			{
				if ( Cursor > 0 )
				{
					Name[Cursor] = '\0';
					Select = Name[Cursor-1];
					Cursor--;
				}
			}
			// 他
			else
			{
				Cursor++;
			}
		}
	}

	return false;
}

//======================================================================================
//
//	メッセージ
//
//======================================================================================
bool CApplication::Message( void )
{
	static long Phase = 0;
	static long Time = 0;

	switch ( Phase )
	{
	//------------------------------------------------------------------------------- メッセージ表示開始
	case 0:
		{
			SetFade( 0 );
			Luna3D::Clear();
			long alpha = LunaMath::FlatSpeed( 0L, 255L, 50, Time );
			RECT src = { 0, 512-24, 256, 512 };
			RECT dest = { 640-20-256, 480-10-24, 640-20, 480-10 };
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_ARGB(alpha,255,255,255), BLEND_ADD );
			if ( ++Time > 50 )
			{
				Time = 0;
				Phase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- メッセージ表示
	case 1:
		{
			Luna3D::Clear();
			RECT src = { 0, 512-24, 256, 512 };
			RECT dest = { 640-20-256, 480-10-24, 640-20, 480-10 };
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_ARGB(255,255,255,255), BLEND_ADD );
			if ( ++Time > 150 )
			{
				Time = 0;
				Phase++;
			}
		}
		break;
	//------------------------------------------------------------------------------- メッセージ表示終了
	case 2:
		{
			Luna3D::Clear();
			long alpha = LunaMath::FlatSpeed( 255L, 0L, 50, Time );
			RECT src = { 0, 512-24, 256, 512 };
			RECT dest = { 640-20-256, 480-10-24, 640-20, 480-10 };
			SubGraphic->BlitToTarget( &dest, &src, D3DCOLOR_ARGB(alpha,255,255,255), BLEND_ADD );
			if ( ++Time > 50 )
			{
				Luna3D::Clear();
				SetScene( SCENE_TITLE );
				Time = 0;
				Phase = 0;
				return true;
			}
		}
		break;
	}

	return false;
}

//======================================================================================
//
//	スコア書き出し
//
//======================================================================================
void CApplication::SaveScore( unsigned long score, char *name )
{
	ScoreData[9].Score = score;
	memset( ScoreData[9].Name, 0x00, sizeof(char[12]) );
	for ( long i = 0; name[i] != '\0'; i++ )
	{
		ScoreData[9].Name[i] = name[i];
	}

	// ソート
	for ( long i = 0; i < 10; i++ )
	{
		for ( long j = 0; j < 10; j++ )
		{
			if ( ScoreData[i].Score > ScoreData[j].Score )
			{
				SCOREDATA sd = ScoreData[i];
				ScoreData[i] = ScoreData[j];
				ScoreData[j] = sd;
			}
		}
	}

	LunaSystem::SaveData( "score.dat", ScoreData, sizeof(SCOREDATA[10]) );
}

//======================================================================================
//
//	スコア読み込み
//
//======================================================================================
void CApplication::LoadScore( void )
{
	LunaSystem::LoadData( "score.dat", ScoreData, sizeof(SCOREDATA[10]) );
}

//======================================================================================
//
//	画面エフェクト
//
//======================================================================================
void CApplication::ScreenEffect( void )
{
	LunaEffect::Begin( BLEND_NORMAL );

	// エフェクト系
	for ( unsigned long i = 0; i < EffectNum; i++ )
	{
		EffectList[i]->Draw();
		EffectList[i]->Release();
	}
	EffectNum = 0;

	// 画面フェード用
	if ( FadeColor )
	{
		RECT dest = { 0, 0, 640, 480 };
		LunaEffect::FillSquare( &dest, FadeColor );
	}

	LunaEffect::End();
}

//======================================================================================
//
//	画面エフェクト追加
//
//======================================================================================
void CApplication::AddEffect( LPCSPRITE p )
{
	// エフェクト追加
	if ( EffectNum < EFFECT_MAX )
	{
		EffectList[EffectNum] = p;
		EffectList[EffectNum]->AddRef();
		EffectNum++;
	}
}

//======================================================================================
//
//	ノーミスボーナスカウンター
//
//======================================================================================
void CApplication::SetNoMissCounter( bool flag )
{
	// 敵を逃したとき
	if ( !flag )
	{
		NoMissCounter = 0;
	}
	// 敵を倒したとき
	else
	{
		if ( ++NoMissCounter == 15 )
		{
			NoMissCounter = 0;
			new CBONUS( BONUS_NOMISS );
		}
	}
}

//======================================================================================
//
//	データ取得
//
//======================================================================================
void CApplication::GetData( unsigned long *score, unsigned long *gold, long *hit )
{
	if ( score != NULL ) *score = lpWindow->Score;
	if ( gold != NULL ) *gold = lpWindow->Gold;
	if ( hit != NULL ) *hit = lpCombo->MaxHit;
}

//======================================================================================
//
//	スプライトデータリセット
//
//======================================================================================
void CApplication::SpriteReset( void )
{
	SpriteList.Release();
	for ( long i = 0; i < 8; i++ )
	{
		Sprite[i][0]->InitDraw();
		Sprite[i][1]->InitDraw();
	}
}

//======================================================================================
//
//	あたり判定
//
//======================================================================================
void CApplication::AllHitCheck( void )
{
	//--------------------------------------------
	// 自機＆妖精とコイン
	//--------------------------------------------
	{
		LPCSPRITE p1 = (LPCSPRITE)SpriteList.Top();
		while ( p1 != NULL )
		{
			if ( p1->Sort == SORT_PLAYER )
			{
				LPCSPRITE p2 = (LPCSPRITE)SpriteList.Top();
				while ( p2 != NULL )
				{
					if ( p2->Sort == SORT_COIN )
					{
						POLYGON2D poly1 = { p1->HitCount, p1->lpHitPt };
						POLYGON2D poly2 = { p2->HitCount, p2->lpHitPt };
						if ( LunaCollision::Polygon_Polygon( poly1, poly2 ) )
						{
							AddGold( 50000 );
							p2->Damage( 0 );
						}
					}
					p2 = (LPCSPRITE)p2->Next;
				}
			}
			p1 = (LPCSPRITE)p1->Next;
		}
	}
	//--------------------------------------------
	// 自機とアイテム
	//--------------------------------------------
	{
		LPCPLAYER p = lpPlayer;
		if ( p != NULL )
		{
			LPCSPRITE p1 = (LPCSPRITE)SpriteList.Top();
			while ( p1 != NULL )
			{
				if ( p1->Sort == SORT_ITEM )
				{
					POLYGON2D poly1 = { p->HitCount, p->lpHitPt };
					POLYGON2D poly2 = { p1->HitCount, p1->lpHitPt };
					if ( LunaCollision::Polygon_Polygon( poly1, poly2 ) )
					{
						p1->Damage( 0 );
					}
				}
				p1 = (LPCSPRITE)p1->Next;
			}
		}
	}
	//--------------------------------------------
	// 自機と敵機＆敵機弾
	//--------------------------------------------
	{
		LPCPLAYER p = lpPlayer;
		if ( p != NULL )
		{
			if ( p->bHit )
			{
				LPCSPRITE p1 = (LPCSPRITE)SpriteList.Top();
				while ( p1 != NULL )
				{
					if ( p1->bHit )
					{
						if ( p1->Sort == SORT_ENEMY )
						{
							POLYGON2D poly1 = { p->HitCount, p->lpHitPt };
							POLYGON2D poly2 = { p1->HitCount, p1->lpHitPt };
							if ( LunaCollision::Polygon_Polygon( poly1, poly2 ) )
							{
								p->Damage( p1->Power );
								goto NEXT;
							}
						}						
						if ( p1->Sort == SORT_ENEMYSHOT )
						{
							POLYGON2D poly1 = { p->HitCount, p->lpHitPt };
							POLYGON2D poly2 = { p1->HitCount, p1->lpHitPt };
							if ( LunaCollision::Polygon_Polygon( poly1, poly2 ) )
							{
								p->Damage( p1->Power );
								p1->Damage( p->Power );
								goto NEXT;
							}
						}
					}
					p1 = (LPCSPRITE)p1->Next;
				}
			}
		}
	}
NEXT:
	//--------------------------------------------
	// 自機弾と敵機
	//--------------------------------------------
	{
		LPCSPRITE p1 = (LPCSPRITE)SpriteList.Top();
		while ( p1 != NULL )
		{
			if ( p1->bHit && (p1->Sort == SORT_ENEMY) )
			{
				LPCSPRITE p2 = (LPCSPRITE)SpriteList.Top();
				while ( p2 != NULL )
				{
					if ( p2->bHit && (p2->Sort == SORT_PLAYERSHOT) )
					{
						POLYGON2D poly1 = { p1->HitCount, p1->lpHitPt };
						POLYGON2D poly2 = { p2->HitCount, p2->lpHitPt };
						if ( LunaCollision::Polygon_Polygon( poly1, poly2 ) )
						{
							p1->Damage( p2->Power );
							p2->Damage( p1->Power );
						}
					}
					p2 = (LPCSPRITE)p2->Next;
				}
			}
			p1 = (LPCSPRITE)p1->Next;
		}
	}
	//--------------------------------------------
	// 自機レーザーと敵機
	//--------------------------------------------
	{
		LPCSPRITE p1 = (LPCSPRITE)SpriteList.Top();
		while ( p1 != NULL )
		{
			if ( p1->bHit && (p1->Sort == SORT_ENEMY) )
			{
				LPCSPRITE p2 = (LPCSPRITE)SpriteList.Top();
				while ( p2 != NULL )
				{
					if ( p2->bHit && (p2->Sort == SORT_PLAYERLASER) )
					{
						long num = p1->HitCount-1;
						for ( long i = 0; i < num; i++ )
						{
							LINE2D l = { 
								p2->lpHitPt[i+0].x, p2->lpHitPt[i+0].y,
								p2->lpHitPt[i+1].x, p2->lpHitPt[i+1].y
							};
							POLYGON2D poly = { p1->HitCount, p1->lpHitPt };
							if ( LunaCollision::Line_Polygon( l, poly ) )
							{
								p1->Damage( p2->Power );
								p2->Damage( p1->Power );
							}
						}
					}
					p2 = (LPCSPRITE)p2->Next;
				}
			}
			p1 = (LPCSPRITE)p1->Next;
		}
	}
}
