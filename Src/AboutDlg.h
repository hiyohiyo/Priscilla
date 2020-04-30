/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once
#include "DialogFx.h"
#include "StaticFx.h"
#include "ButtonFx.h"

class CAboutDlg : public CDialogFx
{
	DECLARE_DYNCREATE(CAboutDlg)

	static const int SIZE_X = 480;
	static const int SIZE_Y = 172;

public:
	CAboutDlg(CWnd* pParent = NULL);
	virtual ~CAboutDlg();

	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void UpdateDialogSize();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnVersion();
	afx_msg void OnLicense();

	CButtonFx m_CtrlCrystalDewWorld;
	CButtonFx m_CtrlVersion;
	CButtonFx m_CtrlLicense;
	CStaticFx m_CtrlEdition;
	CStaticFx m_CtrlRelease;
	CStaticFx m_CtrlCopyright;
};
