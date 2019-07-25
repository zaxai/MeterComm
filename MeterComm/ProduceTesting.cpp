// ProduceTesting.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "ProduceTesting.h"
#include "afxdialogex.h"
#include "MeterPos.h"

// CProduceTesting 对话框
const int nMDIndexOffset=2;//md元素序号与所在test列表里的列序号的偏移
const COLORREF crRight=RGB(60,120,30);
const COLORREF crError=RGB(255,0,0);
const COLORREF crStart=RGB(49,106,197);
const COLORREF crOri=GetSysColor(COLOR_WINDOW);

IMPLEMENT_DYNAMIC(CProduceTesting, CDialogEx)

CProduceTesting::CProduceTesting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProduceTesting::IDD, pParent)
	, m_nTreeCount(0)
	, m_bIsInitFinish(false)
	, m_p_meterpos(NULL)
	, m_dScale(1)
	, m_bIsShowTree(true)
	, m_hEvtExitUpdatePlan(NULL)
	, m_bIsUpdatePlanRun(true)
	, m_nRowFirst(0)
{

}

CProduceTesting::~CProduceTesting()
{
	m_bIsUpdatePlanRun=false;
	WaitForSingleObject(m_hEvtExitUpdatePlan, INFINITE);
	if(m_p_meterpos!=NULL)
	{
		delete m_p_meterpos;
		m_p_meterpos=NULL;
	}
	if(m_hEvtExitUpdatePlan!=NULL)
	{
		CloseHandle(m_hEvtExitUpdatePlan);
		m_hEvtExitUpdatePlan=NULL;
	}
}

void CProduceTesting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TESTING, m_listTest);
	DDX_Control(pDX, IDC_TREE_PLAN, m_treePlan);
	DDX_Control(pDX, IDC_BUTTON_IMPORTPLAN, m_btnImportPlan);
	DDX_Control(pDX, IDC_BUTTON_UPDATENETPLAN, m_btnUpdateNetPlan);
	DDX_Control(pDX, IDC_BUTTON_METERPOS, m_btnMeterPos);
	DDX_Control(pDX, IDC_BUTTON_SINGLEPRO, m_btnSinglePro);
	DDX_Control(pDX, IDC_BUTTON_CONTINUEPRO, m_btnContinuePro);
	DDX_Control(pDX, IDC_BUTTON_ALLPRO, m_btnAllPro);
	DDX_Control(pDX, IDC_BUTTON_STOPPRO, m_btnStopPro);
	DDX_Control(pDX, IDC_BUTTON_OPERATORTREE, m_btnOperatorTree);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT, m_btnZoomOut);
	DDX_Control(pDX, IDC_BUTTON_ZOOMNORMAL, m_btnZoomNormal);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN, m_btnZoomIn);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CProduceTesting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTPLAN, &CProduceTesting::OnBnClickedButtonImportplan)
	ON_BN_CLICKED(IDC_BUTTON_OPERATORTREE, &CProduceTesting::OnBnClickedButtonOperatortree)
	ON_BN_CLICKED(IDC_BUTTON_METERPOS, &CProduceTesting::OnBnClickedButtonMeterpos)
	ON_BN_CLICKED(IDC_BUTTON_SINGLEPRO, &CProduceTesting::OnBnClickedButtonSinglepro)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUEPRO, &CProduceTesting::OnBnClickedButtonContinuepro)
	ON_BN_CLICKED(IDC_BUTTON_ALLPRO, &CProduceTesting::OnBnClickedButtonAllpro)
	ON_BN_CLICKED(IDC_BUTTON_STOPPRO, &CProduceTesting::OnBnClickedButtonStoppro)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CProduceTesting::OnBnClickedButtonZoomout)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMNORMAL, &CProduceTesting::OnBnClickedButtonZoomnormal)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CProduceTesting::OnBnClickedButtonZoomin)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PLAN, &CProduceTesting::OnTvnSelchangedTreePlan)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MSGRECVPRO, &CProduceTesting::OnMsgrecvpro)
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_BUTTON_UPDATENETPLAN, &CProduceTesting::OnBnClickedButtonUpdatenetplan)
END_MESSAGE_MAP()


// CProduceTesting 消息处理程序


BOOL CProduceTesting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_sz_p_wnd[WND_PRODUCETEST]=this;
	if(m_p_meterpos==NULL)
	{
		m_p_meterpos = new CMeterPos(); 
		m_p_meterpos->SetProTest(this);
		m_p_meterpos->Create(IDD_METERPOS, this); 
	}
	InitMenu();
	InitList();
	InsertList();
	InitTree();
	InitButton();
	m_hEvtExitUpdatePlan =CreateEvent(NULL,TRUE,TRUE,NULL);
	while(AfxBeginThread(UpdatePlanThreadFunc,(LPVOID)this)==NULL);
	m_btnUpdateNetPlan.EnableWindow(FALSE);
	m_bIsInitFinish=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CProduceTesting::OnBnClickedButtonImportplan()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("方案文件(*.pn)|*.pn|所有文件(*.*)|*.*||");   
	CString strPlanPath,strPlanTitle;
	CFileDialog fileDlg(TRUE,NULL,NULL,0,szFilter,this);  
	if (IDOK == fileDlg.DoModal()) 
	{
		strPlanPath=fileDlg.GetPathName(); 
		strPlanTitle=strPlanPath.Mid(strPlanPath.ReverseFind(_T('\\'))+1); 
		bool bIsInVec=false;
		int i;
		for(i=0;i<int(m_vec_strPlanPath.size());++i)
		{
			if(m_vec_strPlanPath[i]==strPlanPath)
			{
				bIsInVec=true;
				break;
			}
		}
		HTREEITEM hFirst,hSecond;
		hFirst=m_treePlan.GetChildItem(m_treePlan.GetRootItem());
		while(hFirst)
		{
			if(m_treePlan.GetItemText(hFirst)==_T("本地方案"))
				break;
			hFirst=m_treePlan.GetNextItem(hFirst,TVGN_NEXT);
		}
		if(bIsInVec)
		{
			hSecond=m_treePlan.GetChildItem(hFirst);
			while(hSecond)
			{
				if(m_treePlan.GetItemData(hSecond)==i)
				{
					m_treePlan.SelectItem(hSecond);
					break;
				}
				hSecond=m_treePlan.GetNextItem(hSecond,TVGN_NEXT);
			}
		}
		else
		{
			hSecond=m_treePlan.InsertItem(strPlanTitle,hFirst);
			m_treePlan.SetItemData(hSecond,m_nTreeCount); 
			m_vec_strPlanPath.push_back(strPlanPath);
			m_treePlan.SelectItem(hSecond);
			++m_nTreeCount;
		}
		m_treePlan.Expand(hFirst,TVE_EXPAND);
	}
}


void CProduceTesting::OnBnClickedButtonUpdatenetplan()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hRoot,hFirst;
	hRoot = m_treePlan.GetRootItem();
	hFirst = m_treePlan.GetChildItem(hRoot);
	while (hFirst)
	{
		if (m_treePlan.GetItemText(hFirst) == _T("网络方案"))
			break;
		hFirst = m_treePlan.GetNextItem(hFirst, TVGN_NEXT);
	}
	if (m_treePlan.DeleteItem(hFirst))
	{
		hFirst = m_treePlan.InsertItem(_T("网络方案"), hRoot);
		m_treePlan.SetItemData(hFirst, m_nTreeCount);
		m_vec_strPlanPath.push_back(_T("网络方案"));
		++m_nTreeCount;
	}
	while (AfxBeginThread(UpdatePlanThreadFunc, (LPVOID)this) == NULL);
	m_btnUpdateNetPlan.EnableWindow(FALSE);
}


