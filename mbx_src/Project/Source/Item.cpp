//======================================================================================
//======================================================================================
//
//
//			�A�C�e��
//
//
//======================================================================================
//======================================================================================


//======================================================================================
//	include
//======================================================================================
#include "Item.h"
#include "ItemFx.h"
#include "Sonic.h"
#include "Application.h"
#include "Sound.h"


//======================================================================================
//
//	�R���X�g���N�^
//
//======================================================================================
CITEM::CITEM( float x, float y ) : CSPRITE( SORT_ITEM )
{
	SetHitCount( 5 );
	Px = x;
	Py = y;
	No = 0;
	bHit = true;
	long angle = 2048+RAND( -256, 256 );
	x1 = F(LunaMath::Cos( angle, 3<<16 )) / 65536.0f;
	y1 = F(LunaMath::Sin( angle, 3<<16 )) / 65536.0f;
}

//======================================================================================
//
//	����
//
//======================================================================================
bool CITEM::Action( void )
{
	// ��莞�ԂŐ؂�ւ�
	if ( (++Time%50) == 0 ) ++No %= 5;

	Px += x1;
	Py += y1;

	// �W��܂ŕǂɔ���
	if ( Count < 8 )
	{
		if ( Px-15 <   0 ) { Px = 15;  x1 = -x1; Count++; }
		if ( Px+15 > 640 ) { Px = 625; x1 = -x1; Count++; }
		if ( Py-15 <   0 ) { Py = 15;  y1 = -y1; Count++; }
		if ( Py+15 > 400 ) { Py = 385; y1 = -y1; Count++; }
	}
	else
	{
		if ( Px-15 < -15 ) return true;
		if ( Px+15 > 655 ) return true;
		if ( Py-15 < -15 ) return true;
		if ( Py+15 > 415 ) return true;
	}

	//-------------------------------------------
	// �`�揈��
	//-------------------------------------------
	RECT src = { 190+No*30, 410, 190+No*30+30, 440 };
	RECT dest = { (long)Px-15, (long)Py-15, (long)Px+15, (long)Py+15 };
	Sprite[1][0]->Draw( &dest, D3DCOLOR_XRGB(255,255,255), &src );

	//-------------------------------------------
	// �����蔻��
	//-------------------------------------------
	lpHitPt[0].x = Px-15;	lpHitPt[0].y = Py-15;
	lpHitPt[1].x = Px+15;	lpHitPt[1].y = Py-15;
	lpHitPt[2].x = Px+15;	lpHitPt[2].y = Py+15;
	lpHitPt[3].x = Px-15;	lpHitPt[3].y = Py+15;
	lpHitPt[4] = lpHitPt[0];

	return bDeath;
}

//======================================================================================
//
//	�_���[�W
//
//======================================================================================
void CITEM::Damage( long pow )
{
	bDeath = true;
	bHit = false;

	CSound::Play( SE_ITEM );

	switch ( No )
	{
	case 0: CApplication::SpeedUp();		break;
	case 1: CApplication::ShotLevelUp();	break;
	case 2: CApplication::BitLevelUp();		break;
	case 3: CApplication::FairyUp();		break;
	case 4: CApplication::LifeUp();			break;
	}

	new CITEMFX( Px, Py, No );
	new CSONIC( Px, Py, 200, 60, false, 0, D3DCOLOR_XRGB(255,255,255) );
}