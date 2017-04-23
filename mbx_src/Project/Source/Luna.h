/* encoding = CP932 */
/********************************************************************************/
/*																				*/
/*			DirectX8.1用簡易ライブラリ Luna										*/
/*																				*/
/********************************************************************************/

/********************************************************************************/
/*																				*/
/*	最新版の置いてあるURL														*/
/*																				*/
/*		ゆきいるか																*/
/*			http://www5d.biglobe.ne.jp/~twintail/								*/
/*																				*/
/*	質問等発言場所																*/
/*																				*/
/*		BBS																		*/
/*			ゆきいるか内の掲示板												*/
/*																				*/
/*		E-MAIL																	*/
/*			yukiiruka@mvb.biglobe.ne.jp											*/
/*                                                                              */
/********************************************************************************/

#pragma once

//==========================================================================
// WARNING
//==========================================================================
#pragma warning ( disable: 4100 )
#pragma warning ( disable: 4201 )
#pragma warning ( disable: 4514 )
#pragma warning ( disable: 4127 )
#pragma warning ( disable: 4710 )
#pragma warning ( disable: 4711 )

#pragma inline_depth ( 255 )
#pragma inline_recursion ( on )


//==========================================================================
// DEFINE
//==========================================================================
#define D3D_OVERLOADS
#define DIRECTINPUT_VERSION		(0x0800)


//==========================================================================
// INCLUDE
//==========================================================================
#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dshow.h>
#include <imm.h>
#include <dinput.h>
#include <mmstream.h>
#include <amstream.h>
#include <ddstream.h>
#include <rmxfguid.h>
#include <dmusici.h>
#include <dplay8.h>
#include <dsound.h>


//==========================================================================
// DEFINE
//==========================================================================
#define USE_DIRECT3D			(0x01 << 0)
#define USE_DIRECTSOUND			(0x01 << 1)
#define USE_DIRECTMUSIC			(0x01 << 2)
#define USE_DIRECTINPUT			(0x01 << 3)
#define USE_DIRECTPLAY			(0x01 << 4)
#define USE_AUDIOSTREAM			(0x01 << 5)
#define USE_MEDIASTREAM			(0x01 << 6)
#define USE_FEP					(0x01 << 7)
#define USE_CDDA				(0x01 << 8)
#define USE_MIXER				(0x01 << 9)

#define CPU_FPU					(0x01 << 0)
#define CPU_CPUID				(0x01 << 1)
#define CPU_MMX					(0x01 << 2)
#define CPU_MMX2				(0x01 << 3)
#define CPU_SSE					(0x01 << 4)
#define CPU_SSE2				(0x01 << 5)
#define CPU_3DNOW				(0x01 << 6)
#define CPU_3DNOW2				(0x01 << 7)
#define CPU_CMOV				(0x01 << 8)
#define CPU_FCMOV				(0x01 << 9)
#define CPU_TSC					(0x01 <<10)

//==========================================================================
// DEFINE
//==========================================================================
#define for						if ( false ) ; else for
#define ef						else if
#define F(n)					((float)(n))
#define TIME					(Luna::GetCounter())
#define FTIME					(F(Luna::GetCounter()))

#define SINTABLE_BIT			(12)
#define SINTABLE_SIZE			(1<<SINTABLE_BIT)
#define SINTABLE_MASK			(SINTABLE_SIZE - 1)
#define ATANTABLE_SIZE			(SINTABLE_SIZE >> 3)
#define ATANPOS_1				(ATANTABLE_SIZE << 1)
#define ATANPOS_2				(ATANPOS_1 + ATANPOS_1)
#define ATANPOS_3				(ATANPOS_1 + ATANPOS_2)
#define ATANPOS_4				(ATANPOS_1 + ATANPOS_3)
#define ATANPOS_SHIFT			(SINTABLE_BIT - 3)
#define RANDTABLE_MAX			(1 << 16)
#define RANDTABLE_MASK			(RANDTABLE_MAX - 1)

#define PAI						(3.141592653589793238462643383279f)
#define PAI2					(6.283185307179586476925286766559f)
#define RAND_PAI				(F(LunaMath::Rand(0,62831)*0.0001))
#define RAND(n1,n2)				(LunaMath::Rand(n1,n2))
#define ANGLE2PAI(n)			(F(n&SINTABLE_MASK)*PAI2/F(SINTABLE_SIZE))
#define PAI2ANGLE(n)			((long)(N*SINTABLE_SIZE/PAI2)&SINTABLE_MASK)

#define SAFE_RELEASE(p)			if ( (p) != NULL ) { (p)->Release(); (p) = NULL; }
#define SAFE_DELETE(p)			if ( (p) != NULL ) { delete (p); (p) = NULL; }
#define SAFE_DELETEARRAY(p)		if ( (p) != NULL ) { delete [] (p); (p) = NULL; }
#define SAFE_ALLOC(type,size)	(type*)VirtualAlloc( NULL, sizeof(type)*(size), MEM_COMMIT, PAGE_READWRITE );
#define SAFE_FREE(p)			if ( (p) != NULL ) { VirtualFree( (p), 0, MEM_RELEASE ); (p) = NULL; }

#define LIGHT_MAX				(8)
#define PORT_MAX				(32)
#define JOYSTICK_MAX			(16)
#define WM_MOUSEWHEEL			(0x020A)
#define WM_LUNAAUDIO			(WM_USER+1)


//==========================================================================
// TYPEDEF
//==========================================================================
typedef class LunaSurface		*LSURFACE;
typedef class LunaTexture		*LTEXTURE;
typedef class LunaSprite		*LSPRITE;
typedef class LunaModel			*LMODEL;
typedef class LunaSound			*LSOUND;
typedef class LunaMusic			*LMUSIC;
typedef class LunaAudio			*LAUDIO;
typedef class LunaMedia			*LMEDIA;
typedef class LunaModel			*LMODEL;
typedef class LunaFont			*LFONT;
typedef class LunaGraphicLoader	*LGRAPHICS;
typedef class LunaFixFloat		LFIXFLOAT;


typedef struct D3DXVECTOR3		LVECTOR, *LPLVECTOR;
typedef struct D3DXMATRIX		LMATRIX, *LPLMATRIX;
typedef struct D3DXPLANE		LPLANE, *LPLPANE;


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////																							////
////																							////
////									列挙型													////
////																							////
////																							////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------
// Blending
//-----------------------------------------------------
enum eBlending
{
	BLEND_NONE,
	BLEND_NORMAL,
	BLEND_ADD,
	BLEND_SUB,
	BLEND_MUL,
	BLEND_ONEONE,
	BLEND_BRIGHT,
	BLEND_REVERSE,
};

//-----------------------------------------------------
// LightingType
//-----------------------------------------------------
enum eLightType
{
	LIGHT_DIRECTION,
	LIGHT_POINT,
};

//-----------------------------------------------------
// RenderState
//-----------------------------------------------------
enum eRenderParam
{
	DISABLE				= false,
	ENABLE				= true,

	SHADE_POINT			= 0,
	SHADE_WIRE,
	SHADE_FLAT,
	SHADE_GOURAUD,

	CULL_NONE			= D3DCULL_NONE,
	CULL_RIGHT			= D3DCULL_CW,
	CULL_LEFT			= D3DCULL_CCW,

	FUNC_NEVER			= D3DCMP_NEVER,
	FUNC_LESS			= D3DCMP_LESS,
	FUNC_EQUAL			= D3DCMP_EQUAL,
	FUNC_LESSEQUAL		= D3DCMP_LESSEQUAL,
	FUNC_GREATER		= D3DCMP_GREATER,
	FUNC_NOTEQUAL		= D3DCMP_NOTEQUAL,
	FUNC_GREATEREQUAL	= D3DCMP_GREATEREQUAL,
	FUNC_ALWAYS			= D3DCMP_ALWAYS,

	UNKNOWN				= -1,

	sizeof_RenderParam,
};

//-----------------------------------------------------
// RenderState
//-----------------------------------------------------
enum eRenderState
{
	STATE_CULLING,
	STATE_ZBUFFER,
	STATE_ZWRITE,
	STATE_ZFUNC,
	STATE_ALPHATEST,
	STATE_ALPHAREF,
	STATE_ALPHAFUNC,
	STATE_DITHER,
	STATE_LIGHTING,
	STATE_FOG,
	STATE_SPECULAR,
	STATE_VERTEXCOLOR,
	STATE_SHADING,
	STATE_BLENDING,

	sizeof_RenderState,
};

//-----------------------------------------------------
//	TextureState
//-----------------------------------------------------
enum eTextureState
{
	STATE_COLOROP			= D3DTSS_COLOROP,
	STATE_COLORARG1			= D3DTSS_COLORARG1,
	STATE_COLORARG2			= D3DTSS_COLORARG2,
	STATE_ALPHAOP			= D3DTSS_ALPHAOP,
	STATE_ALPHAARG1			= D3DTSS_ALPHAARG1,
	STATE_ALPHAARG2			= D3DTSS_ALPHAARG2,
	STATE_ADDRESSU			= D3DTSS_ADDRESSU,
	STATE_ADDRESSV			= D3DTSS_ADDRESSV,
	STATE_MAGFILTER			= D3DTSS_MAGFILTER,
	STATE_MINFILTER			= D3DTSS_MINFILTER,

