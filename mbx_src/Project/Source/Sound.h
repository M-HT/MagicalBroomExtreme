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


//======================================================================================
// ENUM
//======================================================================================
enum eSoundType
{
	SE_CURSOR,
	SE_CANCEL,
	SE_DECIDE,
	SE_ITEM,
	SE_LASER,
	SE_ENEMY_SHOT,
	SE_BOSS_LASER,
	SE_CHARGE,
	SE_BOSS,
	SE_BIT,
	SE_COIN,
	SE_EXPLOSION,
	SE_MAGIC,
	SE_SHOT,
	SE_SHOT_HIT,
	SE_BOMB01,
	SE_BOMB02,
	SE_PLAYERHIT,
	SE_BIG_LASER,
	SE_BIG_LASER_CHARGE,
	SE_LOGO,
	BGM_TITLE,
	BGM_STG,
	BGM_BOSS,
};

//======================================================================================
// CLASS
//======================================================================================
class CSound
{
private:
	static BOOL bSound;
	static LSOUND BackGroundMusic;
	static LSOUND BossLaserSnd[32];
	static LSOUND EnemyShotSnd[32];
	static LSOUND BitSnd[32];
	static LSOUND BossSnd[32];
	static LSOUND CoinSnd[32];
	static LSOUND Bomb01Snd[32];
	static LSOUND Bomb02Snd[32];
	static LSOUND ChargeSnd[32];
	static LSOUND LaserSnd[32];
	static LSOUND ItemSnd[32];
	static LSOUND ShotSnd[32];
	static LSOUND ShotHitSnd[32];
	static LSOUND CursorSnd[32];
	static LSOUND CancelSnd;
	static LSOUND DecideSnd;
	static LSOUND PlayerHitSnd;
	static LSOUND MagicSnd;
	static LSOUND ExplosionSnd;
	static LSOUND BgiLaserSnd;
	static LSOUND BgiLaserChargeSnd;
	static LSOUND LogoSnd;

public:
	static void Init( void );
	static void Uninit( void );
	static void Play( eSoundType type );
};