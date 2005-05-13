//==========================================================================
//
//
//		LAG ファイル関連
//
//
//==========================================================================

//==========================================================================
// INCLUDE
//==========================================================================
#include <stdio.h>
#include <string.h>

#include "Lag.h"


//===================================================================================
//
//		ディザ色取得
//
//===================================================================================
long GetDitherColor4( long c,  long x, long y )
{
	// 8階調ディザ行列テーブル
	long DitherTable8[16] = {
		0, 4, 1, 5,
		6, 2, 7, 3,
		1, 5, 0, 4,
		7, 3, 6, 2,
	};
	long d = DitherTable8[((y & 3) << 2) | (x & 3)];

	return (((c * 249) >> 8) >> 1) + (d << 1);
}

//===================================================================================
//
//		ディザ色取得
//
//===================================================================================
long GetDitherColor5( long c,  long x, long y )
{
	// 8階調ディザ行列テーブル
	long DitherTable8[16] = {
		0, 4, 1, 5,
		6, 2, 7, 3,
		1, 5, 0, 4,
		7, 3, 6, 2,
	};
	long d = DitherTable8[((y & 3) << 2) | (x & 3)];

	return ((c * 249) >> 8) + d;
}

//==========================================================================
//
//
//		LAGファイルヘッダー保存
//
//
//==========================================================================
bool SaveLAGHeader( char *file, long ex )
{
	LAGFILEHEADER lfh = { "LAG", ex };

	FILE *fp = fopen( file, "wb" );
	if ( fp == NULL ) return false;

	fwrite( &lfh, sizeof(LAGFILEHEADER), 1, fp );

	fclose( fp );

	return true;
}

//==========================================================================
//
//
//		LAGファイルデータ保存
//
//
//==========================================================================
bool SaveLAG( char *file, char *name, long w, long h, long pitch, PIXELDATA32 *pData, LAGPIXELFORMAT fmt, long reserved )
{
	LAGDATAHEADER ldh = { "", w, h, fmt, reserved };
	strcpy( ldh.name, name );

	FILE *fp = fopen( file, "ab" );
	if ( fp == NULL ) return false;

	//--------------------------------------------------
	// データヘッダ
	//--------------------------------------------------
	fwrite( &ldh, sizeof(LAGDATAHEADER), 1, fp );

	//--------------------------------------------------
	// データ
	//--------------------------------------------------
	switch ( fmt )
	{
	case LAGFMT_A4R4G4B4:
		{
			PIXELDATA16 data;
			for ( long y = 0; y < h; y++ )
			{
				for ( long x = 0; x < w; x++ )
				{
					data.a = GetDitherColor4( pData[x].a, x, y ) >> 4;
					data.r = GetDitherColor4( pData[x].r, x, y ) >> 4;
					data.g = GetDitherColor4( pData[x].g, x, y ) >> 4;
					data.b = GetDitherColor4( pData[x].b, x, y ) >> 4;
					fwrite( &data, sizeof(PIXELDATA16), 1, fp );
				}
				pData += pitch;
			}
		}
		break;
	case LAGFMT_A8R8G8B8:
		{
			PIXELDATA32 data;
			for ( long y = 0; y < h; y++ )
			{
				for ( long x = 0; x < w; x++ )
				{
					data = pData[x];
					fwrite( &data, sizeof(PIXELDATA32), 1, fp );
				}
				pData += pitch;
			}
		}
		break;
	case LAGFMT_A16R16G16B16:
		{
			PIXELDATA64 data;
			for ( long y = 0; y < h; y++ )
			{
				for ( long x = 0; x < w; x++ )
				{
					data.a = (unsigned short)( pData[x].a * 0xFFFF / 0xFF );
					data.r = (unsigned short)( pData[x].r * 0xFFFF / 0xFF );
					data.g = (unsigned short)( pData[x].g * 0xFFFF / 0xFF );
					data.b = (unsigned short)( pData[x].b * 0xFFFF / 0xFF );
					fwrite( &data, sizeof(PIXELDATA64), 1, fp );
				}
				pData += pitch;
			}
		}
		break;
	case LAGFMT_FLOAT:
		{
			PIXELDATAFLOAT data;
			for ( long y = 0; y < h; y++ )
			{
				for ( long x = 0; x < w; x++ )
				{
					data.a = (float)( (double)pData[x].a * ( 1.0 / 255.0 ) );
					data.r = (float)( (double)pData[x].r * ( 1.0 / 255.0 ) );
					data.g = (float)( (double)pData[x].g * ( 1.0 / 255.0 ) );
					data.b = (float)( (double)pData[x].b * ( 1.0 / 255.0 ) );
					fwrite( &data, sizeof(PIXELDATAFLOAT), 1, fp );
				}
				pData += pitch;
			}
		}
		break;
	}

	fclose( fp );

	return true;
}

