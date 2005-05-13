
//==========================================================================
//
//
//		Application ファイル関連
//
//
//==========================================================================


//==========================================================================
// INCLUDE
//==========================================================================
#include <windows.h>
#include <stdio.h>
#include <shlobj.h>

#include "Resource.h"
#include "LagUtil.h"
#include "Lag.h"
#include "DirectDraw.h"


//==========================================================================
// DEFINE
//==========================================================================
#define AppName "LagUtility"


//==========================================================================
// GLOBAL
//==========================================================================
static char DataName[128];
static LAGPIXELFORMAT Format = LAGFMT_A8R8G8B8;


//==========================================================================
// PROTOTYPE
//==========================================================================
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK DlgNameProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
bool InitWindow( HINSTANCE hInst );
bool SetWinCenter( HWND hWnd, long w, long h );
bool FileOpenDialog( HWND hWnd, char *path, char *title, long mode );
bool FileSaveDialog( HWND hWnd, char *path, char *title, long mode );
bool GetFolderPath( char *path );
bool BatchProcess( char *dir, HWND hWnd, long *num );
void ConvertName( char *file );
bool CheckAlphaName( char *file );
void ConvertAlphaName( char *file );
void SetFormat( LAGPIXELFORMAT fmt, HWND hWnd );
bool SaveBitmap32( char *file, long w, long h, long pitch, PIXELDATA32 *pData );


//==========================================================================
//
//
//		WinMain
//
//
//==========================================================================
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow )
{
	//===================================================
	// COM初期化
	//===================================================
	CoInitialize( NULL );

	//===================================================
	// ウィンドウ初期化
	//===================================================
	if ( InitWindow( hInst ) == false ) return false;

	//===================================================
	// メッセージループ
	//===================================================
	MSG msg;
	while ( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	//===================================================
	// COM開放
	//===================================================
	CoUninitialize();

	return msg.wParam;
}


//==========================================================================
//
//
//		ウィンドウ初期化
//
//
//==========================================================================
bool InitWindow( HINSTANCE hInst )
{
	//=================================================
	// ウィンドウクラス登録
	//=================================================
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc      = WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInst;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( GRAY_BRUSH );
	wc.lpszMenuName		= "IDR_MENU";
	wc.lpszClassName	= AppName;
	
	if ( RegisterClass( &wc ) == NULL ) return false;

	//=================================================
	// ウィンドウ生成
	//=================================================
	HWND hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			AppName,
			AppName,
			WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInst,
			NULL
		);
	if ( hWnd == NULL ) return false;

	// ウィンドウ可視
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	return true;
}

