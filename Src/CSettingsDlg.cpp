// CSettingsDlg.cpp : 実装ファイル
//
#include "stdafx.h"
#include "Priscilla.h"
#include "PriscillaDlg.h"
#include "CSettingsDlg.h"

// CSettingsDlg ダイアログ

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogFx(IDD_SETTINGS, pParent)
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

	m_BackgroundName = L"";
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHILD_HOLDER, ChildHolder);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogFx)
END_MESSAGE_MAP()


// CSettingsDlg メッセージ ハンドラー


BOOL CSettingsDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	CRect rc;
	ChildHolder.GetWindowRect(rc);
	ScreenToClient(&rc);
	rc.left = 0;
	rc.top = 0;

	m_SettingsOptionDlg = new CSettingsOptionDlg(this);
	m_SettingsOptionDlg->Create(CSettingsOptionDlg::IDD, &ChildHolder, 0, &ChildHolder);
	m_SettingsOptionDlg->MoveWindow(rc);
	m_SettingsOptionDlg->ShowWindow(SW_SHOW);

	CDialogFx::UpdateDialogSize();

	ChangeZoomType(m_ZoomType);
	SetClientSize(SIZE_X, SIZE_Y, m_ZoomRatio);
	UpdateBackground(TRUE, m_bDarkMode);

	CenterWindow();

	ShowWindow(SW_SHOW);

	return TRUE; 
}
