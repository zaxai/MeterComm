// MeterPos.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "MeterPos.h"
#include "afxdialogex.h"


// CMeterPos 对话框
const int nMDIndexOffset=2;//md元素序号与所在test列表里的列序号的偏移

IMPLEMENT_DYNAMIC(CMeterPos, CDialogEx)

CMeterPos::CMeterPos(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterPos::IDD, pParent)
	, m_p_protest(NULL)
{

}

CMeterPos::~CMeterPos()
{
	int nSize=m_vec_btnCheck.size();
	for(int i=0;i<nSize;++i)
	{
		if(m_vec_btnCheck[i]!=NULL)
		{
			delete m_vec_btnCheck[i];
			m_vec_btnCheck[i]=NULL;
		}
		if(m_vec_edit[i]!=NULL)
		{
			delete m_vec_edit[i];
			m_vec_edit[i]=NULL;
		}
	}
	m_vec_btnCheck.clear();
	m_vec_edit.clear();
}

void CMeterPos::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_GETMETERPOS, m_btnGetMeterPos);
	DDX_Control(pDX, IDC_BUTTON_SETBARCODEINFO, m_btnSetBarCodeInfo);
	DDX_Control(pDX, IDC_BUTTON_SELECTMETER, m_btnSelectMeter);
	DDX_Control(pDX, IDC_BUTTON_UNSELECTMETER, m_btnUnselectMeter);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CMeterPos, CDialogEx)
	ON_MESSAGE(WM_MSGRECVPRO, &CMeterPos::OnMsgrecvpro)
	ON_BN_CLICKED(IDC_CHECK_SELECTALLMETER, &CMeterPos::OnBnClickedCheckSelectallmeter)
	ON_BN_CLICKED(IDC_BUTTON_SELECTMETER, &CMeterPos::OnBnClickedButtonSelectmeter)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECTMETER, &CMeterPos::OnBnClickedButtonUnselectmeter)
	ON_BN_CLICKED(IDC_BUTTON_GETMETERPOS, &CMeterPos::OnBnClickedButtonGetmeterpos)
	ON_BN_CLICKED(IDC_BUTTON_SETBARCODEINFO, &CMeterPos::OnBnClickedButtonSetbarcodeinfo)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_START, IDC_CHECK_START+UINT(g_nMaxTotalPort), &CMeterPos::OnBnClickedCheckMeterpos)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMeterPos 消息处理程序


BOOL CMeterPos::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_METERPOS]=this;
	InsertMeterPos();
	ExecVecSelect();
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMeterPos::SetProTest(CProduceTesting * p_protest)
{
	m_p_protest=p_protest;
}