//==========================================================================
//
//
//		メッセージ処理
//
//
//==========================================================================
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static const long BAR_DIV = 8;
	static long DrawX, DrawY;
	static long DispNoV, DispNoH;
	static long BarPosV, BarPosH;
	static SCROLLINFO ScrollInfoV, ScrollInfoH;

	switch ( msg )
	{
		//====================================================
		// メニューコマンド
		//====================================================
	case WM_COMMAND:
		{
			char path[512] = "";

			switch ( wParam )
			{
			/////////////////////////////////////////// 画像ファイルオープン
			case ID_OPENBMP:
				if ( FileOpenDialog( hWnd, path, "BMPファイル", 1 ) )
				{
					if ( LoadBMP( path, FMT_GRAPHIC ) )
					{
						long w, h;
						GetGraphicSize( &w, &h );
						SetWinCenter( hWnd, w, h );
						// データ名取得
						ConvertName( path );
						// アルファチャンネルデータ自動読み込み
						ConvertAlphaName( path );
						if ( LoadBMP( path, FMT_ALPHA ) )
						{
						}
						DataName[15] = '\0';
						sprintf( path, "%s < %s >", AppName, DataName );
						SetWindowText( hWnd, path ); 
					}
				}
				break;
			/////////////////////////////////////////// アルファ用画像ファイルオープン
			case ID_OPENALPHA:
				if ( FileOpenDialog( hWnd, path, "アルファチャンネル用BMPファイル", 1 ) )
				{
					if ( LoadBMP( path, FMT_ALPHA ) )
					{
					}
				}
				break;
			/////////////////////////////////////////// LAGファイル新規保存
			case ID_SAVE:
				{
					bool ret = false; 
					long w, h;
					GetGraphicSize( &w, &h );
					if ( w < 0 || h < 0 )
					{
						MsgBox( "Error", "書き込むデータがありません" );
					}
					else
					{
						if ( FileSaveDialog( hWnd, path, "LAGファイル", 0 ) )
						{
							// ヘッダ書き込み
							if ( SaveLAGHeader( path, 0 ) )
							{
								// データ取得＆書き込み
								long pitch;
								PIXELDATA32 *pData = LockSurface( &pitch );
								if ( pData != NULL )
								{
									if ( SaveLAG( path, DataName, w, h, pitch, pData, Format, 0 ) )
									{
										ret = true;
									}
									UnlockSurface();
								}
							}

							if ( ret == false ) MsgBox( "Error", "LAGファイルの書き込みに失敗" );
						}
					}
				}
				break;
			/////////////////////////////////////////// LAGファイル追記保存
			case ID_SAVEADD:
				{
					bool ret = false; 
					long w, h;
					GetGraphicSize( &w, &h );
					if ( w < 0 || h < 0 )
					{
						MsgBox( "Error", "書き込むデータがありません" );
					}
					else
					{
						if ( FileOpenDialog( hWnd, path, "LAGファイル", 0 ) )
						{
							// データ取得＆書き込み
							long pitch;
							PIXELDATA32 *pData = LockSurface( &pitch );
							if ( pData != NULL )
							{
								if ( SaveLAG( path, DataName, w, h, pitch, pData, Format, 0 ) )
								{
									ret = true;
								}
								UnlockSurface();
							}

							if ( ret == false ) MsgBox( "Error", "LAGファイルの書き込みに失敗" );
						}
					}
				}
				break;
			/////////////////////////////////////////// 32BitBMPファイル追記保存
			case ID_SAVE32BMP:
				{
					bool ret = false; 
					long w, h;
					GetGraphicSize( &w, &h );
					if ( w < 0 || h < 0 )
					{
						MsgBox( "Error", "書き込むデータがありません" );
					}
					else
					{
						if ( FileSaveDialog( hWnd, path, "32Bitアルファ付きBMPファイル", 1 ) )
						{
							// データ取得＆書き込み
							long pitch;
							PIXELDATA32 *pData = LockSurface( &pitch );
							if ( pData != NULL )
							{
								if ( SaveBitmap32( path, w, h, pitch, pData ) )
								{
									ret = true;
								}
								UnlockSurface();
							}

							if ( ret == false ) MsgBox( "Error", "BMPファイルの書き込みに失敗" );
						}
					}
				}
				break;
			/////////////////////////////////////////// カラーキー
			case ID_COLORKEY:
				RestoreColorKey();
				break;
			/////////////////////////////////////////// バッチ処理
			case ID_BATCH:
				if ( GetFolderPath( path ) )
				{
					long num;
					if ( BatchProcess( path, hWnd, &num ) )
					{
						sprintf( path, "バッチ処理完了\n%d枚の画像をパック化しました", num );
						MsgBox( "Information", path );
					}
				}
				break;
			/////////////////////////////////////////// 左右反転
			case IR_CHANGE_LR:
				GraphciLeftRight();
				break;
			/////////////////////////////////////////// 上下反転
			case ID_CHANGE_UD:
				GraphciUpDown();
				break;
			/////////////////////////////////////////// 画像反転
			case ID_REVERSE:
				GraphciReverse();
			/////////////////////////////////////////// ARGB 16Bitカラー
			case ID_PIXEL_A4R4G4B4:
				SetFormat( LAGFMT_A4R4G4B4, hWnd );
				break;
			/////////////////////////////////////////// ARGB 32Bitカラー
			case ID_PIXEL_A8R8G8B8:
				SetFormat( LAGFMT_A8R8G8B8, hWnd );
				break;
			/////////////////////////////////////////// ARGB 64Bitカラー
			case ID_PIXEL_A16R16G16B16:
				SetFormat( LAGFMT_A16R16G16B16, hWnd );
				break;
			/////////////////////////////////////////// ARGB float型カラー
			case ID_PIXEL_FLOAT:
				SetFormat( LAGFMT_FLOAT, hWnd );
				break;
			/////////////////////////////////////////// 名前変更
			case ID_RENAME:
				long w, h;
				GetGraphicSize( &w, &h );
				if ( w > 0 && h > 0 )
				{
					DialogBox( GetModuleHandle(NULL), "IDD_DLG_NAME", hWnd, (DLGPROC)DlgNameProc);
					sprintf( path, "%s < %s >", AppName, DataName );
					SetWindowText( hWnd, path );
				}
                break;
			/////////////////////////////////////////// 終了
			case ID_CLOSE:
				SendMessage( hWnd, WM_CLOSE, 0, 0 );
				break;
			}
		}
		break;
		//====================================================
		// マウスクリック時
		//====================================================
	case WM_LBUTTONDOWN:
		{
			long r, g, b;
			char str[256] = "";
			// マウス位置
			POINT pt;
			GetCursorPos( &pt );
			ScreenToClient( hWnd, &pt );
			// 画像サイズチェック
			long w, h;
			GetGraphicSize( &w, &h );
			if ( 0 < pt.x && pt.x < w && 0 < pt.y && pt.y < h )
			{
				// マウス位置の色取得
				GetColor( &r, &g, &b, pt.x, pt.y );
				sprintf( str, "RGB(%d,%d,%d)をカラーキーにしますか？", r, g, b );
				if ( MessageBox( hWnd, str, "カラーキー付加", MB_YESNO | MB_ICONQUESTION ) == IDYES)
				{
					SetColorKey( r, g, b );
				}
			}
		}
		break;
		//====================================================
		// ウィンドウ更新時
		//====================================================
	case WM_PAINT:
		{
			Refresh( hWnd, DrawX, DrawY );
		}
		break;
		//====================================================
		// ウィンドウ生成時
		//====================================================
	case WM_CREATE:
		{
			if ( InitDirectX( hWnd ) == false )
			{
				PostMessage( hWnd, WM_CLOSE, 0, 0 );
			}
		}
		break;
		//====================================================
		// ウィンドウ破壊
		//====================================================
	case WM_DESTROY:
		{
			UninitDirectX();
			// 終了
			PostQuitMessage( 0 );
		}
		break;
		//====================================================
		// ウィンドウサイズ変更
		//====================================================
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect( hWnd, &rect );
			DispNoV = (rect.bottom - rect.top) / BAR_DIV;
			DispNoH = (rect.right - rect.left) / BAR_DIV;

			long w, h;
			GetGraphicSize( &w, &h );

			long LineMaxV = w / BAR_DIV;
			long LineMaxH = h / BAR_DIV;

			ScrollInfoV.cbSize = sizeof(SCROLLINFO);
			ScrollInfoV.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
			ScrollInfoV.nMin = 0;
			ScrollInfoV.nMax = LineMaxV;
			ScrollInfoV.nPage = DispNoV;
			ScrollInfoV.nPos = BarPosV;
			SetScrollInfo( hWnd, SB_VERT, &ScrollInfoV, TRUE );

			ScrollInfoH.cbSize = sizeof(SCROLLINFO);
			ScrollInfoH.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
			ScrollInfoH.nMin = 0;
			ScrollInfoH.nMax = LineMaxH;
			ScrollInfoH.nPage = DispNoH;
			ScrollInfoH.nPos = BarPosH;

			SetScrollInfo( hWnd, SB_HORZ, &ScrollInfoH, TRUE );
		}
		break;
		//====================================================
		// 垂直スクロールバー
		//====================================================
	case WM_VSCROLL:
		{
			long dy = 0;
			switch ( LOWORD(wParam) )
			{
			case SB_LINEUP:
				dy = -1;
				break;
			case SB_LINEDOWN:
				dy = +1;
				break;
			case SB_PAGEUP:
				dy = -(long)ScrollInfoV.nPage;
				break;
			case SB_PAGEDOWN:
				dy = +(long)ScrollInfoV.nPage;
				break;
			case SB_THUMBTRACK:
				dy = HIWORD(wParam) - ScrollInfoV.nPos;
				break;
			}
			dy = max( -1 * ScrollInfoV.nPos, min(dy, ScrollInfoV.nMax - ScrollInfoV.nPos) );
			ScrollInfoV.nPos += dy;
			DispNoV = ScrollInfoV.nPos;
			SetScrollInfo( hWnd, SB_VERT, &ScrollInfoV, TRUE );
			DrawY = -(long)(ScrollInfoV.nPos * BAR_DIV);
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;
		//====================================================
		// 水平スクロールバー
		//====================================================
	case WM_HSCROLL:
		{
			long dy = 0;
			switch ( LOWORD(wParam) )
			{
			case SB_LINEUP:
				dy = -1;
				break;
			case SB_LINEDOWN:
				dy = 1;
				break;
			case SB_PAGEUP:
				dy = -(long)ScrollInfoH.nPage;
				break;
			case SB_PAGEDOWN:
				dy = +(long)ScrollInfoH.nPage;
				break;
			case SB_THUMBTRACK:
				dy = HIWORD(wParam) - ScrollInfoH.nPos;
				break;
			}
			dy = max( -1 * ScrollInfoH.nPos, min(dy, ScrollInfoH.nMax - ScrollInfoH.nPos) );
			ScrollInfoH.nPos += dy ;
			DispNoH = ScrollInfoH.nPos;
			SetScrollInfo( hWnd, SB_HORZ, &ScrollInfoH, TRUE );
			DrawX = -(long)(ScrollInfoH.nPos * BAR_DIV);
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//==========================================================================
//
//
//		名前変更ダイアログ用メッセージ処理関数
//
//
//==========================================================================
LRESULT CALLBACK DlgNameProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	char path[256] = "";

	switch ( msg )
	{
	case WM_INITDIALOG:
		{
            SetDlgItemText( hWnd, IDC_EDIT_RENAME, (LPCTSTR)DataName );
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDOK:
                GetDlgItemText( hWnd, IDC_EDIT_RENAME, (LPTSTR)DataName, 128 );
				DataName[15] = '\0';
				EndDialog( hWnd, IDOK );
				return true;
			case IDCANCEL:
				EndDialog( hWnd, IDCANCEL );
				return true;
			}
		}
		break;
	}

	return false;
}

