// ConfigDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Config.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================
// 構造体
//===================================================

// コンフィグ
typedef struct _CONFIGDATA
{
	unsigned char IsWindow;
	unsigned char IsFullColor;
	unsigned char IsFullColorTexture;
}
CONFIGDATA, *LPCONFIGDATA;



//===================================================
// スタティック変数
//===================================================
CONFIGDATA Config;


/////////////////////////////////////////////////////////////////////////////
// CConfigDlg ダイアログ

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg メッセージ ハンドラ

BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	Config.IsWindow				= FALSE;
	Config.IsFullColor			= FALSE;
	Config.IsFullColorTexture	= FALSE;

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CConfigDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CConfigDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CConfigDlg::OnCheck1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	Config.IsWindow = !Config.IsWindow;
}

void CConfigDlg::OnCheck2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	Config.IsFullColor = !Config.IsFullColor;
}

void CConfigDlg::OnCheck3() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	Config.IsFullColorTexture = !Config.IsFullColorTexture;
}

void CConfigDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	FILE *fp = fopen( "config.dat", "wb" );
	if ( fp != NULL )
	{
		fwrite( &Config, sizeof(CONFIGDATA), 1, fp );
		fclose( fp );
	}

	CDialog::OnOK();
}
