/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			アプリケーション管理
//
//
//======================================================================================
//======================================================================================

#pragma once

//======================================================================================
// INCLUDE
//======================================================================================
#include "Base.h"
#include "Player.h"
#include "Window.h"
#include "Combo.h"


//======================================================================================
// ENUM
//======================================================================================
enum eScene
{
	SCENE_LOGO,
	SCENE_TITLE,
	SCENE_STG,
	SCENE_RANKING,
	SCENE_OPTION,
	SCENE_GAMEOVER,
	SCENE_CLEAR,
	sizeof_eScene
};


//======================================================================================
// STRUCT
//======================================================================================
typedef struct SOCEDATA
{
	char Name[12];
	unsigned long Score;
}
SCOREDATA, *LPSCOREDATA;

//======================================================================================
// CLASS
//======================================================================================
class CApplication
{
	enum { EFFECT_MAX = 4096 };

protected:
	static LPCPLAYER lpPlayer;
	static LPCWINDOW lpWindow;
	static LPCCOMBO lpCombo;

	static unsigned long EnemyDestroy;
	static unsigned long NoMissCounter;

	static LPCSPRITE EffectList[EFFECT_MAX];
	static unsigned long EffectNum;
	static D3DCOLOR FadeColor;

	static long ScenePhase;
	static long Difficulty;
	static long Stage;
	static SCOREDATA ScoreData[10];

	static void (*SceneFunc[sizeof_eScene])( void );
	static void SceneLogo( void );
	static void SceneTitle( void );
	static void SceneStg( void );
	static void SceneRanking( void );
	static void SceneOption( void );
	static void SceneGameover( void );
	static void SceneClear( void );

	static void AllHitCheck( void );

public:
	static void Initialize( void );
	static void UnInitialize( void );
	static void Main( void );

	static eScene NowScene;
	static void SetScene( eScene scene );

	static void SaveScore( unsigned long score, char *name );
	static void LoadScore( void );
	static void SetPlayer( LPCPLAYER p ) { lpPlayer = p; }
	static void SetWindow( LPCWINDOW p ) { lpWindow = p; }
	static void SetCombo( LPCCOMBO p ) { lpCombo = p; }

	static void ScreenEffect( void );
	static void AddEffect( LPCSPRITE p );

	static void AddScore( long score ) { lpWindow->AddScore( score ); }
	static void AddGold( long gold ) { lpWindow->AddGold( gold ); }
	static void SetAddRate( long add ) { lpWindow->SetAddRate( add ); }
	static void SetAddForce( long add ) { lpWindow->SetAddForce( add ); }
	static void AddCombo( void ) { lpCombo->Add(); }
	static long GetCombo( void ) { return lpCombo->GetCombo(); }
	static long GetRate( void ) { return lpWindow->GetRate(); }
	static bool BootBomb( bool flag ) { return lpWindow->BootBomb( flag ); }

	static void NextScene( void ) { ScenePhase++; }
	static void DestroyEnemy( void ) { EnemyDestroy++; }

	static LPCPLAYER GetPlayer( void ) { return lpPlayer; }

	static void SpeedUp( void ) { lpPlayer->SpeedUp(); }
	static void ShotLevelUp( void ) { lpPlayer->ShotLevelUp(); }
	static void BitLevelUp( void ) { lpPlayer->BitLevelUp(); }
	static void LifeUp( void ) { lpPlayer->LifeUp(); }
	static void FairyUp( void ) { lpPlayer->FairyUp(); }

	static void SetNoMissCounter( bool flag );

	static void SetFade( D3DCOLOR color ) { FadeColor = color; }

	static void GetData( unsigned long *score, unsigned long *gold, long *hit );
	static void SpriteReset( void );

	static bool NameEntry( unsigned long gold );
	static bool Message( void );

	static void SetLevel( long lv ) { Difficulty = lv; }
	static long GetLevel( void ) { return Difficulty; }
};