	sizeof_TextureState,
};

//-----------------------------------------------------
//	TextureState
//-----------------------------------------------------
enum eTextureParam
{
	OP_CURRENT		= D3DTA_CURRENT,
	OP_DIFFUSE		= D3DTA_DIFFUSE,
	OP_TEXTURE		= D3DTA_TEXTURE,

	OP_DISABLE		= D3DTOP_DISABLE,
	OP_MUL			= D3DTOP_MODULATE,
	OP_MUL2			= D3DTOP_MODULATE2X,
	OP_MUL4			= D3DTOP_MODULATE4X,
	OP_ADD			= D3DTOP_ADD,
	OP_ADDSIGNED	= D3DTOP_ADDSIGNED,
	OP_ADDSIGNED2	= D3DTOP_ADDSIGNED2X,
	OP_SUB			= D3DTOP_SUBTRACT,

	ADDRESS_WRAP	= D3DTADDRESS_WRAP,		// 繰り返し
	ADDRESS_MIRROR	= D3DTADDRESS_MIRROR,	// 反転
	ADDRESS_CLAMP	= D3DTADDRESS_CLAMP,	// テクスチャカラーで塗りつぶし
	ADDRESS_BORDER	= D3DTADDRESS_BORDER,	// 境界色で塗りつぶし

	FILTER_NONE				= D3DTEXF_NONE,				// フィルタなし
	FILTER_POINT			= D3DTEXF_POINT,			// 点フィルタ
	FILTER_LINEAR			= D3DTEXF_LINEAR,			// バイリニアフィルタ
	FILTER_ANISOTROPIC		= D3DTEXF_ANISOTROPIC,		// 異方性フィルタ
	FILTER_FLATCUBIC		= D3DTEXF_FLATCUBIC,		// 平面キューブフィルタ
	FILTER_GAUSSIANCUBIC	= D3DTEXF_GAUSSIANCUBIC,	// ガウスキューブフィルタ

	sizeof_TextureParam,
};

//-----------------------------------------------------
// CullingState
//-----------------------------------------------------
enum eCullState
{
	CS_INSIDE,			// バウンディングボックスは部分的にはフラスタムの内部にあります
	CS_OUTSIDE,			// バウンディングボックスはフラスタムの外部にあります
	CS_INSIDE_SLOW,		// OBBはフラスタムの内部です
	CS_OUTSIDE_SLOW,	// OBBはフラスタムの外部です
};

//-----------------------------------------------------
// TextureTransform
//-----------------------------------------------------
enum eTextureTransform
{
	TEXTRANSFORM_DISABLE,
	TEXTRANSFORM_COUNT1,
	TEXTRANSFORM_COUNT2,
	TEXTRANSFORM_COUNT3,
	TEXTRANSFORM_COUNT4,
	TEXTRANSFORM_PROJECTED,
};

//-----------------------------------------------------
// TextureMap
//-----------------------------------------------------
enum eTextureMap
{
	TEXMAP_CAMERASPACEPOSITION,
	TEXMAP_CAMERASPACENORMAL,
	TEXMAP_CAMERASPACEREFLECTION,
};

//-----------------------------------------------------
// フォント描画転送先
//-----------------------------------------------------
enum eDestination
{
	DST_BACKBUFFER,
	DST_TEXTURE,
	DST_SURFACE,
};

//-----------------------------------------------------
// テクスチャ軸
//-----------------------------------------------------
enum eAxisUV
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
};

//-----------------------------------------------------
// ReverbFx
//-----------------------------------------------------
enum eReverbParam
{
    REVERB_DEFAULT			= DSFX_I3DL2_ENVIRONMENT_PRESET_DEFAULT,
    REVERB_PRESET_GENERIC	= DSFX_I3DL2_ENVIRONMENT_PRESET_GENERIC,
    REVERB_PADDEDCELL		= DSFX_I3DL2_ENVIRONMENT_PRESET_PADDEDCELL,
    REVERB_ROOM				= DSFX_I3DL2_ENVIRONMENT_PRESET_ROOM,
    REVERB_BATHROOM			= DSFX_I3DL2_ENVIRONMENT_PRESET_BATHROOM,
    REVERB_LIVINGROOM		= DSFX_I3DL2_ENVIRONMENT_PRESET_LIVINGROOM,
    REVERB_STONEROOM		= DSFX_I3DL2_ENVIRONMENT_PRESET_STONEROOM,
    REVERB_AUDITORIUM		= DSFX_I3DL2_ENVIRONMENT_PRESET_AUDITORIUM,
    REVERB_CONCERTHALL		= DSFX_I3DL2_ENVIRONMENT_PRESET_CONCERTHALL,
    REVERB_CAVE				= DSFX_I3DL2_ENVIRONMENT_PRESET_CAVE,
    REVERB_ARENA			= DSFX_I3DL2_ENVIRONMENT_PRESET_ARENA,
    REVERB_HANGAR			= DSFX_I3DL2_ENVIRONMENT_PRESET_HANGAR,
    REVERB_CARPETEDHALLWAY	= DSFX_I3DL2_ENVIRONMENT_PRESET_CARPETEDHALLWAY,
    REVERB_HALLWAY			= DSFX_I3DL2_ENVIRONMENT_PRESET_HALLWAY,
    REVERB_STONECORRIDOR	= DSFX_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR,
    REVERB_ALLEY			= DSFX_I3DL2_ENVIRONMENT_PRESET_ALLEY,
    REVERB_FOREST			= DSFX_I3DL2_ENVIRONMENT_PRESET_FOREST,
    REVERB_CITY				= DSFX_I3DL2_ENVIRONMENT_PRESET_CITY,
    REVERB_MOUNTAINS		= DSFX_I3DL2_ENVIRONMENT_PRESET_MOUNTAINS,
    REVERB_QUARRY			= DSFX_I3DL2_ENVIRONMENT_PRESET_QUARRY,
    REVERB_PLAIN			= DSFX_I3DL2_ENVIRONMENT_PRESET_PLAIN,
    REVERB_PARKINGLOT		= DSFX_I3DL2_ENVIRONMENT_PRESET_PARKINGLOT,
    REVERB_SEWERPIPE		= DSFX_I3DL2_ENVIRONMENT_PRESET_SEWERPIPE,
    REVERB_UNDERWATER		= DSFX_I3DL2_ENVIRONMENT_PRESET_UNDERWATER,
    REVERB_SMALLROOM		= DSFX_I3DL2_ENVIRONMENT_PRESET_SMALLROOM,
    REVERB_MEDIUMROOM		= DSFX_I3DL2_ENVIRONMENT_PRESET_MEDIUMROOM,
    REVERB_LARGEROOM		= DSFX_I3DL2_ENVIRONMENT_PRESET_LARGEROOM,
    REVERB_MEDIUMHALL		= DSFX_I3DL2_ENVIRONMENT_PRESET_MEDIUMHALL,
    REVERB_LARGEHALL		= DSFX_I3DL2_ENVIRONMENT_PRESET_LARGEHALL,
    REVERB_PLATE			= DSFX_I3DL2_ENVIRONMENT_PRESET_PLATE
};

//-----------------------------------------------------
// サーフェイスフォーマット
//-----------------------------------------------------
enum eFormat
{
	FORMAT_BACKBUFFER,
	FORMAT_TEXTURE,
	FORMAT_ALPHATEXTURE,
};

//-----------------------------------------------------
// スプライト回転軸
//-----------------------------------------------------
enum eRotateAxis
{
	ROTATE_CENTER,		// 中心
	ROTATE_LT,			// 左上
	ROTATE_LB,			// 左下
	ROTATE_RT,			// 右上
	ROTATE_RB,			// 右下
};


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////																							////
////																							////
////									構造体													////
////																							////
////																							////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------
// トランスフォーム済み頂点
//-----------------------------------------------------
typedef struct TLVERTEXFX {
	float x, y, z, w;
	D3DCOLOR color;
} TLVERTEXFX, *LPTLVERTEXFX;

#define FVF_TLVERTEXFX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

//-----------------------------------------------------
// テクスチャ付トランスフォーム済み頂点
//-----------------------------------------------------
typedef struct TLVERTEX {
	float x, y, z, w;
	D3DCOLOR color;
	float u, v;
} TLVERTEX, *LPTLVERTEX;

#define FVF_TLVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------------------
// テクスチャ付未トランスフォーム頂点
//-----------------------------------------------------
typedef struct VERTEX {
	LVECTOR p;
	LVECTOR n;
	D3DCOLOR color;
	float u, v;
} VERTEX, *LPVERTEX;

#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------------------
// ライティング済み未トランスフォーム頂点
//-----------------------------------------------------
typedef struct LVERTEX {
	LVECTOR p;
	D3DCOLOR color;
} LVERTEX, *LPLVERTEX;

#define FVF_LVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//-----------------------------------------------------
// ライティング済み未トランスフォーム頂点
//-----------------------------------------------------
typedef struct HEIGHTVERTEX {
	LVECTOR p;
	D3DCOLOR color;
	float u, v;
} HEIGHTVERTEX, *LPHEIGHTVERTEX;

#define FVF_HEIGHTVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------------------
// ライン用未トランスフォーム頂点
//-----------------------------------------------------
typedef struct LINEVERTEX
{
	LVECTOR	p;
	D3DCOLOR color;
}
LINEVERTEX, *LPLINEVERTEX;

