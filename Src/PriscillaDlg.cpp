﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Priscilla.h"
#include "PriscillaDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPriscillaDlg::CPriscillaDlg(CWnd* pParent /*=NULL*/)
	: CMainDialog(CPriscillaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconMini = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);
	m_SizeX = SIZE_X;
	m_SizeY = SIZE_Y;
	m_AdminMode = IsUserAnAdmin();

	m_AboutDlg = NULL;

	m_DefaultTheme = L"Default";
	m_RecommendTheme = L"Default";
	m_ThemeKeyName = L"Theme";
	m_BackgroundName = L"Background";
}

CPriscillaDlg::~CPriscillaDlg()
{
}

void CPriscillaDlg::DoDataExchange(CDataExchange* pDX)
{
	CMainDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_METER1, m_Meter1);
	DDX_Control(pDX, IDC_LIST1, m_List1);
}

BEGIN_MESSAGE_MAP(CPriscillaDlg, CMainDialog)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXIT, OnExit)
	ON_COMMAND(ID_HELP_ABOUT, OnAbout)
	ON_COMMAND(ID_ZOOM_100, &CPriscillaDlg::OnZoom100)
	ON_COMMAND(ID_ZOOM_125, &CPriscillaDlg::OnZoom125)
	ON_COMMAND(ID_ZOOM_150, &CPriscillaDlg::OnZoom150)
	ON_COMMAND(ID_ZOOM_200, &CPriscillaDlg::OnZoom200)
	ON_COMMAND(ID_ZOOM_250, &CPriscillaDlg::OnZoom250)
	ON_COMMAND(ID_ZOOM_300, &CPriscillaDlg::OnZoom300)
	ON_COMMAND(ID_ZOOM_AUTO, &CPriscillaDlg::OnZoomAuto)
	ON_COMMAND(ID_HELP, &CPriscillaDlg::OnHelp)
	ON_COMMAND(ID_HELP_CRYSTALDEWWORLD, &CPriscillaDlg::OnCrystalDewWorld)
	ON_COMMAND(ID_FONT_SETTING, &CPriscillaDlg::OnFontSetting)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPriscillaDlg::OnButton1)
END_MESSAGE_MAP()

LRESULT CPriscillaDlg::OnQueryEndSession(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

BOOL CPriscillaDlg::CheckThemeEdition(CString name)
{
	if(name.Find(L".") != 0) { return TRUE; }

	return FALSE;
}

BOOL CPriscillaDlg::OnInitDialog()
{
	CMainDialog::OnInitDialog();

	m_hAccelerator = ::LoadAccelerators(AfxGetInstanceHandle(),
		                                MAKEINTRESOURCE(IDR_ACCELERATOR));

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIconMini, FALSE);

	TCHAR str[256];
	GetPrivateProfileString(L"Setting", L"FontFace", GetDefaultFont(), str, 256, m_Ini);
	m_FontFace = str;

	m_FontScale = GetPrivateProfileInt(L"Setting", L"FontScale", 100, m_Ini);
	if (m_FontScale > 200 || m_FontScale < 50)
	{
		m_FontScale = 100;
		m_FontRatio = 1.0;
	}
	else
	{
		m_FontRatio = m_FontScale / 100.0;
	}

	InitThemeLang();
	InitMenu();
	ChangeTheme(m_CurrentTheme);
	ChangeLang(m_CurrentLang);

	UpdateData(FALSE);

	ChangeZoomType(m_ZoomType);
	
	m_SizeX = SIZE_X;
	m_SizeY = SIZE_Y;

	SetWindowTitle(L"");

	SetClientSize((int)(m_SizeX * m_ZoomRatio), (int)(m_SizeY * m_ZoomRatio));

	m_bShowWindow = TRUE;

	CenterWindow();

	//// Init Controls
	m_Static1.SetWindowTextW(L"Project Priscilla");
	m_Meter1.SetMeter(TRUE, 0.5);
	m_Meter1.SetWindowTextW(L"50%");

	m_Combo1.AddString(L"Author: hiyohiyo");
	m_Combo1.AddString(L"Project Priscilla");
	m_Combo1.AddString(L"Crystal Dew World");

	DWORD style = ListView_GetExtendedListViewStyle(m_List1.m_hWnd);
	style |= LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES |*/ LVS_EX_LABELTIP;
	ListView_SetExtendedListViewStyle(m_List1.m_hWnd, style);

	UpdateDialogSize();

	m_bInitializing = FALSE;

	SetForegroundWindow();

	return TRUE;
}

