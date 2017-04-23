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
enum eVoiceType
{
	VOICE_START,
	VOICE_GAMEOVER,

	VOICE_SPEEDUP,
	VOICE_POWERUP,
	VOICE_BITUP,
	VOICE_FAIRY,
	VOICE_LIFEUP,
	VOICE_BONUS,

	VOICE_BOMB,

	VOICE_CLEAR,
	VOICE_CLEAR01,
	VOICE_CLEAR02,
	VOICE_CLEAR03,

	VOICE_DAMAGE01,
	VOICE_DAMAGE02,

	VOICE_ENERGYMAX,
	VOICE_NODAMAGE,
	VOICE_NOMISS,
	VOICE_RATEUP,
};

//======================================================================================
// CLASS
//======================================================================================
class CVoice
{
private:
	static BOOL bVoice;

	static LSOUND Start;
	static LSOUND GameOver;
	static LSOUND SpeedUp;
	static LSOUND PowerUp;
	static LSOUND BitUp;
	static LSOUND Fairy;
	static LSOUND LifeUp;
	static LSOUND Bonus;
	static LSOUND Bomb;
	static LSOUND Clear;
	static LSOUND Clear01;
	static LSOUND Clear02;
	static LSOUND Clear03;
	static LSOUND Damage01;
	static LSOUND Damage02;
	static LSOUND EnergyMax;
	static LSOUND NoDamage;
	static LSOUND NoMiss;
	static LSOUND RateUp;

public:
	static void Init( void );
	static void Uninit( void );
	static void Play( eVoiceType type );
};