#define FVF_LINEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//-----------------------------------------------------
// カリング用
//-----------------------------------------------------
typedef struct CULLINFO
{
    LVECTOR VecFrustum[8];		// ビューフラスタムの角
    LPLANE PlaneFrustum[6];		// ビューフラスタムの平面
}
CULLINFO, *LPCULLINFO;

//-----------------------------------------------------
// 姿勢制御用
//-----------------------------------------------------
typedef struct LSTYLE
{
	LVECTOR Front;
	LVECTOR Up;
	LVECTOR Right;
}
LSTYLE, *LPLSTYLE;

//-----------------------------------------------------
// ２次元上の点
//-----------------------------------------------------
typedef struct POINT2D
{
	float x, y;
}
POINT2D, *LPPOINT2D;

//-----------------------------------------------------
// ２次元上の円
//-----------------------------------------------------
typedef struct CIRCLE2D
{
	POINT2D p;
	float r;
}
CIRCLE2D, *LPCIRCLE2D;

//-----------------------------------------------------
// ２次元上の四角
//-----------------------------------------------------
typedef struct RECT2D
{
	float x1, y1;
	float x2, y2;
}
RECT2D, *LPRECT2D;

//-----------------------------------------------------
// ２次元上の多角形
//-----------------------------------------------------
typedef struct POLYGON2D
{
	long count;
	LPPOINT2D pt;
}
POLYGON2D, *LPPOLYGON2D;

//-----------------------------------------------------
// ２次元上の線
//-----------------------------------------------------
typedef struct LINE2D
{
	POINT2D a, b;
}
LINE2D, *LPLINE2D;

//-----------------------------------------------------
// ３次元上の点
//-----------------------------------------------------
typedef struct POINT3D
{
	float x, y, z;
}
POINT3D, *LPPOINT3D;

//-----------------------------------------------------
// ３次元上の円（球
//-----------------------------------------------------
typedef struct CIRCLE3D
{
	POINT3D p;
	float r;
}
CIRCLE3D, *LPCIRCLE3D;

//-----------------------------------------------------
// ３次元上の線
//-----------------------------------------------------
typedef struct LINE3D
{
	POINT3D a;
	POINT3D b;
}
LINE3D, *LPLINE3D;

//-----------------------------------------------------
// ３次元上の面
//-----------------------------------------------------
typedef struct PLANE3D
{
	float nx, ny, nz, d;
}
PLANE3D, *LPPLANE3D;

//-----------------------------------------------------
// ３次元上の箱
//-----------------------------------------------------
typedef struct BOX3D
{
	POINT3D pt[8];
	PLANE3D plane[6];
}
BOX3D, *LPBOX3D;

//-----------------------------------------------------
// 回転キー
//-----------------------------------------------------
typedef struct {
	DWORD			Time;
	D3DXQUATERNION	Quat;
} KEYROTATION;

//-----------------------------------------------------
// 拡大縮小キー
//-----------------------------------------------------
typedef struct {
	DWORD			Time;
	LVECTOR			Scale;	
} KEYSCALING;

//-----------------------------------------------------
// 移動キー
//-----------------------------------------------------
typedef struct {
	DWORD			Time;
	LVECTOR			Pos;	
} KEYTRANSLATION;

//-----------------------------------------------------
// 行列キー
//-----------------------------------------------------
typedef struct {
	DWORD			Time;
	LMATRIX			Matrix;
} KEYMATRIX;

//-----------------------------------------------------
// 隣接面用
//-----------------------------------------------------
typedef struct ADJOINING
{
	DWORD Face[3];
	LVECTOR v[3][2];
}
ADJOINING, *LPADJOINING;

// マウスデータ
typedef struct MOUSEDATA
{
	long X, Y, W;
	BOOL ClickL;
	BOOL ClickR;
	BOOL ClickW;
}
MOUSEDATA, *LPMOUSEDATA;

//-----------------------------------------------------
// キーボードデータ
//-----------------------------------------------------
typedef struct KEYBOARDDATA
{
	char Key[256];
}
KEYBOARDDATA, *LPKEYBOARDDATA;

//-----------------------------------------------------
// ジョイスティックデータ
//-----------------------------------------------------
typedef struct JOYSTICKDATA
{
	long Px, Py, Pz;		// 軸
	long Rx, Ry, Rz;		// 回転軸
	long Vx, Vy, Vz;		// 軸の速度
	long VRx, VRy, VRz;		// 軸の角速度
	long Ax, Ay, Az;		// 軸の加速度
	long ARx, ARy, ARz;		// 軸の角加速度
	long Fx, Fy, Fz;		// 軸のフォース
	long FRx, FRy, FRz;		// 軸のトルク
	long Slider[2];			// スライダー
	long Pov[4];			// POV
	char Button[16];		// ボタン
}
JOYSTICKDATA, *LPJOYSTICKDATA;	// 32Bitアライメント必須

//-----------------------------------------------------
// 軸データ
//-----------------------------------------------------
typedef struct AXISDATA
{
	long Ax, Ay, Az;
}
AXISDATA, *LPAXISDATA;

//-----------------------------------------------------
// ジョイスティック統括
//-----------------------------------------------------
typedef struct JOYSTICKDATA2
{
	LPDIRECTINPUTDEVICE8 lpDevice;
	JOYSTICKDATA Data;
	short bPolling;
	short bForceFeedback;
	AXISDATA Center;
	AXISDATA Max;
}
JOYSTICKDATA2, *LPJOYSTICKDATA2;

//-----------------------------------------------------
// ピクセルフォーマット
//-----------------------------------------------------
enum LAGPIXELFORMAT
{
	LAGFMT_A4R4G4B4,
	LAGFMT_A8R8G8B8,
	LAGFMT_A16R16G16B16,
	LAGFMT_FLOAT,
};

//-----------------------------------------------------
// 16Bitピクセルデータ
//-----------------------------------------------------
typedef struct PIXELDATA16
{
	unsigned short a	: 4;
	unsigned short r	: 4;
	unsigned short g	: 4;
	unsigned short b	: 4;
}
PIXELDATA16;

//-----------------------------------------------------
// 32Bitピクセルデータ
//-----------------------------------------------------
typedef struct PIXELDATA32
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
}
PIXELDATA32;

//-----------------------------------------------------
// 64Bitピクセルデータ
//-----------------------------------------------------
typedef struct PIXELDATA64
{
	unsigned short b;
	unsigned short g;
	unsigned short r;
	unsigned short a;
}
PIXELDATA64;

//-----------------------------------------------------
// float型ピクセルデータ
//-----------------------------------------------------
typedef struct PIXELDATAFLOAT
{
	float b;
	float g;
	float r;
	float a;
}
PIXELDATAFLOAT;

//-----------------------------------------------------
// フォント描画用
//-----------------------------------------------------
typedef struct DRAWDATA
{
	long x1, y1, x2, y2;
	long pitch;
	void *pbits;
}
DRAWDATA, *LPDRAWDATA;


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////																							////
////																							////
////									クラス													////
////																							////
////																							////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//==========================================================================
// リスト用基底クラス
//==========================================================================
typedef class CListBase
{
protected:
	long ReferenceCount;

public:
	CListBase *Prev;
	CListBase *Next;

	CListBase();
	virtual ~CListBase();

	virtual long Release( void );
	virtual void AddRef( void );
}
CListBase, *LPCListBase;

//==========================================================================
// リスト管理クラス
//==========================================================================
typedef class CList
{
protected:
	LPCListBase ListTop;
	LPCListBase ListEnd;
	long Count;

public:
	CList();
	~CList();

	void Init( void );
	LPCListBase Top( void );
	LPCListBase End( void );
	void InsertTop( LPCListBase t );
	void InsertEnd( LPCListBase t );
	void Erase( LPCListBase t );
	void Release( void );
	LPCListBase Data( long no );
	long Size( void );
}
CList, *LPCList;

//==========================================================================
// CLASS
//==========================================================================
class Luna
{
protected:
	static BOOL InitAPI( void );
	static void UnInitAPI( void );
	static BOOL InitTimer( unsigned long fps );
	static void UnInitTimer( void );
	static BOOL InitWindow( void );
	static void WaitFrame( void );
	static void CheckCPU( void );
	static void DropFile( HDROP hDrop );
	static void GetSysInfo( void );
	static void GetDriveInfo( void );
	static char *GetOSInfo( void );
	static void DrawInitInfo( long x, long y, char *text );

public:
	static void SetInfo( void );
	static void Init( void );
	static void UnInit( void );
	static void MainLoop( void );

	static __int64 GetSystemCounter( void );
	static __int64 GetSecoundCount( void );
	static BOOL IsCPUCounter( void );

	static void AddFPS( long no );
	static void AddPolygon( long num );

	static LRESULT CALLBACK LunaDefaultMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static WINAPI Drive( HINSTANCE hinst, char *cmd );

