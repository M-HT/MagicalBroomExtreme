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
// INCLUDE
//======================================================================================
#include "stdio.h"
#include "Sound.h"


//======================================================================================
// DEFINE
//======================================================================================
#define SOUND_FILE "sound.lsd"


//======================================================================================
//
//	データメンバ
//
//======================================================================================
BOOL CSound::bSound;
LSOUND CSound::BackGroundMusic;
LSOUND CSound::BossLaserSnd[32];
LSOUND CSound::EnemyShotSnd[32];
LSOUND CSound::Bomb01Snd[32];
LSOUND CSound::Bomb02Snd[32];
LSOUND CSound::CoinSnd[32];
LSOUND CSound::LaserSnd[32];
LSOUND CSound::ChargeSnd[32];
LSOUND CSound::ItemSnd[32];
LSOUND CSound::BitSnd[32];
LSOUND CSound::BossSnd[32];
LSOUND CSound::ShotSnd[32];
LSOUND CSound::ShotHitSnd[32];
LSOUND CSound::CursorSnd[32];
LSOUND CSound::CancelSnd;
LSOUND CSound::DecideSnd;
LSOUND CSound::PlayerHitSnd;
LSOUND CSound::MagicSnd;
LSOUND CSound::ExplosionSnd;
LSOUND CSound::BgiLaserSnd;
LSOUND CSound::BgiLaserChargeSnd;
LSOUND CSound::LogoSnd;


//======================================================================================
//
//	サウンド初期化
//
//======================================================================================
void CSound::Init( void )
{
	// サウンドファイルの有無チェック
	bSound = LunaSystem::FileExist( SOUND_FILE );

	// サウンド
	if ( bSound )
	{
		BackGroundMusic = NULL;
		for ( long i = 0; i < 32; i++ )
		{
			ShotSnd[i] = LunaSound::Load( SOUND_FILE, "shot" );
			ShotHitSnd[i] = LunaSound::Load( SOUND_FILE, "shot_hit" );
			CoinSnd[i] = LunaSound::Load( SOUND_FILE, "coin" );
			BitSnd[i] = LunaSound::Load( SOUND_FILE, "bit" );
			BossSnd[i] = LunaSound::Load( SOUND_FILE, "boss" );
			Bomb01Snd[i] = LunaSound::Load( SOUND_FILE, "bomb01" );
			Bomb02Snd[i] = LunaSound::Load( SOUND_FILE, "bomb02" );
			ItemSnd[i] = LunaSound::Load( SOUND_FILE, "item" );
			LaserSnd[i] = LunaSound::Load( SOUND_FILE, "laser" );
			ChargeSnd[i] = LunaSound::Load( SOUND_FILE, "charge" );
			EnemyShotSnd[i] = LunaSound::Load( SOUND_FILE, "enemy_shot" );
			BossLaserSnd[i] = LunaSound::Load( SOUND_FILE, "boss_laser" );
			CursorSnd[i] = LunaSound::Load( SOUND_FILE, "cursor" );
		}
		BgiLaserChargeSnd = LunaSound::Load( SOUND_FILE, "big_laser_charge" );
		BgiLaserSnd = LunaSound::Load( SOUND_FILE, "big_laser" );
		PlayerHitSnd = LunaSound::Load( SOUND_FILE, "hit" );
		CancelSnd = LunaSound::Load( SOUND_FILE, "cancel" );
		DecideSnd = LunaSound::Load( SOUND_FILE, "decide" );
		MagicSnd = LunaSound::Load( SOUND_FILE, "magic" );
		ExplosionSnd = LunaSound::Load( SOUND_FILE, "explosion" );
		LogoSnd = LunaSound::Load( SOUND_FILE, "logo" );
	}
}