void CProduceTesting::OnBnClickedButtonOperatortree()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsShowTree=!m_bIsShowTree;
	if(m_bIsShowTree)
	{
		GetDlgItem(IDC_TREE_PLAN)->ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_BUTTON_OPERATORTREE,_T("隐藏方案"));
	}
	else
	{
		GetDlgItem(IDC_TREE_PLAN)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_BUTTON_OPERATORTREE,_T("显示方案"));
	}
	CRect rc;
	GetClientRect(&rc);
	ChangeSize(IDC_LIST_TESTING,rc); 
	ChangeSize(IDC_EDIT_CONFIGURE,rc); 
	ZoomList(m_dScale);
	Invalidate();
}


void CProduceTesting::OnBnClickedButtonMeterpos()
{
	// TODO: 在此添加控件通知处理程序代码
	m_p_meterpos->ShowWindow(SW_SHOW);
}


void CProduceTesting::OnBnClickedButtonSinglepro()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!g_nTotalSelectedPort)
	{
		AfxMessageBox(_T("表位选择个数:0"));
		return;
	}
	POSITION pos;
	pos=m_listTest.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nRow=m_listTest.GetNextSelectedItem(pos);
		int nRowMax=nRow+1;
		SetListBlank(nRow,nRowMax,nMDIndexOffset,nMDIndexOffset+g_nTotalPort);
		for(int i=0;i<g_nTotalPort;++i)
		{
			g_vec_md[i].m_p_WndOfSource=this;
			g_vec_md[i].m_nRow=nRow;
			g_vec_md[i].m_nRowMax=nRowMax;
		}
		g_mdMeterDevice.m_p_WndOfSource=this;
		g_mdMeterDevice.m_nRow=nRow;
		g_mdMeterDevice.m_nRowMax=nRowMax;
		CheckFirstRow(nRow);
	}
	else
	{
		AfxMessageBox(_T("未选择一条方案命令"));
	}
}


void CProduceTesting::OnBnClickedButtonContinuepro()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!g_nTotalSelectedPort)
	{
		AfxMessageBox(_T("表位选择个数:0"));
		return;
	}
	POSITION pos;
	pos=m_listTest.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nRow=m_listTest.GetNextSelectedItem(pos);
		int nRowMax=m_listTest.GetItemCount();
		SetListBlank(nRow,nRowMax,nMDIndexOffset,nMDIndexOffset+g_nTotalPort);
		for(int i=0;i<g_nTotalPort;++i)
		{
			g_vec_md[i].m_p_WndOfSource=this;
			g_vec_md[i].m_nRow=nRow;
			g_vec_md[i].m_nRowMax=nRowMax;
		}
		g_mdMeterDevice.m_p_WndOfSource=this;
		g_mdMeterDevice.m_nRow=nRow;
		g_mdMeterDevice.m_nRowMax=nRowMax;
		CheckFirstRow(nRow);
	}
	else
	{
		AfxMessageBox(_T("未选择一条方案命令"));
	}
}


void CProduceTesting::OnBnClickedButtonAllpro()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!g_nTotalSelectedPort)
	{
		AfxMessageBox(_T("表位选择个数:0"));
		return;
	}
	if(m_listTest.GetItemCount()>0)
	{
		int nRow=0;
		int nRowMax=m_listTest.GetItemCount();
		SetListBlank(nRow,nRowMax,nMDIndexOffset,nMDIndexOffset+g_nTotalPort);
		for(int i=0;i<g_nTotalPort;++i)
		{
			g_vec_md[i].m_p_WndOfSource=this;
			g_vec_md[i].m_nRow=nRow;
			g_vec_md[i].m_nRowMax=nRowMax;
		}
		g_mdMeterDevice.m_p_WndOfSource=this;
		g_mdMeterDevice.m_nRow=nRow;
		g_mdMeterDevice.m_nRowMax=nRowMax;
		CheckFirstRow(nRow);
	}
	else
	{
		AfxMessageBox(_T("未选择一个方案"));
	}
}


void CProduceTesting::OnBnClickedButtonStoppro()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bIsStop=true;
	for(int i=0;i<g_nTotalPort;++i)
	{
		ResetEvent(g_vec_md[i].m_hEvtThread);
		g_vec_md[i].m_p_zdp->StopComm();
	}
	for(int i=0;i<g_nTotalPort;++i)
	{
		WaitForSingleObject(g_vec_md[i].m_hEvtStop, INFINITE);
	}
	if(g_nIsUsingMD)
	{
		ResetEvent(g_mdMeterDevice.m_hEvtThread);
		g_mdMeterDevice.m_p_zdp->StopComm();
		WaitForSingleObject(g_mdMeterDevice.m_hEvtStop, INFINITE);
	}
	for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
	{
		if(g_sz_p_wnd[i])
			g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,TRUE,MSGUSER_ENABLEBTN);
	}
}


void CProduceTesting::OnBnClickedButtonZoomout()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dScale*=0.8;
	ZoomList(m_dScale);
}


void CProduceTesting::OnBnClickedButtonZoomnormal()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dScale=1;
	ZoomList(m_dScale);
}


void CProduceTesting::OnBnClickedButtonZoomin()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dScale*=1.2;
	ZoomList(m_dScale);
}


void CProduceTesting::InitMenu(void)
{
	menuProTest.LoadMenu(IDR_MENU_PRODUCETESTING);
}


void CProduceTesting::InitList(void)
{
	m_listTest.SetExtendedStyle(m_listTest.GetExtendedStyle()|LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 
	m_listTest.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40, 0);   
	m_listTest.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 100, 1); 
}

void CProduceTesting::InsertList(void)
{
	int nColumnCount=m_listTest.GetHeaderCtrl()->GetItemCount();
	if(nColumnCount<nMDIndexOffset)
		return;
	m_listTest.SetRedraw(FALSE);
	for(int i=0;i<nColumnCount-nMDIndexOffset;++i)
		m_listTest.DeleteColumn(nMDIndexOffset);
	CRect rc;
	m_listTest.GetClientRect(&rc);
	int nWidth=rc.Width()-40-100-10;
	if(nWidth<0)
		nWidth=0;
	CString strTemp;
	if(g_nTotalPort)
		nWidth/=g_nTotalPort;
	for(int i=0;i<g_nTotalPort;++i)
	{
		strTemp.Format(_T("表%d"),i+1);
		m_listTest.InsertColumn(i+nMDIndexOffset,strTemp,LVCFMT_LEFT,nWidth,i+nMDIndexOffset);
	}
	m_listTest.SetRedraw(TRUE);
}

