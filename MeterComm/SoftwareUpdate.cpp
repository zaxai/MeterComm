// SoftwareUpdate.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "SoftwareUpdate.h"
#include "afxdialogex.h"


// CSoftwareUpdate 对话框

IMPLEMENT_DYNAMIC(CSoftwareUpdate, CDialogEx)

CSoftwareUpdate::CSoftwareUpdate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoftwareUpdate::IDD, pParent)
	, m_hEvtExitCheckUpdate(NULL)
{

}

CSoftwareUpdate::~CSoftwareUpdate()
{
	WaitForSingleObject(m_hEvtExitCheckUpdate, INFINITE);
	if(m_hEvtExitCheckUpdate!=NULL)
	{
		CloseHandle(m_hEvtExitCheckUpdate);
		m_hEvtExitCheckUpdate=NULL;
	}
}

void CSoftwareUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CSoftwareUpdate, CDialogEx)
	ON_MESSAGE(WM_MSGRECVPRO, &CSoftwareUpdate::OnMsgrecvpro)
	ON_BN_CLICKED(IDOK, &CSoftwareUpdate::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSoftwareUpdate 消息处理程序


BOOL CSoftwareUpdate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitButton();
	m_btnOK.EnableWindow(FALSE);
	SetDlgItemText(IDC_STATIC_CURVERSION,g_strVersion);
	m_hEvtExitCheckUpdate =CreateEvent(NULL,TRUE,TRUE,NULL);
	while(AfxBeginThread(CheckSoftwareUpdateThreadFunc,(LPVOID)this)==NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

UINT CSoftwareUpdate::CheckSoftwareUpdateThreadFunc(LPVOID lpParam)
{
	CSoftwareUpdate * p_swupdate=(CSoftwareUpdate *)lpParam;
	ResetEvent(p_swupdate->m_hEvtExitCheckUpdate);
	CString strDataIn,strDataOut,strHead,strPath;
	strDataIn=g_strVersion;
	strHead.Format(_T("[%06d02]"),strDataIn.GetLength()+10);
	strDataIn=strHead+strDataIn;
	if(!SocketCommunication(strDataIn,strDataOut))
	{
		if(p_swupdate->m_hWnd)
			p_swupdate->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ERROFGETSERVERINFO); 
	}
	else
	{
		if(strDataOut.Mid(0,1)==_T("[")&&strDataOut.Mid(9,1)==_T("]")&&_ttoi(strDataOut.Mid(1,6))==strDataOut.GetLength()&&strDataOut.Mid(7,2)==_T("02"))
		{
			strDataOut.Delete(0,10);
			SendMessageTimeout(p_swupdate->m_hWnd,WM_MSGRECVPRO,(WPARAM)&strDataOut,MSGUSER_UPDATELATESTVERSION,SMTO_BLOCK,500,NULL); 
		}
		else
		{
			if(p_swupdate->m_hWnd)
				p_swupdate->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ERROFGETSERVERINFO); 
		}
	}
	SetEvent(p_swupdate->m_hEvtExitCheckUpdate);
	return 0;
}

bool CSoftwareUpdate::SocketCommunication(const CString & strDataIn,CString & strDataOut)
{
	ZStringSocket zSock;
	int nRtn = ZSocket::ERROR_OK;
	CGlobalVariable gvariable;
	SendMessageTimeout(g_sz_p_wnd[WND_METERCOMM]->m_hWnd,WM_MSGRECVPRO,(WPARAM)&gvariable,MSGUSER_GETSERVERINFO,SMTO_BLOCK,500,NULL); 
	CString & strIP=gvariable.g_strServerIP;
	CString & strPort=gvariable.g_strServerPort;
	CString & strTimeOut=gvariable.g_strServerTimeOut;
	zSock.SetTimeOut(ZSocket::TIMEOUT_CONT,_ttoi(strTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_SEND,_ttoi(strTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_RECV,_ttoi(strTimeOut));
	nRtn=zSock.Connect(strIP,strPort);
	if(nRtn)
		return false;
	nRtn=zSock.StringSend(strDataIn);
	if(nRtn)
		return false;
	nRtn=zSock.StringRecv(strDataOut);
	if(nRtn)
		return false;
	return true;
}

afx_msg LRESULT CSoftwareUpdate::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case MSGUSER_ERROFGETSERVERINFO:
		{
			SetDlgItemText(IDC_STATIC_LATESTVERSION,_T("网络错误"));
		}
		break;
	case MSGUSER_UPDATELATESTVERSION:
		{
			CString * p_str=(CString *)wParam;
			std::vector<CString> vec_strDatas;
			ZUtil::StrSplit(*p_str,vec_strDatas,_T('*'),p_str->Right(1)==_T('*'));
			SetDlgItemText(IDC_STATIC_LATESTVERSION,vec_strDatas[0]);
			if(vec_strDatas[0]!=g_strVersion)
			{
				switch(_ttoi(vec_strDatas[1]))
				{
				case 0:
					break;
				case 1:
					{
						m_btnOK.EnableWindow(TRUE);
					}
					break;
				case 2:
					{
						m_btnOK.EnableWindow(TRUE);
						m_btnCancel.EnableWindow(FALSE);
					}
					break;
				}
			}
		}
		break;
	}
	return 0;
}


void CSoftwareUpdate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SoftWareUpdate();
	CDialogEx::OnOK();
}

void CSoftwareUpdate::SoftWareUpdate(void)
{
	CString strUpdatePath,strParameters;
	strUpdatePath=ZUtil::GetExeCatalogPath()+_T("\\SoftwareUpdate.exe");
	CString & strIP=g_v.g_strServerIP;
	CString & strPort=g_v.g_strServerPort;
	CString & strTimeOut=g_v.g_strServerTimeOut;
	strParameters=_T(" ")+strIP+_T(" ")+strPort+_T(" \"")+ZUtil::GetExePath()+_T("\"");
	HINSTANCE hRtn=ShellExecute(0,_T("open"),strUpdatePath,strParameters,NULL,SW_SHOWNORMAL);
	int nRtn=(int)hRtn;
	if(nRtn>32)
		GetParent()->PostMessage(WM_CLOSE); 
	else 
	{
		CString strShow;
		strShow.Format(_T("更新失败，错误代码:%d"),nRtn);
		AfxMessageBox(strShow);
	}
}


BOOL CSoftwareUpdate::OnEraseBkgnd(CDC* pDC)
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

void CSoftwareUpdate::InitButton(void)
{
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
}

void CSoftwareUpdate::SetBtnColor(ZButton & btn,const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

HBRUSH CSoftwareUpdate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
			if(nID!=IDC_STATIC_LATESTVERSION)
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