//======================================================================================
//
//	サウンド開放
//
//======================================================================================
void CSound::Uninit( void )
{
	if ( bSound )
	{
		for ( long i = 0; i < 32; i++ )
		{
			ShotSnd[i]->Release();
			ShotHitSnd[i]->Release();
			CoinSnd[i]->Release();
			BitSnd[i]->Release();
			BossSnd[i]->Release();
			Bomb01Snd[i]->Release();
			Bomb02Snd[i]->Release();
			CursorSnd[i]->Release();
			LaserSnd[i]->Release();
			ChargeSnd[i]->Release();
			EnemyShotSnd[i]->Release();
			BossLaserSnd[i]->Release();
			ItemSnd[i]->Release();
		}
		BgiLaserChargeSnd->Release();
		BgiLaserSnd->Release();
		MagicSnd->Release();
		ExplosionSnd->Release();
		PlayerHitSnd->Release();
		CancelSnd->Release();
		DecideSnd->Release();
		LogoSnd->Release();
	}
}

//======================================================================================
//
//	サウンド再生
//
//======================================================================================
void CSound::Play( eSoundType type )
{
	if ( bSound )
	{
		unsigned long NowTime = Luna::GetCounter();

		switch ( type )
		{
		case BGM_TITLE:
			{
				if ( BackGroundMusic != NULL ) BackGroundMusic->Release();
				BackGroundMusic = LunaSound::Load( SOUND_FILE, "bgm00" );
				BackGroundMusic->Play( true );
			}
			break;
		case BGM_STG:
			{
				if ( BackGroundMusic != NULL ) BackGroundMusic->Release();
				BackGroundMusic = LunaSound::Load( SOUND_FILE, "bgm01" );
				BackGroundMusic->Play( true );
			}
			break;
		case BGM_BOSS:
			{
				if ( BackGroundMusic != NULL ) BackGroundMusic->Release();
				BackGroundMusic = LunaSound::Load( SOUND_FILE, "bgm02" );
				BackGroundMusic->Play( true );
			}
			break;
		case SE_LOGO:
			{
				LogoSnd->Stop();
				LogoSnd->Play();
			}
			break;
		case SE_EXPLOSION:
			{
				ExplosionSnd->Stop();
				ExplosionSnd->Play();
			}
			break;
		case SE_MAGIC:
			{
				MagicSnd->Stop();
				MagicSnd->Play();
			}
			break;
		case SE_CANCEL:
			{
				CancelSnd->Stop();
				CancelSnd->Play();
			}
			break;
		case SE_DECIDE:
			{
				DecideSnd->Stop();
				DecideSnd->Play();
			}
			break;
		case SE_PLAYERHIT:
			{
				PlayerHitSnd->Stop();
				PlayerHitSnd->Play();
			}
			break;
		case SE_BIG_LASER:
			{
				BgiLaserSnd->Stop();
				BgiLaserSnd->Play();
			}
			break;
		case SE_BIG_LASER_CHARGE:
			{
				BgiLaserChargeSnd->Stop();
				BgiLaserChargeSnd->Play();
			}
			break;
		case SE_SHOT:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					ShotSnd[cnt]->Stop();
					ShotSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_SHOT_HIT:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					ShotHitSnd[cnt]->Stop();
					ShotHitSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_BOMB01:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					Bomb01Snd[cnt]->Stop();
					Bomb01Snd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_BOMB02:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					Bomb02Snd[cnt]->Stop();
					Bomb02Snd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_CURSOR:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					CursorSnd[cnt]->Stop();
					CursorSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_ITEM:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					ItemSnd[cnt]->Stop();
					ItemSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_CHARGE:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					ChargeSnd[cnt]->Stop();
					ChargeSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_LASER:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					LaserSnd[cnt]->Stop();
					LaserSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_COIN:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					CoinSnd[cnt]->Stop();
					CoinSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_BOSS:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					BossSnd[cnt]->Stop();
					BossSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_BIT:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					BitSnd[cnt]->Stop();
					BitSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_ENEMY_SHOT:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					EnemyShotSnd[cnt]->Stop();
					EnemyShotSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		case SE_BOSS_LASER:
			{
				// ３フレーム以内での多重再生はブロック
				static unsigned long OldTime = 0;
				if ( NowTime - OldTime > 2  )
				{
					OldTime = NowTime;
					static long cnt = 0;
					BossLaserSnd[cnt]->Stop();
					BossLaserSnd[cnt]->Play();
					++cnt &= 31;
				}
			}
			break;
		}
	}
}
