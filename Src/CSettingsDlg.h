#pragma once

#include "CSettingsOptionDlg.h"

// CSettingsDlg ダイアログ
#include "DialogFx.h"
#include "StaticFx.h"
#include "ButtonFx.h"

class CSettingsDlg : public CDialogFx
{
	DECLARE_DYNAMIC(CSettingsDlg)
	static const int SIZE_X = 480;
	static const int SIZE_Y = 300;

public:
	CSettingsDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSettingsDlg();

	CSettingsOptionDlg* m_SettingsOptionDlg;

	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic ChildHolder;
};
