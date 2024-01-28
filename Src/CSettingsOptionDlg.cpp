// CSettingsOptionDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Priscilla.h"
#include "PriscillaDlg.h"
#include "CSettingsOptionDlg.h"

// CSettingsOptionDlg ダイアログ

IMPLEMENT_DYNAMIC(CSettingsOptionDlg, CDialog)

CSettingsOptionDlg::CSettingsOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogFx(IDD_SETTINGS_OPTION, pParent)
{
	CMainDialogFx* p = (CMainDialogFx*)pParent;

	m_ZoomType = p->GetZoomType();
	m_FontScale = p->GetFontScale();
	m_FontRatio = p->GetFontRatio();
	m_FontFace = p->GetFontFace();
	m_CurrentLangPath = p->GetCurrentLangPath();
	m_DefaultLangPath = p->GetDefaultLangPath();
	m_ThemeDir = p->GetThemeDir();
	m_CurrentTheme = p->GetCurrentTheme();
	m_DefaultTheme = p->GetDefaultTheme();
	m_Ini = p->GetIniPath();
	m_bDisableDarkMode = p->IsDisableDarkMode();

	m_BackgroundName = L"Background";
}

CSettingsOptionDlg::~CSettingsOptionDlg()
{
}

void CSettingsOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsOptionDlg, CDialogFx)
END_MESSAGE_MAP()


// CSettingsOptionDlg メッセージ ハンドラー


BOOL CSettingsOptionDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	CDialogFx::UpdateDialogSize();

	ChangeZoomType(m_ZoomType);
	SetClientSize(SIZE_X, SIZE_Y, m_ZoomRatio);
	UpdateBackground(TRUE, m_bDarkMode);

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
