/*
** Version  1.0.0.2
** Date     2019.02.23
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "ZListEdit.h"
#include <vector>
#define IDC_EDIT_MODIFY 2000
// ZListCtrl

class ZListCtrl : public CListCtrl,public ZListEdit::KillFocusCallBack
{
	DECLARE_DYNAMIC(ZListCtrl)
public:
	class ItemInfo
	{
	public:
		int m_nItem;
		int m_nSubItem;
	};
	class ModifyCallBack
	{
	public:
		virtual void OnModifyItem(const ItemInfo & ii) = 0;
	};
public:
	ZListCtrl();
	ZListCtrl(ModifyCallBack * p_callback);
	virtual ~ZListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
private:
	ZListEdit m_editModify;
	BOOL m_nEnableEdit;
	ItemInfo m_ii;
	ModifyCallBack * m_p_callback;
	std::vector<std::vector<COLORREF>> m_vec2_crText;
	std::vector<std::vector<COLORREF>> m_vec2_crBk;
	std::vector<std::vector<BOOL>> m_vec2_nEnableFlag;
	COLORREF m_crTextDefault;
	COLORREF m_crBkDefault;
	BOOL m_nEnableFlagDefault;
	COLORREF m_crEditText;
	COLORREF m_crEditBk;
	void DeleteElementByRow(int nRow);
	void DeleteElementByColumn(int nCol);
	void InsertElementByRow(int nRow);
	void InsertElementByColumn(int nCol);
protected:
	virtual void OnEditKillFocus(void);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void EnableEdit(BOOL  nEnable);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	int InsertItem(const LVITEM* pItem);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);
	int InsertColumn(int nCol, const LVCOLUMN* pColumn);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems(void);
	BOOL DeleteColumn(int nCol);
	BOOL SetItemTextColor(int nItem = -1, int nSubItem = -1, COLORREF cr = RGB(0, 0, 0), BOOL bRedraw = TRUE);//Set One Cell
	BOOL SetItemBkColor(int nItem = -1, int nSubItem = -1, COLORREF cr = RGB(255, 255, 255), BOOL bRedraw = TRUE);//Set One Cell
	BOOL SetItemEnableFlag(int nItem = -1, int nSubItem = -1, BOOL nEnable = TRUE);//Set One Cell
	BOOL SetRowTextColor(int nItem = -1, COLORREF cr = RGB(0, 0, 0), BOOL bRedraw = TRUE);//Set One Row
	BOOL SetRowBkColor(int nItem = -1,  COLORREF cr = RGB(255, 255, 255), BOOL bRedraw = TRUE);//Set One Row
	BOOL SetRowEnableFlag(int nItem = -1, BOOL nEnable = TRUE);//Set One Row
	BOOL SetColumnTextColor(int nSubItem = -1, COLORREF cr = RGB(0, 0, 0), BOOL bRedraw = TRUE);//Set One Column
	BOOL SetColumnBkColor(int nSubItem = -1, COLORREF cr = RGB(255, 255, 255), BOOL bRedraw = TRUE);//Set One Column
	BOOL SetColumnEnableFlag(int nSubItem = -1, BOOL nEnable = TRUE);//Set One Column
	BOOL SetTextColor(COLORREF cr = RGB(0, 0, 0), BOOL bRedraw = TRUE);//Set All Cells
	BOOL SetBkColor(COLORREF cr = RGB(255, 255, 255), BOOL bRedraw = TRUE);//Set All Cells
	BOOL SetEnableFlag(BOOL nEnable = TRUE);//Set All Cells
	BOOL SetTextDefaultColor(COLORREF cr, BOOL bRedraw = TRUE);
	BOOL SetBkDefaultColor(COLORREF cr, BOOL bRedraw = TRUE);
	BOOL SetDefaultEnableFlag(BOOL nEnable);
	BOOL SetEditTextColor(COLORREF cr, BOOL bRedraw = TRUE);
	BOOL SetEditBkColor(COLORREF cr, BOOL bRedraw = TRUE);
};