//==========================================================================
//
//
//		LAGファイル読み込み
//
//
//==========================================================================
long LoadLAG( char *file, char *name, long *w, long *h, void **ppData, LAGPIXELFORMAT *fmt )
{
	long size = 0;
	bool read = false;
	long width, height;
	LAGPIXELFORMAT format;
	FILE *fp = NULL;
	LAGDATAHEADER ldh = { "" };
	LAGFILEHEADER lgh = { "" };

	// ファイルオープン
	fp = fopen( file, "rb" );
	if ( fp == NULL ) goto EXIT_FUNC;

	// ファイルフォーマットの確認
	fread( &lgh, sizeof(LAGFILEHEADER), 1, fp );
	if ( strcmp( lgh.chunk, "LAG" ) != 0 ) goto EXIT_FUNC;

	// それでは探しましょうかね
	while ( true )
	{
		memset( &ldh, 0x00, sizeof(LAGDATAHEADER) );

		// データ取得
		fread( &ldh, sizeof(LAGDATAHEADER), 1, fp );

		// ダメなら終了
		if ( feof( fp ) ) break;

		// 正解？
		if ( strcmp( ldh.name, name ) == 0 )
		{
			// O.K.
			read = true;
			break;
		}
		else
		{
			// 次
			long data_size = GetDataSize( ldh.width, ldh.height, ldh.format );
			fseek( fp, data_size, SEEK_CUR );
		}
	}

	if ( read == false ) goto EXIT_FUNC;

	width = ldh.width;
	height = ldh.height;
	format = ldh.format;

	if ( w != NULL ) *w = width;
	if ( h != NULL ) *h = height;
	if ( fmt != NULL ) *fmt = format;

	size = GetDataSize( ldh.width, ldh.height, ldh.format );

	if ( ppData == NULL ) goto EXIT_FUNC;

	// データ展開
	fread( *ppData, size, 1, fp );

EXIT_FUNC:
	if ( fp != NULL ) fclose( fp );

	return size;
}

//==========================================================================
//
//
//		ピクセルデータサイズ取得
//
//
//==========================================================================
long GetDataSize( long w, long h, LAGPIXELFORMAT fmt )
{
	long size = 0;

	switch ( fmt )
	{
	case LAGFMT_A4R4G4B4:
		size = w * h * sizeof(PIXELDATA16);
		break;
	case LAGFMT_A8R8G8B8:
		size = w * h * sizeof(PIXELDATA32);
		break;
	case LAGFMT_A16R16G16B16:
		size = w * h * sizeof(PIXELDATA64);
		break;
	case LAGFMT_FLOAT:
		size = w * h * sizeof(PIXELDATAFLOAT);
		break;
	}

	return size;
}

