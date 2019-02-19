#pragma once
#include "ZEdit.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>

#define IDC_EDIT_MODIFY 2000
#define WM_MSGRECVPRO WM_USER+1
#define WM_EDITKILLFOCUS WM_USER+2
// ZListCtrl
struct ItemInfo
{
	int m_nItem;
	int m_nSubItem;
};

class ZListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(ZListCtrl)

public:
	ZListCtrl();
	virtual ~ZListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
private:
	ItemInfo m_ii;
	ZEdit m_editModify;
	bool m_bEnableEdit;
	std::vector<std::vector<COLORREF>> m_vec2_crText;
	std::vector<std::vector<COLORREF>> m_vec2_crBk;
	void DeleteColorByRow(int nItem);
	void DeleteColorByColumn(int nCol);
	void InsertColorByRow(int nItem);
	void InsertColorByColumn(int nCol);
	COLORREF m_crTextDefault;
	COLORREF m_crBkDefault;
protected:
	afx_msg LRESULT OnEditkillfocus(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void EnableEdit(const bool & bEnable);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int InsertItem(const LVITEM* pItem);
	int InsertItem(int nItem,LPCTSTR lpszItem);
	int InsertItem(int nItem, LPCTSTR lpszItem,int nImage);
	int InsertItem(UINT nMask,int nItem,LPCTSTR lpszItem,UINT nState,UINT nStateMask,int nImage,LPARAM lParam);
	int InsertColumn(int nCol,const LVCOLUMN* pColumn);
	int InsertColumn(int nCol,LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT,int nWidth = -1,int nSubItem = -1);
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems(void);
	BOOL DeleteColumn(int nCol);
	BOOL SetItemTextColor(int nItem = -1, int nSubItem = -1, COLORREF color = RGB(0,0,0), BOOL bRedraw = TRUE);
	BOOL SetItemBkColor(int nItem = -1, int nSubItem = -1, COLORREF color = RGB(255,255,255), BOOL bRedraw = TRUE);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL SetTextColor(COLORREF cr, BOOL bRedraw = TRUE);
	BOOL SetBkColor(COLORREF cr, BOOL bRedraw = TRUE);
	BOOL SetTextDefaultColor(COLORREF cr, BOOL bRedraw = TRUE);
	BOOL SetBkDefaultColor(COLORREF cr, BOOL bRedraw = TRUE);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
