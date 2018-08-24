// NetWorkSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "NetWorkSetting.h"
#include "afxdialogex.h"


// CNetWorkSetting 对话框

IMPLEMENT_DYNAMIC(CNetWorkSetting, CDialogEx)

CNetWorkSetting::CNetWorkSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetWorkSetting::IDD, pParent)
	, m_strServerIP(_T("zaxai.nat123.net"))
	, m_strServerPort(_T("44535"))
	, m_strServerTimeOut(_T("5000"))
	, m_strNetCptIP(_T("10.0.17.99"))
	, m_strNetCptPort(_T("6666")) 
	, m_strNetCptTimeOut(_T("5000"))
	, m_nDllSel(2)
	, m_nIsRemote(0)
{

}

CNetWorkSetting::~CNetWorkSetting()
{
}

void CNetWorkSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_ENCRYPTOR, m_comboEncryptor);
}


BEGIN_MESSAGE_MAP(CNetWorkSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNetWorkSetting::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ISREMOTE, &CNetWorkSetting::OnBnClickedCheckIsremote)
	ON_CBN_SELCHANGE(IDC_COMBO_ENCRYPTOR, &CNetWorkSetting::OnCbnSelchangeComboEncryptor)
END_MESSAGE_MAP()


// CNetWorkSetting 消息处理程序


BOOL CNetWorkSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_EDIT_SERVERIP,m_strServerIP);
	SetDlgItemText(IDC_EDIT_SERVERPORT,m_strServerPort);
	SetDlgItemText(IDC_EDIT_SERVERTIMEOUT,m_strServerTimeOut);
	SetDlgItemText(IDC_EDIT_NETCPTIP,m_strNetCptIP);
	SetDlgItemText(IDC_EDIT_NETCPTPORT,m_strNetCptPort);
	SetDlgItemText(IDC_EDIT_NETCPTTIMEOUT,m_strNetCptTimeOut);
	CString sz_strEncryptor[] = { _T("09本地加密机"),_T("13国网加密机"),_T("13陕西地电加密机"),_T("15南网加密机"),_T("17国网698加密机") };
	for (int i = 0; i<sizeof(sz_strEncryptor) / sizeof(CString); ++i)
		m_comboEncryptor.AddString(sz_strEncryptor[i]);
	m_comboEncryptor.SetCurSel(m_nDllSel);
	((CButton *)GetDlgItem(IDC_CHECK_ISREMOTE))->SetCheck(m_nIsRemote);
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNetWorkSetting::SetParam(const CString & strServerIP, const CString & strServerPort, const CString & strServerTimeOut,const CString & strNetCptIP, const CString & strNetCptPort, const CString & strNetCptTimeOut, const BOOL & nDllSel, const BOOL & nIsRemote)
{
	m_strServerIP=strServerIP;
	m_strServerPort=strServerPort;
	m_strServerTimeOut=strServerTimeOut;
	m_strNetCptIP=strNetCptIP;
	m_strNetCptPort=strNetCptPort;
	m_strNetCptTimeOut=strNetCptTimeOut;
	m_nDllSel = nDllSel;
	m_nIsRemote = nIsRemote;
}

void CNetWorkSetting::GetParam(CString & strServerIP, CString & strServerPort, CString & strServerTimeOut, CString & strNetCptIP, CString & strNetCptPort, CString & strNetCptTimeOut, BOOL & nDllSel, BOOL & nIsRemote)
{
	strServerIP=m_strServerIP;
	strServerPort=m_strServerPort;
	strServerTimeOut=m_strServerTimeOut;
	strNetCptIP=m_strNetCptIP;
	strNetCptPort=m_strNetCptPort;
	strNetCptTimeOut=m_strNetCptTimeOut;
	nDllSel = m_nDllSel;
	nIsRemote = m_nIsRemote;
}

void CNetWorkSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_SERVERIP,m_strServerIP);
	GetDlgItemText(IDC_EDIT_SERVERPORT,m_strServerPort);
	GetDlgItemText(IDC_EDIT_SERVERTIMEOUT,m_strServerTimeOut);
	GetDlgItemText(IDC_EDIT_NETCPTIP,m_strNetCptIP);
	GetDlgItemText(IDC_EDIT_NETCPTPORT,m_strNetCptPort);
	GetDlgItemText(IDC_EDIT_NETCPTTIMEOUT,m_strNetCptTimeOut);
	CDialogEx::OnOK();
}


BOOL CNetWorkSetting::OnEraseBkgnd(CDC* pDC)
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

void CNetWorkSetting::InitButton(void)
{
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
}

void CNetWorkSetting::SetBtnColor(ZButton & btn,const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

HBRUSH CNetWorkSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	static CBrush brush;
	if (brush.m_hObject == NULL)
		brush.CreateSolidBrush(RGB(230, 248, 226));
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			UINT nID = pWnd->GetDlgCtrlID();
			if (nID != IDC_CHECK_ISREMOTE)
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


void CNetWorkSetting::OnBnClickedCheckIsremote()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nIsRemote = ((CButton *)GetDlgItem(IDC_CHECK_ISREMOTE))->GetCheck();
}


void CNetWorkSetting::OnCbnSelchangeComboEncryptor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDllSel = m_comboEncryptor.GetCurSel();
	switch (m_nDllSel)
	{
	case 1:m_strNetCptIP = _T("10.0.17.99"); break;
	case 2:m_strNetCptIP = _T("10.0.17.109"); break;
	case 3:m_strNetCptIP = _T("10.0.17.108"); break;
	case 4:m_strNetCptIP = _T("10.0.17.107"); break;
	}
	SetDlgItemText(IDC_EDIT_NETCPTIP, m_strNetCptIP);
}
