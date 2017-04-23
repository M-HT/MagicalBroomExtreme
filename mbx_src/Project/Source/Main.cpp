/* encoding = CP932 */


//=====================================================================
// INCLDUE
//=====================================================================
#include "Application.h"
#include "Io.h"
#include "BossLaser.h"

//=====================================================================
// STRUCT
//=====================================================================
// コンフィグ
typedef struct _CONFIGDATA
{
	unsigned char IsWindow;
	unsigned char IsFullColor;
	unsigned char IsFullColorTexture;
}
CONFIGDATA, *LPCONFIGDATA;

//=====================================================================
// GLOBAL
//=====================================================================
CList SpriteList;
LTEXTURE SubGraphic;
LSPRITE Sprite[10][2];
RECT RefreshSrc = { 0, 0, 640, 480 };
RECT RefreshDest = { 0, 0, 640, 480 };
long QuakePhase = -1;
long QuakeType = -1;
CONFIGDATA Config = { 0, 0, 0 };


//=====================================================================
//
//		初期設定
//
//=====================================================================
void Luna::SetInfo( void )
{
	FILE *fp = fopen( "config.dat", "rb" );
	if ( fp != NULL )
	{
		fread( &Config, sizeof(CONFIGDATA), 1, fp );
		fclose( fp );
	}

	Luna::SetScreenMode( 640, 480, Config.IsWindow );

	// ウィンドウタイトル
	Luna::SetWindowTitle( "まぢかるぶる～むえくすとりぃむ" );

	// スクリーンモード毎の設定
	if ( !Config.IsWindow )
	{
		// マウスの表示設定
		Luna::SetShowMouse( FALSE );
		// IME邪魔なので消去
		LunaSystem::SetShowIME( FALSE );
	}
	else
	{
		// FPS表示設定
		Luna::SetShowFPS( true );
	}

	// フルカラー
	if ( Config.IsFullColor ) Luna3D::EnableFullColorMode();

	// フルカラーテクスチャ
	if ( Config.IsFullColorTexture ) Luna3D::EnableFullColorTexture();

	// ログファイル名
	Luna::SetLogFile( "Boot_Log.txt" );

	// FPS設定
	Luna::SetFPS( 50 );

	// 使用するDirectX
	Luna::SetUseOption( USE_DIRECT3D | USE_DIRECTSOUND | USE_DIRECTINPUT );
}

//=====================================================================
//
//		初期化処理
//
//=====================================================================
void Luna::Init( void )
{
	SpriteList.Init();

	// グラフィック読み込み
	LTEXTURE Graphic = LunaTexture::LoadLAG( "graphic.lag", "graphic", TRUE );
	SubGraphic = LunaTexture::LoadLAG( "graphic.lag", "sub_graphic", TRUE );

	// スプライト生成
	for ( long i = 0; i < 10; i++ )
	{
		if ( i == 6 )
		{
			Sprite[i][0] = LunaSprite::Create( 512, Graphic, BLEND_NORMAL );
			RECT uv = { 0 };
			Sprite[i][1] = LunaSprite::CreateLaser( LASER_DIV, &uv, Graphic, BLEND_ADD );
		}
		else
		{
			Sprite[i][0] = LunaSprite::Create( 512, Graphic, BLEND_NORMAL );
			Sprite[i][1] = LunaSprite::Create( 512, Graphic, BLEND_ADD );
		}
	}

	// 開放
	Graphic->Release();

	// ディザ
	Luna3D::SetRenderState( STATE_DITHER, ENABLE );

	// バイリニアフィルタ
	Luna3D::SetSmoothFilter( TRUE );

	// 初期化
	CApplication::Initialize();
}

//=====================================================================
//
//		終了処理
//
//=====================================================================
void Luna::UnInit( void )
{
	// IME復帰
	LunaSystem::SetShowIME( true );

	// 終了
	CApplication::UnInitialize();

	// スプライト
	for ( long i = 0; i < 8; i++ )
	{
		Sprite[i][0]->Release();
		Sprite[i][1]->Release();
	}

	SpriteList.Release();
	SubGraphic->Release();
}

//=====================================================================
//
//		メインループ
//
//=====================================================================
void Luna::MainLoop( void )
{
	// キー情報更新
	IO::RefreshKeyState();

	// シーン呼び出し
	CApplication::Main();

	// 画面振動処理
	QuakeScreen();

	// 画面更新
	Luna3D::Refresh( &RefreshDest, &RefreshSrc );
}

//=====================================================================
//
//		画面振動処理
//
//=====================================================================
void QuakeScreen( void )
{
	static long Angle, Time;
	long x = 320, y = 240, scale = 0;

	switch ( QuakeType )
	{
	/////////////////////////////////////////////////////////// 縦
	case 0:
		switch ( QuakePhase )
		{
		case 0:
			Angle = 0;
			Time = 0;
			QuakePhase++;
			break;
		case 1:
			scale = LunaMath::FlatSpeed( 6L, 0L, 50, Time );
			y = 240 + LunaMath::Sin( Angle+=384, scale );
			if ( ++Time == 50 ) QuakePhase++;
			break;
		}
		break;
	/////////////////////////////////////////////////////////// 横
	case 1:
		switch ( QuakePhase )
		{
		case 0:
			Angle = 0;
			Time = 0;
			QuakePhase++;
			break;
		case 1:
			scale = LunaMath::FlatSpeed( 8L, 0L, 50, Time );
			x = 320 + LunaMath::Sin( Angle+=384 , scale );
			if ( ++Time == 50 ) QuakePhase++;
			break;
		}
		break;
	}

	// カメラ設定
	RECT src = { 0, 0, 640, 480 };
	RECT dest = { x-320, y-240, x+320, y+240 };
	SetCamera( &dest, &src );
}

//=====================================================================
//
//		画面振動設定
//
//=====================================================================
void SetQuake( long type )
{
	QuakePhase = 0;
	QuakeType = type;
}

//=====================================================================
//
//		更新画面設定
//
//=====================================================================
void SetCamera( RECT *dest, RECT *src )
{
	RefreshDest = *dest;
	RefreshSrc = *src;
}