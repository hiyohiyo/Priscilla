/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "Priscilla.h"
#include "PriscillaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPriscillaApp, CWinApp)
END_MESSAGE_MAP()

CPriscillaApp theApp;

CPriscillaApp::CPriscillaApp()
{
}

BOOL CPriscillaApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CPriscillaDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}