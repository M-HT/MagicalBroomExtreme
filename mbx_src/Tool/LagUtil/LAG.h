//==========================================================================
//
//
//		LAG ファイル関連
//
//
//==========================================================================

#pragma once

//==========================================================================
// STRUCT
//==========================================================================

// ピクセルフォーマット
enum LAGPIXELFORMAT
{
	LAGFMT_A4R4G4B4,
	LAGFMT_A8R8G8B8,
	LAGFMT_A16R16G16B16,
	LAGFMT_FLOAT,
};

// 16Bitピクセルデータ
typedef struct PIXELDATA16
{
	unsigned short a	: 4;
	unsigned short r	: 4;
	unsigned short g	: 4;
	unsigned short b	: 4;
}
PIXELDATA16;

// 32Bitピクセルデータ
typedef struct PIXELDATA32
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
}
PIXELDATA32;

// 64Bitピクセルデータ
typedef struct PIXELDATA64
{
	unsigned short b;
	unsigned short g;
	unsigned short r;
	unsigned short a;
}
PIXELDATA64;

// float型ピクセルデータ
typedef struct PIXELDATAFLOAT
{
	float b;
	float g;
	float r;
	float a;
}
PIXELDATAFLOAT;

// LAGファイルヘッダ
typedef struct LAGFILEHEADER
{
	char chunk[4];
	long reserved;
}
LAGFILEHEADER;

// LAGデータヘッダ
typedef struct LAGDATAHEADER
{
	char name[16];
	long width;
	long height;
	LAGPIXELFORMAT format;
	long reserved;
}
LAGDATAHEADER;

//==========================================================================
// PROTOTYPE
//==========================================================================x
bool SaveLAGHeader( char *file, long ex );
bool SaveLAG( char *file, char *name, long w, long h, long pitch, PIXELDATA32 *pData, LAGPIXELFORMAT fmt, long reserved );
long LoadLAG( char *file, char *name, long *w, long *h, void **ppData, LAGPIXELFORMAT *fmt );
long GetDataSize( long w, long h, LAGPIXELFORMAT fmt );
void ConvertFormat( long w, long h, LAGPIXELFORMAT src_fmt, void *pSrc, LAGPIXELFORMAT dest_fmt, void **ppDest );