//==========================================================================
//
//
//		ウィンドウを画面中央に設定
//
//
//==========================================================================
bool SetWinCenter( HWND hWnd, long w, long h )
{
	// ウィンドウサイズチェック
	if ( w < 0 || h < 0 ) GetWindowSize( hWnd, &w, &h );

	if ( w < 128 ) w = 128;
	if ( h < 128 ) h = 128;

	// メニューやタイトルバーを含むウィンドウのサイズ取得
	long window_w = w + GetSystemMetrics(SM_CXEDGE) + GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXDLGFRAME);
	long window_h = h + GetSystemMetrics(SM_CYEDGE) + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU);

	// 画面の中央にくるようにする
	long x = GetSystemMetrics(SM_CXSCREEN)/2 - window_w/2;
	long y = GetSystemMetrics(SM_CYSCREEN)/2 - window_h/2;

	// 左上がはみ出す場合 (0,0) にくるようにする
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;

	// ウィンドウ位置更新
	SetWindowPos(
			hWnd,
			HWND_TOP,
			x,
			y,
			window_w, window_h,
			SWP_SHOWWINDOW
		);

	return true;
}

//==========================================================================
//
//
//		ウィンドウサイズ取得
//
//
//==========================================================================
void GetWindowSize( HWND hWnd, long *w, long *h )
{
	RECT rect = { 0, 0, 0, 0 };
	GetClientRect( hWnd, &rect );

	if ( w != NULL ) *w = rect.right - rect.left;
	if ( h != NULL ) *h = rect.bottom - rect.top;
}