afx_msg LRESULT CMeterPos::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_GETSENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		{
			p_md->m_p_dlt645->Set15Param(g_v.Get645Param());
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			p_md->m_p_dlt698->Set16Param(g_v.Get698Param());
		}
		break;
		}
	}
	break;
	case MSGUSER_ENCODE:
	{
		CMainData * p_md = (CMainData *)wParam;
		CString strText;
		switch (p_md->m_nRtn)
		{
		case 0:
			break;
		default:
			strText = p_md->m_strError;
		}
		if (p_md->m_nRtn)
		{
			m_vec_edit[p_md->m_nColumn - nMDIndexOffset]->SetWindowText(strText);
		}
	}
	break;
	case MSGUSER_UPDATESENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		CString strText;
		switch (p_md->m_nRtn)
		{
		case ZDevicePort::ERROR_OK:
			break;
		case ZDevicePort::ERROR_INVALIDPORT:
			strText = _T("串口未打开!");
			break;
		case ZDevicePort::ERROR_SEND_TIMEOUT:
			strText = _T("串口发送超时!");
			break;
		case ZDevicePort::ERROR_SEND_STOP:
			strText = _T("发送已停止!");
			break;
		default:
			strText = _T("串口发送失败!");
		}
		if (p_md->m_nRtn)
		{
			m_vec_edit[p_md->m_nColumn - nMDIndexOffset]->SetWindowText(strText);
		}
	}
	break;
	case MSGUSER_UPDATERECVDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		{
			CString strText;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				CString strExt = p_md->m_strDataValue.Right(3);
				if ((strExt == _T("91H") || strExt == _T("94H")) && p_md->m_p_dlt645->GetDataItem() == _T("04000401"))
				{
					strText = p_md->m_p_dlt645->GetMeterAddr();
				}
				if (strExt == _T("93H") || strExt == _T("95H"))
				{
					strText = p_md->m_p_dlt645->GetMeterAddr();
				}
			}
			break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText = _T("串口未打开!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
				strText = _T("串口接收超时!");
				break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText = _T("接收已停止!");
				break;
			default:
				strText = _T("串口接收失败!");
			}
			m_vec_edit[p_md->m_nColumn - nMDIndexOffset]->SetWindowText(strText);
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			CString strText;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				if (p_md->m_p_dlt698->IsSplitRecv())
					p_md->m_nFlagUpdateRecvData = FLAG_GOTOENCODE;
				CString strReqResCode = p_md->m_strDataValue.Right(3);
				strReqResCode = strReqResCode.Left(2);
				if ((strReqResCode == _T("85") || strReqResCode == _T("86") || strReqResCode == _T("90")) && p_md->m_strDataValue.Find(_T("obj=\"40010200\"")) != -1)
					strText=p_md->m_p_dlt698->GetServerAddr();
			}
			break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText = _T("串口未打开!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
				strText = _T("串口接收超时!");
				break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText = _T("接收已停止!");
				break;
			default:
				strText = _T("串口接收失败!");
			}
			m_vec_edit[p_md->m_nColumn - nMDIndexOffset]->SetWindowText(strText);
		}
		break;
		}
	}
	break;
	case MSGUSER_ENDONCELOOP:
	{
		CMainData * p_md = (CMainData *)wParam;
		static int nPortAdd = 0;
		if (g_bIsStop)
			nPortAdd = 0;
		else
		{
			int nRowNext = p_md->m_nRow + 1;
			if (nRowNext >= p_md->m_nRowMax)
			{
				++nPortAdd;
				if (nPortAdd == g_nTotalSelectedPort)
				{
					nPortAdd = 0;
					for (int i = 0; i < sizeof(g_sz_p_wnd) / sizeof(CWnd *); ++i)
					{
						if (g_sz_p_wnd[i])
							g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO, TRUE, MSGUSER_ENABLEBTN);
					}
				}
			}
		}
	}
	break;
	case MSGUSER_PORTOPEN:
	case MSGUSER_PORTCLOSE:
	{
		InsertMeterPos();
		ExecVecSelect();
	}
	break;
	case MSGUSER_ENABLEBTN:
	{
		EnableBtn(wParam);
	}
	break;
	case MSGUSER_UPDATEMETERADD:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		{
			m_vec_edit[p_md->m_nColumn - nMDIndexOffset]->SetWindowText(p_md->m_p_dlt645->GetMeterAddr());
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			m_vec_edit[p_md->m_nColumn - nMDIndexOffset]->SetWindowText(p_md->m_p_dlt698->GetServerAddr());
		}
		break;
		}
	}
	break;
	}
	return 0;
}


void CMeterPos::InsertMeterPos(void)
{
	int nSize=m_vec_btnCheck.size();
	for(int i=0;i<nSize;++i)
	{
		if(m_vec_btnCheck[i]!=NULL)
		{
			delete m_vec_btnCheck[i];
			m_vec_btnCheck[i]=NULL;
		}
		if(m_vec_edit[i]!=NULL)
		{
			delete m_vec_edit[i];
			m_vec_edit[i]=NULL;
		}
	}
	m_vec_btnCheck.clear();
	m_vec_edit.clear();
	g_vec_bIsSelect.clear();
	CButton * p_btn=NULL;
	CEdit * p_edit=NULL;
	CString strText;
	CRect rc;
	GetDlgItem(IDC_CHECK_SELECTALLMETER)->GetClientRect(&rc);
	rc.bottom+=20;
	const int nStartLeft=rc.left+20;
	const int nStartTop=rc.bottom;
	const int nBtnWidth=60,nBtnHeight=20,nEditWidth=90,nEditHeight=20;
	for(int i=0;i<g_nTotalPort;++i)
	{
		g_vec_bIsSelect.push_back(false);
		if(i%4==0)
		{
			rc.left=nStartLeft;
			rc.top=rc.bottom+10;
			rc.bottom=rc.top+nBtnHeight;
			rc.right=rc.left+nBtnWidth;
		}
		else
		{
			rc.left=rc.right+10;
			rc.right=rc.left+nBtnWidth;
		}
		strText.Format(_T("表位%d"),i+1);
		p_btn=new CButton();
		p_btn->Create(strText,WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,rc,this,IDC_CHECK_START+i);
		p_btn->SetFont(this->GetFont());
		p_btn->SetCheck(TRUE);
		m_vec_btnCheck.push_back(p_btn);
		rc.left=rc.right;
		rc.right=rc.left+nEditWidth;
		p_edit=new CEdit();
		p_edit->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,rc,this,IDC_EDIT_START+i);
		p_edit->ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);
		p_edit->SetFont(this->GetFont());
		m_vec_edit.push_back(p_edit);
	}
	int nLine=g_nTotalPort/4;
	if(g_nTotalPort%4)
		++nLine;
	CPoint pt(nStartLeft,nStartTop+10);
	ClientToScreen(&pt);
	int nTop=pt.y+nLine*(nBtnHeight+10);
	ChangeSize(IDOK,nTop);
	ChangeSize(IDCANCEL,nTop);
	ChangeSize(IDC_BUTTON_SETBARCODEINFO,nTop);
	int nHeight=ChangeSize(IDC_BUTTON_GETMETERPOS,nTop);
	GetWindowRect(&rc);
	rc.bottom=nTop+nHeight+10;
	MoveWindow(rc);
}