void CPriscillaDlg::UpdateDialogSize()
{
	ShowWindow(SW_HIDE);

	SetClientSize((int)(m_SizeX * m_ZoomRatio), (int)(m_SizeY * m_ZoomRatio), 1);
	UpdateBackground(TRUE);
	SetControlFont();

	////
	//// InitControl
	////
	m_Static1.InitControl(8, 8, 464, 24, m_ZoomRatio, &m_BgDC, NULL, 0, SS_LEFT, OwnerDrawTransparent | m_bHighContrast);
	m_Button1.InitControl(8, 40, 72, 48, m_ZoomRatio, &m_BgDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage | m_bHighContrast);
	m_Meter1.InitControl(88, 40, 192, 48, m_ZoomRatio, &m_BgDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage | m_bHighContrast);
	m_Combo1.InitControl(288, 40, 184, 300, m_ZoomRatio, &m_BgDC, NULL, 0, ES_LEFT, OwnerDrawGlass | m_bHighContrast, m_ComboBg, m_ComboBgSelected, m_Glass, m_GlassAlpha);
	m_Edit1.InitControl(8, 100, 464, 40, m_ZoomRatio, &m_BgDC, NULL, 0, ES_LEFT, OwnerDrawGlass | m_bHighContrast);
	m_List1.InitControl(8, 148, 464, 144, 464, 144, m_ZoomRatio, &m_BgDC, OwnerDrawGlass | m_bHighContrast);

	m_Button1.SetHandCursor();
	m_Combo1.SetCurSel(0);
	m_Combo1.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_List1.SetGlassColor(m_Glass, m_GlassAlpha);
	m_List1.DeleteAllItems();
	while (m_List1.DeleteColumn(0)) {}
	m_List1.InsertColumn(0, L"Column0", LVCFMT_LEFT, (int)(100 * m_ZoomRatio), 0);
	m_List1.InsertColumn(1, L"Column1", LVCFMT_LEFT, (int)(100 * m_ZoomRatio), 0);
	m_List1.InsertColumn(2, L"Column2", LVCFMT_LEFT, (int)(260 * m_ZoomRatio), 0);
	m_List1.InsertItem(0, L"Item0-0");
	m_List1.InsertItem(1, L"Item1-0");
	m_List1.InsertItem(2, L"Item2-0");
	m_List1.SetItemText(0, 1, L"Item0-1");
	m_List1.SetItemText(0, 2, L"Item0-2");
	m_List1.SetItemText(1, 1, L"Item1-1");
	m_List1.SetItemText(1, 2, L"Item1-2");
	m_List1.SetItemText(2, 1, L"Item2-1");
	m_List1.SetItemText(2, 2, L"Item2-2");

	m_List1.EnableHeaderOwnerDraw(TRUE);

	m_List1.SetTextColor1(m_ListText1);
	m_List1.SetTextColor2(m_ListText2);
	m_List1.SetBkColor1(m_ListBg1);
	m_List1.SetBkColor2(m_ListBg2);
	m_List1.SetLineColor1(m_ListLine1);
	m_List1.SetLineColor2(m_ListLine2);

	COMBOBOXINFO info = { 0 };
	info.cbSize = sizeof(COMBOBOXINFO);
	m_Combo1.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);

	Invalidate();

	ShowWindow(SW_SHOW);
}

void CPriscillaDlg::SetControlFont()
{
	////
	//// Set Control's Font
	////
	m_Static1.SetFontEx(m_FontFace, 16, 24, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);
	m_Button1.SetFontEx(m_FontFace, 16, 24, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_Meter1.SetFontEx(m_FontFace, 16, 24, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);
	m_Combo1.SetFontEx(m_FontFace, 16, 24, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_Edit1.SetFontEx(m_FontFace, 32, m_ZoomRatio, m_FontRatio, m_EditText, FW_BOLD);
	m_List1.SetFontEx(m_FontFace, m_ZoomRatio, m_FontRatio);

	m_Combo1.SetFontHeight(36, m_ZoomRatio, m_FontRatio);
	m_Combo1.SetItemHeightEx(-1, 44, m_ZoomRatio, m_FontRatio);
	for (int i = 0; i < m_Combo1.GetCount(); i++)
	{
		m_Combo1.SetItemHeightEx(i, 32, m_ZoomRatio, m_FontRatio);
	}
}

BOOL CPriscillaDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( 0 != ::TranslateAccelerator(m_hWnd, m_hAccelerator, pMsg) )
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPriscillaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CMainDialog::OnPaint();
	}
}

HCURSOR CPriscillaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPriscillaDlg::OnOK()
{

}