//==========================================================================
//
//
//		ファイルオープンファイアログ
//
//
//==========================================================================
bool FileOpenDialog( HWND hWnd, char *path, char *title, long mode )
{
	char temp[512] = "";

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof(OPENFILENAME) );
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = ((mode)?("BMPファイル(*.bmp)\0*.bmp\0"):("LAGファイル(*.lag)\0*.lag\0"));
	ofn.lpstrDefExt = ((mode)?("BMP"):("LAG"));
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = temp;
	ofn.nMaxFile = 512;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if ( GetOpenFileName( &ofn ) != 0 )
	{
		if ( strlen( temp ) > 0 )
		{
			strcpy( path, temp );
			return true;
		}
	}

	return false;
}

//==========================================================================
//
//
//		ファイルセーブダイアログ
//
//
//==========================================================================
bool FileSaveDialog( HWND hWnd, char *path, char *title, long mode )
{
	char temp[512] = "";
	
	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof(OPENFILENAME) );
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = ((mode)?("BMPファイル(*.bmp)\0*.bmp\0"):("LAGファイル(*.lag)\0*.lag\0"));
	ofn.lpstrDefExt = ((mode)?("BMP"):("LAG"));
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = temp;
	ofn.nMaxFile = 512;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	if ( GetOpenFileName( &ofn ) != 0 )
	{
		if ( strlen( temp ) > 0 )
		{
			strcpy( path, temp );
			return true;
		}
	}

	return false;
}

