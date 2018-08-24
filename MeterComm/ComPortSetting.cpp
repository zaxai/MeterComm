// ComPortSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "ComPortSetting.h"
#include "afxdialogex.h"

// CComPortSetting 对话框
const int nMDIndexOffset=2;//md元素序号与所在test列表里的列序号的偏移

IMPLEMENT_DYNAMIC(CComPortSetting, CDialogEx)

CComPortSetting::CComPortSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CComPortSetting::IDD, pParent)
{

}

CComPortSetting::~CComPortSetting()
{
}

void CComPortSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_comboComPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudRate);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_comboParity);
	DDX_Control(pDX, IDC_COMBO_BYTESIZE, m_comboByteSize);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_comboStopBits);
	DDX_Control(pDX, IDC_COMBO_COMPORTMD, m_comboComPortMD);
	DDX_Control(pDX, IDC_COMBO_BAUDRATEMD, m_comboBaudRateMD);
	DDX_Control(pDX, IDC_COMBO_PARITYMD, m_comboParityMD);
	DDX_Control(pDX, IDC_COMBO_BYTESIZEMD, m_comboByteSizeMD);
	DDX_Control(pDX, IDC_COMBO_STOPBITSMD, m_comboStopBitsMD);
	DDX_Control(pDX, IDC_CHECK_ISMDCOMPORTOPEN, m_checkIsMDComPortOpen);
	DDX_Control(pDX, IDC_BUTTON_SETCOMPORT, m_btnSetComPort);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CComPortSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETCOMPORT, &CComPortSetting::OnBnClickedButtonSetcomport)
	ON_BN_CLICKED(IDC_CHECK_ISMDCOMPORTOPEN, &CComPortSetting::OnBnClickedCheckIsmdcomportopen)
	ON_MESSAGE(WM_MSGRECVPRO, &CComPortSetting::OnMsgrecvpro)
	ON_EN_CHANGE(IDC_EDIT_COMPORTNUM, &CComPortSetting::OnEnChangeEditComportnum)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CComPortSetting 消息处理程序


