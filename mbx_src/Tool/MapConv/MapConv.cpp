
//==============================================================
// INCLUDE
//==============================================================
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


//==============================================================
// STRUCT
//==============================================================
typedef struct EVENTDATA
{
	short Time;
	short Type;
	short Sx;
	short Sy;
	short Temp[2];
	long Life;
}
EVENTDATA, *LPEVENTDATA;


//==============================================================
//
//		文字列 -> 数字
//
//==============================================================
long strtolong( char *str )
{
	long result = 0;

	// 16進数
	if ( str[1] == 'x' || str[1] == 'X' )
	{
		long cnt = 2;
		while ( true )
		{
			if ( '0' <= str[cnt] && str[cnt] <= '9' ||
				 'a' <= str[cnt] && str[cnt] <= 'f' ||
				 'A' <= str[cnt] && str[cnt] <= 'F' )
			{
				result *= 16;
				switch ( str[cnt] )
				{
				case '1': result += 1; break;
				case '2': result += 2; break;
				case '3': result += 3; break;
				case '4': result += 4; break;
				case '5': result += 5; break;
				case '6': result += 6; break;
				case '7': result += 7; break;
				case '8': result += 8; break;
				case '9': result += 9; break;
				case 'a':
				case 'A': result += 10; break;
				case 'b':
				case 'B': result += 11; break;
				case 'c':
				case 'C': result += 12; break;
				case 'd':
				case 'D': result += 13; break;
				case 'e':
				case 'E': result += 14; break;
				case 'f':
				case 'F': result += 15; break;
				}
				cnt++;
			}
			else
			{
				break;
			}
		}
	}
	// 10進数
	else
	{
		long Bai = 1;
		long cnt = 0;
		if ( str[cnt] == '-' )
		{
			Bai = -1;
			cnt++;
		}
		while ( true )
		{
			if ( '0' <= str[cnt] && str[cnt] <= '9' )
			{
				result *= 10;
				result += str[cnt++] - '0';
			}
			else
			{
				break;
			}
		}
		result *= Bai;
	}

	return result;
}

//==============================================================
//
//		コンバート
//
//==============================================================
void Convert( char *file, FILE *fp )
{
	long cnt;
	char path[256] = "";
	char buff1[256] = "";
	char buff2[256] = "";
	char *temp;
	FILE *rfp;
	EVENTDATA EvData;

	// 読み込みファイル
	rfp = fopen( file, "rt" );
	if ( rfp != NULL )
	{
		bool bEnd = false;
		while ( bEnd == false )
		{
			// テキストから1行読み込み
			fgets( buff1, 256, rfp );

			bEnd = (feof( rfp ) != 0 );

			if ( buff1[0] == '/' || buff1[1] == '/' ) continue;

			cnt = 0;
			for ( long i = 0; buff1[i] != '\0'; i++ )
			{
				if ( buff1[i] == ',' || buff1[i] == 'x' || buff1[i] == '-' )
				{
					buff2[cnt++] = buff1[i];
				}
				else if ( '0' <= buff1[i] && buff1[i] <= '9' )
				{
					buff2[cnt++] = buff1[i];
				}
			}

			// 各要素取得
			temp = strtok( buff2, "," );
			EvData.Time = (short)strtolong( temp );

			temp = strtok( NULL, "," );
			EvData.Type = (short)strtolong( temp );

			temp = strtok( NULL, "," );
			EvData.Sx = (short)strtolong( temp );

			temp = strtok( NULL, "," );
			EvData.Sy = (short)strtolong( temp );

			temp = strtok( NULL, "," );
			EvData.Temp[0] = (short)strtolong( temp );

			temp = strtok( NULL, "," );
			EvData.Temp[1] = (short)strtolong( temp );

			temp = strtok( NULL, "," );
			EvData.Life = strtolong( temp );

			printf( "%d, %d, %d, %d, %d, %d, %d\n",
				EvData.Time, EvData.Type, EvData.Sx, EvData.Sy, EvData.Temp[0], EvData.Temp[1], EvData.Life );

			// データ書き込み
			fwrite( &EvData, sizeof(EVENTDATA), 1, fp );
		}
		fclose( rfp );
	}
}

//==============================================================
//
//		メイン
//
//==============================================================
void main( int arg, char *args[] )
{
	long len;
	FILE *fp;
	char path[256] = "";
	char temp[256] = "";
	char name[256] = "";
	char guid[4] = "MAP";

	// フォルダのパス取得
	len = (long)strlen( args[0] );
	while ( args[0][len] != '\\' ) args[0][len--] = '\0';

	// ディレクトリ名
	printf( "Directory = %s\n", args[0] );

	for ( long i = 1; i < arg; i++ )
	{
		// 読み込むファイル
		strcpy( temp, args[1] );

		// ファイル名取得
		len = (long)strlen( args[1] );
		while ( args[1][len] != '.' ) len--;
		args[1][len] = '\0';

		len = (long)strlen( args[1] );
		while ( args[1][len] != '\\' ) if ( --len < 0 ) break;
		strcpy( name, &args[1][len+1] );

		// ファイル名
		printf( "FileName = %s\n", name );

		// 出力先ファイル名
		sprintf( path, "%s%s.map", args[0], name );

		// 出力先ファイル名
		printf( "Output = %s\n", path );

		// ファイルオープン
		fp = fopen( path, "wb" );
		if ( fp == NULL ) return;

		// ヘッダ書き込み
		fwrite( guid, sizeof(char), 4, fp );

		// データ読み込み
		Convert( temp, fp );

		fclose( fp );
	}
}
