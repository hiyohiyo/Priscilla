/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT              
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard component
#include <afxext.h>         // Extended MFC
#include <afxdtctl.h>		// MFC IE4 Common Control support
#include <afxcmn.h>			// MFC Windows Common Control support

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma warning(disable : 4996)

//------------------------------------------------
// Option Flags
//------------------------------------------------

// For Task Tray Icon Feature
// #define OPTION_TASK_TRAY

//------------------------------------------------
// Global Sttings
//------------------------------------------------

#define PRODUCT_NAME				L"Project Priscilla"
#define PRODUCT_VERSION				L""
#define PRODUCT_SHORT_NAME			L"Priscilla"
#ifdef _M_ARM
#define PRODUCT_EDITION				L"ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION				L"ARM64"
#elif _M_X64
#define PRODUCT_EDITION				L"x64"
#else
#define PRODUCT_EDITION				L""
#endif

#define PRODUCT_RELEASE				L"2020/04/30"
#define PRODUCT_COPY_YEAR			L"2020"
#define PRODUCT_COPYRIGHT			L"© 2020 hiyohiyo"
#define PRODUCT_LICENSE				L"The MIT License"

#define THEME_DIR					L"Resource\\Theme\\"
#define LANGUAGE_DIR				L"Resource\\Language\\"

#define MENU_THEME_INDEX			1
#define MENU_LANG_INDEX				3

#define DEFAULT_THEME				L"Default"
#define DEFAULT_LANGUAGE			L"English"

#define TIMER_UPDATE_DIALOG			500

#define DEFAULT_FONT_FACE_1			L"Segoe UI"
#define DEFAULT_FONT_FACE_2			L"Tahoma"


#define URL_CRYSTAL_DEW_WORLD_JA	L"https://crystalmark.info/ja/"
#define URL_CRYSTAL_DEW_WORLD_EN 	L"https://crystalmark.info/en/"

#define	URL_VERSION_JA				L"https://github.com/hiyohiyo/Priscilla"
#define	URL_VERSION_EN				L"https://github.com/hiyohiyo/Priscilla"
#define	URL_LICENSE_JA				L"https://github.com/hiyohiyo/Priscilla"
#define	URL_LICENSE_EN				L"https://github.com/hiyohiyo/Priscilla"

#define URL_HTML_HELP_JA			L"https://github.com/hiyohiyo/Priscilla"
#define URL_HTML_HELP_EN 			L"https://github.com/hiyohiyo/Priscilla"

