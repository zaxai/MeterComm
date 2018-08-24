// ZListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ZListCtrl.h"

// ZListCtrl
const COLORREF g_crBlue=RGB(19,190,236);
const int nEditKillFocus=1000;

IMPLEMENT_DYNAMIC(ZListCtrl, CListCtrl)

ZListCtrl::ZListCtrl()
: m_bEnableEdit(false)
{
	m_crTextDefault=GetSysColor(COLOR_WINDOWTEXT);
	m_crBkDefault=GetSysColor(COLOR_WINDOW);
}

ZListCtrl::~ZListCtrl()
{
}


BEGIN_MESSAGE_MAP(ZListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_EDITKILLFOCUS, &ZListCtrl::OnEditkillfocus)
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &ZListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// ZListCtrl 消息处理程序




void ZListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_editModify.m_hWnd == NULL)
	{
		m_editModify.Create(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_TABSTOP|WS_EX_TOOLWINDOW,CRect(0,0,0,0),this,IDC_EDIT_MODIFY);
		m_editModify.ShowWindow(SW_HIDE);
		m_editModify.SetFont(GetFont());
	}
	CListCtrl::PreSubclassWindow();
}


void ZListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bEnableEdit)
	{
		LVHITTESTINFO lvinfo;
		lvinfo.pt = point;
		lvinfo.flags = LVHT_ABOVE;
		int nItem=SubItemHitTest(&lvinfo);
		if(nItem!=-1)
		{
			m_ii.m_nItem=lvinfo.iItem;
			m_ii.m_nSubItem=lvinfo.iSubItem;
			CRect rc;
			if(lvinfo.iSubItem==0&&GetHeaderCtrl()->GetItemCount()>1)
			{
				GetSubItemRect(lvinfo.iItem,lvinfo.iSubItem+1,LVIR_BOUNDS,rc);
				rc.right=rc.left;
				rc.left=0;
			}
			else
				GetSubItemRect(lvinfo.iItem,lvinfo.iSubItem,LVIR_BOUNDS,rc);
			CString str=GetItemText(lvinfo.iItem,lvinfo.iSubItem); 
			if(m_editModify.m_hWnd)
			{
				m_editModify.MoveWindow(rc);
				m_editModify.SetWindowText(str);
				m_editModify.ShowWindow(SW_SHOW);
				m_editModify.SetFocus();
				m_editModify.SetSel(str.GetLength(),str.GetLength());
			}
		}
	}
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


afx_msg LRESULT ZListCtrl::OnEditkillfocus(WPARAM wParam, LPARAM lParam)
{
	CString str; 
	m_editModify.GetWindowText(str);
	m_editModify.ShowWindow(SW_HIDE);
	SetItemText(m_ii.m_nItem,m_ii.m_nSubItem,str);  
	if(GetParent())
		GetParent()->PostMessage(WM_MSGRECVPRO,(WPARAM)&m_ii,nEditKillFocus);
	return 0;
}