//==========================================================================
//
//
//		フォルダ選択
//
//
//==========================================================================
bool GetFolderPath( char *path )
{
	//============================================
	// フォルダ選択
	//============================================
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	LPMALLOC pMalloc;
	bool result = false;

	if SUCCEEDED( SHGetMalloc( &pMalloc ) )
	{
		ZeroMemory( &bi, sizeof(BROWSEINFO) );

		bi.lpszTitle		= "フォルダ選択";
		bi.ulFlags			= BIF_RETURNONLYFSDIRS;

		pidl = SHBrowseForFolder( &bi );

		if ( pidl != NULL )
		{
			if ( SHGetPathFromIDList( pidl, path ) == TRUE )
			{
				result = true;
			}
			pMalloc->Free( pidl );
		}
		pMalloc->Release();
	}

	return result;
}

//==========================================================================
//
//
//		名前変換
//
//
//==========================================================================
void ConvertName( char *file )
{
	char temp[256];
	strcpy( temp, file );

	long num = -1;

	// ファイル名をすべて小文字に
	num = 0;
	while ( temp[num] != '\0' )
	{
		if ( 'A' <= temp[num] && temp[num] <= 'Z' )
		{
			temp[num] += ('a' - 'A');
		}
		num++;
	}

	// 絶対パスからファイル名取り出し
	num = strlen( temp );
	while ( temp[num] != '\\' ) if ( --num < 0 ) break;
	char *p = temp + num + 1;

	// 拡張子削除
	num = strlen( p );
	while ( true )
	{
		char c = p[num];
		p[num] = '\0';
		num--;
		if ( c == '.' ) break;
	}

	// データ名クリア
	memset( DataName, 0x00, 128 );

	// データ名コピー
	strcpy( DataName, p );
}

//==========================================================================
//
//
//		名前チェック
//
//
//==========================================================================
bool CheckAlphaName( char *file )
{
	char temp[256];
	strcpy( temp, file );

	long num = -1;

	// 拡張子削除
	num = strlen( temp );
	while ( true )
	{
		char c = temp[num];
		temp[num] = '\0';
		num--;
		if ( c == '.' ) break;
	}

	// アルファ用ファイルかチェック
	num = strlen( temp );

	return ( temp[num-2] == '_' && (temp[num-1] == 'a' || temp[num-1] == 'A') );
}

//==========================================================================
//
//
//		アルファチャンネル名に変更
//
//
//==========================================================================
void ConvertAlphaName( char *file )
{
	long num = -1;
	char flt[16] = "";

	// 拡張子保存
	num = strlen( file );
	while ( true )
	{
		if ( file[num--] == '.' ) break;
	}
	strcpy( flt, &file[num+1] );
	file[num+1] = '\0';

	// アルファ名付加
	strcat( file, "_a" );
	strcat( file, flt );
}