afx_msg LRESULT CProduceTesting::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_GETSENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		CString strOrderName;
		strOrderName = m_vec2_strCurPlan[p_md->m_nRow][0];
		p_md->m_nProtocolType = _ttoi(m_vec2_strCurPlan[p_md->m_nRow][6]);
		p_md->m_nUserDefined = _ttoi(m_vec2_strCurPlan[p_md->m_nRow][7]);
		if (strOrderName == _T("线程等待"))
		{
			p_md->m_dwThreadTimeWait = _tcstoul(m_vec2_strCurPlan[p_md->m_nRow][3], NULL, 10);
			m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crRight);
			p_md->m_nFlagGetSendData = FLAG_THREADWAIT;
		}
		else
		{
			switch (p_md->m_nProtocolType % 10)
			{
			case CMainData::PROTOCOL_DLT645:
			{
				p_md->m_p_protl = p_md->m_p_dlt645;
				p_md->m_pf_checkcall = ZDLT645_2007::Check645_2007;
				g_v.g_strDeviceAddr = _T("");
				g_v.g_strFuncCode = m_vec2_strCurPlan[p_md->m_nRow][1];
				g_v.g_strDataItem = m_vec2_strCurPlan[p_md->m_nRow][2];
				g_v.g_strTgtData = ExecTgtOrCorrectData(p_md, m_vec2_strCurPlan[p_md->m_nRow][3]);
				g_v.g_strGrade = m_vec2_strCurPlan[p_md->m_nRow][5].Right(2);
				g_v.g_strPsWd = m_vec2_strCurPlan[p_md->m_nRow][5].Left(6);
				g_v.g_strOperaCode = _T("00000000");
				p_md->m_p_dlt645->Set15Param(g_v.Get645Param());
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crStart);
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
			break;
			case CMainData::PROTOCOL_RMT:
			{
				p_md->m_p_protl = p_md->m_p_rmtProtl;
				p_md->m_pf_checkcall = ZRMTProtocol::CheckRMTProtocol;
				p_md->m_p_rmtProtl->InitParam(m_vec2_strCurPlan[p_md->m_nRow][1], m_vec2_strCurPlan[p_md->m_nRow][3]);
				for (int i = 0; i < g_nTotalPort; ++i)
				{
					if (g_vec_bIsSelect[i])
						m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crStart);
				}
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
			break;
			case CMainData::PROTOCOL_DLT698:
			{
				p_md->m_p_protl = p_md->m_p_dlt698;
				p_md->m_pf_checkcall = ZDLT698_45::Check698_45;
				g_v.g_strDeviceAddr = _T("");
				g_v.g_strFuncCode = m_vec2_strCurPlan[p_md->m_nRow][1];
				g_v.g_strDataItem = m_vec2_strCurPlan[p_md->m_nRow][2];
				g_v.g_strTgtData = ExecTgtOrCorrectData(p_md, m_vec2_strCurPlan[p_md->m_nRow][3]);
				g_v.g_strGrade = m_vec2_strCurPlan[p_md->m_nRow][5].Right(2);
				p_md->m_p_dlt698->Set16Param(g_v.Get698Param());
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crStart);
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
			break;
			default:
			{
				p_md->m_p_protl = NULL;
				p_md->m_pf_checkcall = NULL;
				CString strText(_T("协议类型错误"));
				for (int i = 0; i < g_nTotalPort; ++i)
				{
					if (g_vec_bIsSelect[i])
					{
						m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, strText);
						m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
					}
				}
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
			break;
			}
		}
	}
	break;
	case MSGUSER_ENCODE:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		case CMainData::PROTOCOL_DLT698:
		{
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
				m_listTest.SetItemText(p_md->m_nRow, p_md->m_nColumn, strText);
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crError);
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
		}
		break;
		case CMainData::PROTOCOL_RMT:
		{
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
				for (int i = 0; i < g_nTotalPort; ++i)
				{
					if (g_vec_bIsSelect[i])
					{
						m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, strText);
						m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
					}
				}
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
		}
		break;
		}
	}
	break;
	case MSGUSER_UPDATESENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		case CMainData::PROTOCOL_DLT698:
		{
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
				m_listTest.SetItemText(p_md->m_nRow, p_md->m_nColumn, strText);
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crError);
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
		}
		break;
		case CMainData::PROTOCOL_RMT:
		{
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
				for (int i = 0; i < g_nTotalPort; ++i)
				{
					if (g_vec_bIsSelect[i])
					{
						m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, strText);
						m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
					}
				}
				m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
			}
		}
		break;
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
			bool bResult = false;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				strText = p_md->m_strDataValue;
				CString strCorrectData;
				strCorrectData = ExecTgtOrCorrectData(p_md, m_vec2_strCurPlan[p_md->m_nRow][4]);
				bResult = ResultCompare645(strText, strCorrectData);
				CString strExt = strText.Right(3);
				if ((strExt == _T("91H") || strExt == _T("94H")) && p_md->m_p_dlt645->GetDataItem() == _T("04000401"))
				{
					m_p_meterpos->PostMessage(WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_UPDATEMETERADD);
				}
				if (strExt == _T("93H") || strExt == _T("95H"))
				{
					m_p_meterpos->PostMessage(WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_UPDATEMETERADD);
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
			m_listTest.SetItemText(p_md->m_nRow, p_md->m_nColumn, strText);
			if (bResult)
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crRight);
			else
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crError);
			m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
		}
		break;
		case CMainData::PROTOCOL_RMT:
		{
			CString strText;
			static DWORD dwNowTick = 0, dwEndTick = 0;
			static std::map<int, CString> mapMeterError;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				CString strOrderType = m_vec2_strCurPlan[p_md->m_nRow][1];
				if ((strOrderType == _T("20") || strOrderType == _T("33")) && p_md->m_strDataValue.Right(3) == _T("FFH"))
				{
					if (p_md->m_strDataValue.Left(4) == _T("0000"))
					{
						p_md->m_nFlagUpdateRecvData = FLAG_GOTORECV;
						dwNowTick = ::GetTickCount();
						if (strOrderType == _T("20"))
							dwEndTick = dwNowTick + DWORD(p_md->m_p_rmtProtl->GetMeterErrorWaitTime()*1000.0f);
						else
							dwEndTick = dwNowTick + DWORD(p_md->m_p_rmtProtl->GetClockErrorWaitTime()*1000.0f);
						mapMeterError.clear();
					}
				}
				else if ((strOrderType == _T("20") || strOrderType == _T("33")) && (p_md->m_strDataValue.Right(3) == _T("A0H") || p_md->m_strDataValue.Right(3) == _T("B1H")))
				{
					bool bRtn = ExecError(p_md, mapMeterError);
					dwNowTick = ::GetTickCount();
					if (!bRtn&&dwNowTick < dwEndTick)
						p_md->m_nFlagUpdateRecvData = FLAG_GOTORECV;
					else
					{
						for (int i = 0; i < g_nTotalPort; ++i)
						{
							if (g_vec_bIsSelect[i])
							{
								if (mapMeterError.find(i) == mapMeterError.end())
								{
									m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, _T("串口接收超时!"));
									m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
								}
							}
						}
					}
				}
				else
				{
					bool bResult = ResultCompareRMT(p_md->m_strDataValue, m_vec2_strCurPlan[p_md->m_nRow][4]);
					if (bResult)
					{
						for (int i = 0; i < g_nTotalPort; ++i)
						{
							if (g_vec_bIsSelect[i])
							{
								m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, p_md->m_strDataValue);
								m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crRight);
							}
						}
					}
					else
					{
						for (int i = 0; i < g_nTotalPort; ++i)
						{
							if (g_vec_bIsSelect[i])
							{
								m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, p_md->m_strDataValue);
								m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
							}
						}
					}
				}
			}
			break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText = _T("串口未打开!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
			{
				CString strOrderType = m_vec2_strCurPlan[p_md->m_nRow][1];
				dwNowTick = ::GetTickCount();
				if ((strOrderType == _T("20") || strOrderType == _T("33")) && dwNowTick < dwEndTick)
				{
					p_md->m_nFlagUpdateRecvData = FLAG_GOTORECV;
				}
				else if ((strOrderType == _T("20") || strOrderType == _T("33")) && dwNowTick >= dwEndTick)
				{
					for (int i = 0; i < g_nTotalPort; ++i)
					{
						if (g_vec_bIsSelect[i])
						{
							if (mapMeterError.find(i) == mapMeterError.end())
							{
								m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, _T("串口接收超时!"));
								m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
							}
						}
					}
				}
				else
				{
					for (int i = 0; i < g_nTotalPort; ++i)
					{
						if (g_vec_bIsSelect[i])
						{
							m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, _T("串口接收超时!"));
							m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
						}
					}
				}
			}
			break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText = _T("接收已停止!");
				break;
			default:
				strText = _T("串口接收失败!");
			}
			if (p_md->m_nRtn != 0 && p_md->m_nRtn != 4)
			{
				for (int i = 0; i < g_nTotalPort; ++i)
				{
					if (g_vec_bIsSelect[i])
					{
						m_listTest.SetItemText(p_md->m_nRow, i + nMDIndexOffset, strText);
						m_listTest.SetItemBkColor(p_md->m_nRow, i + nMDIndexOffset, crError);
					}
				}
			}
			m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			CString strText;
			bool bResult = false;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				if (p_md->m_p_dlt698->IsSplitRecv())
					p_md->m_nFlagUpdateRecvData = FLAG_GOTOENCODE;
				CString strCorrectData;
				strCorrectData = ExecTgtOrCorrectData(p_md, m_vec2_strCurPlan[p_md->m_nRow][4]);
				bResult = ResultCompare698(p_md->m_strDataValue, strCorrectData, p_md, strText);
				CString strReqResCode = p_md->m_strDataValue.Right(3);
				strReqResCode = strReqResCode.Left(2);
				if ((strReqResCode == _T("85") || strReqResCode == _T("86") || strReqResCode == _T("90")) && p_md->m_strDataValue.Find(_T("obj=\"40010200\"")) != -1)
					m_p_meterpos->PostMessage(WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_UPDATEMETERADD);
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
			m_listTest.SetItemText(p_md->m_nRow, p_md->m_nColumn, strText);
			if (bResult)
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crRight);
			else
				m_listTest.SetItemBkColor(p_md->m_nRow, p_md->m_nColumn, crError);
			m_listTest.EnsureVisible(p_md->m_nRow, FALSE);
		}
		break;
		}
	}
	break;
	case MSGUSER_ENDONCELOOP:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType / 10)
		{
		case CMainData::TYPE_METER:
		{
			static int nPortAdd = 0;
			if (g_bIsStop)
				nPortAdd = 0;
			else
			{
				switch (p_md->m_nUserDefined)
				{
				case CMainData::UD_ONEBYONE:
				{
					for (int i = p_md->m_nColumn + 1 - nMDIndexOffset; i < g_nTotalPort; ++i)
					{
						if (g_vec_bIsSelect[i])
						{
							SetEvent(g_vec_md[i].m_hEvtThread);
							break;
						}
					}
				}
				break;
				case CMainData::UD_GOTOFIRST:
				{
					p_md->m_nRow = m_nRowFirst - 1;
				}
				break;
				}
				int nRowNext = p_md->m_nRow + 1;
				if (nRowNext < p_md->m_nRowMax)
				{
					int nProtocolType = _ttoi(m_vec2_strCurPlan[nRowNext][6]);
					int nUserDefined = _ttoi(m_vec2_strCurPlan[nRowNext][7]);
					switch (nProtocolType / 10)
					{
					case CMainData::TYPE_METER:
					{
						switch (nUserDefined)
						{
						case CMainData::UD_ONEBYONE:
						{
							ResetEvent(p_md->m_hEvtThread);
							++nPortAdd;
							if (nPortAdd == g_nTotalSelectedPort)
							{
								nPortAdd = 0;
								SetEvent(g_vec_md[g_nFstMeterIndex].m_hEvtThread);
							}
						}
						break;
						case CMainData::UD_ROWTOGETHER:
						{
							ResetEvent(p_md->m_hEvtThread);
							++nPortAdd;
							if (nPortAdd == g_nTotalSelectedPort)
							{
								nPortAdd = 0;
								for (int i = 0; i < g_nTotalPort; ++i)
								{
									if (g_vec_bIsSelect[i])
										SetEvent(g_vec_md[i].m_hEvtThread);
								}
							}
						}
						break;
						}
					}
					break;
					case CMainData::TYPE_METERDEVICE:
					{
						ResetEvent(p_md->m_hEvtThread);
						++nPortAdd;
						if (nPortAdd == g_nTotalSelectedPort)
						{
							nPortAdd = 0;
							g_mdMeterDevice.m_nRow = nRowNext;
							SetEvent(g_mdMeterDevice.m_hEvtThread);
						}
					}
					break;
					}
				}
				else
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
		case CMainData::TYPE_METERDEVICE:
		{
			if (!g_bIsStop)
			{
				switch (p_md->m_nUserDefined)
				{
				case CMainData::UD_GOTOFIRST:
				{
					p_md->m_nRow = m_nRowFirst - 1;
				}
				break;
				}
				int nRowNext = p_md->m_nRow + 1;
				if (nRowNext < p_md->m_nRowMax)
				{
					int nProtocolType = _ttoi(m_vec2_strCurPlan[nRowNext][6]);
					int nUserDefined = _ttoi(m_vec2_strCurPlan[nRowNext][7]);
					switch (nProtocolType / 10)
					{
					case CMainData::TYPE_METER:
					{
						ResetEvent(p_md->m_hEvtThread);
						switch (nUserDefined)
						{
						case CMainData::UD_ONEBYONE:
						{
							for (int i = 0; i < g_nTotalPort; ++i)
							{
								if (g_vec_bIsSelect[i])
									g_vec_md[i].m_nRow = nRowNext;
							}
							SetEvent(g_vec_md[g_nFstMeterIndex].m_hEvtThread);
						}
						break;
						default:
						{
							for (int i = 0; i < g_nTotalPort; ++i)
							{
								if (g_vec_bIsSelect[i])
								{
									g_vec_md[i].m_nRow = nRowNext;
									SetEvent(g_vec_md[i].m_hEvtThread);
								}
							}
						}
						}
					}
					break;
					}
				}
				else
				{
					for (int i = 0; i < sizeof(g_sz_p_wnd) / sizeof(CWnd *); ++i)
					{
						if (g_sz_p_wnd[i])
							g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO, TRUE, MSGUSER_ENABLEBTN);
					}
				}
			}
		}
		break;
		default:
		{
			int nRowNext = p_md->m_nRow + 1;
			if (nRowNext >= p_md->m_nRowMax)
			{
				for (int i = 0; i < sizeof(g_sz_p_wnd) / sizeof(CWnd *); ++i)
				{
					if (g_sz_p_wnd[i])
						g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO, TRUE, MSGUSER_ENABLEBTN);
				}
			}
		}
		break;
		}
	}
	break;
	case MSGUSER_PORTOPEN:
	case MSGUSER_PORTCLOSE:
	{
		InsertList();
	}
	break;
	case MSGUSER_ENABLEBTN:
	{
		EnableBtn(wParam);
	}
	break;
	case MSGUSER_ERROFGETSERVERINFO:
	{
		AfxMessageBox(_T("获取网络方案失败！请检查本地网络状况及远程服务端配置！"));
	}
	break;
	case MSGUSER_UPDATENETPLAN:
	{
		CString * p_strPath = (CString *)wParam;
		HTREEITEM hFirst, hSecond;
		hFirst = m_treePlan.GetChildItem(m_treePlan.GetRootItem());
		while (hFirst)
		{
			if (m_treePlan.GetItemText(hFirst) == _T("网络方案"))
				break;
			hFirst = m_treePlan.GetNextItem(hFirst, TVGN_NEXT);
		}
		CString strPlanTitle;
		strPlanTitle = p_strPath->Mid(p_strPath->ReverseFind(_T('\\')) + 1);
		hSecond = m_treePlan.InsertItem(strPlanTitle, hFirst);
		m_treePlan.SetItemData(hSecond, m_nTreeCount);
		m_vec_strPlanPath.push_back(*p_strPath);
		++m_nTreeCount;
		m_treePlan.Expand(hFirst, TVE_EXPAND);
	}
	break;
	case MSGUSER_ENDUPDATENETPLAN:
	{
		m_btnUpdateNetPlan.EnableWindow(TRUE);
	}
	break;
	case MSGUSER_ALLPRO:
	{
		OnBnClickedButtonAllpro();
	}
	break;
	}
	return 0;
}