HBRUSH ZListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch(nCtlColor)
	{
	case CTLCOLOR_EDIT:
		{
			pDC->SetBkColor(g_crBlue);
			//pDC->SetTextColor(crWhite);
		}
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void ZListCtrl::EnableEdit(const bool & bEnable)
{
	m_bEnableEdit=bEnable;
}


BOOL ZListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;
	if(pHDN->hdr.code==HDN_ITEMCHANGEDW||pHDN->hdr.code == HDN_ITEMCHANGEDA)        
	{
		if(m_bEnableEdit&&m_editModify.IsWindowVisible())
		{
			CRect rc;
			if(m_ii.m_nSubItem==0&&GetHeaderCtrl()->GetItemCount()>1)
			{
				GetSubItemRect(m_ii.m_nItem,m_ii.m_nSubItem+1,LVIR_BOUNDS,rc);
				rc.right=rc.left;
				rc.left=0;
			}
			else
				GetSubItemRect(m_ii.m_nItem,m_ii.m_nSubItem,LVIR_BOUNDS,rc);
			if(m_editModify.m_hWnd)
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
	if(m_bEnableEdit&&m_editModify.IsWindowVisible())
	{
		CRect rc;
		if(m_ii.m_nSubItem==0&&GetHeaderCtrl()->GetItemCount()>1)
		{
			GetSubItemRect(m_ii.m_nItem,m_ii.m_nSubItem+1,LVIR_BOUNDS,rc);
			rc.right=rc.left;
			rc.left=0;
		}
		else
			GetSubItemRect(m_ii.m_nItem,m_ii.m_nSubItem,LVIR_BOUNDS,rc);
		if(m_editModify.m_hWnd)
		{
			m_editModify.MoveWindow(rc);
			m_editModify.Invalidate();
		}
	}
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

int ZListCtrl::InsertItem(const LVITEM* pItem)
{
	const int nRtn = CListCtrl::InsertItem(pItem);
	if (nRtn >= 0)
		InsertColorByRow(pItem->iItem);
	return nRtn;
}

int ZListCtrl::InsertItem(int nItem,LPCTSTR lpszItem)
{
	const int nRtn = CListCtrl::InsertItem(nItem, lpszItem);
	if (nRtn >= 0)
		InsertColorByRow(nItem);
	return nRtn;
}

int ZListCtrl::InsertItem(int nItem, LPCTSTR lpszItem,int nImage)
{
	const int nRtn = CListCtrl::InsertItem(nItem, lpszItem, nImage);
	if (nRtn >= 0)
		InsertColorByRow(nItem);
	return nRtn;
}

int ZListCtrl::InsertItem(UINT nMask,int nItem,LPCTSTR lpszItem,UINT nState,UINT nStateMask,int nImage,LPARAM lParam)
{
	const int nRtn = CListCtrl::InsertItem(nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam);
	if (nRtn >= 0)
		InsertColorByRow(nItem);
	return nRtn;
}

int ZListCtrl::InsertColumn(int nCol,const LVCOLUMN* pColumn)
{
	const int nRtn = CListCtrl::InsertColumn(nCol, pColumn);
	if (nRtn >= 0)
		InsertColorByColumn(nCol);
	return nRtn;
}

int ZListCtrl::InsertColumn(int nCol,LPCTSTR lpszColumnHeading, int nFormat,int nWidth,int nSubItem)
{
	const int nRtn = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
	if (nRtn >= 0)
		InsertColorByColumn(nCol);
	return nRtn;
}

BOOL ZListCtrl::DeleteItem(int nItem)
{
	const BOOL nRtn=CListCtrl::DeleteItem(nItem);
	if(nRtn)
		DeleteColorByRow(nItem);
	return nRtn;
}

BOOL ZListCtrl::DeleteAllItems(void)
{
	int nCount=GetItemCount();
	const BOOL nRtn=CListCtrl::DeleteAllItems();
	if(nRtn)
	{
		for(int i=0;i<nCount;++i)
			DeleteColorByRow(0);
	}
	return nRtn;
}

BOOL ZListCtrl::DeleteColumn(int nCol)
{
	const BOOL nRtn=CListCtrl::DeleteColumn(nCol);
	if(nRtn)
		DeleteColorByColumn(nCol);
	return nRtn;
}

void ZListCtrl::DeleteColorByRow(int nItem)
{
	std::vector<std::vector<COLORREF>>::iterator iter=m_vec2_crText.begin();
	for(int i=0;i<nItem;++i)
		++iter;
	m_vec2_crText.erase(iter);
	iter=m_vec2_crBk.begin();
	for(int i=0;i<nItem;++i)
		++iter;
	m_vec2_crBk.erase(iter);
}

void ZListCtrl::DeleteColorByColumn(int nCol)
{
	for(int i=0;i<GetItemCount();++i)
	{
		std::vector<COLORREF>::iterator iter=m_vec2_crText[i].begin();
		for(int j=0;j<nCol;++j)
			++iter;
		m_vec2_crText[i].erase(iter);
		iter=m_vec2_crBk[i].begin();
		for(int j=0;j<nCol;++j)
			++iter;
		m_vec2_crBk[i].erase(iter);
	}
}

void ZListCtrl::InsertColorByRow(int nItem)
{
	std::vector<COLORREF> vec_text,vec_bk;
	for(int i=0;i<GetHeaderCtrl()->GetItemCount();++i)
	{
		vec_text.push_back(m_crTextDefault);
		vec_bk.push_back(m_crBkDefault);
	}
	std::vector<std::vector<COLORREF>>::iterator iter=m_vec2_crText.begin();
	for(int i=0;i<nItem;++i)
		++iter;
	m_vec2_crText.insert(iter,vec_text);
	iter=m_vec2_crBk.begin();
	for(int i=0;i<nItem;++i)
		++iter;
	m_vec2_crBk.insert(iter,vec_bk);
}

void ZListCtrl::InsertColorByColumn(int nCol)
{
	for(int i=0;i<GetItemCount();++i)
	{
		std::vector<COLORREF>::iterator iter=m_vec2_crText[i].begin();
		for(int j=0;j<nCol;++j)
			++iter;
		m_vec2_crText[i].insert(iter,m_crTextDefault);
		iter=m_vec2_crBk[i].begin();
		for(int j=0;j<nCol;++j)
			++iter;
		m_vec2_crBk[i].insert(iter,m_crBkDefault);
	}
}

BOOL ZListCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF color, BOOL bRedraw)
{
	if(m_vec2_crText.empty())
		return FALSE;
	if(nItem<0||nItem>=int(m_vec2_crText.size()))
		return FALSE;
	if(nSubItem<0||nSubItem>=int(m_vec2_crText[0].size()))
		return FALSE;
	m_vec2_crText[nItem][nSubItem]=color;
	if(bRedraw)
		Invalidate();
	return TRUE;
}
BOOL ZListCtrl::SetItemBkColor(int nItem, int nSubItem, COLORREF color, BOOL bRedraw)
{
	if(m_vec2_crBk.empty())
		return FALSE;
	if(nItem<0||nItem>=int(m_vec2_crBk.size()))
		return FALSE;
	if(nSubItem<0||nSubItem>=int(m_vec2_crBk[0].size()))
		return FALSE;
	m_vec2_crBk[nItem][nSubItem]=color;
	if(bRedraw)
		Invalidate();
	return TRUE;
}

void ZListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	if(lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
	    *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if(lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
	    *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if(lplvcd->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
    {
		int nItem=static_cast<int>(lplvcd->nmcd.dwItemSpec);  
        int nSubItem=lplvcd->iSubItem;
		if(nItem<int(m_vec2_crText.size())&&nSubItem<int(m_vec2_crText[nItem].size()))
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

BOOL ZListCtrl::SetTextColor(COLORREF cr, BOOL bRedraw)
{
	int nRow,nColumn;
	nRow=m_vec2_crText.size();
	if(nRow)
	{
		nColumn=m_vec2_crText[0].size();
		for(int i=0;i<nRow;++i)
		{
			for(int j=0;j<nColumn;++j)
				m_vec2_crText[i][j]=cr;
		}
	}
	if(bRedraw)
		Invalidate();
	return TRUE;
}

BOOL ZListCtrl::SetBkColor(COLORREF cr, BOOL bRedraw)
{
	int nRow,nColumn;
	nRow=m_vec2_crBk.size();
	if(nRow)
	{
		nColumn=m_vec2_crBk[0].size();
		for(int i=0;i<nRow;++i)
		{
			for(int j=0;j<nColumn;++j)
				m_vec2_crBk[i][j]=cr;
		}
	}
	if(bRedraw)
		Invalidate();
	return TRUE;
}

BOOL ZListCtrl::SetTextDefaultColor(COLORREF cr, BOOL bRedraw)
{
	m_crTextDefault=cr;
	if(bRedraw)
		Invalidate();
	return TRUE;
}

BOOL ZListCtrl::SetBkDefaultColor(COLORREF cr, BOOL bRedraw)
{
	m_crBkDefault=cr;
	if(bRedraw)
		Invalidate();
	return TRUE;
}