	static BOOL IsSizing( void );
	static void SetLogFile ( char *file );
	static void SetUseOption( long flag );
	static void SetWindowTitle( char *title );
	static void SetScreenMode( long w, long h, BOOL window = false, BOOL resize = false );
	static void SetShowMouse( BOOL flag );
	static void SetFPS( long fps );
	static void SetWindowMode( BOOL flag );
	static void SetShowFPS( BOOL flag );
	static void SetDropCallBack( void (*func)( char **, long ) );
	static void SetMsgCallBack( LRESULT (CALLBACK *func)( HWND, UINT, WPARAM, LPARAM ) );
	static void SetActiveWindow( BOOL flag );
	static void SetMenu( char *name );

	static void GetWindowTitle( char *title );
	static BOOL GetActiveWindow( void );
	static BOOL GetWindowMode( void );
	static void GetScreenSize( long *w, long *h );
	static void GetWindowPos( long *x, long *y );
	static HINSTANCE GetInstanceHandle( void );
	static HWND GetWindowHandle( void );
	static void GetMemorySize( unsigned long *total, unsigned long *free );
	static char *GetCommnadLine( long index );
	static long GetCommnadLineNum( void );
	static unsigned long GetCounter( void );
	static float GetFrameTime();

	static unsigned long GetFPS( void );
	static void MsgBox( char *title, char *string,... );
	static BOOL SelectBox( char *title, char *string,... );

	static void Exit( void );
	static char *GetDir( void );
};

//======================================================================
// CLASS
//======================================================================
class Luna3D
{
protected:
	static BOOL CreateDevice( void );
	static BOOL GetAdapterInfo( void );
	static BOOL GetDeviceInfo( BOOL hal );
	static BOOL Create3DDevice( void );
	static BOOL InitD3DSetting( void );
	static void SetWindowMode( long width, long height );
	static void SetDeviceParam( BOOL window );

public:
	static BOOL Init( void );
	static void UnInit( void );

	static BOOL CheckLost( void );
	static BOOL Reset( long w = -1, long h = -1 );

	static void EnableEmulationMode( void );
	static void EnableFullColorMode( void );
	static void EnableVSyncWait( void );
	static void EnableFullColorTexture( void );

	static char *GetFormat( D3DFORMAT fmt );
	static char *GetD3DError( HRESULT hr, char *string = NULL );
	static D3DFORMAT GetTextureFormat( void );
	static D3DFORMAT GetAlphaTextureFormat( void );
	static BOOL BeginScene( void );
	static void EndScene( void );
	static void SetClipArea( long x1, long y1, long x2, long y2 );
	static void SetViewport( void );
	static void GetViewport( RECT *pRect );
	static void SetBlending( eBlending blend );
	static void SetSmoothFilter( BOOL flag );
	static void SetTexture( LTEXTURE tex );

	static void ClearZ( void );
	static void Clear( void );
	static void Clear( long x1, long y1, long x2, long y2 );
	static void ColorFill( D3DCOLOR color );
	static void ColorFill( D3DCOLOR color, long x1, long y1, long x2, long y2 );
	static void Refresh( HWND hWnd = NULL );
	static void Refresh( RECT *dest, RECT *src, HWND hWnd = NULL );
	static BOOL RestoreTarget( void );

	static LPDIRECT3DDEVICE8 GetDevice( void );
	static D3DFORMAT GetBBFormat( void );
	static LPDIRECT3DSURFACE8 GetBackBuffer( void );

	static void SetFogData( D3DCOLOR color, float near_z, float far_z );
	static void SetRenderState( eRenderState state, long param );
	static void SetTextureState( eTextureState state, eTextureParam param );
	static void SetTextureFactor( eTextureParam ColorOp, eTextureParam AlphaOp, D3DCOLOR color = 0x00000000 );
	static void UpdateBoundingBox( LVECTOR *pBBMin, LVECTOR *pBBMax, LVECTOR *pBBOut, LPLANE *pBBPlane, LMATRIX *pMat );
	static void DrawBox( LVECTOR *pvec, D3DCOLOR color );
	static void SetTextureTransform( eTextureTransform trans, LMATRIX *pMat );
	static void SetTextureTransformType( eTextureMap map );

	static BOOL LockBackBuffer( void **ppBits, long *pPitch );
	static void UnlockBackBuffer( void );

	class Camera
	{
	protected:
		static void UpdateCullInfo( void );
		static BOOL EdgeIntersectsFace( LVECTOR* pEdges, LVECTOR* pFacePoints, LPLANE* pPlane );
		static void CreateViewMatrix( LMATRIX *pMat );

	public:
		static void SetViewport( long x1, long y1, long x2, long y2 );
		static void SetProjection( float fov, float near_z, float far_z );
		static void SetProjection( LMATRIX *mat );
		static void SetViewMatrix( LMATRIX *mat );
		static void GetViewMatrix( LMATRIX *mat );
		static void GetProjectionMatrix( LMATRIX *mat );
		static void GetInvProjectionMatrix( LMATRIX *mat );
		static void InitTransform( void );
		static void RotationX( float angle );
		static void RotationY( float angle );
		static void RotationZ( float angle );
		static void RotationAxis( float angle, LVECTOR *vec );
		static void Translate( float px, float py, float pz );
		static void Transform( LMATRIX *mat );
		static void LookAt( LVECTOR *position, LVECTOR *target, float bank = 0 );
		static void LookAtModel( LMODEL obj, long no = -1, float bank = 0 );
		static void GetPosition( float *x, float *y, float *z );
		static void DynamicTransform( void );
		static void VirtualTransform( void );
		static eCullState CullObject( LVECTOR* pVecBounds, LPLANE* pPlaneBounds );
	};

	class Light
	{
	protected:
	public:
		static void SetLight( BOOL flag, long no );
		static void SetType( eLightType type, long no );
		static void SetDiffuse( float r, float g, float b, long no );
		static void SetAmbient( float r, float g, float b, long no );
		static void SetSpecular( float r, float g, float b, long no );
		static void SetDirection( LVECTOR *dir, long no );
		static void SetPosition( LVECTOR *pos, long no );
	};
};

//==========================================================================
// サウンドクラス
//==========================================================================
class LunaAudio : public CListBase
{
protected:
	IGraphBuilder	*lpGB;
	IMediaSeeking	*lpMS;
	IMediaEventEx	*lpME;
	IVideoWindow	*lpVW;
	IMediaControl	*lpMC;
	IBasicAudio		*lpBA;

	__int64 StartPos, EndPos;
	BOOL bLoad, bPlay, bLoop, bPause;

	LunaAudio( char *file );						// コンストラクタ
	~LunaAudio();									// デストラクタ

public:
	static BOOL Init( void );						// 初期化
	static void UnInit( void );						// 終了

	static void Event( void );
	static LAUDIO Load( char *file );

	// 各種関数
	virtual void Play( BOOL loop );					// 再生
	virtual void Stop( void );						// 停止
	virtual BOOL IsPlaying( void );					// 再生チェック
	virtual void Pause( void );						// 一時停止
	virtual void SetSpeed( float speed );			// 再生速度変更
	virtual void SetVolume( long param );			// ボリューム変更
	virtual void SetPan( long param );				// パン移動
};



//==========================================================================
// CLASS
//==========================================================================
class LunaCD
{
protected:
	static void PlayAll( long track1, long track2, BOOL loop );

public:
	static void DeviceOpen( void );
	static void DeviceClose( void );
	static void LoopCheck( void );

	static void GetInfo( void );

	static void Open( void );
	static void Close( void );

	static void Play( void );
	static void Loop( void );
	static void Play( long track );
	static void Loop( long track );
	static void Play( long track1, long track2 );
	static void Loop( long track1, long track2 );

	static void Stop( void );
	static void Pause( void );
};

class LunaMixer
{
private:
	static void GetMixerDev( unsigned long id, HMIXER &hMixer );
	static BOOL GetMixerLineControl( MIXERLINE &ml, HMIXER &hMixer );
	static BOOL SetMixerControlDetails( HMIXER &hMixer );

public:
	static BOOL DeviceOpen( void );
	static void DeviceClose( void );

	static BOOL SetVolume( long vol );
	static void GetDefaultVolume( long *vol );
};