int CMeterPos::ChangeSize(UINT nID,const int & nTop)
{
	CRect rc;
	GetDlgItem(nID)->GetWindowRect(&rc);
	int nHeight=rc.Height();
	rc.top=nTop;
	rc.bottom=rc.top+nHeight;
	ScreenToClient(&rc);
	GetDlgItem(nID)->MoveWindow(rc);
	return nHeight;
}

void CMeterPos::OnBnClickedCheckSelectallmeter()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<g_nTotalPort;++i)
	{
		m_vec_btnCheck[i]->SetCheck(((CButton *)GetDlgItem(IDC_CHECK_SELECTALLMETER))->GetCheck());
	}
	ExecVecSelect();
}


void CMeterPos::OnBnClickedButtonSelectmeter()
{
	// TODO: 在此添加控件通知处理程序代码
	SelectMeter(TRUE);
}


void CMeterPos::OnBnClickedButtonUnselectmeter()
{
	// TODO: 在此添加控件通知处理程序代码
	SelectMeter(FALSE);
}

void CMeterPos::OnBnClickedButtonGetmeterpos()
{
	// TODO: 在此添加控件通知处理程序代码
	ExecVecSelect();
	if(!g_nTotalPort)
		return;
	switch (g_v.g_nProtocolType % 10)
	{
	case CMainData::PROTOCOL_DLT645:
	{
		g_v.g_strDeviceAddr = _T("AAAAAAAAAAAA");
		g_v.g_strFuncCode = _T("13");
		for (int i = 0; i < g_nTotalPort; ++i)
		{
			if (g_vec_bIsSelect[i])
			{
				m_vec_edit[i]->SetWindowText(_T(""));
				g_vec_md[i].m_nProtocolType = g_v.g_nProtocolType;
				g_vec_md[i].m_p_protl = g_vec_md[i].m_p_dlt645;
				g_vec_md[i].m_pf_checkcall = ZDLT645_2007::Check645_2007;
				g_vec_md[i].m_p_WndOfSource = this;
				g_vec_md[i].m_nRow = 0;
				g_vec_md[i].m_nRowMax = 1;
				SetEvent(g_vec_md[i].m_hEvtThread);
			}
		}
	}
	break;
	case CMainData::PROTOCOL_DLT698:
	{
		g_v.g_strDeviceAddr = _T("AAAAAAAAAAAA");
		g_v.g_strFuncCode = _T("0501");
		g_v.g_strDataItem = _T("40010200");
		for (int i = 0; i < g_nTotalPort; ++i)
		{
			if (g_vec_bIsSelect[i])
			{
				m_vec_edit[i]->SetWindowText(_T(""));
				g_vec_md[i].m_nProtocolType = g_v.g_nProtocolType;
				g_vec_md[i].m_p_protl = g_vec_md[i].m_p_dlt698;
				g_vec_md[i].m_pf_checkcall = ZDLT698_45::Check698_45;
				g_vec_md[i].m_p_WndOfSource = this;
				g_vec_md[i].m_nRow = 0;
				g_vec_md[i].m_nRowMax = 1;
				SetEvent(g_vec_md[i].m_hEvtThread);
			}
		}
	}
	break;
	default:
		return;
	}
	if(g_nTotalSelectedPort)
	{
		for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
		{
			if(g_sz_p_wnd[i])
				g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,FALSE,MSGUSER_ENABLEBTN);
		}
	}
	g_bIsStop=false;
}

void CMeterPos::OnBnClickedButtonSetbarcodeinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBarcodeInfo();
}

