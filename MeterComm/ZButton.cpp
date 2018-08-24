// ZButton.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "ZButton.h"


// ZButton

IMPLEMENT_DYNAMIC(ZButton, CButton)

ZButton::ZButton()
: m_nIsFlat(FALSE)
, m_nMouseOnButton(FALSE)
, m_nDrawTransparent(FALSE)
, m_nIsPressed(FALSE)
, m_nIsFocused(FALSE)
, m_nIsDisabled(FALSE)
, m_nIsDefault(FALSE)
, m_nDrawBorder(FALSE)
, m_p_bmpBkOld(NULL)
, m_nIconIn(-1)
, m_nIconOut(-1)
, m_nBmpIn(-1)
, m_nBmpOut(-1)
{
	m_crColors[COLOR_BK_IN]=::GetSysColor(COLOR_WINDOW);
	m_crColors[COLOR_BK_OUT]=::GetSysColor(COLOR_WINDOW);
	m_crColors[COLOR_TEXT_IN]=::GetSysColor(COLOR_WINDOWTEXT);
	m_crColors[COLOR_TEXT_OUT]=::GetSysColor(COLOR_WINDOWTEXT);
}

ZButton::~ZButton()
{
	/*if(m_dcBk.m_hDC!= NULL)
	{
		m_dcBk.SelectObject(m_p_bmpBkOld);
	}*/
}


BEGIN_MESSAGE_MAP(ZButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// ZButton 消息处理程序




void ZButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT nBS;
	nBS=GetButtonStyle();
	nBS=nBS&BS_TYPEMASK;
	if (nBS==BS_DEFPUSHBUTTON)
		m_nIsDefault=TRUE;
	else
		m_nIsDefault=FALSE;
	if(GetParent())
		GetParent()->GetFont()->GetLogFont(&m_logfont);
	ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
	m_tooltip.Create(this,TTS_ALWAYSTIP );
	m_tooltip.AddTool(this,_T(""));
	m_tooltip.SetDelayTime(TTDT_AUTOPOP,5000); 
	CButton::PreSubclassWindow();
}

BOOL ZButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tooltip.RelayEvent(pMsg);
	return CButton::PreTranslateMessage(pMsg);
}

BOOL ZButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}


void ZButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nIsPressed=TRUE;
	Invalidate();
	CButton::OnLButtonDown(nFlags, point);
}


void ZButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nIsPressed=FALSE;
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}


void ZButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tm;  
	tm.cbSize = sizeof(TRACKMOUSEEVENT);  
	tm.dwFlags = TME_LEAVE;     
	tm.dwHoverTime = 500;    
	tm.hwndTrack = m_hWnd;  
	_TrackMouseEvent(&tm);
	m_nMouseOnButton=TRUE;
	Invalidate();
	CButton::OnMouseMove(nFlags, point);
}


void ZButton::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nMouseOnButton=FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}

void ZButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	CDC * pDC =CDC::FromHandle(lpDrawItemStruct->hDC);
	m_nIsFocused=(lpDrawItemStruct->itemState & ODS_FOCUS);
	m_nIsDisabled=(lpDrawItemStruct->itemState & ODS_DISABLED);
	CustDraw(pDC);
}

void ZButton::CustDraw(CDC * pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC dcMem;                                                          
	CBitmap bmp;                                                         
	dcMem.CreateCompatibleDC(pDC);                                   
	bmp.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height()); 
	CBitmap * p_bmpOld=dcMem.SelectObject(&bmp);
	DrawBackground(&dcMem,rcClient);
	DrawBorder(&dcMem,rcClient);
	DrawText(&dcMem,rcClient);
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}