//======================================================================
// CLASS
//======================================================================
class LunaCollision
{
public:
	// ２Ｄ
	static bool Point_Point( POINT2D &pt1, POINT2D pt2 );				// 点と線
	static bool Point_Line( POINT2D &pt, LINE2D &l );					// 点と線
	static bool Point_Rect( POINT2D &pt, RECT2D &rect );				// 点と四角
	static bool Point_Circle( POINT2D &pt, CIRCLE2D &cir );				// 点と円
	static bool Point_Polygon( POINT2D &pt, POLYGON2D &poly );			// 点と多角形
	static bool Rect_Rect( RECT2D &rect1, RECT2D &rect2 );				// 四角と四角
	static bool Rect_Circle( RECT2D &rect, CIRCLE2D &cir );				// 四角と円
	static bool Rect_Line( RECT2D &rect, LINE2D &l );					// 四角と線
	static bool Rect_Polygon( RECT2D &rect, POLYGON2D &poly );			// 四角と多角形
	static bool Circle_Circle( CIRCLE2D &cir1, CIRCLE2D &cir2 );		// 円と円
	static bool Circle_Line( CIRCLE2D &cir, LINE2D &l );				// 円と線
	static bool Circle_Polygon( CIRCLE2D &cir, POLYGON2D &poly );		// 円と多角形
	static bool Line_Line( LINE2D &l1, LINE2D &l2 );					// 線と線
	static bool Line_Polygon( LINE2D &l, POLYGON2D &poly );				// 線と多角形
	static bool Polygon_Polygon( POLYGON2D &poly1, POLYGON2D &poly2 );	// 多角形と多角形
	// ３Ｄ
	static bool Point_Point_3D( POINT3D &pt1, POINT3D &pt2 );			// 点と点
	static bool Point_Circle_3D( POINT3D &pt, CIRCLE3D &cir );			// 点と球
	static bool Point_Line_3D( POINT3D &pt, LINE3D &l );				// 点と線
	static bool Point_Plane_3D( POINT3D &pt, PLANE3D &pln );			// 点と面
	static bool Point_Box_3D( POINT3D &pt, BOX3D &b );					// 点と箱
	static bool Circle_Circle_3D( CIRCLE3D &cir1, CIRCLE3D &cir2 );		// 球と球
	static bool Circle_Line_3D( CIRCLE3D &cir, LINE3D &l );				// 球と線
	static bool Circle_Plane( CIRCLE3D &cir, PLANE3D &pln );			// 球と面
	static bool Circle_Box_3D( CIRCLE3D &cir, BOX3D &b );				// 球と箱
	static bool Line_Line_3D( LINE3D &l1, LINE3D &l2 );					// 線と線
	static bool Line_Plane_3D( LINE3D &l, PLANE3D &pln );				// 線と面
	static bool Line_Box_3D( LINE3D &l, BOX3D &b );						// 線と箱
	static bool Plane_Plane_3D( PLANE3D &pln1, PLANE3D &pln2 );			// 面と面
	static bool Box_Box_3D( BOX3D &b1, BOX3D &b2 );						// 箱と箱
};

//==========================================================================
// CLASS
//==========================================================================
class LunaDebug
{
public:
	static BOOL Init( void );
	static void UnInit( void );

	static BOOL OpenWindow( void );
	static void CloseWindow( void );

	static void ClearWindow( void );
	static void SendMessage( char *str,... );
};


//==========================================================================
// CLASS
//==========================================================================
class LunaEffect
{
	enum { SONIC_VERNUM = 48 };

public:
	static BOOL Init( void );
	static void UnInit( void );

	static void Gradation( RECT *dest, D3DCOLOR *color );

	static void Line( long x1, long y1, long x2, long y2, D3DCOLOR color );
	static void LineSquare( RECT *dest, D3DCOLOR color );
	static void LineTriangle( long x, long y, long range, long angle, D3DCOLOR color );
	static void LinePolygon( long x, long y, long range, long angle, long count, D3DCOLOR color );
	static void LinePolygonEx( long x, long y, long range, long angle, float scale, long count, D3DCOLOR color );

	static void FillSquare( RECT *dest, D3DCOLOR color );
	static void FillTriangle( long x, long y, long range, long angle, D3DCOLOR color );
	static void FillPolygon( long x, long y, long range, long angle, long count, D3DCOLOR color );
	static void FillPolygonEx( long x, long y, long range, long angle, float scale, long count, D3DCOLOR color );

	static void DrawPixel( long x, long y, D3DCOLOR color );
	static void DrawCircle( long cx, long cy, long range, long w, long h, D3DCOLOR color );
	static void DrawCircleFill( long cx, long cy, long range, long w, long h, D3DCOLOR color );

	static void Sonic( long cx, long cy, long r1, float r2, D3DCOLOR color );
	static void SonicRotate( long cx, long cy, long r1, float r2, float yaw, float pitch, float roll, D3DCOLOR color );

	static void TriangleRotate( long x, long y, long range, float yaw, float pitch, float roll, D3DCOLOR color, BOOL light );

	static void Star( long x, long y, long range, long angle, D3DCOLOR color );

	static void DrawLaser( long count, long step, float *x, float *y, float *w, long *angle, D3DCOLOR *color );

	static void Begin( eBlending blend );
	static void End( void );
};


//==========================================================================
// CLASS
//==========================================================================
class LunaFEP
{
public:
	static BOOL Init( void );
	static void UnInit( void );
	static BOOL IsOpen( void );
	static void BackChar( void );
	static void AddChar( unsigned int param );
	static long GetConvertPos( void );
	static long GetCursorCount( void );
	static long GetCountCandidate( void );
	static long GetCandidateStart( void );
	static long GetCandidateEnd( void );
	static long GetCandidateIndex( void );
	static void GetCandidateString( char *string, long Index );
	static BOOL GetStatus( char *string, unsigned long size );
	static void GetString( char *string, long size );
	static void GetInputString( char *string, long size );
	static void ClearInputString( void );
};

//======================================================================
// CLASS
//======================================================================
class LunaFixFloat
{
protected:
	long Value;

	void Multiply( LunaFixFloat &f0, long n1, long n2 );
	void Division( LunaFixFloat &f0, long n1, long n2 );

public:
	// コンストラクタ
	LunaFixFloat( void );
	LunaFixFloat( long nValue );
	LunaFixFloat( float fValue );
	LunaFixFloat( const LunaFixFloat& src );

	// 関数
	long GetInt( void );
	float GetFloat( void );
	float GetFix( void );
	void SetValue( long nValue );
	void SetValue( float fValue );
	void SetValue( long nHigh, long nLow );
	void SetValue( long nHigh, float fLow );

	// オペレーター
	void operator  = ( LunaFixFloat& src );
	void operator -= ( LunaFixFloat& src );
	void operator += ( LunaFixFloat& src );
	void operator *= ( LunaFixFloat& src );
	void operator /= ( LunaFixFloat& src );

	LunaFixFloat operator - ( LunaFixFloat& src );
	LunaFixFloat operator + ( LunaFixFloat& src );
	LunaFixFloat operator * ( LunaFixFloat& src );
	LunaFixFloat operator / ( LunaFixFloat& src );

	bool operator <  ( LunaFixFloat& src );
	bool operator >  ( LunaFixFloat& src );
	bool operator <= ( LunaFixFloat& src );
	bool operator >= ( LunaFixFloat& src );
	bool operator == ( LunaFixFloat& src );

	void operator  = ( long nValue );
	void operator += ( long nValue );
	void operator -= ( long nValue );
	void operator *= ( long nValue );
	void operator /= ( long nValue );

	LunaFixFloat operator + ( long nValue );
	LunaFixFloat operator - ( long nValue );
	LunaFixFloat operator * ( long nValue );
	LunaFixFloat operator / ( long nValue );

	bool operator <  ( long nValue );
	bool operator >  ( long nValue );
	bool operator <= ( long nValue );
	bool operator >= ( long nValue );
	bool operator == ( long nValue );

	void operator  = ( float fValue );
	void operator += ( float fValue );
	void operator -= ( float fValue );
	void operator *= ( float fValue );
	void operator /= ( float fValue );

	LunaFixFloat operator + ( float fValue );
	LunaFixFloat operator - ( float fValue );
	LunaFixFloat operator * ( float fValue );
	LunaFixFloat operator / ( float fValue );

	bool operator <  ( float fValue );
	bool operator >  ( float fValue );
	bool operator <= ( float fValue );
	bool operator >= ( float fValue );
	bool operator == ( float fValue );
};


//======================================================================
// CLASS
//======================================================================
class LunaFont : public CListBase
{
protected:
	HDC hdc;
	HFONT hFont, hOldFont;
	TEXTMETRIC tm;
	char *pBuffer;
	CListBase *lpTarget;
	D3DFORMAT TargetFormat;
	eDestination Destination;
	long TargetWidth;
	long TargetHeight;
	BOOL IsAlpha;

	LunaFont();
	virtual ~LunaFont();

	virtual long GetFontBuffer( long code, GLYPHMETRICS *pgm, char **lpData, BOOL alpha );
	virtual BOOL LockTarget( void **ppBits, long *pPitch );
	virtual void UnlockTarget( void );
	virtual void DrawPixel1555( long c, LPDRAWDATA pDst, LPDRAWDATA pSrc );
	virtual void DrawPixel565( long c, LPDRAWDATA pDst, LPDRAWDATA pSrc );
	virtual void DrawPixel4444( long c, LPDRAWDATA pDst, LPDRAWDATA pSrc );
	virtual void DrawPixel8888( long c, LPDRAWDATA pDst, LPDRAWDATA pSrc );
	virtual void DrawPixel4444AA( long c, LPDRAWDATA pDst, LPDRAWDATA pSrc );
	virtual void DrawPixel8888AA( long c, LPDRAWDATA pDst, LPDRAWDATA pSrc );
	virtual BOOL Clip( LPDRAWDATA pDst, LPDRAWDATA pSrc, long w, long h );

public:
	static BOOL Init( void );
	static void UnInit( void );
	static LFONT CreateFont( char *face, long size, BOOL italic, BOOL bold );

	virtual void SetDestination( eDestination dst = DST_BACKBUFFER, CListBase *tar = NULL );
	virtual BOOL Begin( void );
	virtual void End( void );
	virtual long DrawText( long x, long y, D3DCOLOR color, char *str,... );
	virtual long DrawTextAA( long x, long y, D3DCOLOR color, char *str,... );
};

//==========================================================================
// CLASS
//==========================================================================
class LunaGraphicLoader : public CListBase
{
protected:
	PIXELDATA32 *pBits;
	long Width;
	long Height;
	long Pitch;