bool CMeterPos::SetBarcodeInfo(void)
{
	CString strBarCode;
	for(int i=0;i<g_nTotalPort;++i)
	{
		if(g_vec_bIsSelect[i])
		{
			m_vec_edit[i]->GetWindowText(strBarCode);
			if(CMainData::m_nBarCodeLength!=strBarCode.GetLength())
			{
				CString str;
				str.Format(_T("表%d条码长度%d位,与方案%d位不符，请重新输入"),i+1,strBarCode.GetLength(),CMainData::m_nBarCodeLength);
				AfxMessageBox(str);
				m_vec_edit[i]->SetFocus();
				m_vec_edit[i]->SetSel(0,-1);
				return false;
			}
			g_vec_md[i].m_strTgtMeterAddr =strBarCode.Mid(CMainData::m_nMeterAddStart-1,CMainData::m_nMeterAddEnd-CMainData::m_nMeterAddStart+1);
			g_vec_md[i].m_strTgtMeterNO=strBarCode.Mid(CMainData::m_nMeterNOStart-1,CMainData::m_nMeterNOEnd-CMainData::m_nMeterNOStart+1);
			g_vec_md[i].m_strTgtPropertyNO=strBarCode.Mid(CMainData::m_nPropertyNOStart-1,CMainData::m_nPropertyNOEnd-CMainData::m_nPropertyNOStart+1);
		}
	}
	return true;
}

void CMeterPos::OnBnClickedCheckMeterpos(UINT nID)
{
	ExecVecSelect();
}

void CMeterPos::SelectMeter(const BOOL & nIsSelect)
{
	int nStart,nEnd;
	nStart=GetDlgItemInt(IDC_EDIT_STARTMETER);
	nEnd=GetDlgItemInt(IDC_EDIT_ENDMETER);
	--nStart;
	nStart=nStart>0?nStart:0;
	nEnd=nEnd>g_nTotalPort?g_nTotalPort:nEnd;
	for(int i=nStart;i<nEnd;++i)
	{
		m_vec_btnCheck[i]->SetCheck(nIsSelect);
	}
	ExecVecSelect();
}


void CMeterPos::ExecVecSelect(void)
{
	g_nTotalSelectedPort=0;
	g_nFstMeterIndex=-1;
	for(int i=0;i<g_nTotalPort;++i)
	{
		if(m_vec_btnCheck[i]->GetCheck())
		{
			if(g_nFstMeterIndex==-1)
				g_nFstMeterIndex=i;
			g_vec_bIsSelect[i]=true;
			++g_nTotalSelectedPort;
		}
		else
			g_vec_bIsSelect[i]=false;
	}
}


void CMeterPos::EnableBtn(const BOOL & bEnable)
{
	CWnd * p_wndChild=NULL;
	p_wndChild=GetWindow(GW_CHILD);
	while(p_wndChild!=NULL)
	{
		if(p_wndChild!=GetDlgItem(IDOK)&&p_wndChild!=GetDlgItem(IDCANCEL))
			p_wndChild->EnableWindow(bEnable);
		p_wndChild=p_wndChild->GetNextWindow(GW_HWNDNEXT);
	}
}

BOOL CMeterPos::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_BKGND);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap * p_bmpOld=dcMem.SelectObject(&bmp);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	CRect rc; 
	GetClientRect(&rc);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMeterPos::InitButton(void)
{
	SetBtnColor(m_btnGetMeterPos,_T(""));
	SetBtnColor(m_btnSetBarCodeInfo,_T(""));
	SetBtnColor(m_btnSelectMeter,_T(""));
	SetBtnColor(m_btnUnselectMeter,_T(""));
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
	m_btnSelectMeter.SetFlat(TRUE);
	m_btnUnselectMeter.SetFlat(TRUE);
	m_btnSelectMeter.DrawBorder(TRUE);
	m_btnUnselectMeter.DrawBorder(TRUE);
}

void CMeterPos::SetBtnColor(ZButton & btn,const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

HBRUSH CMeterPos::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	static CBrush brush;
	if(brush.m_hObject==NULL)
		brush.CreateSolidBrush(RGB(230,248,226));
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			if(pWnd->GetDlgCtrlID()==IDC_STATIC_METERTO)
			{
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH)GetStockObject(NULL_BRUSH);
			}
			else
				return brush;
		}
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CMeterPos::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN||pMsg->message==WM_SYSKEYDOWN)   
	{
		UINT nKey=pMsg->wParam;
		if(nKey==VK_RETURN)
		{
			CWnd * p_wnd=GetFocus();
			int nSize=m_vec_edit.size();
			int i,j;
			for(i=0;i<nSize;++i)
			{
				if(p_wnd->m_hWnd==m_vec_edit[i]->m_hWnd)
				{
					for(j=i+1;j<nSize;++j)
					{
						if(g_vec_bIsSelect[j])
						{
							m_vec_edit[j]->SetFocus();
							m_vec_edit[j]->SetSel(0,-1);
							return TRUE;
						}
					}
					if(j>=nSize)
					{
						if(SetBarcodeInfo())
						{
							m_p_protest->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ALLPRO);
							break;
						}
						else
							return TRUE;
					}
				}
			}
			if(i>=nSize)
				return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