BOOL CComPortSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_COMPORTSET]=this;
	int i;
	CString strTemp;
	for(i=0;i<g_nMaxTotalPort;++i)
	{
		ZDevicePort zmp;
		strTemp.Format(_T("COM%d"),i+1);
		if(zmp.OpenPort(strTemp))
		{
			m_comboComPort.AddString(strTemp);
			m_comboComPortMD.AddString(strTemp);
		}
	}
	if(m_comboComPort.GetCount())
	{
		m_comboComPort.SetCurSel(0);
		m_comboComPortMD.SetCurSel(m_comboComPortMD.GetCount()-1);
		SetDlgItemText(IDC_EDIT_COMPORTNUM,_T("1"));
	}
	CString strBaudRate[]={_T("300"),_T("600"),_T("1200"),_T("2400"),_T("4800"),_T("9600"), _T("19200"),_T("38400"),_T("115200")};
	for(i=0;i<sizeof(strBaudRate)/sizeof(CString);++i)
	{
		m_comboBaudRate.AddString(strBaudRate[i]);
		m_comboBaudRateMD.AddString(strBaudRate[i]);
	}
	m_comboBaudRate.SetCurSel(3);
	m_comboBaudRateMD.SetCurSel(8);
	CString strParity[]={_T("None"),_T("Even"),_T("Odd"),_T("Mark"),_T("Space")};
	for(i=0;i<sizeof(strParity)/sizeof(CString);++i)
	{
		m_comboParity.AddString(strParity[i]);
		m_comboParityMD.AddString(strParity[i]);
	}
	m_comboParity.SetCurSel(1);
	m_comboParityMD.SetCurSel(0);
	for(i=0;i<4;++i)
	{
		strTemp.Format(_T("%d"),i+5);
		m_comboByteSize.InsertString(i,strTemp);
		m_comboByteSizeMD.InsertString(i,strTemp);
	}
	m_comboByteSize.SetCurSel(3);
	m_comboByteSizeMD.SetCurSel(3);
	CString strStopBits[]={_T("1"),_T("1.5"),_T("2")};
	for(i=0;i<sizeof(strStopBits)/sizeof(CString);++i)
	{
		m_comboStopBits.AddString(strStopBits[i]);
		m_comboStopBitsMD.AddString(strStopBits[i]);
	}
	m_comboStopBits.SetCurSel(0);
	m_comboStopBitsMD.SetCurSel(0);
	SetDlgItemText(IDC_EDIT_TIMEOUTSEND,_T("3000"));
	SetDlgItemText(IDC_EDIT_TIMEOUTSENDMD,_T("3000"));
	SetDlgItemText(IDC_EDIT_TIMEOUTRECV,_T("5000"));
	SetDlgItemText(IDC_EDIT_TIMEOUTRECVMD,_T("15000"));
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CComPortSetting::OnBnClickedButtonSetcomport()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_comboComPort.GetCount()==0)
	{
		AfxMessageBox(_T("没有可用串口！"));
		return;
	}
	static CString strComInfo;
	static BOOL nIsOpen=FALSE;
	if(!nIsOpen)
	{
		int nStart,nNum,nMD;
		CString strTemp;
		m_comboComPort.GetWindowText(strTemp);
		strComInfo=_T("串口:")+strTemp+_T(";");
		nStart=_ttoi(strTemp.Mid(3));
		GetDlgItemText(IDC_EDIT_COMPORTNUM,strTemp);
		strComInfo+=_T("串口数:")+strTemp+_T(";");
		nNum=_ttoi(strTemp);
		m_comboComPortMD.GetWindowText(strTemp);
		nMD=_ttoi(strTemp.Mid(3));
		CString strBaudRate,strParity,strByteSize,strStopBits,strBaudRateMD,strParityMD,strByteSizeMD,strStopBitsMD;
		m_comboBaudRate.GetWindowText(strBaudRate);
		m_comboParity.GetWindowText(strParity);
		m_comboByteSize.GetWindowText(strByteSize);
		m_comboStopBits.GetWindowText(strStopBits);
		m_comboBaudRateMD.GetWindowText(strBaudRateMD);
		m_comboParityMD.GetWindowText(strParityMD);
		m_comboByteSizeMD.GetWindowText(strByteSizeMD);
		m_comboStopBitsMD.GetWindowText(strStopBitsMD);
		strComInfo+=strBaudRate+_T(",")+strParity+_T(",")+strByteSize+_T(",")+strStopBits+_T(";");
		int nTimeoutOutSend,nTimeoutOutRecv,nTimeoutOutSendMD,nTimeoutOutRecvMD;
		nTimeoutOutSend=GetDlgItemInt(IDC_EDIT_TIMEOUTSEND);
		nTimeoutOutSendMD=GetDlgItemInt(IDC_EDIT_TIMEOUTSENDMD);
		nTimeoutOutRecv=GetDlgItemInt(IDC_EDIT_TIMEOUTRECV);
		nTimeoutOutRecvMD=GetDlgItemInt(IDC_EDIT_TIMEOUTRECVMD);
		g_nFstPort=nStart;
		g_nTotalPort=nNum;
		g_nIsUsingMD=m_checkIsMDComPortOpen.GetCheck();
		CString strPort;
		for(int i=0;i<nNum;++i)
		{
			strPort.Format(_T("COM%d"),i+nStart);
			CMainData md;
			g_vec_md.push_back(md);
			g_vec_md[i].m_nProtocolType=0;
			g_vec_md[i].InitMD(strPort);
			g_vec_md[i].m_p_zdp->SetPortState(strBaudRate,strParity,strByteSize,strStopBits);
			g_vec_md[i].m_p_zdp->SetTimeOutSend(nTimeoutOutSend);
			g_vec_md[i].m_p_zdp->SetTimeOutRecv(nTimeoutOutRecv);
			g_vec_md[i].m_p_MainDlg=g_sz_p_wnd[WND_METERCOMM];
			g_vec_md[i].m_nColumn=i+nMDIndexOffset;
		}
		if(g_nIsUsingMD)
		{
			strPort.Format(_T("COM%d"),nMD);
			g_mdMeterDevice.m_nProtocolType=0;
			g_mdMeterDevice.InitMD(strPort);
			g_mdMeterDevice.m_p_zdp->SetPortState(strBaudRateMD,strParityMD,strByteSizeMD,strStopBitsMD);
			g_mdMeterDevice.m_p_zdp->SetTimeOutSend(nTimeoutOutSendMD);
			g_mdMeterDevice.m_p_zdp->SetTimeOutRecv(nTimeoutOutRecvMD);
			g_mdMeterDevice.m_p_MainDlg=g_sz_p_wnd[WND_METERCOMM];
			strComInfo+=_T("启用台体:是");
		}
		else
			strComInfo+=_T("启用台体:否");
		//g_vec_md容器会自动扩容，导致元素地址变更，因此要等容器稳定了后创建线程传参
		for(int i=0;i<nNum;++i)
		{
			while(AfxBeginThread(CMeterCommView::PortCommThreadFunc,(LPVOID)&g_vec_md[i])==NULL);
		}
		if(g_nIsUsingMD)
		{
			while(AfxBeginThread(CMeterCommView::PortCommThreadFunc,(LPVOID)&g_mdMeterDevice)==NULL);
		}
		for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
		{
			if(g_sz_p_wnd[i])
				g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,(WPARAM)&strComInfo,MSGUSER_PORTOPEN);
		}
		SetDlgItemText(IDC_BUTTON_SETCOMPORT,_T("关闭串口"));
	}
	else
	{
		g_nTotalPort=0;
		g_nIsUsingMD=FALSE;
		for(int i=0;i<int(g_vec_md.size());++i)
		{
			g_vec_md[i].ClearMD();
		}
		g_mdMeterDevice.ClearMD();
		g_vec_md.clear();
		strComInfo=_T("串口未打开");
		for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
		{
			if(g_sz_p_wnd[i])
				g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,(WPARAM)&strComInfo,MSGUSER_PORTCLOSE);
		}
		SetDlgItemText(IDC_BUTTON_SETCOMPORT,_T("打开串口"));
	}
	EnableSet(nIsOpen);
	nIsOpen=!nIsOpen;
}


