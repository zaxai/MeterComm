// ZListCtrl.cpp: 实现文件
//

#include "stdafx.h"
#include "ZListCtrl.h"


// ZListCtrl

IMPLEMENT_DYNAMIC(ZListCtrl, CListCtrl)

ZListCtrl::ZListCtrl()
	: m_editModify(this)
	, m_nEnableEdit(FALSE)
	, m_p_callback(NULL)
	, m_nEnableFlagDefault(FALSE)
{
	m_crTextDefault = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBkDefault = ::GetSysColor(COLOR_WINDOW);
	m_crEditText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crEditBk = ::GetSysColor(COLOR_WINDOW);
}

ZListCtrl::ZListCtrl(ModifyCallBack * p_callback)
	: m_editModify(this)
	, m_nEnableEdit(FALSE)
	, m_p_callback(p_callback)
	, m_nEnableFlagDefault(FALSE)
{
	m_crTextDefault = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBkDefault = ::GetSysColor(COLOR_WINDOW);
	m_crEditText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crEditBk = ::GetSysColor(COLOR_WINDOW);
}

ZListCtrl::~ZListCtrl()
{
}


BEGIN_MESSAGE_MAP(ZListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &ZListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// ZListCtrl 消息处理程序
void ZListCtrl::OnEditKillFocus(void)
{
	CString str;
	m_editModify.GetWindowText(str);
	m_editModify.ShowWindow(SW_HIDE);
	SetItemText(m_ii.m_nItem, m_ii.m_nSubItem, str);
	if (m_p_callback)
		m_p_callback->OnModifyItem(m_ii);
}


void ZListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nEnableEdit)
	{
		LVHITTESTINFO lvinfo;
		lvinfo.pt = point;
		lvinfo.flags = LVHT_ABOVE;
		int nItem = SubItemHitTest(&lvinfo);
		if (nItem != -1)
		{
			m_ii.m_nItem = lvinfo.iItem;
			m_ii.m_nSubItem = lvinfo.iSubItem;
			if (m_vec2_nEnableFlag[m_ii.m_nItem][m_ii.m_nSubItem])
			{
				CRect rc;
				if (lvinfo.iSubItem == 0 && GetHeaderCtrl()->GetItemCount() > 1)
				{
					GetSubItemRect(lvinfo.iItem, lvinfo.iSubItem + 1, LVIR_BOUNDS, rc);
					rc.right = rc.left;
					rc.left = 0;
				}
				else
					GetSubItemRect(lvinfo.iItem, lvinfo.iSubItem, LVIR_BOUNDS, rc);
				CString str = GetItemText(lvinfo.iItem, lvinfo.iSubItem);
				if (m_editModify.m_hWnd)
				{
					m_editModify.MoveWindow(rc);
					m_editModify.SetWindowText(str);
					m_editModify.ShowWindow(SW_SHOW);
					m_editModify.SetFocus();
					m_editModify.SetSel(str.GetLength(), str.GetLength());
				}
			}
		}
	}
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


HBRUSH ZListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
	{
		pDC->SetTextColor(m_crEditText);
		pDC->SetBkColor(m_crEditBk);
	}
	break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void ZListCtrl::EnableEdit(BOOL  nEnable)
{
	m_nEnableEdit = nEnable;
	if (m_editModify.m_hWnd == NULL)
	{
		m_editModify.Create(WS_CHILD | WS_VISIBLE | ES_CENTER | WS_TABSTOP | WS_EX_TOOLWINDOW, CRect(0, 0, 0, 0), this, IDC_EDIT_MODIFY);
		m_editModify.ShowWindow(SW_HIDE);
		m_editModify.SetFont(GetFont());
	}
}


BOOL ZListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;
	if (pHDN->hdr.code == HDN_ITEMCHANGEDW || pHDN->hdr.code == HDN_ITEMCHANGEDA)
	{
		if (m_nEnableEdit&&m_editModify.IsWindowVisible())
		{
			CRect rc;
			if (m_ii.m_nSubItem == 0 && GetHeaderCtrl()->GetItemCount() > 1)
			{
				GetSubItemRect(m_ii.m_nItem, m_ii.m_nSubItem + 1, LVIR_BOUNDS, rc);
				rc.right = rc.left;
				rc.left = 0;
			}
			else
				GetSubItemRect(m_ii.m_nItem, m_ii.m_nSubItem, LVIR_BOUNDS, rc);
			if (m_editModify.m_hWnd)
			{
				m_editModify.MoveWindow(rc);
				m_editModify.Invalidate();
			}
		}
	}
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}


void ZListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nEnableEdit&&m_editModify.IsWindowVisible())
	{
		CRect rc;
		if (m_ii.m_nSubItem == 0 && GetHeaderCtrl()->GetItemCount() > 1)
		{
			GetSubItemRect(m_ii.m_nItem, m_ii.m_nSubItem + 1, LVIR_BOUNDS, rc);
			rc.right = rc.left;
			rc.left = 0;
		}
		else
			GetSubItemRect(m_ii.m_nItem, m_ii.m_nSubItem, LVIR_BOUNDS, rc);
		if (m_editModify.m_hWnd)
		{
			m_editModify.MoveWindow(rc);
			m_editModify.Invalidate();
		}
	}
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void ZListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (lplvcd->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		int nItem = static_cast<int>(lplvcd->nmcd.dwItemSpec);
		int nSubItem = lplvcd->iSubItem;
		if (nItem<int(m_vec2_crText.size()) && nSubItem<int(m_vec2_crText[nItem].size()))
		{
			lplvcd->clrText = m_vec2_crText[nItem][nSubItem];
			lplvcd->clrTextBk = m_vec2_crBk[nItem][nSubItem];
		}
		else
		{
			lplvcd->clrText = m_crTextDefault;
			lplvcd->clrTextBk = m_crBkDefault;
		}
		*pResult = CDRF_DODEFAULT;
	}
}


int ZListCtrl::InsertItem(const LVITEM* pItem)
{
	const int nRtn = CListCtrl::InsertItem(pItem);
	if (nRtn >= 0)
		InsertElementByRow(pItem->iItem);
	return nRtn;
}


int ZListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	const int nRtn = CListCtrl::InsertItem(nItem, lpszItem);
	if (nRtn >= 0)
		InsertElementByRow(nItem);
	return nRtn;
}


int ZListCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{
	const int nRtn = CListCtrl::InsertItem(nItem, lpszItem, nImage);
	if (nRtn >= 0)
		InsertElementByRow(nItem);
	return nRtn;
}


int ZListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
	const int nRtn = CListCtrl::InsertItem(nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam);
	if (nRtn >= 0)
		InsertElementByRow(nItem);
	return nRtn;
}


int ZListCtrl::InsertColumn(int nCol, const LVCOLUMN* pColumn)
{
	const int nRtn = CListCtrl::InsertColumn(nCol, pColumn);
	if (nRtn >= 0)
		InsertElementByColumn(nCol);
	return nRtn;
}


int ZListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	const int nRtn = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
	if (nRtn >= 0)
		InsertElementByColumn(nCol);
	return nRtn;
}


BOOL ZListCtrl::DeleteItem(int nItem)
{
	const BOOL nRtn = CListCtrl::DeleteItem(nItem);
	if (nRtn)
		DeleteElementByRow(nItem);
	return nRtn;
}


BOOL ZListCtrl::DeleteAllItems(void)
{
	int nCount = GetItemCount();
	const BOOL nRtn = CListCtrl::DeleteAllItems();
	if (nRtn)
	{
		for (int i = 0; i < nCount; ++i)
			DeleteElementByRow(0);
	}
	return nRtn;
}


BOOL ZListCtrl::DeleteColumn(int nCol)
{
	const BOOL nRtn = CListCtrl::DeleteColumn(nCol);
	if (nRtn)
		DeleteElementByColumn(nCol);
	return nRtn;
}


void ZListCtrl::DeleteElementByRow(int nRow)
{
	m_vec2_crText.erase(m_vec2_crText.begin()+ nRow);
	m_vec2_crBk.erase(m_vec2_crBk.begin()+ nRow);
	m_vec2_nEnableFlag.erase(m_vec2_nEnableFlag.begin() + nRow);
}


void ZListCtrl::DeleteElementByColumn(int nCol)
{
	for (int i = 0; i < GetItemCount(); ++i)
	{
		m_vec2_crText[i].erase(m_vec2_crText[i].begin()+ nCol);
		m_vec2_crBk[i].erase(m_vec2_crBk[i].begin()+nCol);
		m_vec2_nEnableFlag[i].erase(m_vec2_nEnableFlag[i].begin() + nCol);
	}
}


void ZListCtrl::InsertElementByRow(int nRow)
{
	std::vector<COLORREF> vec_text, vec_bk;
	std::vector<BOOL> vec_enableflag;
	vec_text.resize(GetHeaderCtrl()->GetItemCount(), m_crTextDefault);
	vec_bk.resize(GetHeaderCtrl()->GetItemCount(), m_crBkDefault);
	vec_enableflag.resize(GetHeaderCtrl()->GetItemCount(), m_nEnableFlagDefault);
	m_vec2_crText.insert(m_vec2_crText.begin()+ nRow, vec_text);
	m_vec2_crBk.insert(m_vec2_crBk.begin()+ nRow, vec_bk);
	m_vec2_nEnableFlag.insert(m_vec2_nEnableFlag.begin() + nRow, vec_enableflag);
}


void ZListCtrl::InsertElementByColumn(int nCol)
{
	for (int i = 0; i < GetItemCount(); ++i)
	{
		m_vec2_crText[i].insert(m_vec2_crText[i].begin()+nCol, m_crTextDefault);
		m_vec2_crBk[i].insert(m_vec2_crBk[i].begin()+nCol, m_crBkDefault);
		m_vec2_nEnableFlag[i].insert(m_vec2_nEnableFlag[i].begin() + nCol, m_nEnableFlagDefault);
	}
}


