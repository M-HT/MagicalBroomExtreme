

//=================================================================
// INCLUDE
//=================================================================
#include "stdio.h"
#include "windows.h"

//=================================================================
// LINK
//=================================================================
#pragma comment ( lib, "winmm.lib" )

//=================================================================
// GLOBAL
//=================================================================
char DataName[32];

//=================================================================
//
//		Waveファイル読み込み
//
//=================================================================
void ConvertWave( char *file, FILE *fp )
{
	FILE *rfp = NULL;
	bool result = false;
	long RiffDataSize;
	long fmtDataSize;
	long DataSize;
	long FormatSize;
	// fmt サブチャンク
	WAVEFORMATEX wf;

	//--------------------------------------------------------
	// ファイルオープン
	//--------------------------------------------------------
	rfp = fopen( file, "rb" );
	if ( rfp == NULL )
	{
		puts( "Can't open file" );
		return;
	}

	//--------------------------------------------------------
	// "RIFF"の文字列
	//--------------------------------------------------------
	if ( fgetc( rfp ) != 'R' || fgetc( rfp ) != 'I' || fgetc( rfp ) != 'F' || fgetc( rfp ) != 'F' )
	{
		puts( "This file isn't wavefile" );
		fclose( rfp );
		return;
	}

	//--------------------------------------------------------
	// RIFFデータサイズ
	//--------------------------------------------------------
	fread( &RiffDataSize, sizeof(long), 1, rfp );

	//--------------------------------------------------------
	// "WAVE"の文字列
	//--------------------------------------------------------
	if ( fgetc( rfp ) != 'W' || fgetc( rfp ) != 'A' || fgetc( rfp ) != 'V' || fgetc( rfp ) != 'E' )
	{
		puts( "This file isn't wavefile" );
		fclose( rfp );
		return;
	}

	//========================================================
	// Foramt チャンク
	//========================================================
	// "fmt "の文字列
	if ( fgetc( rfp ) != 'f' || fgetc( rfp ) != 'm' || fgetc( rfp ) != 't' || fgetc( rfp ) != ' ' )
	{
		puts( "This file isn't wavefile" );
		fclose( rfp );
		return;
	}

	//--------------------------------------------------------
	// fmtデータサイズ
	//--------------------------------------------------------
	fread( &fmtDataSize, sizeof(long), 1, rfp );

	//--------------------------------------------------------
	// フォーマット情報取得
	//--------------------------------------------------------
	FormatSize = sizeof(WAVEFORMATEX)-sizeof(WORD);
	fread( &wf, FormatSize, 1, rfp );

	// フォーマット
	if ( wf.wFormatTag != 1 )
	{
		// 圧縮ファイルは勘弁（T-T
		puts( "Not sopport compressed file" );
		fclose( rfp );
		return;
	}

	// 拡張データは使用しませんよ～
    wf.cbSize = 0;

	// ログ
	printf( "  Channel  => %d\n", wf.nChannels );
	printf( "  Sample   => %d\n", wf.nSamplesPerSec );
	printf( "  Byte     => %d\n", wf.nAvgBytesPerSec );
	printf( "  Block    => %d\n", wf.nBlockAlign ); 
	printf( "  Bit      => %d\n", wf.wBitsPerSample );

	// 次だにょ～
	fseek( rfp, fmtDataSize-(sizeof(WAVEFORMATEX)-sizeof(WORD)), SEEK_CUR );

	//========================================================
	// Dataチャンク検索
	//========================================================
	while ( true )
	{
		// なんのチャンクかな～
		char guid[5] = "";
		fread( guid, sizeof(char), 4, rfp );
		printf( "Chunk => %s\n", guid );
		// ちょっとまて～
		if ( feof( rfp ) )
		{
			puts( "Can't find Wave Data" );
			fclose( rfp );
			return;
		}
		// いかがですか？
		if ( strcmp( "data", guid ) == 0 )
		{
			// ビバ
			break;
		}
		else
		{
			// なんでぇ違うんかよ
			DWORD size;
			fread( &size, sizeof(DWORD), 1, rfp );
			// 次いくべ
			fseek( rfp, size, SEEK_CUR );
		}
	}

	//--------------------------------------------------------
	// データサイズ
	//--------------------------------------------------------
	fread( &DataSize, sizeof(long), 1, rfp );

	// データ取得
	LPBYTE lpSoundData = new BYTE [ DataSize ];
	fread( lpSoundData, sizeof(BYTE), DataSize, rfp );

	printf( "  DataSize => %.3lf Kbyte\n", DataSize/1024.0 );

	//--------------------------------------------------------
	// データ書き込み
	//--------------------------------------------------------
	fwrite( DataName, sizeof(char), 32, fp );
	fwrite( &DataSize, sizeof(long), 1, fp );
	fwrite( &wf, sizeof(WAVEFORMATEX), 1, fp );
	fwrite( lpSoundData, sizeof(BYTE), DataSize, fp );

	// データ開放
	delete [] lpSoundData;
}