void ZButton::DrawBackground(CDC * pDC,const CRect & rc)
{
	COLORREF crBk;
	HICON hIcon=NULL;
	CBitmap bmpBk;      
	BOOL nBmpRslt=FALSE;
	if(m_nMouseOnButton)
	{
		crBk=m_crColors[COLOR_BK_IN];
		hIcon=AfxGetApp()->LoadIcon(m_nIconIn); 
		nBmpRslt=bmpBk.LoadBitmap(m_nBmpIn);
	}
	else
	{
		crBk=m_crColors[COLOR_BK_OUT];
		hIcon=AfxGetApp()->LoadIcon(m_nIconOut); 
		nBmpRslt=bmpBk.LoadBitmap(m_nBmpOut);
	}
	CBrush brushBk(crBk);
	pDC->FillRect(&rc,&brushBk); 
	if(m_nDrawTransparent)
	{
		if(GetParent())
		{
			CRect rcInParent(rc);
			CClientDC dcParent(GetParent());
			ClientToScreen(&rcInParent);
			GetParent()->ScreenToClient(&rcInParent);
			if(m_dcBk.m_hDC == NULL)
			{
				m_dcBk.CreateCompatibleDC(&dcParent);
				m_bmpBk.CreateCompatibleBitmap(&dcParent, rc.Width(), rc.Height());
				m_p_bmpBkOld=m_dcBk.SelectObject(&m_bmpBk);
				m_dcBk.BitBlt(0,0,rc.Width(),rc.Height(),&dcParent,rcInParent.left,rcInParent.top,SRCCOPY);
			}
			pDC->BitBlt(0,0,rc.Width(),rc.Height(),&m_dcBk,0,0,SRCCOPY);
		}
	}
	if(hIcon)
	{
		CRect rcIcon(rc);
		if(m_nIsPressed)
			rcIcon.OffsetRect(1,1);
		pDC->DrawIcon(rcIcon.left,rcIcon.top,hIcon);
	}
	if(nBmpRslt)
	{
		CRect rcBmp(rc);
		if(m_nIsPressed)
			rcBmp.OffsetRect(1,1);
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap * p_bmpBkOld=dcMem.SelectObject(&bmpBk);
		BITMAP bmpInfo;
		bmpBk.GetBitmap(&bmpInfo);
		pDC->SetStretchBltMode(HALFTONE);
		pDC->StretchBlt(rcBmp.left,rcBmp.top,rcBmp.Width(),rcBmp.Height(),&dcMem,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
		dcMem.SelectObject(p_bmpBkOld);
	}
}

void ZButton::DrawBorder(CDC * pDC,const CRect & rc)
{
	if(m_nIsPressed)
	{
		if(m_nIsFlat)
		{
			if(m_nDrawBorder)
				pDC->Draw3dRect(rc,::GetSysColor(COLOR_BTNSHADOW),::GetSysColor(COLOR_BTNHILIGHT));
		}
		else    
		{
			CBrush brBtnShadow(::GetSysColor(COLOR_BTNSHADOW));
			pDC->FrameRect(rc, &brBtnShadow);
		}
	}
	else // ...else draw non pressed button
	{
		CPen penBtnHiLight(PS_SOLID,0,::GetSysColor(COLOR_BTNHILIGHT)); // White
		CPen pen3DLight(PS_SOLID,0,::GetSysColor(COLOR_3DLIGHT));       // Light gray
		CPen penBtnShadow(PS_SOLID,0,::GetSysColor(COLOR_BTNSHADOW));   // Dark gray
		CPen pen3DDKShadow(PS_SOLID,0,::GetSysColor(COLOR_3DDKSHADOW)); // Black

		if(m_nIsFlat)
		{
			if(m_nMouseOnButton && m_nDrawBorder)
				pDC->Draw3dRect(rc,::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		}
		else
		{
			// Draw top-left borders
			// White line
			CPen* pOldPen = pDC->SelectObject(&penBtnHiLight);
			pDC->MoveTo(rc.left,rc.bottom-1);
			pDC->LineTo(rc.left,rc.top);
			pDC->LineTo(rc.right,rc.top);
			// Light gray line
			pDC->SelectObject(pen3DLight);
			pDC->MoveTo(rc.left+1,rc.bottom-1);
			pDC->LineTo(rc.left+1,rc.top+1);
			pDC->LineTo(rc.right,rc.top+1);
			// Draw bottom-right borders
			// Black line
			pDC->SelectObject(pen3DDKShadow);
			pDC->MoveTo(rc.left,rc.bottom-1);
			pDC->LineTo(rc.right-1,rc.bottom-1);
			pDC->LineTo(rc.right-1,rc.top-1);
			// Dark gray line
			pDC->SelectObject(penBtnShadow);
			pDC->MoveTo(rc.left+1,rc.bottom-2);
			pDC->LineTo(rc.right-2,rc.bottom-2);
			pDC->LineTo(rc.right-2,rc.top);
			//
			pDC->SelectObject(pOldPen);
		} 
	}
	if((m_nIsFocused||m_nIsDefault)&&!m_nIsFlat)
	{
		CRect rcFocus(rc);
		rcFocus.DeflateRect(3, 3);
		pDC->DrawFocusRect(&rcFocus);
		if(m_nIsDefault)
			m_nIsDefault=FALSE;
	}
}

void ZButton::DrawText(CDC * pDC,const CRect & rc)
{
	CRect rcText(rc);
	CString strBtn;
	GetWindowText(strBtn);
	pDC->SetBkMode(TRANSPARENT);
	if(m_nIsDisabled)
		pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
	else
	{
		if(m_nMouseOnButton)
			pDC->SetTextColor(m_crColors[COLOR_TEXT_IN]);
		else
			pDC->SetTextColor(m_crColors[COLOR_TEXT_OUT]);
		if(m_nIsPressed)
			rcText.OffsetRect(1,1);
	}
	CFont font;
	font.CreateFontIndirect(&m_logfont);
	CFont * p_fontOld=pDC->SelectObject(&font);
	pDC->DrawText(strBtn,rcText,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(p_fontOld);
}

void ZButton::SetFont(const LOGFONT & logfont)
{
	m_logfont=logfont;
}


LOGFONT ZButton::GetFont(void)
{
	return m_logfont;
}

void ZButton::SetFlat(BOOL nFlat)
{
	m_nIsFlat=nFlat;
	Invalidate();
} 

void ZButton::DrawBorder(BOOL nDrawBorder)
{
	m_nDrawBorder=nDrawBorder;
	Invalidate();
} 

void ZButton::DrawTransparent(BOOL nDrawTransparent)
{
	m_nDrawTransparent=nDrawTransparent;
	if(m_nDrawTransparent)
	{
		if(m_dcBk.m_hDC!= NULL)
		{
			m_dcBk.SelectObject(m_p_bmpBkOld);
			m_bmpBk.DeleteObject();
			m_dcBk.DeleteDC();
		}
	}
	Invalidate();
} 

BOOL ZButton::SetColor(BYTE cColorIndex, COLORREF crColor)
{
	if (cColorIndex >= MAX_COLORS)	
		return FALSE;
	m_crColors[cColorIndex] = crColor;
	Invalidate();
	return TRUE;
}

void ZButton::SetTooltipText(const CString & strTip)
{
	m_tooltip.UpdateTipText(strTip,this);
}

void ZButton::SetIcon(const unsigned int & nIconIn, const unsigned int & nIconOut)
{
	m_nIconIn=nIconIn;
	m_nIconOut=nIconOut;
	Invalidate();
}

void ZButton::SetBitmap(const unsigned int & nBmpIn, const unsigned int & nBmpOut)
{
	m_nBmpIn=nBmpIn;
	m_nBmpOut=nBmpOut;
	Invalidate();
}