	virtual long GetDitherColor5( long c,  long x, long y );
	virtual long GetDitherColor4( long c,  long x, long y );
	virtual void ConvertFormat( long w, long h, LAGPIXELFORMAT src_fmt, void *pSrc, LAGPIXELFORMAT dest_fmt, void **ppDest );

	virtual char *LoadSPI( HMODULE hDLL, void *lpData, unsigned long FileSize );
	virtual BOOL LoadBMP_1( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );
	virtual BOOL LoadBMP_2( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );
	virtual BOOL LoadBMP_4( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );
	virtual BOOL LoadBMP_8( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );
	virtual BOOL LoadBMP_16( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );
	virtual BOOL LoadBMP_24( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );
	virtual BOOL LoadBMP_32( BITMAPINFO *lpBmpInfo, unsigned char *pBmpBits );

	// LAG関連
	virtual long LoadLAG( char *file, char *name, long *w, long *h, void **ppData, LAGPIXELFORMAT *fmt );
	virtual long GetLAGDataSize( long w, long h, LAGPIXELFORMAT fmt );

	LunaGraphicLoader();
	virtual ~LunaGraphicLoader();

public:
	static BOOL Init( void );
	static void UnInit( void );
	static LGRAPHICS Create( void );

	virtual char *LoadLAG( char *file, char *name );
	virtual char *LoadSPI( char *file, char *spi_dir );

	virtual void GetPixelData( void **ppBits, long *ppitch );
	virtual void GetGraphicSize( long *pw, long *ph );

	virtual BOOL CopyToSurface( void *dest, long pitch, D3DFORMAT fmt );
};

//======================================================================
// CLASS
//======================================================================
class LunaInput
{
protected:
	static BOOL InitKeyboard( void );
	static BOOL InitJoystick( void );
	static BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, void *pContext );
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, void* pContext );

public:
	static BOOL Init( void );
	static void UnInit( void );

	static void SetMouseWheel( short w );
	static void RefreshMouse( void );
	static void RefreshKeyboard( void );
	static void RefreshKeyboardBuffer( void );
	static void RefreshJoystick( void );

	static void KeyBufferFlush( void );
	static BOOL KeyBufferLoop( unsigned long *key );
	static BOOL GetKeyData( unsigned long key );
	static const LPMOUSEDATA GetMouseData( void );
	static const LPJOYSTICKDATA GetJoystickData( unsigned long no );
};


//==========================================================================
// 算術クラス
//==========================================================================
class LunaMath
{
public:
	static void Init( void );
	static void UnInit( void );
	static long Rand( long Start, long End );
	static long FlatSpeed( long start, long end, long all, long now );
	static float FlatSpeed( float start, float end, long all, long now );
	static long AddSpeed( long start, long end, long all, long now );
	static float AddSpeed( float start, float end, long all, long now );
	static long SubSpeed( long start, long end, long all, long now );
	static float SubSpeed( float start, float end, long all, long now );
	static float Bezier( float p1, float p2, float p3, float t );
	static float Neville( float p1, float p2, float p3, float t );
	static void RotatePoint( long *x, long *y, long angle );
	static void RotatePoint( float *x, float *y, long angle );
	static long double GetPai( void );
	static long Sin( long angle );
	static long Cos( long angle );
	static long Sin( long angle, long scale );
	static long Cos( long angle, long scale );
	static long Atan( long x, long y );

	static float __cdecl SinF( float angle );
	static float __cdecl CosF( float angle );
	static float __cdecl TanF( float angle );
	static void __cdecl SinCosF( float angle, float *s, float *c );
};

//==========================================================================
// ムービークラス
//==========================================================================
class LunaMedia : public CListBase
{
private:
	IDirectDrawSurface*			lpSurface;
	LTEXTURE					Texture;
	IMediaStream*				STMediaStream;
	IMultiMediaStream*			STMultiMediaStream;
	IDirectDrawMediaStream*		STMediaStreamDD;
	IDirectDrawStreamSample*	STStreamSample;
	STREAM_TIME					MovieTime;
	long						Sx, Sy;
	BOOL						bLoad, bPlay, bLoop;

	LunaMedia( char *file );
	~LunaMedia();
	void BlitToTexture( void );

public:
	static BOOL Init( void );
	static void UnInit( void );
	static LMEDIA Load( char *file );

	void GetSize( long *w, long *h );
	void Play( BOOL loop );
	void Stop( void );
	BOOL Rendering( void );
	LTEXTURE GetTexture( void );
};


//=====================================================================
// マテリアル
//=====================================================================
class CMATERIAL : public CListBase
{
	friend class CMESH;
	friend class LunaModel;

protected:
	D3DMATERIAL8 Material;		// マテリアル
	LTEXTURE Texture;			// テクスチャ
	DWORD IndexStart;
	DWORD FaceCount;

	// レンダリングステート
	BOOL IsStateChange;						// ステートの変更有無
	long RenderState[sizeof_RenderState];	// ステート

	//===========================================================================
	// メンバ関数
	//===========================================================================
	CMATERIAL();
	virtual ~CMATERIAL();
};

//=====================================================================
// メッシュクラス
//=====================================================================
class CMESH : public CListBase
{
	friend class LunaModel;
	friend class CFRAME;

protected:
	char Name[64];						// メッシュ名
	CList Materials;							// マテリアルリスト
	CFRAME *lpParentFrame;						// 親のフレーム
	DWORD IndexCount;							// インデックス数
	DWORD VertexCount;							// 頂点数
	LPVERTEX lpVertex;							// 頂点
	unsigned short *lpIndex;					// インデックス
	LPDIRECT3DVERTEXBUFFER8 lpVB;				// 頂点バッファ
	LPDIRECT3DINDEXBUFFER8 lpIB;				// インデックスバッファ

	// 輪郭抽出用
	BOOL IsDrawEdge;							// 輪郭描画
	BOOL *lpIsCulling;							// 面の表裏判定
	LPLINEVERTEX lpEdge;						// 輪郭線用
	LPADJOINING lpAdjoinig;						// 隣接面データ
	LPLVECTOR lpScreen;							// 変換後の頂点格納用

	// バウンディング
	LVECTOR Center;								// メッシュの中心
	BOOL IsDrawBox;								// ボックス表示の有無
	D3DCOLOR BoxColor;							// 箱の色
	BOOL IsProjCull;							// プロジェクションカリングの有無
	LVECTOR BBMin, BBMax;						// バウンディングボックス
	LVECTOR WorldBB[8];							// ワールド座標系でのバウンディングボックス
	LPLANE PlaneBB[6];							// ワールド座標系でのバウンディングボックスの面

	// レンダリングステート
	BOOL IsStateChange;							// ステートの変更有無
	long RenderState[sizeof_RenderState];		// ステート

	//===========================================================================
	// メンバ関数
	//===========================================================================
	CMESH();
	virtual ~CMESH();

	virtual void SetStateChange( BOOL flag1, BOOL flag2 );
	virtual void SetRenderState( eRenderState state, long param, long material );
	virtual void ComputeNormal( void );				// 法線算出
	virtual void ComputeBox( void );				// バウンディングボックス
	virtual void ComputeAdjancy( void );			// 隣接面データ取得
	virtual void Render( void );					// レンダリング
	virtual void RenderEdge( void );				// 輪郭描画
	virtual void InitBuffer( void );				// バッファ初期化
	virtual void RefreshVertex( void );				// 頂点データ更新
	virtual void RefreshIndex( void );				// インデックスデータ更新
	virtual void SetEdgeColor( D3DCOLOR color );	// 輪郭線色
	virtual void ComputeVertexLighting( long num, LVECTOR *pdir, D3DCOLOR *pcolor, BOOL bMaterial );
	virtual void SetVertexColor( D3DCOLOR color );	// 頂点カラー設定
	virtual void SetTexture( LTEXTURE tex, long material );
	virtual void SetDiffuse( float a, float r, float g, float b,  long material );
	virtual void SetAmbient( float r, float g, float b, long material );
	virtual void SetSpecular( float r, float g, float b, long material );
	virtual void SetEmissive( float r, float g, float b, long material );
};

//=====================================================================
// フレームクラス
//=====================================================================
class CFRAME : public CListBase
{
	friend class CMESH;
	friend class LunaModel;

protected:
	char Name[64];		// フレーム名

    LMATRIX TransMatrix;		// ペアレント用変換行列
    LMATRIX TempMatrix;			// 一時変換格納用行列

	CList ParentFrames;			// フレームリスト
	CList Models;				// モデルリスト

	CFRAME *lpAnimeFrame;

	// アニメーションデータ
	DWORD RotKeyNum;
	DWORD ScaleKeyNum;
	DWORD TransKeyNum;
	DWORD MatKeyNum;
	KEYROTATION*	lpRotation;
	KEYSCALING*		lpScaling;
	KEYTRANSLATION*	lpTranslate;
	KEYMATRIX*		lpMatrix;

	//===========================================================================
	// メンバ関数
	//===========================================================================
	CFRAME();
	virtual ~CFRAME();

	virtual void Render( void );
	virtual void UpdateFrames( LMATRIX *pmat, float time );

	// アニメーション用マトリックス
	virtual LMATRIX *GetRotateMatrix( float time );
	virtual LMATRIX *GetScaleMatrix( float time );
	virtual LMATRIX *GetPositionMatrix( float time );
	virtual LMATRIX *GetAnimeMatrix( float time );
};