void CProduceTesting::InitTree(void)
{
	m_vec_strPlanPath.clear();
	m_nTreeCount=0;
	HTREEITEM hRoot,hFirst;
	hRoot = m_treePlan.InsertItem(_T("方案"));  
	m_treePlan.SetItemData(hRoot,m_nTreeCount); 
	m_vec_strPlanPath.push_back(_T("方案"));
	++m_nTreeCount;
	hFirst = m_treePlan.InsertItem(_T("本地方案"),hRoot); 
	m_treePlan.SetItemData(hFirst,m_nTreeCount); 
	m_vec_strPlanPath.push_back(_T("本地方案"));
	++m_nTreeCount;
	hFirst = m_treePlan.InsertItem(_T("网络方案"),hRoot);
	m_treePlan.SetItemData(hFirst,m_nTreeCount); 
	m_vec_strPlanPath.push_back(_T("网络方案"));
	++m_nTreeCount;
	ExpandTree(m_treePlan,m_treePlan.GetRootItem());
}

void CProduceTesting::ExpandTree(CTreeCtrl & Tree,HTREEITEM hTreeItem)
{
	if(!hTreeItem)
		return;
	if(!Tree.ItemHasChildren(hTreeItem))
		return;
	HTREEITEM hNextItem = Tree.GetChildItem(hTreeItem);
	while (hNextItem != NULL)
	{
		ExpandTree(Tree,hNextItem);
		hNextItem = Tree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	Tree.Expand(hTreeItem,TVE_EXPAND);
}

void CProduceTesting::OnTvnSelchangedTreePlan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hItem=m_treePlan.GetSelectedItem(); 
	if(hItem&&hItem!=m_treePlan.GetRootItem()&&m_treePlan.GetParentItem(hItem)!=m_treePlan.GetRootItem())
		InsertPlan(m_vec_strPlanPath[m_treePlan.GetItemData(hItem)]);
}