//==========================================================================
//
//
//		バッチ処理
//
//
//==========================================================================
bool BatchProcess( char *dir, HWND hWnd, long *num )
{
	WIN32_FIND_DATA FindData;
	char SavePath[256] = "";
	char FilePath[256] = "";

	//====================================================
	// ファイルのワイルドカード設定
	//====================================================
	sprintf( FilePath, "%s\\*.bmp", dir );

	//====================================================
	// 一つ目のファイル
	//====================================================
	HANDLE hFile = FindFirstFile( FilePath, &FindData );
	if ( hFile == INVALID_HANDLE_VALUE ) return false;

	//====================================================
	// 書き込みファイル用ダイアログ
	//====================================================
	if ( FileSaveDialog( hWnd, SavePath, "LAGファイル保存", 0 ) == false ) return false;

	//====================================================
	// ファイルヘッダ書き込み
	//====================================================
	if ( SaveLAGHeader( SavePath, 0 ) == false ) return false;

	//====================================================
	// ファイル列挙
	//====================================================
	(*num) = 0;
	while ( true )
	{
		char *p = FindData.cFileName;
		if ( p[1] != ':' )
		{
			char temp[256] = "";
			sprintf( temp, "%s\\%s", dir, p );
			p = temp;
		}
		// アルファチャンネル用ファイルかチェック
		if ( !CheckAlphaName( p ) )
		{
			(*num)++;
			// 画像読み込み
			if ( LoadBMP( p, FMT_GRAPHIC ) )
			{
				long w, h;
				GetGraphicSize( &w, &h );
				SetWinCenter( hWnd, w, h );
				// データ名取得
				ConvertName( p );
				// アルファチャンネルデータ自動読み込み
				ConvertAlphaName( p );
				LoadBMP( p, FMT_ALPHA );
				// データ取得＆書き込み
				long pitch;
				PIXELDATA32 *pData = LockSurface( &pitch );
				if ( pData != NULL )
				{
					if ( SaveLAG( SavePath, DataName, w, h, pitch, pData, Format, 0 ) == false )
					{
						MsgBox( "Error", "データ書き込みに失敗" );
						UnlockSurface();
						return false;
					}
					UnlockSurface();
				}
			}
		}
		// 次のファイル
		if ( FindNextFile( hFile, &FindData ) == FALSE ) break;
	}

	return true;
}

//==========================================================================
//
//
//		ピクセルフォーマット設定
//
//
//==========================================================================
void SetFormat( LAGPIXELFORMAT fmt, HWND hWnd )
{
	long no = -1;

	switch ( fmt )
	{
	case LAGFMT_A4R4G4B4:
		no = 0;
		break;
	case LAGFMT_A8R8G8B8:
		no = 1;
		break;
	case LAGFMT_A16R16G16B16:
		no = 2;
		break;
	case LAGFMT_FLOAT:
		no = 3;
		break;
	}

	const long id[] = { ID_PIXEL_A4R4G4B4, ID_PIXEL_A8R8G8B8, ID_PIXEL_A16R16G16B16, ID_PIXEL_FLOAT };

	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	mii.fMask = MIIM_STATE;

	for ( int i = 0; i < 4; i++ )
	{
		mii.fState = MFS_UNCHECKED;
		SetMenuItemInfo( GetMenu( hWnd ), id[i], false, &mii );
	}

	mii.fState = MFS_CHECKED;
	SetMenuItemInfo( GetMenu( hWnd ), id[no], false, &mii );

	Format = fmt;
}


//==========================================================================
//
//
//		32Bit Bitmap ファイル保存
//
//
//==========================================================================
bool SaveBitmap32( char *file, long w, long h, long pitch, PIXELDATA32 *pData )
{
	//================================================
	// ビットマップ構造体確保
	//================================================
	BITMAPFILEHEADER bmpfh;
	BITMAPINFOHEADER bmpih;

	bmpfh.bfType			= ('B' << 0) | ('M' << 8);
	bmpfh.bfSize			= (w*h*4) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpfh.bfReserved1		= 0;
	bmpfh.bfReserved2		= 0;
	bmpfh.bfOffBits			= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmpih.biSize			= sizeof(BITMAPINFOHEADER);
	bmpih.biWidth			= w;
	bmpih.biHeight			= h;
	bmpih.biPlanes			= 1;
	bmpih.biBitCount		= 32;
	bmpih.biCompression		= 0;
	bmpih.biSizeImage		= 0;
	bmpih.biXPelsPerMeter	= 0;
	bmpih.biYPelsPerMeter	= 0;
	bmpih.biClrUsed			= 0;
	bmpih.biClrImportant	= 0;

	//================================================
	// データ書き出し
	//================================================
	FILE *fp = fopen( file, "wb" );
	if ( fp == NULL ) return false;

	// ヘッダ
	fwrite( &bmpfh, sizeof(BITMAPFILEHEADER), 1, fp );
	fwrite( &bmpih, sizeof(BITMAPINFOHEADER), 1, fp );

	// データ;
	pData += (h-1) * pitch;
	for ( int y = 0; y < h; y++ )
	{
		for ( long x = 0; x < w; x++ )
		{
			fwrite( &pData[x], sizeof(PIXELDATA32), 1, fp );
		}
		pData -= pitch;
	}

	return true;
}