//=====================================================================
// モデルクラス
//=====================================================================
class LunaModel : public CListBase
{
	friend class CMESH;

protected:
	LVECTOR Center;							// メッシュの中心

	// 行列
	LMATRIX TransformMatrix;
	LSTYLE Style;

	// データ
	CList AnimeFrames;						// アニメーション用フレーム
	CFRAME *lpMasterFrame;					// 大元のフレーム
	CFRAME **lpFrameData;					// フレームリスト用
	unsigned long FrameCount;				// フレーム数
	CMESH **lpMeshData;						// モデルリスト用
	unsigned long MeshCount;				// モデル数

	// レンダリングステート
	BOOL IsStateChange;						// ステートの変更有無
	long RenderState[sizeof_RenderState];	// ステート

	// Ｘファイル読み込み用
	virtual BOOL Load( char *file );
	virtual void LoadFrame( LPDIRECTXFILEDATA lpdxfd, CFRAME *lpParentFrame );
	virtual void LoadMesh( LPDIRECTXFILEDATA lpdxfd, CFRAME *lpParentFrame );
	virtual void LoadTransformMatrix( LPDIRECTXFILEDATA lpdxfd, CFRAME *lpParentFrame );
	virtual void LoadAnimationSet( LPDIRECTXFILEDATA lpdxfd );
	virtual void LoadAnimation( LPDIRECTXFILEDATA lpdxfd );
	virtual BOOL ConvertMeshData( CMESH *pm, LPD3DXMESH pmesh );
	virtual BOOL ConvertMaterialData( CMESH *pm, LPD3DXMATERIAL pmat, long num );
	virtual CFRAME *FindFrame( char *name, CFRAME *pf );
	virtual void CreateFrameList( CFRAME *pf );
	virtual void CreateMeshList( CFRAME *pf );
	virtual void CreateDataList( void );

	// 各種設定
	virtual void SetStateChange( BOOL flag );
	virtual void ComputeCenter( CFRAME *pf, LMATRIX *pMat );

	LunaModel();
	virtual ~LunaModel();

	virtual void Reset( void );
	virtual void Restore( void );

public:
	static BOOL Init( void );
	static void UnInit( void );

	static void ResetAll( void );
	static void RestoreAll( void );

	// モデルデータ生成関連
	static LMODEL LoadXFile( char *file, char *tex = NULL );
	static LMODEL CreatePlaneXY( long x, long y, float sx, float sy, float addr );
	static LMODEL CreatePlaneYZ( long y, long z, float sy, float sz, float addr );
	static LMODEL CreatePlaneZX( long z, long x, float sz, float sx, float addr );
	static LMODEL CreateCube( float sx, float sy, float sz, long x, long y, long z );
	static LMODEL CreateSphere( float radius, long slices, long stacks );

	// レンダリング
	virtual void Render( void );

	// 変換関係
	virtual void InitTransform( void );
	virtual void DynamicTransform( float time = 0 );
	virtual void Translation( float px, float py, float pz );
	virtual void RotationX( float rot );
	virtual void RotationY( float rot );
	virtual void RotationZ( float rot );
	virtual void RotationXYZ( float rotx, float roty, float rotz );
	virtual void RotationAxis( float rot, LVECTOR *pvec );
	virtual void Scaling( float sx, float sy, float sz );

	virtual void Translation( long no, float px, float py, float pz );
	virtual void RotationX( long no, float rot );
	virtual void RotationY( long no, float rot );
	virtual void RotationZ( long no, float rot );
	virtual void RotationXYZ( long no, float rotx, float roty, float rotz );
	virtual void RotationAxis( long no, float rot, LVECTOR *pvec );
	virtual void Scaling( long no, float sx, float sy, float sz );

	// 変換行列
	virtual void GetMatrix( LMATRIX *mat );
	virtual void SetMatrix( LMATRIX *mat );
	virtual void GetMatrix( long no, LMATRIX *mat );
	virtual void SetMatrix( long no, LMATRIX *mat );

	// 姿勢制御
	virtual void InitStyle( LVECTOR *pfront = NULL, LVECTOR *pup = NULL, LVECTOR *pright = NULL );
	virtual void SetStyle( LSTYLE *pStyle );
	virtual void GetStyle( LSTYLE *pStyle );
	virtual void StyleRolling( float angle );
	virtual void StylePitching( float angle );
	virtual void StyleHeading( float angle );
	virtual void StyleTransform( void );

	// プロジェクションカリング
	virtual void SetProjCulling( BOOL flag );

	// レンダーステート設定
	virtual void SetStateChangeLevel( long lv );
	virtual void SetRenderState( eRenderState state, long param );
	virtual void SetRenderState( eRenderState state, long param, long mesh = -1, long material = -1 );

	// 各種設定
	virtual void SetDrawBox( BOOL flag, D3DCOLOR color, long mesh = -1 );
	virtual void SetDrawEdge( BOOL flag, D3DCOLOR color, long mesh = -1 );

	// 頂点カラー
	virtual void SetVertexColor( D3DCOLOR color, long mesh = -1 );
	virtual void ComputeVertexLighting( long num, LVECTOR *pdir, D3DCOLOR *pcolor, BOOL bMaterial = FALSE, long mesh = -1 );

	// 質感
	virtual void SetTexture( LTEXTURE tex, long mesh = -1, long material = -1 );
	virtual void SetDiffuse( float a, float r, float g, float b, long mesh = -1, long material = -1 );
	virtual void SetAmbient( float r, float g, float b, long mesh = -1, long material = -1 );
	virtual void SetSpecular( float r, float g, float b, long mesh = -1, long material = -1 );
	virtual void SetEmissive( float r, float g, float b, long mesh = -1, long material = -1 );

	// UV関係
	virtual void ComputePlanerUV( eAxisUV axis, long mesh = -1 );
	virtual void ComputeSphereUV( eAxisUV axis, long mesh = -1 );

	// データ取得
	virtual void GetWorldPosition( LVECTOR *pos, long mesh = -1 );
	virtual void GetCameraPosition( LVECTOR *pos, long mesh = -1 );
	virtual void GetScreenPosition( LVECTOR *ppos, long mesh = -1 );
	virtual void GetBoundingBox( LVECTOR *min, LVECTOR *max, long mesh );
};


//==========================================================================
// オーディオクラス
//==========================================================================
class LunaMusic : public CListBase
{
protected:
	IDirectMusicPerformance8	*lpPerformance;
	IDirectMusicSegment8		*lpSegment;
	long ChannelNo;
	BOOL bLoad, bPlay, bLoop, bPause;				// フラグ

	static BOOL SelectPort( long port_no );

	LunaMusic( char *file );
	~LunaMusic();

public:
	static BOOL Init( void );
	static void UnInit( void );

	static LMUSIC Load( char *file );

	void Play( BOOL loop );
	void Stop( void );

	static void SetVolume( long param );			// ボリューム変更
	static void SetPan( long param );				// パン移動
};

//==========================================================================
// CLASS
//==========================================================================
class LunaNetwork
{
public:
	static BOOL Init( void );
	static void UnInit( void );
};



//==========================================================================
// CLASS
//==========================================================================
class LunaReg
{
public:
	static BOOL Create( char *keyname );
	static void Close( void );
	static BOOL Delete( char *keyname );

	static BOOL SetNumber( char *value, unsigned long data );
	static BOOL SetString( char *value, char *data );
	static BOOL SetBinary( char *value, unsigned char *data, unsigned long size );

	static BOOL GetNumber( char *value, unsigned long *data );
	static BOOL GetString( char *value, char *data, unsigned long size );
	static BOOL GetBinary( char *value, unsigned char *data, unsigned long size );
};

//==========================================================================
// サウンドクラス
//==========================================================================
class LunaSound : public CListBase
{
protected:
	IDirectSoundBuffer8			*lpSoundBuffer;		// サウンドバッファ
	BOOL						bLoad;
	BOOL						bPlay;
	BOOL						bLoop;
	BOOL						bPause;

	virtual unsigned long GetWaveData( unsigned char *pData, unsigned char *&lpMasterData, void *lpwf );
	virtual long GetChunk( unsigned char *pData, unsigned char *&lpData, unsigned long *lpSize );
	virtual long CheckWaveData( unsigned char *lpFormat, void *lpwf );

	LunaSound();									// コンストラクタ
	virtual ~LunaSound();							// デストラクタ
	virtual void LoadWave( char *file );
	virtual void LoadWave( char *pack, char *data );


public:
	static BOOL Init( void );						// 初期化
	static void UnInit( void );						// 終了
	static BOOL InitDirectSound( LPDIRECTSOUND8 *lpDS, LPDIRECTSOUNDBUFFER *lpDSB );

	static IDirectSound8 *GetDirectSound( void );
	static LSOUND Load( char *file );
	static LSOUND Load( char *pack, char *data );

	// 各種関数
	virtual void Play( BOOL loop = FALSE );			// 再生
	virtual void Stop( void );						// 停止
	virtual BOOL IsPlay( void );					// 再生チェック
	virtual void Pause( void );						// 一時停止
	virtual void SetVolume( long param );			// ボリューム変更
	virtual void SetPan( long param );				// パン移動