void CProduceTesting::InsertPlan(const CString & strPlanPath)
{
	CString strPlanData,strConfigure;
	m_listTest.DeleteAllItems();
	if(!ZUtil::ReadFile2CString(strPlanPath,strPlanData))
	{	
		AfxMessageBox(_T("打开方案失败！"));
		return;
	}
	strPlanData.Remove(_T('\r'));
	int nConfigFlag;
	nConfigFlag=strPlanData.Find(_T('>'));
	if(nConfigFlag<0)
	{
		AfxMessageBox(_T("方案配置格式错误！"));
		return;
	}
	strConfigure=strPlanData.Left(nConfigFlag+1);
	std::vector<CString> vec_strConfig;
	ZUtil::StrSplit(strConfigure,vec_strConfig,_T('|'),strConfigure.Right(1)==_T('|'));
	if(int(vec_strConfig.size())<15)
	{
		AfxMessageBox(_T("方案配置信息不全！"));
		return;
	}
	CMainData::m_nBarCodeLength=_ttoi(vec_strConfig[1]);
	CMainData::m_nMeterAddStart=_ttoi(vec_strConfig[3]);
	CMainData::m_nMeterAddEnd=_ttoi(vec_strConfig[5]);
	CMainData::m_nMeterNOStart=_ttoi(vec_strConfig[7]);
	CMainData::m_nMeterNOEnd=_ttoi(vec_strConfig[9]);
	CMainData::m_nPropertyNOStart=_ttoi(vec_strConfig[11]);
	CMainData::m_nPropertyNOEnd=_ttoi(vec_strConfig[13]);
	SetDlgItemText(IDC_EDIT_CONFIGURE,strConfigure);
	strPlanData=strPlanData.Mid(nConfigFlag+2);
	std::vector<CString> vec_strDatas;
	ZUtil::StrSplit(strPlanData,vec_strDatas,_T('\n'),strPlanData.Right(1)==_T('\n'));
	m_listTest.SetRedraw(FALSE);
	m_vec2_strCurPlan.clear();
	CString strTemp;
	int nSize,nSubSize,i,j;
	nSize=vec_strDatas.size();
	for(i=0,j=0;i<nSize;++i)
	{
		std::vector<CString> vec_strRowData;
		ZUtil::StrSplit(vec_strDatas[i],vec_strRowData,_T(','),vec_strDatas[i].Right(1)==_T(','));
		nSubSize=vec_strRowData.size();
		if(nSubSize!=8)
			continue;
		m_vec2_strCurPlan.push_back(vec_strRowData);
		strTemp.Format(_T("%d"),j+1);
		m_listTest.InsertItem(j,strTemp);
		m_listTest.SetItemText(j,1,vec_strRowData[0]);
		++j;
	}
	m_listTest.SetRedraw(TRUE);
}

void CProduceTesting::SetListBlank(const int & nRowStart, const int & nRowEnd,const int & nColumnStart, const int & nColumnEnd)
{
	for(int i=nRowStart;i<nRowEnd;++i)
	{
		for(int j=nColumnStart;j<nColumnEnd;++j)
		{
			m_listTest.SetItemText(i,j,_T(""));
			m_listTest.SetItemBkColor(i,j,crOri);
		}
	}
}


CString CProduceTesting::HexString2BinString(const CString & strHex)
{
	CString strBit;
	int nSize=strHex.GetLength()*4;
	CString strZero(_T('0'),nSize);
	TCHAR * p_tc=new TCHAR[nSize+1];
	int nValue=_tcstol(strHex,NULL,16);
	_itot_s(nValue,p_tc,nSize+1,2);
	strBit=p_tc;
	delete []p_tc;
	if(strBit.GetLength()<nSize)
		strBit=strZero.Left(nSize-strBit.GetLength())+strBit;
	else
		strBit=strBit.Left(nSize);
	return strBit;
}


CString CProduceTesting::DecString2HexString(const CString & strDec)
{
	CString strZero8(_T('0'), 8);
	CString strHex;
	strHex.Format(_T("%X"), _ttoi(strDec));
	if (strHex.GetLength()<strDec.GetLength())
		strHex = strZero8.Left(strDec.GetLength() - strHex.GetLength()) + strHex;
	else
		strHex = strHex.Right(strDec.GetLength());
	return strHex;
}


CString CProduceTesting::CharString2HexString(const CString & strChar)
{
	CString strHex,strByte;
	for (int i = 0; i<strChar.GetLength(); ++i)
	{
		strByte.Format(_T("%02X"), strChar[i]);
		strHex += strByte;
	}
	return strHex;
}


bool CProduceTesting::ResultCompare(CString strResultData, const CString & strCorrectData)
{
	int nLineIndex = strCorrectData.Find(_T('_'));
	if (nLineIndex != -1)
	{
		double dValue, dCorrectValueDown, dCorrectValueUp;
		dValue = _ttof(strResultData);
		dCorrectValueDown = _ttof(strCorrectData.Left(nLineIndex));
		dCorrectValueUp = _ttof(strCorrectData.Mid(nLineIndex + 1));
		if (dValue >= dCorrectValueDown && dValue <= dCorrectValueUp)
			return true;
		else
			return false;
	}
	else
	{
		if (strCorrectData.Find(_T("β"))!=-1)
			strResultData = HexString2BinString(strResultData);
		int nSize = strCorrectData.GetLength()<strResultData.GetLength() ? strCorrectData.GetLength() : strResultData.GetLength();
		for (int i = 0; i<nSize; ++i)
		{
			if (strCorrectData[i] == _T('ω') || strCorrectData[i] == _T('β'))
				continue;
			if (strCorrectData[i] != strResultData[i])
				return false;
		}
		return true;
	}
}

bool CProduceTesting::ResultCompare645(const CString & strResult, const CString & strCorrectData)
{
	bool bRtn=false;
	CString strExt;
	strExt=strResult.Right(3);
	if(strExt.Left(1)!=_T("D")&&strExt.Left(1)!=_T("C")&&strExt.Left(1)!=_T("F"))
	{
		if(strExt.Left(2)==_T("91")|| strExt.Left(2) == _T("B1"))
		{
			CString strResultData =strResult.Left(strResult.Find(_T("@")));
			bRtn = ResultCompare(strResultData, strCorrectData);
		}
		else if(strExt.Left(2)==_T("83"))
		{
			CString strDataItem=strResult.Mid(strResult.Find(_T("@"))+1,8);
			if(strDataItem==_T("078001FF"))
			{
				CString strResultData =strResult.Left(strResult.Find(_T("@")));
				bRtn = ResultCompare(strResultData, strCorrectData);
			}
			else
				bRtn=true;
		}
		else
			bRtn=true;
	}
	return bRtn;
}

bool CProduceTesting::ResultCompareRMT(const CString & strResult, const CString & strCorrectData)
{
	bool bRtn=false;
	CString strExt;
	strExt=strResult.Right(3);
	if(strExt!=_T("88H"))
	{
		if(strExt.Left(2)==_T("FF"))
		{
			CString strResultData =strResult.Left(strResult.Find(_T("@")));
			bRtn = ResultCompare(strResultData, strCorrectData);
		}
		else
			bRtn=true;
	}
	return bRtn;
}