//=================================================================
//
//		ファイルオープンダイアログ
//
//=================================================================
bool SaveDialog( char *path )
{
	ZeroMemory( path, 256 );
	
	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof(OPENFILENAME) );
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrTitle = "パックファイルの保存";
	ofn.lpstrFilter = "lsdファイル(*.lwd)\0*.lsd\0";
	ofn.lpstrDefExt = "lsd";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = path;
	ofn.nMaxFile = 256;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	
	if ( !GetOpenFileName( &ofn ) ) return false;

	return true;
}

//=================================================================
//
//	メイン
//
//=================================================================
void main( int arg, char *args[] )
{
	char FilePath[256];
	char SavePath[256];
	char *AbsPath;
	HANDLE hFile;
	FILE *fp;
	WIN32_FIND_DATA FindData;

	AbsPath = args[0];

	//---------------------------------------------------
	// フォルダのパス取得
	//---------------------------------------------------
	for ( long i = (long)strlen(AbsPath); AbsPath[i] != '\\'; i-- ) AbsPath[i] = '\0';

	//---------------------------------------------------
	// ファイルのワイルドカード設定
	//---------------------------------------------------
	sprintf( FilePath, "%s*.wav", AbsPath );

	//---------------------------------------------------
	// 一つ目のファイル
	//---------------------------------------------------
	hFile = FindFirstFile( FilePath, &FindData );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		puts( "Can't find WaveFile in this folder..." );
		return;
	}

	//---------------------------------------------------
	// 書き込みファイル用ダイアログ
	//---------------------------------------------------
	if ( SaveDialog( SavePath ) )
	{
		//------------------------------------
		// ヘッダ書き込み
		//------------------------------------
		fp = fopen( SavePath, "wb" );
		if ( fp == NULL )
		{
			puts( "Can't Open WriteFile" );
			return;
		}

		// ヘッダ
		struct RWFFILEHEADER
		{
			char guid[4];
		}
		rwh = { 'L', 'S', 'D' };

		// ファイルヘッダ書き込み
		fwrite( &rwh, sizeof(RWFFILEHEADER), 1, fp );

		//------------------------------------
		// ファイル列挙
		//------------------------------------
		while ( true )
		{
			// 読み込みファイル
			char buff[256];
			sprintf( buff, "%s%s", AbsPath, FindData.cFileName );
			char *p = FindData.cFileName;

			ZeroMemory( DataName, sizeof(char)*32 );

			// データ名
			for ( long i = 0; p[i] != '.'; i++ )
			{
				if ( 'A' <= p[i] && p[i] <= 'Z' )
				{
					p[i] += 'a' - 'A';
				}
				DataName[i] = p[i];
			}
			puts( DataName );

			// Waveデータ処理
			ConvertWave( buff, fp );

			// 次のファイル
			if ( FindNextFile( hFile, &FindData ) == FALSE ) break;
		}

		// ファイルクローズ
		fclose( fp );
	}

	puts( "Press any key" );
	getchar();
}