	// エフェクト関連
	virtual void DeleteFx( void );					// エフェクト削除
	virtual void SetReverb( eReverbParam param );	// リバーブエフェクト
	virtual void SetEcho( void );					// エコーエフェクト
	virtual void SetChorus( void );					// コーラスエフェクト
	virtual void SetCompressor( void );				// コンプレッッションエフェクト
	virtual void SetDistortion( void );				// ディストーションエフェクト
	virtual void SetFlanger( void );				// フランジエフェクト
	virtual void SetGargle( void );					// ガーグルエフェクト
	virtual void SetParamEQ( void );				// パラメトリック・イコライザエフェクト
};


//==========================================================================
// CLASS
//==========================================================================
class LunaSprite : public CListBase
{
	enum { LASER_MAX = 256 };

protected:
	BOOL bLaser;
	LTEXTURE lpTex;
	LPTLVERTEX lpVertex[LASER_MAX];
	LPDIRECT3DVERTEXBUFFER8 lpVB[LASER_MAX];
	eBlending Blend;
	long Division;
	long NumPolygon;
	long MaxPolygon;
	RECT RectUV;

	virtual BOOL AddPolygon( LPTLVERTEX pVx, BOOL clip );
	virtual BOOL ClipVertex( LPTLVERTEX pVx );
	virtual void RefreshVertex( void );

	LunaSprite( long num, LTEXTURE ptex, eBlending blend );
	LunaSprite( long div, RECT *uv, LTEXTURE ptex, eBlending blend );
	virtual ~LunaSprite();

	virtual void Reset( void );
	virtual void Restore( void );

public:
	static LSPRITE Create( long num, LTEXTURE ptex, eBlending blend );
	static LSPRITE CreateLaser( long div, RECT *uv, LTEXTURE ptex, eBlending blend );

	static BOOL Init( void );
	static void UnInit( void );

	static void ResetAll( void );
	static void RestoreAll( void );

	static void AllRendering( void );

	virtual void DrawLaser( float *x, float *y, float *w, long *rot, D3DCOLOR *c, float z = 0 );
	virtual void DrawLaser( float *x, float *y, float *w, long *rot, D3DCOLOR *c, RECT *uv, float z = 0 );
	virtual BOOL Draw( RECT *dest, D3DCOLOR color, RECT *src, BOOL clip = TRUE, float z = 0 );
	virtual BOOL DrawRotate( RECT *dest, D3DCOLOR color, float angle, RECT *src, BOOL clip = TRUE, eRotateAxis axis = ROTATE_CENTER, float z = 0 );
	virtual BOOL DrawRotate( RECT *dest, D3DCOLOR color, float yaw, float pitch, float roll, RECT *src, BOOL clip = TRUE );
	virtual void Rendering( BOOL state = TRUE );
	virtual void InitDraw( void );
};


//==========================================================================
// CLASS
//==========================================================================
class LunaSSTP
{
protected:
	static BOOL SendMessage( char *SendBuffer, char *State, char *Server, long Port );

public:
	static BOOL InitWinSock( void );
	static void UninitWinSock( void );
	static BOOL IsSakura( void );

	static char* GetHostName( void );
	static char* GetIpAddress( void );

	static BOOL SendMessage( char *message, char *status = NULL, char *option = NULL, char *client = NULL, char *server = "127.0.0.1", long port = 9801 );
	static BOOL GetSakuraName( char *Sakura = NULL, char *Kero = NULL );
};


//======================================================================
// サーフェイスクラス
//======================================================================
class LunaSurface : public CListBase
{
protected:
	D3DFORMAT Format;
	LPDIRECT3DSURFACE8 lpSurface;
	BOOL IsTarget;
	BOOL IsDepth;
	long Width;
	long Height;

	LunaSurface( void );
	virtual ~LunaSurface();

	virtual void Reset( void );
	virtual void Restore( void );

public:

	static BOOL Init( void );
	static void UnInit( void );

	static void ResetAll( void );
	static void RestoreAll( void );

	static LSURFACE Load( char *file, eFormat fmt );
	static LSURFACE LoadLAG( char *pack, char *data, eFormat fmt );
	static LSURFACE LoadSPI( char *pack, char *dir, eFormat fmt );
	static LSURFACE Create( long w, long h, eFormat fmt );
	static LSURFACE CreateZBuffer( long w, long h );
	static LSURFACE CreateRenderTarget( long w, long h );

	virtual long GetWidth( void );
	virtual long GetHeight( void );
	virtual D3DFORMAT GetFormat( void );

	virtual BOOL SetRenderTarget( void );
	virtual BOOL SetDepthBuffer( void );

	virtual void CopyFromBackBuffer( long px, long py, long x1, long y1, long x2, long y2 );
	virtual void CopyFromTexture( LTEXTURE tex, long px, long py, long x1, long y1, long x2, long y2 );
	virtual void CopyToTexture( LTEXTURE tex, long px, long py, long x1, long y1, long x2, long y2 );

	virtual void Blit( long px, long py, long x1, long y1, long x2, long y2 );

	virtual BOOL Lock( void **ppBits, long *pPitch );
	virtual void Unlock( void );
};


//======================================================================
// テクスチャクラス
//======================================================================
class LunaTexture : public CListBase
{
protected:
	LPDIRECT3DTEXTURE8	lpTexture;
	LPDIRECT3DSURFACE8	lpSurface;
	LPDIRECT3DSURFACE8	lpBackUp;
	D3DFORMAT			Format;
	long				Width;
	long				Height;
	long				ImageWidth;
	long				ImageHeight;
	float				AspectWidth;
	float				AspectHeight;
	BOOL				bTarget;
	BOOL				bActive;

	LunaTexture( void );
	virtual ~LunaTexture();

	virtual void Reset( void );
	virtual void Restore( void );

public:
	static BOOL Init( void );
	static void UnInit( void );

	static void ResetAll( void );
	static void RestoreAll( void );

	static LTEXTURE Load( char *file, BOOL alpha = false );
	static LTEXTURE LoadLAG( char *pack, char *data, BOOL alpha = false );
	static LTEXTURE LoadSPI( char *file, char *dir = NULL, BOOL alpha = false );
	static LTEXTURE Create( long w, long h, BOOL alpha = false );
	static LTEXTURE CreateBackBufferFormat( long w, long h );
	static LTEXTURE CreateTarget( long w, long h, BOOL alpha = false );

	static void SetColorKey( D3DCOLOR color );
	static void SetOffset( float offset );
	static float GetOffset( void );

	virtual BOOL IsLoad( void );
	virtual char *Refresh( void );

	virtual long GetWidth( void );
	virtual long GetHeight( void );
	virtual long GetImageWidth( void );
	virtual long GetImageHeight( void );
	virtual float GetAspectWidth( void );
	virtual float GetAspectHeight( void );
	virtual D3DFORMAT GetFormat( void );
	virtual IDirect3DTexture8 *GetTexture( void );
	virtual IDirect3DSurface8 *GetSurface( void );

	virtual void Fill( D3DCOLOR color = 0x00000000 );

	virtual BOOL SetRenderTarget( void );
	virtual BOOL BlitToBackBuffer( RECT *dest, RECT *src, D3DCOLOR color, eBlending blend );
	virtual BOOL BlitToTarget( RECT *dest, RECT *src, D3DCOLOR color, eBlending blend, float z = 0 );
	virtual BOOL CopyBackBuffer( RECT *pDst );

	virtual BOOL Lock( void **ppBits, long *pPitch );
	virtual void Unlock( void );

	virtual BOOL SaveBMP( char *file );
};


//==========================================================================
// システムクラス
//==========================================================================
class LunaSystem
{
public:
	static BOOL CheckVolumeLabel( char *label, char *path );
	static BOOL RunApplication( char *Dir, char *AppName );
	static void OpenBrowser( char *url );
	static BOOL SetWallPaper( char *file, long type = 0, BOOL reboot = false );
	static void SaveScreen( char *file );
	static char *GetWindowsDir( void );
	static BOOL CopyFile( char *src, char *dest );
	static BOOL DeleteFile( char *src );
	static unsigned long GetFileSize( char *file );
	static BOOL SaveData( char *file, void *pData, unsigned long size );
	static BOOL LoadData( char *file, void *pData, unsigned long size );
	static BOOL FolderSelect( char *dir );
	static BOOL FileOpenDialog( char *path, char *title, char *filter, char *ext );
	static BOOL FileSaveDialog( char *path, char *title, char *filter, char *ext );
	static BOOL CreateDirectory( char *name );
	static BOOL DeleteDirectory( char *dest );
	static BOOL CopyDirectory( char *src, char *dest, float *lpPer = NULL );
	static BOOL CreateLinkFile( char *ShortcutFile, char *Description, char *TargetFile );
	static BOOL GetDesktopFolder( char *path, unsigned long size );
	static void SetShowIME( BOOL flag );
	static void ClearMemory( void );
	static BOOL FileExist( char *file );

	static BOOL ReadFileOpen( char *file, HANDLE *phFile );
	static BOOL WriteFileOpen( char *file, HANDLE *phFile );
	static void CloseFile( HANDLE hFile );
	static unsigned long FileWrite( HANDLE hFile, void *pData, unsigned long size );
	static unsigned long FileRead( HANDLE hFile, void *pData, unsigned long size );
};