bool CProduceTesting::ResultCompare698(const CString & strResult, const CString & strCorrectData, CMainData * p_md, CString & strText)
{
	bool bRtn = true;
	std::vector<CString> vec_strObj, vec_strTValue, vec_strDValue;
	std::vector<CString> vec_strCorrectData;
	CString strXml, strReqResCode,strResultData;
	strXml = strResult;
pro90:
	strReqResCode = strXml.Right(3);
	strReqResCode = strReqResCode.Left(2);
	strXml = strXml.Left(strXml.GetLength() - 4);
	if (strReqResCode == _T("EE"))
	{
		strText = strResult;
		bRtn = false;
	}
	else if (strReqResCode == _T("85")|| strReqResCode == _T("86")|| strReqResCode == _T("87"))
	{
		p_md->m_p_dlt698->GetXmlObjValue(strXml, vec_strObj, vec_strTValue, vec_strDValue);
		ZUtil::StrSplit(strCorrectData, vec_strCorrectData, _T('#'), strCorrectData.Right(1) == _T('#'));
		int nSize = vec_strObj.size() > vec_strCorrectData.size() ? vec_strCorrectData.size() : vec_strObj.size();
		for (int i = 0; i < (int)vec_strObj.size(); ++i)
		{
			if (strReqResCode == _T("85"))
				strResultData = vec_strTValue[i];
			else if(strReqResCode == _T("86"))
				strResultData = vec_strDValue[i];
			else
			{
				if(vec_strTValue[i].IsEmpty())
					strResultData = vec_strDValue[i];
				else
					strResultData = vec_strTValue[i];
			}
			if (i < nSize)
			{
				if (ResultCompare(strResultData, vec_strCorrectData[i]))
					strText += strResultData + _T("#");
				else
				{
					strText += _T("E:") + strResultData + _T("#");
					bRtn = false;
				}
			}
			else
			{
				strText += strResultData + _T("#");
			}
		}
		strText.Delete(strText.GetLength() - 1, 1);
	}
	else if (strReqResCode == _T("90"))
	{
		CString strExt = strXml.Right(4);
		if (strExt.Left(1) != _T("-") || strExt.Right(1) != _T("H"))
		{
			CString strTValue, strDValue;
			p_md->m_p_dlt698->GetXmlValue(strXml, strTValue, strDValue);
			strText += strDValue;
			strText.Delete(strText.GetLength() - 1, 1);
			bRtn = false;
		}
		else
			goto pro90;
	}
	else
	{
		strText = strResult;
		strResultData = strResult.Left(strResult.ReverseFind(_T('-')));
		bRtn = ResultCompare(strResultData, strCorrectData);
	}
	return bRtn;
}

void CProduceTesting::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_bIsInitFinish&&nType!=SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		ChangeSize(IDCANCEL,rc); 
		ChangeSize(IDOK,rc); 
		ChangeSize(IDC_BUTTON_ZOOMIN,rc); 
		ChangeSize(IDC_BUTTON_ZOOMNORMAL,rc); 
		ChangeSize(IDC_BUTTON_ZOOMOUT,rc); 
		ChangeSize(IDC_BUTTON_OPERATORTREE,rc); 
		ChangeSize(IDC_LIST_TESTING,rc); 
		ChangeSize(IDC_EDIT_CONFIGURE,rc); 
		ChangeSize(IDC_BUTTON_STOPPRO,rc); 
		ChangeSize(IDC_BUTTON_ALLPRO,rc); 
		ChangeSize(IDC_BUTTON_CONTINUEPRO,rc); 
		ChangeSize(IDC_BUTTON_SINGLEPRO,rc); 
		ChangeSize(IDC_TREE_PLAN,rc); 
		ZoomList(m_dScale);
		InitButton();
		Invalidate();
	}
}

