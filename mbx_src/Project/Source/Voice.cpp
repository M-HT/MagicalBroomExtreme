/* encoding = CP932 */
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
#include "Voice.h"


//======================================================================================
// DEFINE
//======================================================================================
#define VOICE_FILE "Voice.lsd"


//======================================================================================
//
//	データメンバ
//
//======================================================================================
BOOL CVoice::bVoice;
LSOUND CVoice::Start;
LSOUND CVoice::GameOver;
LSOUND CVoice::SpeedUp;
LSOUND CVoice::PowerUp;
LSOUND CVoice::BitUp;
LSOUND CVoice::Fairy;
LSOUND CVoice::LifeUp;
LSOUND CVoice::Bonus;
LSOUND CVoice::Bomb;
LSOUND CVoice::Clear;
LSOUND CVoice::Clear01;
LSOUND CVoice::Clear02;
LSOUND CVoice::Clear03;
LSOUND CVoice::Damage01;
LSOUND CVoice::Damage02;
LSOUND CVoice::EnergyMax;
LSOUND CVoice::NoDamage;
LSOUND CVoice::NoMiss;
LSOUND CVoice::RateUp;


//======================================================================================
//
//	ボイス初期化
//
//======================================================================================
void CVoice::Init( void )
{
	// ボイスファイルの有無チェック
	bVoice = LunaSystem::FileExist( VOICE_FILE );

	// ボイス
	if ( bVoice )
	{
		Start		= LunaSound::Load( VOICE_FILE, "start" );
		GameOver	= LunaSound::Load( VOICE_FILE, "gameover" );
		SpeedUp		= LunaSound::Load( VOICE_FILE, "speedup" );
		PowerUp		= LunaSound::Load( VOICE_FILE, "powerup" );
		BitUp		= LunaSound::Load( VOICE_FILE, "bitup" );
		Fairy		= LunaSound::Load( VOICE_FILE, "fairy" );
		LifeUp		= LunaSound::Load( VOICE_FILE, "lifeup" );
		Bonus		= LunaSound::Load( VOICE_FILE, "bonus" );
		Bomb		= LunaSound::Load( VOICE_FILE, "bomb" );
		Clear		= LunaSound::Load( VOICE_FILE, "clear" );
		Clear01		= LunaSound::Load( VOICE_FILE, "clear01" );
		Clear02		= LunaSound::Load( VOICE_FILE, "clear02" );
		Clear03		= LunaSound::Load( VOICE_FILE, "clear03" );
		Damage01	= LunaSound::Load( VOICE_FILE, "damage01" );
		Damage02	= LunaSound::Load( VOICE_FILE, "damage02" );
		EnergyMax	= LunaSound::Load( VOICE_FILE, "energymax" );
		NoDamage	= LunaSound::Load( VOICE_FILE, "nodamage" );
		NoMiss		= LunaSound::Load( VOICE_FILE, "nomiss" );
		RateUp		= LunaSound::Load( VOICE_FILE, "rateup" );
	}
}


//======================================================================================
//
//	ボイス開放
//
//======================================================================================
void CVoice::Uninit( void )
{
	if ( bVoice )
	{
		Start->Release();
		GameOver->Release();
		SpeedUp->Release();
		PowerUp->Release();
		BitUp->Release();
		Fairy->Release();
		LifeUp->Release();
		Bonus->Release();
		Bomb->Release();
		Clear->Release();
		Clear01->Release();
		Clear02->Release();
		Clear03->Release();
		Damage01->Release();
		Damage02->Release();
		EnergyMax->Release();
		NoDamage->Release();
		NoMiss->Release();
		RateUp->Release();
	}
}

//======================================================================================
//
//	ボイス再生
//
//======================================================================================
void CVoice::Play( eVoiceType type )
{
	if ( bVoice )
	{
		static LSOUND BeforeVoice = Start;

		switch ( type )
		{
		case VOICE_START:
			Start->Stop();
			Start->Play();
			break;
		case VOICE_GAMEOVER:
			GameOver->Stop();
			GameOver->Play();
			break;
		case VOICE_SPEEDUP:
			SpeedUp->Stop();
			SpeedUp->Play();
			break;
		case VOICE_POWERUP:
			PowerUp->Stop();
			PowerUp->Play();
			break;
		case VOICE_BITUP:
			BitUp->Stop();
			BitUp->Play();
			break;
		case VOICE_FAIRY:
			Fairy->Stop();
			Fairy->Play();
			break;
		case VOICE_LIFEUP:
			LifeUp->Stop();
			LifeUp->Play();
			break;
		case VOICE_BONUS:
			Bonus->Stop();
			Bonus->Play();
			break;
		case VOICE_CLEAR:
			Clear->Stop();
			Clear->Play();
			break;
		case VOICE_CLEAR01:
			Clear01->Stop();
			Clear01->Play();
			break;
		case VOICE_CLEAR02:
			Clear02->Stop();
			Clear02->Play();
			break;
		case VOICE_CLEAR03:
			Clear03->Stop();
			Clear03->Play();
			break;
		case VOICE_ENERGYMAX:
			EnergyMax->Stop();
			EnergyMax->Play();
			break;
		case VOICE_NODAMAGE:
			NoDamage->Stop();
			NoDamage->Play();
			break;
		case VOICE_NOMISS:
			NoMiss->Stop();
			NoMiss->Play();
			break;
		case VOICE_RATEUP:
			RateUp->Stop();
			RateUp->Play();
			break;
		case VOICE_BOMB:
			BeforeVoice->Stop();
			BeforeVoice = Bomb;
			Bomb->Stop();
			Bomb->Play();
			break;
		case VOICE_DAMAGE01:
			BeforeVoice->Stop();
			BeforeVoice = Damage01;
			Damage01->Stop();
			Damage01->Play();
			break;
		case VOICE_DAMAGE02:
			BeforeVoice->Stop();
			BeforeVoice = Damage02;
			Damage02->Stop();
			Damage02->Play();
			break;
		}
	}
}