void CPriscillaDlg::OnExit()
{
	OnCancel();
}

void CPriscillaDlg::OnAbout()
{
	m_AboutDlg = new CAboutDlg(this);
	m_AboutDlg->Create(CAboutDlg::IDD, m_AboutDlg, ID_HELP_ABOUT, this);
}

void CPriscillaDlg::OnCancel()
{
	CMainDialog::OnCancel();
}

void CPriscillaDlg::EnableMenus()
{
	CMenu *menu = GetMenu();
	for (int i = 0; i < menu->GetMenuItemCount(); i++)
	{
		menu->EnableMenuItem(i, MF_BYPOSITION | MF_ENABLED);
	}
	SetMenu(menu);
}

void CPriscillaDlg::DisableMenus()
{
	CMenu* menu = GetMenu();
	for (int i = 0; i < menu->GetMenuItemCount(); i++)
	{
		menu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	}
	SetMenu(menu);
}

void CPriscillaDlg::ChangeLang(CString LangName)
{
	m_CurrentLangPath.Format(L"%s\\%s.lang", (LPTSTR)m_LangDir.GetString(), (LPTSTR)LangName.GetString());

	CString cstr;
	CMenu *menu = GetMenu();
	CMenu subMenu;

	cstr = i18n(L"Menu", L"FILE");
	menu->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, cstr);
	cstr = i18n(L"Menu", L"THEME");
	menu->ModifyMenu(1, MF_BYPOSITION | MF_STRING, 1, cstr);
	cstr = i18n(L"Menu", L"HELP");
	menu->ModifyMenu(2, MF_BYPOSITION | MF_STRING, 2, cstr);
	cstr = i18n(L"Menu", L"LANGUAGE");
	if(cstr.Find(L"Language") >= 0)
	{
		cstr = L"&Language";
		menu->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, cstr);
	}
	else
	{
		menu->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, cstr + L"(&Language)");
	}

	cstr = i18n(L"Menu", L"FILE_EXIT") + L"\tAlt + F4";
	menu->ModifyMenu(ID_FILE_EXIT, MF_STRING, ID_FILE_EXIT, cstr);

	cstr = i18n(L"Menu", L"HELP") + L"\tF1";
	menu->ModifyMenu(ID_HELP, MF_STRING, ID_HELP, cstr);
	cstr = i18n(L"Menu", L"HELP_ABOUT");
	menu->ModifyMenu(ID_HELP_ABOUT, MF_STRING, ID_HELP_ABOUT, cstr);

	// Theme
	subMenu.Attach(menu->GetSubMenu(MENU_THEME_INDEX)->GetSafeHmenu());
	cstr = i18n(L"Menu", L"ZOOM");
	subMenu.ModifyMenu(0, MF_BYPOSITION, 0, cstr);
	subMenu.Detach();

	cstr = i18n(L"Menu", L"AUTO");
	menu->ModifyMenu(ID_ZOOM_AUTO, MF_STRING, ID_ZOOM_AUTO, cstr);

	cstr = i18n(L"Menu", L"FONT_SETTING") + L"\tCtrl + F";
	menu->ModifyMenu(ID_FONT_SETTING, MF_STRING, ID_FONT_SETTING, cstr);

	CheckRadioZoomType();

	SetMenu(menu);

	WritePrivateProfileString(L"Setting", L"Language", LangName, m_Ini);
}

BOOL CPriscillaDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Select Theme
	if (WM_THEME_ID <= wParam && wParam < WM_THEME_ID + (UINT)m_MenuArrayTheme.GetSize())
	{
		CMainDialog::OnCommand(wParam, lParam);
		return TRUE;
	}

	// Select Language
	if(WM_LANGUAGE_ID <= wParam && wParam < WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize())
	{
		CMenu menu;
		CMenu subMenu;
		CMenu subMenuAN;
		CMenu subMenuOZ;
		menu.Attach(GetMenu()->GetSafeHmenu());
		subMenu.Attach(menu.GetSubMenu(MENU_LANG_INDEX)->GetSafeHmenu());
		subMenuAN.Attach(subMenu.GetSubMenu(0)->GetSafeHmenu());
		subMenuOZ.Attach(subMenu.GetSubMenu(1)->GetSafeHmenu());

		m_CurrentLang = m_MenuArrayLang.GetAt(wParam - WM_LANGUAGE_ID);
		ChangeLang(m_MenuArrayLang.GetAt(wParam - WM_LANGUAGE_ID));
		subMenuAN.CheckMenuRadioItem(WM_LANGUAGE_ID, WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize(),
									(UINT)wParam, MF_BYCOMMAND);
		subMenuOZ.CheckMenuRadioItem(WM_LANGUAGE_ID, WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize(),
									(UINT)wParam, MF_BYCOMMAND);

		subMenuOZ.Detach();
		subMenuAN.Detach();
		subMenu.Detach();
		menu.Detach();
	}

	return CMainDialog::OnCommand(wParam, lParam);
}

