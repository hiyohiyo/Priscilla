#pragma once


// CSettingsOptionDlg ダイアログ
#include "DialogFx.h"
#include "StaticFx.h"
#include "ButtonFx.h"

class CSettingsOptionDlg : public CDialogFx
{
	DECLARE_DYNAMIC(CSettingsOptionDlg)
	static const int SIZE_X = 1000;
	static const int SIZE_Y = 1000;

public:
	CSettingsOptionDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSettingsOptionDlg();

	enum { IDD = IDD_SETTINGS_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