BOOL ZListCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF cr, BOOL bRedraw)
{
	if (m_vec2_crText.empty())
		return FALSE;
	if (nItem < 0 || nItem >= int(m_vec2_crText.size()))
		return FALSE;
	if (nSubItem < 0 || nSubItem >= int(m_vec2_crText[0].size()))
		return FALSE;
	m_vec2_crText[nItem][nSubItem] = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetItemBkColor(int nItem, int nSubItem, COLORREF cr, BOOL bRedraw)
{
	if (m_vec2_crBk.empty())
		return FALSE;
	if (nItem < 0 || nItem >= int(m_vec2_crBk.size()))
		return FALSE;
	if (nSubItem < 0 || nSubItem >= int(m_vec2_crBk[0].size()))
		return FALSE;
	m_vec2_crBk[nItem][nSubItem] = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetItemEnableFlag(int nItem, int nSubItem, BOOL nEnable)
{
	if (m_vec2_nEnableFlag.empty())
		return FALSE;
	if (nItem < 0 || nItem >= int(m_vec2_nEnableFlag.size()))
		return FALSE;
	if (nSubItem < 0 || nSubItem >= int(m_vec2_nEnableFlag[0].size()))
		return FALSE;
	m_vec2_nEnableFlag[nItem][nSubItem] = nEnable;
	return TRUE;
}


BOOL ZListCtrl::SetRowTextColor(int nItem, COLORREF cr, BOOL bRedraw)
{
	if (m_vec2_crText.empty())
		return FALSE;
	if (nItem < 0 || nItem >= int(m_vec2_crText.size()))
		return FALSE;
	for (COLORREF & crText : m_vec2_crText[nItem])
		crText = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetRowBkColor(int nItem, COLORREF cr, BOOL bRedraw)
{
	if (m_vec2_crBk.empty())
		return FALSE;
	if (nItem < 0 || nItem >= int(m_vec2_crBk.size()))
		return FALSE;
	for (COLORREF & crBk : m_vec2_crBk[nItem])
		crBk = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetRowEnableFlag(int nItem, BOOL nEnable)
{
	if (m_vec2_nEnableFlag.empty())
		return FALSE;
	if (nItem < 0 || nItem >= int(m_vec2_nEnableFlag.size()))
		return FALSE;
	for (BOOL & nEnableFlag : m_vec2_nEnableFlag[nItem])
		nEnableFlag = nEnable;
	return TRUE;
}


BOOL ZListCtrl::SetColumnTextColor(int nSubItem, COLORREF cr, BOOL bRedraw)
{
	if (m_vec2_crText.empty())
		return FALSE;
	for (std::vector<COLORREF> & vec_crText : m_vec2_crText)
	{
		if (nSubItem < 0 || nSubItem >= int(vec_crText.size()))
			return FALSE;
		vec_crText[nSubItem] = cr;
	}
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetColumnBkColor(int nSubItem, COLORREF cr, BOOL bRedraw)
{
	if (m_vec2_crBk.empty())
		return FALSE;
	for (std::vector<COLORREF> & vec_crBk : m_vec2_crBk)
	{
		if (nSubItem < 0 || nSubItem >= int(vec_crBk.size()))
			return FALSE;
		vec_crBk[nSubItem] = cr;
	}
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetColumnEnableFlag(int nSubItem, BOOL nEnable)
{
	if (m_vec2_nEnableFlag.empty())
		return FALSE;
	for (std::vector<BOOL> & vec_nEnableFlag : m_vec2_nEnableFlag)
	{
		if (nSubItem < 0 || nSubItem >= int(vec_nEnableFlag.size()))
			return FALSE;
		vec_nEnableFlag[nSubItem] = nEnable;
	}
	return TRUE;
}


BOOL ZListCtrl::SetTextColor(COLORREF cr, BOOL bRedraw)
{
	for (std::vector<COLORREF> & vec_crText : m_vec2_crText)
	{
		for (COLORREF & crText : vec_crText)
			crText = cr;
	}
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetBkColor(COLORREF cr, BOOL bRedraw)
{
	for (std::vector<COLORREF> & vec_crBk : m_vec2_crBk)
	{
		for (COLORREF & crBk : vec_crBk)
			crBk = cr;
	}
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetEnableFlag(BOOL nEnable)
{
	for (std::vector<BOOL> & vec_nEnableFlag : m_vec2_nEnableFlag)
	{
		for (BOOL & nEnableFlag : vec_nEnableFlag)
			nEnableFlag = nEnable;
	}
	return TRUE;
}


BOOL ZListCtrl::SetTextDefaultColor(COLORREF cr, BOOL bRedraw)
{
	m_crTextDefault = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetBkDefaultColor(COLORREF cr, BOOL bRedraw)
{
	m_crBkDefault = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetDefaultEnableFlag(BOOL nEnable)
{
	m_nEnableFlagDefault = nEnable;
	return TRUE;
}


BOOL ZListCtrl::SetEditTextColor(COLORREF cr, BOOL bRedraw)
{
	m_crEditText = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}


BOOL ZListCtrl::SetEditBkColor(COLORREF cr, BOOL bRedraw)
{
	m_crEditBk = cr;
	if (bRedraw)
		Invalidate();
	return TRUE;
}