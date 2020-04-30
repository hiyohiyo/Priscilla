﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "CommonFx.h"
#include <atlimage.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

class CEditFx : public CEdit
{
	DECLARE_DYNAMIC(CEditFx);

public:
	// Constructors
	CEditFx();
	virtual ~CEditFx();

	// Control
	BOOL InitControl(int x, int y, int width, int height, double zoomRatio, CDC* bgDC, LPCWSTR imagePath,
		 int imageCount = 0, DWORD textAlign = ES_LEFT, int renderMode = OwnerDrawImage);
	void SetMargin(int top, int left, int bottom, int right, double zoomRatio);
	CSize GetSize(void);
	void SetDrawFrame(BOOL bDrawFrame);
	void SetGlassColor(COLORREF glassColor, BYTE glassAlpha);
	void Adjust();

	// Font
	void SetFontEx(CString face, int size, double zoomRatio, double fontRatio = 1.0,
		 COLORREF textColor = RGB(0, 0, 0), LONG fontWeight = FW_NORMAL);

protected:
	// Image
	BOOL LoadBitmap(LPCTSTR fileName);
	BOOL LoadBitmap(HBITMAP hBitmap);
	void SetBgReload(void);
	BOOL SetBitmap(CBitmap& bitmap);
	void LoadCtrlBg(CDC* drawDC);
	void SetupControlImage(CBitmap& bgBitmap, CBitmap& ctrlBitmap);

	// MessageMap
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	// Control
	int m_X;
	int m_Y;
	CSize m_CtrlSize;
	CRect m_Margin;
	int m_RenderMode;
	BOOL m_bHighContrast;

	// Glass
	COLORREF m_GlassColor;
	BYTE m_GlassAlpha;

	// Image
	CString m_ImagePath;
	int m_ImageCount;
	CDC* m_BgDC;
	CBitmap m_BgBitmap;
	CBrush m_BgBrush;
	BOOL m_bBgBitmapInit;
	BOOL m_bBgLoad;
	CBitmap m_CtrlBitmap;
	CImage m_CtrlImage;

	// Font
	DWORD m_TextAlign;
	CFont m_Font;
	COLORREF m_TextColor;
};