//==========================================================================
//
//
//		任意のフォーマットに変換
//
//
//==========================================================================
void ConvertFormat( long w, long h, LAGPIXELFORMAT src_fmt, void *pSrc, LAGPIXELFORMAT dest_fmt, void **ppDest )
{
	for ( long y = 0; y < h; y++ )
	{
		for ( long x = 0; x < w; x++ )
		{
			switch ( src_fmt )
			{
			// ---------------------------------------------------------------------------------------- 転送元が ARGB 16 bit
			case LAGFMT_A4R4G4B4:
				switch ( dest_fmt )
				{
				// ----------------------------------------------------------------------- 転送先が ARGB 16 bit
				case LAGFMT_A4R4G4B4:
					(*(PIXELDATA16 **)ppDest)[x + y*w].a = ((PIXELDATA16 *)pSrc)[x + y*w].a;
					(*(PIXELDATA16 **)ppDest)[x + y*w].r = ((PIXELDATA16 *)pSrc)[x + y*w].r;
					(*(PIXELDATA16 **)ppDest)[x + y*w].g = ((PIXELDATA16 *)pSrc)[x + y*w].g;
					(*(PIXELDATA16 **)ppDest)[x + y*w].b = ((PIXELDATA16 *)pSrc)[x + y*w].b;
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 32 bit
				case LAGFMT_A8R8G8B8:
					(*(PIXELDATA32 **)ppDest)[x + y*w].a = (unsigned char)((((PIXELDATA16 *)pSrc)[x + y*w].a)*0xFF/0x0F);
					(*(PIXELDATA32 **)ppDest)[x + y*w].r = (unsigned char)((((PIXELDATA16 *)pSrc)[x + y*w].r)*0xFF/0x0F);
					(*(PIXELDATA32 **)ppDest)[x + y*w].g = (unsigned char)((((PIXELDATA16 *)pSrc)[x + y*w].g)*0xFF/0x0F);
					(*(PIXELDATA32 **)ppDest)[x + y*w].b = (unsigned char)((((PIXELDATA16 *)pSrc)[x + y*w].b)*0xFF/0x0F);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 64 bit
				case LAGFMT_A16R16G16B16:
					(*(PIXELDATA64 **)ppDest)[x + y*w].a = (unsigned short)(((PIXELDATA16 *)pSrc)[x + y*w].a*0xFFFF/0x0F);
					(*(PIXELDATA64 **)ppDest)[x + y*w].r = (unsigned short)(((PIXELDATA16 *)pSrc)[x + y*w].r*0xFFFF/0x0F);
					(*(PIXELDATA64 **)ppDest)[x + y*w].g = (unsigned short)(((PIXELDATA16 *)pSrc)[x + y*w].g*0xFFFF/0x0F);
					(*(PIXELDATA64 **)ppDest)[x + y*w].b = (unsigned short)(((PIXELDATA16 *)pSrc)[x + y*w].b*0xFFFF/0x0F);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 128 bit
				case LAGFMT_FLOAT:
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].a = ((PIXELDATA16 *)pSrc)[x + y*w].a*(1.0f/0x0F);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].r = ((PIXELDATA16 *)pSrc)[x + y*w].r*(1.0f/0x0F);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].g = ((PIXELDATA16 *)pSrc)[x + y*w].g*(1.0f/0x0F);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].b = ((PIXELDATA16 *)pSrc)[x + y*w].b*(1.0f/0x0F);
					break;
				}
				break;
			// ---------------------------------------------------------------------------------------- 転送元が ARGB 32 bit
			case LAGFMT_A8R8G8B8:
				switch ( dest_fmt )
				{
				// ----------------------------------------------------------------------- 転送先が ARGB 16 bit
				case LAGFMT_A4R4G4B4:
					(*(PIXELDATA16 **)ppDest)[x + y*w].a = ((PIXELDATA32 *)pSrc)[x + y*w].a*0x0F/0xFF;
					(*(PIXELDATA16 **)ppDest)[x + y*w].r = ((PIXELDATA32 *)pSrc)[x + y*w].r*0x0F/0xFF;
					(*(PIXELDATA16 **)ppDest)[x + y*w].g = ((PIXELDATA32 *)pSrc)[x + y*w].g*0x0F/0xFF;
					(*(PIXELDATA16 **)ppDest)[x + y*w].b = ((PIXELDATA32 *)pSrc)[x + y*w].b*0x0F/0xFF;
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 32 bit
				case LAGFMT_A8R8G8B8:
					(*(PIXELDATA32 **)ppDest)[x + y*w].a = (unsigned char)((((PIXELDATA32 *)pSrc)[x + y*w].a));
					(*(PIXELDATA32 **)ppDest)[x + y*w].r = (unsigned char)((((PIXELDATA32 *)pSrc)[x + y*w].r));
					(*(PIXELDATA32 **)ppDest)[x + y*w].g = (unsigned char)((((PIXELDATA32 *)pSrc)[x + y*w].g));
					(*(PIXELDATA32 **)ppDest)[x + y*w].b = (unsigned char)((((PIXELDATA32 *)pSrc)[x + y*w].b));
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 64 bit
				case LAGFMT_A16R16G16B16:
					(*(PIXELDATA64 **)ppDest)[x + y*w].a = (unsigned short)(((PIXELDATA32 *)pSrc)[x + y*w].a*0xFFFF/0xFF);
					(*(PIXELDATA64 **)ppDest)[x + y*w].r = (unsigned short)(((PIXELDATA32 *)pSrc)[x + y*w].r*0xFFFF/0xFF);
					(*(PIXELDATA64 **)ppDest)[x + y*w].g = (unsigned short)(((PIXELDATA32 *)pSrc)[x + y*w].g*0xFFFF/0xFF);
					(*(PIXELDATA64 **)ppDest)[x + y*w].b = (unsigned short)(((PIXELDATA32 *)pSrc)[x + y*w].b*0xFFFF/0xFF);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 128 bit
				case LAGFMT_FLOAT:
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].a = ((PIXELDATA32 *)pSrc)[x + y*w].a*(1.0f/0xFF);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].r = ((PIXELDATA32 *)pSrc)[x + y*w].r*(1.0f/0xFF);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].g = ((PIXELDATA32 *)pSrc)[x + y*w].g*(1.0f/0xFF);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].b = ((PIXELDATA32 *)pSrc)[x + y*w].b*(1.0f/0xFF);
					break;
				}
				break;
			// ---------------------------------------------------------------------------------------- 転送元が ARGB 64 bit
			case LAGFMT_A16R16G16B16:
				switch ( dest_fmt )
				{
				// ----------------------------------------------------------------------- 転送先が ARGB 16 bit
				case LAGFMT_A4R4G4B4:
					(*(PIXELDATA16 **)ppDest)[x + y*w].a = ((PIXELDATA64 *)pSrc)[x + y*w].a*0x0F/0xFFFF;
					(*(PIXELDATA16 **)ppDest)[x + y*w].r = ((PIXELDATA64 *)pSrc)[x + y*w].r*0x0F/0xFFFF;
					(*(PIXELDATA16 **)ppDest)[x + y*w].g = ((PIXELDATA64 *)pSrc)[x + y*w].g*0x0F/0xFFFF;
					(*(PIXELDATA16 **)ppDest)[x + y*w].b = ((PIXELDATA64 *)pSrc)[x + y*w].b*0x0F/0xFFFF;
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 32 bit
				case LAGFMT_A8R8G8B8:
					(*(PIXELDATA32 **)ppDest)[x + y*w].a = (unsigned char)((((PIXELDATA32 *)pSrc)[x + y*w].a)*0xFF/0xFFFF);
					(*(PIXELDATA32 **)ppDest)[x + y*w].r = (unsigned char)((((PIXELDATA64 *)pSrc)[x + y*w].r)*0xFF/0xFFFF);
					(*(PIXELDATA32 **)ppDest)[x + y*w].g = (unsigned char)((((PIXELDATA64 *)pSrc)[x + y*w].g)*0xFF/0xFFFF);
					(*(PIXELDATA32 **)ppDest)[x + y*w].b = (unsigned char)((((PIXELDATA64 *)pSrc)[x + y*w].b)*0xFF/0xFFFF);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 64 bit
				case LAGFMT_A16R16G16B16:
					(*(PIXELDATA64 **)ppDest)[x + y*w].a = (unsigned short)(((PIXELDATA64 *)pSrc)[x + y*w].a);
					(*(PIXELDATA64 **)ppDest)[x + y*w].r = (unsigned short)(((PIXELDATA64 *)pSrc)[x + y*w].r);
					(*(PIXELDATA64 **)ppDest)[x + y*w].g = (unsigned short)(((PIXELDATA64 *)pSrc)[x + y*w].g);
					(*(PIXELDATA64 **)ppDest)[x + y*w].b = (unsigned short)(((PIXELDATA64 *)pSrc)[x + y*w].b);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 128 bit
				case LAGFMT_FLOAT:
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].a = ((PIXELDATA64 *)pSrc)[x + y*w].a*(1.0f/0xFFFF);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].r = ((PIXELDATA64 *)pSrc)[x + y*w].r*(1.0f/0xFFFF);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].g = ((PIXELDATA64 *)pSrc)[x + y*w].g*(1.0f/0xFFFF);
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].b = ((PIXELDATA64 *)pSrc)[x + y*w].b*(1.0f/0xFFFF);
					break;
				}
				break;
			// ---------------------------------------------------------------------------------------- 転送元が ARGB 128 bit
			case LAGFMT_FLOAT:
				switch ( dest_fmt )
				{
				// ----------------------------------------------------------------------- 転送先が ARGB 16 bit
				case LAGFMT_A4R4G4B4:
					(*(PIXELDATA16 **)ppDest)[x + y*w].a = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].a*0x0F);
					(*(PIXELDATA16 **)ppDest)[x + y*w].r = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].r*0x0F);
					(*(PIXELDATA16 **)ppDest)[x + y*w].g = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].g*0x0F);
					(*(PIXELDATA16 **)ppDest)[x + y*w].b = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].b*0x0F);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 32 bit
				case LAGFMT_A8R8G8B8:
					(*(PIXELDATA32 **)ppDest)[x + y*w].a = (unsigned char)((((PIXELDATAFLOAT *)pSrc)[x + y*w].a)*0xFF);
					(*(PIXELDATA32 **)ppDest)[x + y*w].r = (unsigned char)((((PIXELDATAFLOAT *)pSrc)[x + y*w].r)*0xFF);
					(*(PIXELDATA32 **)ppDest)[x + y*w].g = (unsigned char)((((PIXELDATAFLOAT *)pSrc)[x + y*w].g)*0xFF);
					(*(PIXELDATA32 **)ppDest)[x + y*w].b = (unsigned char)((((PIXELDATAFLOAT *)pSrc)[x + y*w].b)*0xFF);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 64 bit
				case LAGFMT_A16R16G16B16:
					(*(PIXELDATA64 **)ppDest)[x + y*w].a = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].a*0xFFFF);
					(*(PIXELDATA64 **)ppDest)[x + y*w].r = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].r*0xFFFF);
					(*(PIXELDATA64 **)ppDest)[x + y*w].g = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].g*0xFFFF);
					(*(PIXELDATA64 **)ppDest)[x + y*w].b = (unsigned short)(((PIXELDATAFLOAT *)pSrc)[x + y*w].b*0xFFFF);
					break;
				// ----------------------------------------------------------------------- 転送先が ARGB 128 bit
				case LAGFMT_FLOAT:
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].a = ((PIXELDATAFLOAT *)pSrc)[x + y*w].a;
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].r = ((PIXELDATAFLOAT *)pSrc)[x + y*w].r;
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].g = ((PIXELDATAFLOAT *)pSrc)[x + y*w].g;
					(*(PIXELDATAFLOAT **)ppDest)[x + y*w].b = ((PIXELDATAFLOAT *)pSrc)[x + y*w].b;
					break;
				}
				break;
			}
		}
	}
}