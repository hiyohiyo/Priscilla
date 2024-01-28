/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "AboutDlg.h"
#include "CSettingsDlg.h"
#include "FontSelectionDlg.h"

#include "DialogFx.h"
#include "MainDialogFx.h"
#include "ButtonFx.h"
#include "StaticFx.h"
#include "EditFx.h"
#include "ComboBoxFx.h"
#include "ListCtrlFx.h"
#include "SliderCtrlFx.h"
#include "ScrollBarFx.h"
#include "UtilityFx.h"
#include "OsInfoFx.h"

class CPriscillaDlg : public CMainDialogFx
{
public:
	CPriscillaDlg(CWnd* pParent = NULL);
	~CPriscillaDlg();

	enum { IDD = IDD_MAIN };

	static const int SIZE_X = 488;
	static const int SIZE_Y = 300;
	static const int SIZE_MIN_Y = 300;
	static const int OFFSET_X = 0;
	static const int MAX_METER_LENGTH = 192;

	void ChangeLang(CString LangName);
	void UpdateDialogSize();
	void SetWindowTitle(CString message);

protected:
	void SetControlFont();
	BOOL CheckRadioZoomType(int id, int value);
	void CheckRadioZoomType();
	void EnableMenus();
	void DisableMenus();

	virtual BOOL CheckThemeEdition(CString name);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnQueryEndSession(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnZoom100();
	afx_msg void OnZoom125();
	afx_msg void OnZoom150();
	afx_msg void OnZoom200();
	afx_msg void OnZoom250();
	afx_msg void OnZoom300();
	afx_msg void OnZoomAuto();
	afx_msg void OnDisableDarkMode();
	afx_msg void OnExit();
	afx_msg void OnSaveImage();
	afx_msg void OnAbout();
	afx_msg void OnFontSetting();
	afx_msg void OnHelp();
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	HICON m_hIcon;
	HICON m_hIconMini;
	HACCEL m_hAccelerator;
	BOOL m_AdminMode;

	int m_SizeX;
	int m_SizeY;

	CAboutDlg* m_AboutDlg;
	CSettingsDlg* m_SettingsDlg;

	CButtonFx m_Button1;
	CButtonFx m_Button2;
	CButtonFx m_Button3;
	CButtonFx m_Button4;

	CComboBoxFx m_Combo1;
	CEditFx m_Edit1;
	CStaticFx m_Static1;
	CStaticFx m_Static2;
	CStaticFx m_Static3;
	CStaticFx m_Meter1;
	CListCtrlFx m_List1;
	CSliderCtrlFx m_Slider1;
	CScrollBarFx m_Scrollbar1;

public:
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
};