void CPriscillaDlg::OnZoom100()
{
	if (CheckRadioZoomType(ID_ZOOM_100, 100))
	{
		UpdateDialogSize();
	}
}

void CPriscillaDlg::OnZoom125()
{
	if (CheckRadioZoomType(ID_ZOOM_125, 125))
	{
		UpdateDialogSize();
	}
}

void CPriscillaDlg::OnZoom150()
{
	if (CheckRadioZoomType(ID_ZOOM_150, 150))
	{
		UpdateDialogSize();
	}
}

void CPriscillaDlg::OnZoom200()
{
	if (CheckRadioZoomType(ID_ZOOM_200, 200))
	{
		UpdateDialogSize();
	}
}

void CPriscillaDlg::OnZoom250()
{
	if (CheckRadioZoomType(ID_ZOOM_250, 250))
	{
		UpdateDialogSize();
	}
}

void CPriscillaDlg::OnZoom300()
{
	if (CheckRadioZoomType(ID_ZOOM_300, 300))
	{
		UpdateDialogSize();
	}
}

void CPriscillaDlg::OnZoomAuto()
{
	if (CheckRadioZoomType(ID_ZOOM_AUTO, 0))
	{
		UpdateDialogSize();
	}
}

BOOL CPriscillaDlg::CheckRadioZoomType(int id, int value)
{
	if(m_ZoomType == value)
	{
		return FALSE;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_ZOOM_100, ID_ZOOM_AUTO, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_ZoomType = value;

	CString cstr;
	cstr.Format(L"%d", value);
	WritePrivateProfileString(L"Setting", L"ZoomType", cstr, m_Ini);

	ChangeZoomType(m_ZoomType);

	return TRUE;
}

void CPriscillaDlg::CheckRadioZoomType()
{
	int id = ID_ZOOM_AUTO;

	switch(m_ZoomType)
	{
	case 100: id = ID_ZOOM_100;	break;
	case 125: id = ID_ZOOM_125;	break;
	case 150: id = ID_ZOOM_150;	break;
	case 200: id = ID_ZOOM_200;	break;
	case 250: id = ID_ZOOM_250;	break;
	case 300: id = ID_ZOOM_300;	break;
	default:  id = ID_ZOOM_AUTO;	break;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_ZOOM_100, ID_ZOOM_AUTO, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();
}

void CPriscillaDlg::OnHelp()
{
	if (GetUserDefaultLCID() == 0x0411) // Japanese
	{
		OpenUrl(URL_HTML_HELP_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_HTML_HELP_EN);
	}
}

void CPriscillaDlg::OnCrystalDewWorld()
{
	if (GetUserDefaultLCID() == 0x0411) // Japanese
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_EN);
	}
}

void CPriscillaDlg::OnFontSetting()
{
	CFontSelectionDlg fontSelection(this);
	if (fontSelection.DoModal() == IDOK)
	{
		m_FontFace = fontSelection.GetFontFace();
		m_FontScale = fontSelection.GetFontScale();
		m_FontRatio = m_FontScale / 100.0;

		CString cstr;
		WritePrivateProfileString(L"Setting", L"FontFace", L"\"" + m_FontFace + L"\"", m_Ini);
		cstr.Format(L"%d", m_FontScale);
		WritePrivateProfileString(L"Setting", L"FontScale", cstr, m_Ini);

		UpdateDialogSize();
	}
}

void CPriscillaDlg::SetWindowTitle(CString message)
{
	CString title;

	if (!message.IsEmpty())
	{
		title.Format(L"%s - %s", PRODUCT_SHORT_NAME, message.GetString());
	}
	else
	{
		title.Format(L"%s %s %s", PRODUCT_NAME, PRODUCT_VERSION, PRODUCT_EDITION);
	}

	if (m_AdminMode)
	{
		title += L" [ADMIN]";
	}

	SetWindowText(title);
}

void CPriscillaDlg::OnButton1()
{
	CString cstr;
	int value = rand() % 100;
	cstr.Format(L"%d%%", value);
	m_Meter1.SetWindowTextW(cstr);
	m_Meter1.SetMeter(TRUE, value / 100.0);
}