void CProduceTesting::ChangeSize(UINT nID,const CRect & rcParent)
{
	CWnd *pWnd=GetDlgItem(nID); 
	if(pWnd!=NULL) 
	{
		static CRect rcLast;
		CRect rc; 
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		int nHeight=rc.Height();
		int nWidth=rc.Width();
		switch(nID)
		{
		case IDCANCEL:
			{
				rc.right=rcParent.right-10;
				rc.left=rc.right-nWidth;
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDOK:
			{
				rc.right=rcLast.left-10;
				rc.left=rc.right-nWidth;
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_BUTTON_ZOOMIN:
		case IDC_BUTTON_ZOOMNORMAL:
		case IDC_BUTTON_ZOOMOUT:
		case IDC_BUTTON_OPERATORTREE:
			{
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_LIST_TESTING:
			{
				rc.right=rcParent.right-10;
				CRect rcZoomOut;
				GetDlgItem(IDC_BUTTON_ZOOMOUT)->GetWindowRect(&rcZoomOut);
				ScreenToClient(&rcZoomOut);
				rc.bottom=rcZoomOut.top-10;
				if(m_bIsShowTree)
					rc.left=rcZoomOut.left;
				else
					rc.left=rcParent.left+10;
			}
			break;
		case IDC_EDIT_CONFIGURE:
			{
				rc.right=rcParent.right-10;
				if(m_bIsShowTree)
					rc.left=rcLast.left;
				else
					rc.left=rcParent.left+10;
			}
			break;
		case IDC_BUTTON_STOPPRO:
			{
				rc.right=rcParent.right-10;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_BUTTON_ALLPRO:
		case IDC_BUTTON_CONTINUEPRO:
		case IDC_BUTTON_SINGLEPRO:
			{
				rc.right=rcLast.left-10;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_TREE_PLAN:
			{
				CRect rcList;
				GetDlgItem(IDC_LIST_TESTING)->GetWindowRect(&rcList);
				ScreenToClient(&rcList);
				rc.bottom=rcList.bottom;
			}
			break;
		}
		rcLast=rc;
		pWnd->MoveWindow(rc,FALSE);
	}
}

void CProduceTesting::CheckFirstRow(const int & nRowFirst)
{
	m_nRowFirst = nRowFirst;
	for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
	{
		if(g_sz_p_wnd[i])
			g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,FALSE,MSGUSER_ENABLEBTN);
	}
	int nProtocolType,nUserDefined;
	nProtocolType=_ttoi(m_vec2_strCurPlan[nRowFirst][6]);
	nUserDefined=_ttoi(m_vec2_strCurPlan[nRowFirst][7]);
	switch(nProtocolType/10)
	{
	case CMainData::TYPE_METER:
		{
			switch(nUserDefined)
			{
			case CMainData::UD_ONEBYONE:
				{
					SetEvent(g_vec_md[g_nFstMeterIndex].m_hEvtThread);
				}
				break;
			default:
				{
					for(int i=0;i<g_nTotalPort;++i)
					{
						if(g_vec_bIsSelect[i])
							SetEvent(g_vec_md[i].m_hEvtThread);
					}
				}
			}
		}
		break;
	case CMainData::TYPE_METERDEVICE:
		{
			SetEvent(g_mdMeterDevice.m_hEvtThread);
		}
		break;
	}
	g_bIsStop=false;
}

bool CProduceTesting::ExecError(CMainData * p_md,std::map<int,CString> & mapMeterError)
{
	std::vector<CString> vec_strData;
	CString strValue(p_md->m_strDataValue);
	strValue=strValue.Left(strValue.Find(_T('@')));
	ZUtil::StrSplit(strValue,vec_strData,_T('#'),strValue.Right(1)==_T('#'));
	CString strCorrectData=m_vec2_strCurPlan[p_md->m_nRow][4];
	int nLineIndex=strCorrectData.Find(_T('_'));
	double dValue,dCorrectValueDown,dCorrectValueUp;
	if(nLineIndex!=-1)
	{
		dCorrectValueDown=_ttof(strCorrectData.Left(nLineIndex));
		dCorrectValueUp=_ttof(strCorrectData.Mid(nLineIndex+1));
	}
	int nSize=_ttoi(vec_strData[0]);
	int nIndex;
	CString strMeterError;
	for(int i=0;i<nSize;++i)
	{
		nIndex=_ttoi(vec_strData[2*i+1]);
		strMeterError=vec_strData[2*i+2];
		if(nIndex>=(int)g_vec_bIsSelect.size()||!g_vec_bIsSelect[nIndex])
			continue;
		if(mapMeterError.find(nIndex)!=mapMeterError.end())
			mapMeterError[nIndex]=strMeterError;
		else
			mapMeterError.insert(std::pair<int,CString>(nIndex,strMeterError));
		m_listTest.SetItemText(p_md->m_nRow,nIndex+nMDIndexOffset,strMeterError);
		if(nLineIndex!=-1)
		{
			dValue=_ttof(strMeterError);
			if(dValue>=dCorrectValueDown&&dValue<=dCorrectValueUp)
				m_listTest.SetItemBkColor(p_md->m_nRow,nIndex+nMDIndexOffset,crRight);
			else
				m_listTest.SetItemBkColor(p_md->m_nRow,nIndex+nMDIndexOffset,crError);
		}
		else
		{
			if(strMeterError==strCorrectData)
				m_listTest.SetItemBkColor(p_md->m_nRow,nIndex+nMDIndexOffset,crRight);
			else
				m_listTest.SetItemBkColor(p_md->m_nRow,nIndex+nMDIndexOffset,crError);
		}
	}
	if(int(mapMeterError.size())==g_nTotalSelectedPort)
		return true;
	else
		return false;
}

void CProduceTesting::EnableBtn(const BOOL & bEnable)
{
	GetDlgItem(IDC_BUTTON_SINGLEPRO)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_CONTINUEPRO)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_ALLPRO)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_IMPORTPLAN)->EnableWindow(bEnable);
	GetDlgItem(IDC_TREE_PLAN)->EnableWindow(bEnable);
	CMenu* pPopup = menuProTest.GetSubMenu(0);
	if (pPopup)
	{
		if (bEnable)
		{
			pPopup->EnableMenuItem(IDC_BUTTON_SINGLEPRO, MF_BYCOMMAND | MF_ENABLED);
			pPopup->EnableMenuItem(IDC_BUTTON_CONTINUEPRO, MF_BYCOMMAND | MF_ENABLED);
			pPopup->EnableMenuItem(IDC_BUTTON_ALLPRO, MF_BYCOMMAND | MF_ENABLED);
			pPopup->EnableMenuItem(IDC_BUTTON_IMPORTPLAN, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(IDC_BUTTON_SINGLEPRO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pPopup->EnableMenuItem(IDC_BUTTON_CONTINUEPRO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pPopup->EnableMenuItem(IDC_BUTTON_ALLPRO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pPopup->EnableMenuItem(IDC_BUTTON_IMPORTPLAN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}
}


CString CProduceTesting::ExecTgtOrCorrectData(CMainData * p_md, const CString & strData)
{
	CString strRtnData(strData);
	CString strReplace;
	strReplace = p_md->m_strTgtMeterAddr;
	ReplaceSubString(strRtnData, _T("{METERADDR}"), strReplace);
	LimitLength(strReplace,12,false);
	ReplaceSubString(strRtnData, _T("{HMETERADDR}"), strReplace);
	strReplace = p_md->m_strTgtMeterNO;
	ReplaceSubString(strRtnData, _T("{METERNO}"), strReplace);
	LimitLength(strReplace, 12, false);
	ReplaceSubString(strRtnData, _T("{HMETERNO}"), strReplace);
	strReplace = p_md->m_strTgtPropertyNO;
	ReplaceSubString(strRtnData, _T("{PROPERTYNO}"), strReplace);
	strReplace = CharString2HexString(strReplace);
	LimitLength(strReplace, 64, true);
	ReplaceSubString(strRtnData, _T("{HPROPERTYNO}"), strReplace);
	CString strYYYY, strYY, strMM, strDD, strWW, strhhmmss, strmils, strTime, strTimeHex;
	SYSTEMTIME stTime;
	::GetLocalTime(&stTime);
	strYYYY.Format(_T("%04d"), stTime.wYear);
	strYY = strYYYY.Right(2);
	strMM.Format(_T("%02d"), stTime.wMonth);
	strDD.Format(_T("%02d"), stTime.wDay);
	strWW.Format(_T("%02d"), stTime.wDayOfWeek);
	strhhmmss.Format(_T("%02d%02d%02d"), stTime.wHour, stTime.wMinute, stTime.wSecond);
	strmils.Format(_T("%04d"), stTime.wMilliseconds);
	strTime = strYYYY + strMM + strDD + strWW + strhhmmss + strmils;
	ReplaceSubString(strRtnData, _T("{YYMMDDWW}"), strYY + strMM + strDD + strWW);//04000101
	ReplaceSubString(strRtnData, _T("{hhmmss}"), strhhmmss);//04000102 or time
	ReplaceSubString(strRtnData, _T("{YYMMDDWWhhmmss}"), strYY + strMM + strDD + strWW + strhhmmss);//0400010C
	ReplaceSubString(strRtnData, _T("{YYMMDDhhmmss}"), strYY + strMM + strDD + strhhmmss);//广播校时
	ReplaceSubString(strRtnData, _T("{YYYYMMDD}"), strYYYY + strMM + strDD);//生产日期
	ReplaceSubString(strRtnData, _T("{YYYY-MM-DD}"), strYYYY + _T("-") + strMM + _T("-") + strDD);//生产日期
	ReplaceSubString(strRtnData, _T("{YYMMDD}"), strYY + strMM + strDD);//生产日期
	ReplaceSubString(strRtnData, _T("{YY-MM-DD}"), strYY + _T("-") + strMM + _T("-") + strDD);//生产日期
	ReplaceSubString(strRtnData, _T("{YYYYMMDDWWhhmmssmils}"), strTime);//date_time
	ReplaceSubString(strRtnData, _T("{YYYYMMDDhhmmss}"), strYYYY + strMM + strDD + strhhmmss);//date_time_s
	ReplaceSubString(strRtnData, _T("{YYYYMMDDWW}"), strYYYY + strMM + strDD + strWW);//date
	strTimeHex = DecString2HexString(strYYYY);
	for (int i = 0; i < 6; ++i)
		strTimeHex += DecString2HexString(strTime.Mid(2 * i + 4, 2));
	strTimeHex += DecString2HexString(strmils);
	ReplaceSubString(strRtnData, _T("{HYYYYMMDDWWhhmmssmils}"), strTimeHex);//date_time hex
	ReplaceSubString(strRtnData, _T("{HYYYYMMDDhhmmss}"), strTimeHex.Mid(0, 8) + strTimeHex.Mid(10, 6));//date_time_s hex
	ReplaceSubString(strRtnData, _T("{HYYYYMMDDWW}"), strTimeHex.Mid(0, 10));//date hex
	ReplaceSubString(strRtnData, _T("{Hhhmmss}"), strTimeHex.Mid(10, 6));//time hex
	if (strData.Find(_T("hhmmss"))!=-1&& strRtnData.Find(_T("_")) != -1)
	{
		std::vector<CString> vec_strData1;
		ZUtil::StrSplit(strRtnData, vec_strData1, _T('_'), strRtnData.Right(1) == _T('_'));
		for (int i = 0; i<2; ++i)
		{
			std::vector<CString> vec_strData2;
			double dData = 0;
			if (vec_strData1[i].Find(_T("+")) != -1)
			{
				ZUtil::StrSplit(vec_strData1[i], vec_strData2, _T('+'), vec_strData1[i].Right(1) == _T('+'));
				CTime curTime(stTime);
				CTimeSpan timeSpan(0, 0, 0, _ttoi(vec_strData2[1]));
				vec_strData1[i] = (curTime + timeSpan).Format(_T("%Y%m%d")) + _T("0") + (curTime + timeSpan).Format(_T("%w%H%M%S"));
				vec_strData1[i] = vec_strData1[i].Right(vec_strData2[0].GetLength());
			}
			if (vec_strData1[i].Find(_T("-")) != -1)
			{
				ZUtil::StrSplit(vec_strData1[i], vec_strData2, _T('-'), vec_strData1[i].Right(1) == _T('-'));
				CTime curTime(stTime);
				CTimeSpan timeSpan(0, 0, 0, _ttoi(vec_strData2[1]));
				vec_strData1[i] = (curTime - timeSpan).Format(_T("%Y%m%d")) + _T("0") + (curTime - timeSpan).Format(_T("%w%H%M%S"));
				vec_strData1[i] = vec_strData1[i].Right(vec_strData2[0].GetLength());
			}
		}
		strRtnData = vec_strData1[0] + _T('_') + vec_strData1[1];
	}
	if (strRtnData.Find(_T("|")) != -1)
	{
		std::vector<CString> vec_strData;
		ZUtil::StrSplit(strRtnData, vec_strData, _T('|'), strRtnData.Right(1) == _T('|'));
		CString strTemp;
		int nSize = vec_strData.size();
		for (int i = 0; i<nSize; ++i)
		{
			if (i % 2 == 0)
				strTemp += vec_strData[i];
			else
				strTemp += CharString2HexString(vec_strData[i]);
		}
		strRtnData = strTemp;
	}
	return strRtnData;
}


void CProduceTesting::ReplaceSubString(CString & strDest, const CString & strSub, const CString & strReplace)
{
	if (strDest.Find(strSub) != -1)
		strDest.Replace(strSub, strReplace);
}


void CProduceTesting::LimitLength(CString & strData, int nLen, bool bIsZeroBack)
{
	CString strZero64(_T('0'), 64);
	if (strData.GetLength() < nLen)
	{
		if (bIsZeroBack)
			strData = strData + strZero64.Left(nLen - strData.GetLength());
		else
			strData = strZero64.Left(nLen - strData.GetLength()) + strData;
	}
	else
		strData = strData.Right(nLen);
}


void CProduceTesting::ZoomList(const double & dScale)
{
	CRect rc;
	m_listTest.GetClientRect(&rc);
	int nWidth=rc.Width()-40-100-10;
	if(nWidth<0)
		nWidth=0;
	if(g_nTotalPort)
		nWidth/=g_nTotalPort;
	nWidth=int(nWidth*dScale);
	m_listTest.SetRedraw(FALSE);
	for(int i=0;i<g_nTotalPort;++i)
	{
		m_listTest.SetColumnWidth(i+nMDIndexOffset,nWidth);
	}
	m_listTest.SetRedraw(TRUE);
}


BOOL CProduceTesting::OnEraseBkgnd(CDC* pDC)
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

void CProduceTesting::InitButton(void)
{
	SetBtnColor(m_btnImportPlan,_T(""));
	SetBtnColor(m_btnUpdateNetPlan, _T(""));
	SetBtnColor(m_btnMeterPos,_T(""));
	SetBtnColor(m_btnSinglePro,_T(""));
	SetBtnColor(m_btnContinuePro,_T(""));
	SetBtnColor(m_btnAllPro,_T(""));
	SetBtnColor(m_btnStopPro,_T(""));
	SetBtnColor(m_btnOperatorTree,_T(""));
	SetBtnColor(m_btnZoomOut,_T(""));
	SetBtnColor(m_btnZoomNormal,_T(""));
	SetBtnColor(m_btnZoomIn,_T(""));
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
	m_btnZoomOut.SetFlat(TRUE);
	m_btnZoomNormal.SetFlat(TRUE);
	m_btnZoomIn.SetFlat(TRUE);
	m_btnZoomOut.DrawBorder(TRUE);
	m_btnZoomNormal.DrawBorder(TRUE);
	m_btnZoomIn.DrawBorder(TRUE);
}

void CProduceTesting::SetBtnColor(ZButton & btn,const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN,RGB(230,248,226));
	btn.SetColor(ZButton::COLOR_BK_OUT,RGB(230,248,226));
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

void CProduceTesting::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CMenu* pPopup = menuProTest.GetSubMenu(0);
	if (pPopup)
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

UINT CProduceTesting::UpdatePlanThreadFunc(LPVOID lpParam)
{
	CProduceTesting * p_protest=(CProduceTesting *)lpParam;
	ResetEvent(p_protest->m_hEvtExitUpdatePlan);
	CString strDataIn,strDataOut,strHead,strPath;
	strHead.Format(_T("[%06d03]"), strDataIn.GetLength() + 10);
	strDataIn = strHead + strDataIn;
	if(!SocketCommunication(strDataIn,strDataOut))
	{
		if(p_protest->m_hWnd)
			p_protest->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ERROFGETSERVERINFO); 
	}
	else
	{
		if(strDataOut.Mid(0,1)==_T("[")&&strDataOut.Mid(9,1)==_T("]")&&_ttoi(strDataOut.Mid(1,6))==strDataOut.GetLength()&&strDataOut.Mid(7,2)==_T("03"))
		{
			::CreateDirectory(ZUtil::GetExeCatalogPath()+_T("\\plan"), NULL);
			::CreateDirectory(ZUtil::GetExeCatalogPath()+_T("\\plan\\network"), NULL);
			strDataOut.Delete(0,10);
			std::vector<CString> vec_strDatas;
			ZUtil::StrSplit(strDataOut,vec_strDatas,_T('*'),strDataOut.Right(1)==_T('*'));
			int nSize=vec_strDatas.size();
			for(int i=0;i<nSize;++i)
			{
				if(!p_protest->m_bIsUpdatePlanRun)
					break;
				strDataIn=vec_strDatas[i];
				strHead.Format(_T("[%06d04]"),strDataIn.GetLength()+10);
				strDataIn=strHead+strDataIn;
				int nRtn=0;
				ZFileSocket sock;
				CGlobalVariable gvariable;
				SendMessageTimeout(g_sz_p_wnd[WND_METERCOMM]->m_hWnd,WM_MSGRECVPRO,(WPARAM)&gvariable,MSGUSER_GETSERVERINFO,SMTO_BLOCK,500,NULL); 
				CString & strIP=gvariable.g_strServerIP;
				CString & strPort=gvariable.g_strServerPort;
				CString & strTimeOut=gvariable.g_strServerTimeOut;
				nRtn=sock.Connect(strIP,strPort);
				if(nRtn)
					continue;
				nRtn=sock.StringSend(strDataIn);
				if(nRtn)
					continue;
				strPath=ZUtil::GetExeCatalogPath()+_T("\\plan\\network\\")+strDataIn.Mid(10);
				sock.SetFilePath(strPath);
				ULONGLONG llLength=0;
				CString strLength,strFinish, strCurTimes;
				nRtn=sock.StringRecv(strLength);
				if(nRtn)
					goto end;
				llLength=_ttoi64(strLength);
				sock.SetFileLength(llLength);
				strCurTimes.Format(_T("%lld"), sock.GetCurTimes());
				nRtn=sock.StringSend(strCurTimes);
				if(nRtn)
					goto end;
				nRtn=sock.FileRecv();
				if(nRtn)
					goto end;
				SendMessageTimeout(p_protest->m_hWnd,WM_MSGRECVPRO,(WPARAM)&strPath,MSGUSER_UPDATENETPLAN,SMTO_BLOCK,500,NULL); 
end:
				sock.StringRecv(strFinish);
			}
		}
		else
		{
			if(p_protest->m_hWnd)
				p_protest->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ERROFGETSERVERINFO); 
		}
	}
	if (p_protest->m_hWnd)
		p_protest->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_ENDUPDATENETPLAN);
	SetEvent(p_protest->m_hEvtExitUpdatePlan);
	return 0;
}

bool CProduceTesting::SocketCommunication(const CString & strDataIn,CString & strDataOut)
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

