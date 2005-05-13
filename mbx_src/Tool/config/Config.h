// Config.h : CONFIG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_CONFIG_H__18BBEEB7_6E30_4FFE_97A2_50BFE49B799C__INCLUDED_)
#define AFX_CONFIG_H__18BBEEB7_6E30_4FFE_97A2_50BFE49B799C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CConfigApp:
// このクラスの動作の定義に関しては Config.cpp ファイルを参照してください。
//

class CConfigApp : public CWinApp
{
public:
	CConfigApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfigApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CConfigApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFIG_H__18BBEEB7_6E30_4FFE_97A2_50BFE49B799C__INCLUDED_)