void CComPortSetting::OnBnClickedCheckIsmdcomportopen()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_checkIsMDComPortOpen.GetCheck())
	{
		if(!CheckMDComPortAvailable())
		{
			m_checkIsMDComPortOpen.SetCheck(0);
			AfxMessageBox(_T("台体串口号与通信串口重复！"));
		}
	}
}


bool CComPortSetting::CheckMDComPortAvailable(void)
{
	int nStart,nNum,nMD;
	CString strTemp;
	m_comboComPort.GetWindowText(strTemp);
	nStart=_ttoi(strTemp.Mid(3));
	nNum=GetDlgItemInt(IDC_EDIT_COMPORTNUM);
	m_comboComPortMD.GetWindowText(strTemp);
	nMD=_ttoi(strTemp.Mid(3));
	if(nMD>=nStart&&nMD<nStart+nNum)
		return false;
	else
		return true;
}


afx_msg LRESULT CComPortSetting::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case MSGUSER_ENABLEBTN:
		{
			EnableBtn(wParam);
		}
		break;
	}
	return 0;
}

void CComPortSetting::EnableBtn(const BOOL & bEnable)
{
	GetDlgItem(IDC_BUTTON_SETCOMPORT)->EnableWindow(bEnable);
}

void CComPortSetting::EnableSet(const BOOL & bEnable)
{
	CWnd * p_wndChild=NULL;
	p_wndChild=GetWindow(GW_CHILD);
	while(p_wndChild!=NULL)
	{
		if(p_wndChild!=GetDlgItem(IDOK)&&p_wndChild!=GetDlgItem(IDCANCEL)&&p_wndChild!=GetDlgItem(IDC_BUTTON_SETCOMPORT))
			p_wndChild->EnableWindow(bEnable);
		p_wndChild=p_wndChild->GetNextWindow(GW_HWNDNEXT);
	}
}

void CComPortSetting::OnEnChangeEditComportnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(m_comboComPort.GetCount()==0)
	{
		AfxMessageBox(_T("没有可用串口！"));
		return;
	}
	int nNum=GetDlgItemInt(IDC_EDIT_COMPORTNUM);
	int nStart;
	CString strTemp;
	m_comboComPort.GetWindowText(strTemp);
	nStart=_ttoi(strTemp.Mid(3));
	if(nNum<0)
	{
		AfxMessageBox(_T("串口数不能为负数!"));
		SetDlgItemInt(IDC_EDIT_COMPORTNUM,0);
	}
	else
	{
		if(m_checkIsMDComPortOpen.GetCheck()&&!CheckMDComPortAvailable())
		{
			AfxMessageBox(_T("台体串口号与通信串口重复！"));
			SetDlgItemInt(IDC_EDIT_COMPORTNUM,0);
		}
		int nIndex=m_comboComPort.GetCurSel();
		int i;
		for(i=0;i<nNum;++i)
		{
			if(nIndex+i>=m_comboComPort.GetCount())	
				break;
			else
			{
				m_comboComPort.GetLBText(nIndex+i,strTemp);
				if(_ttoi(strTemp.Mid(3))!=nStart+i)
					break;
			}
		}
		if(i<nNum)
		{
			strTemp.Format(_T("COM%d不存在!"),nStart+i);
			AfxMessageBox(strTemp);
			SetDlgItemInt(IDC_EDIT_COMPORTNUM,0);
		}
	}
}


BOOL CComPortSetting::OnEraseBkgnd(CDC* pDC)
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

void CComPortSetting::InitButton(void)
{
	SetBtnColor(m_btnSetComPort,_T(""));
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
}

void CComPortSetting::SetBtnColor(ZButton & btn,const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

HBRUSH CComPortSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
			UINT nID=pWnd->GetDlgCtrlID();
			if(nID!=IDC_CHECK_ISMDCOMPORTOPEN&&nID!=IDC_EDIT_COMPORTNUM&&nID!=IDC_EDIT_TIMEOUTSEND&&nID!=IDC_EDIT_TIMEOUTRECV&&nID!=IDC_EDIT_TIMEOUTSENDMD&&nID!=IDC_EDIT_TIMEOUTRECVMD)
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
