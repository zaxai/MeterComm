/*
** Version  1.0.0.1
** Date     2019.02.23
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once


// ZButton

class ZButton : public CButton
{
	DECLARE_DYNAMIC(ZButton)

public:
	ZButton();
	virtual ~ZButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	enum COLOR
	{
		COLOR_BK_IN = 0,
		COLOR_BK_OUT,
		COLOR_TEXT_IN,
		COLOR_TEXT_OUT,
		MAX_COLORS
	};
protected:
	BOOL m_nIsFlat;
	BOOL m_nMouseOnButton;
	BOOL m_nDrawTransparent;
	BOOL m_nIsPressed;
	BOOL m_nIsFocused;
	BOOL m_nIsDisabled;
	BOOL m_nIsDefault;
	BOOL m_nDrawBorder;
	LOGFONT m_logfont;
	CDC m_dcBk;
	CBitmap m_bmpBk;
	CBitmap *m_p_bmpBkOld;
	COLORREF m_crColors[MAX_COLORS];
	CToolTipCtrl m_tooltip;
	unsigned int m_nIconIn;
	unsigned int m_nIconOut;
	unsigned int m_nBmpIn;
	unsigned int m_nBmpOut;
public:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void CustDraw(CDC * pDC);
	void DrawBackground(CDC * pDC, const CRect & rc);
	void DrawBorder(CDC * pDC, const CRect & rc);
	void DrawText(CDC * pDC, const CRect & rc);
	void SetFont(const LOGFONT & logfont);
	LOGFONT GetFont(void);
	void SetFlat(BOOL nFlat);
	void DrawBorder(BOOL nDrawBorder);
	void DrawTransparent(BOOL nDrawTransparent);
	BOOL SetColor(BYTE cColorIndex, COLORREF crColor);
	void SetTooltipText(const CString & strTip);
	void SetIcon(const unsigned int & nIconIn = -1, const unsigned int & nIconOut = -1);
	void SetBitmap(const unsigned int & nBmpIn = -1, const unsigned int & nBmpOut = -1);
};


