/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "resource.h"

class CPriscillaApp : public CWinApp
{
public:
	CPriscillaApp();

public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

extern CPriscillaApp theApp;