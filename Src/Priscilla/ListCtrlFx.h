﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "CommonFx.h"
#include "HeaderCtrlFx.h"
#include <atlimage.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

class CListCtrlFx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlFx)

public:
	CListCtrlFx();
	virtual ~CListCtrlFx();

	void SetTextColor1(COLORREF color);
	void SetTextColor2(COLORREF color);
	void SetTextSelected(COLORREF color);
	void SetBkColor1(COLORREF color);
	void SetBkColor2(COLORREF color);
	void SetBkSelected(COLORREF color);
	void SetLineColor1(COLORREF color);
	void SetLineColor2(COLORREF color);
	void SetGlassColor(COLORREF glassColor, BYTE glassAlpha);

	COLORREF GetTextColor1();
	COLORREF GetTextColor2();
	COLORREF GetTextSelected();
	COLORREF GetBkColor1();
	COLORREF GetBkColor2();
	COLORREF GetBkSelected();
	COLORREF GetLineColor1();
	COLORREF GetLineColor2();

	BOOL InitControl(int x, int y, int width, int height, int maxWidth, int maxHeight, double zoomRatio, CDC* bkDC, int renderMode, BOOL bHighContrast, BOOL bDarkMode);
	void SetFontEx(CString face, int size, double zoomRatio, double fontRatio, LONG fontWeight, BYTE fontRender);
	void EnableHeaderOwnerDraw(BOOL bOwnerDraw);

protected:
	virtual void PreSubclassWindow();

	void SetupControlImage(CBitmap& bkBitmap, CBitmap& ctrlBitmap);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);

	int m_X;
	int m_Y;
	BOOL m_bNT6orLater;
	CSize m_CtrlSize;
	CRect m_Margin;
	int m_RenderMode;
	BOOL m_bHighContrast;
	BOOL m_bDarkMode;
	CHeaderCtrlFx m_Header;

	COLORREF m_TextColor1;
	COLORREF m_TextColor2;
	COLORREF m_TextSelected;
	COLORREF m_BkColor1;
	COLORREF m_BkColor2;
	COLORREF m_BkSelected;
	COLORREF m_LineColor1;
	COLORREF m_LineColor2;

	CFont    m_Font;
	CImageList m_Image;
	CDC* m_BkDC;

	// Glass
	COLORREF m_GlassColor;
	BYTE m_GlassAlpha;

	// Image
	CBitmap m_BkBitmap;
	CBitmap m_CtrlBitmap;
	CImage m_CtrlImage;
};


