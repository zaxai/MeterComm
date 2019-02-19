// CCardTesting.cpp: 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "CCardTesting.h"
#include "afxdialogex.h"


// CCardTesting 对话框

IMPLEMENT_DYNAMIC(CCardTesting, CDialogEx)

CCardTesting::CCardTesting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CARDTESTING, pParent)
	, m_nCardType(0)
{

}

CCardTesting::~CCardTesting()
{
	for (ZListCtrl * p_list : m_vec_listItem)
	{
		if (p_list != NULL)
		{
			delete p_list;
			p_list = NULL;
		}
	}
}

void CCardTesting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CARDTYPE, m_listCardType);
	DDX_Control(pDX, IDC_TAB_CARDCONTENT, m_tabCardContent);
}


BEGIN_MESSAGE_MAP(CCardTesting, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_CARDTYPE, &CCardTesting::OnLbnSelchangeListCardtype)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CARDCONTENT, &CCardTesting::OnTcnSelchangeTabCardcontent)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_READ, &CCardTesting::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CCardTesting::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCardTesting::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CCardTesting 消息处理程序


BOOL CCardTesting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitListBox();
	InitTab();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCardTesting::InitListBox()
{
	// TODO: 在此处添加实现代码.
	m_listCardType.AddString(_T("参数预置卡"));
	m_listCardType.AddString(_T("费控转换卡"));
	m_listCardType.AddString(_T("检测卡"));
	m_listCardType.AddString(_T("增加金额卡"));
	m_listCardType.AddString(_T("修改表号卡"));
	m_listCardType.SetCurSel(m_nCardType);
}


void CCardTesting::InitTab()
{
	// TODO: 在此处添加实现代码.
	InsertTab(m_nCardType);
}


void CCardTesting::InsertTab(int nCardType)
{
	if (m_tabCardContent.DeleteAllItems())
	{
		for (CListCtrl * p_list : m_vec_listItem)
		{
			if (p_list != NULL)
			{
				delete p_list;
				p_list = NULL;
			}
		}
		m_vec_listItem.clear();
		std::vector<CCard> vec_cardhead;
		std::vector<CCard> vec_carditem;
		std::vector<CCard> vec_cardcontent;
		CString strSql;
		strSql.Format(_T("SELECT * FROM CardHead WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 1"), g_v.g_nProtocolType,g_v.g_nDllType, nCardType);
		g_v.g_chdb.Select(strSql, vec_cardhead);
		if (vec_cardhead.size())
		{
			int nFileNum = vec_cardhead[0].GetFileNum();
			for (int i = 0; i < nFileNum; ++i)
			{
				m_tabCardContent.InsertItem(i, (vec_cardhead[0].GetFile())[i]);
				ZListCtrl * p_list = NULL;
				CRect rc;
				m_tabCardContent.GetClientRect(&rc);
				rc.left += 2;
				rc.right -= 2;
				rc.top += 23;
				rc.bottom -= 2;
				p_list = new ZListCtrl();
				p_list->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | WS_TABSTOP, rc, &m_tabCardContent, IDC_LIST_START + i);
				p_list->SetExtendedStyle(p_list->GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
				p_list->SetTextDefaultColor(RGB(192, 64, 0));
				p_list->EnableEdit(true);
				p_list->InsertColumn(0, _T("数据项"), LVCFMT_LEFT, rc.Width() * 4 / 9, 0);
				p_list->InsertColumn(1, _T("内容"), LVCFMT_LEFT, rc.Width() * 4 / 9, 1);
				strSql.Format(_T("SELECT * FROM CardItem WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 1"), g_v.g_nProtocolType, g_v.g_nDllType, nCardType);
				g_v.g_cidb.Select(strSql, vec_carditem);
				strSql.Format(_T("SELECT * FROM CardContent WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 1"), g_v.g_nProtocolType, g_v.g_nDllType, nCardType);
				g_v.g_ccdb.Select(strSql, vec_cardcontent);
				if (vec_carditem.size()&& vec_cardcontent.size())
				{
					CString strItem = (vec_carditem[0].GetFile())[i];
					CString strContent = (vec_cardcontent[0].GetFile())[i];
					std::vector<CString> vec_strItem,vec_strContent;
					ZUtil::StrSplit(strItem, vec_strItem,_T('#'),strItem.Right(1)==_T('#'));
					ZUtil::StrSplit(strContent, vec_strContent, _T('#'), strContent.Right(1) == _T('#'));
					int nSize = vec_strItem.size();
					for (int j=0;j<nSize;++j)
					{
						p_list->InsertItem(j,vec_strItem[j]);
						p_list->SetItemText(j,1,vec_strContent[j]);
					}
				}
				m_vec_listItem.push_back(p_list);
			}
			for (ZListCtrl * p_list : m_vec_listItem)
				p_list->ShowWindow(SW_HIDE);
			if(nFileNum)
				m_vec_listItem[0]->ShowWindow(SW_SHOW);
		}
	}
}


void CCardTesting::OnLbnSelchangeListCardtype()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel=m_listCardType.GetCurSel();
	if (nSel != LB_ERR)
	{
		m_nCardType = nSel;
		InsertTab(m_nCardType);
	}
}


void CCardTesting::OnTcnSelchangeTabCardcontent(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nSel = m_tabCardContent.GetCurSel();
	if (nSel != -1)
	{
		for (CListCtrl * p_list : m_vec_listItem)
			p_list->ShowWindow(SW_HIDE);
		m_vec_listItem[nSel]->ShowWindow(SW_SHOW);
	}
}


BOOL CCardTesting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_BKGND);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	CRect rc;
	GetClientRect(&rc);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CCardTesting::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (g_v.g_nProtocolType)
	{
		case CMainData::PROTOCOL_DLT645:
		{
			switch (g_v.g_nDllType)
			{
			case 5:
			{
				CString strCardNum, strAppBina, strMoney, strTimeZoneParam;
				ZDLT645_2007::ReadParamPresetCard18SGC(strCardNum, strAppBina, strMoney, strTimeZoneParam);
			}
			break;
			}
		}
		break;
	}
}


void CCardTesting::OnBnClickedButtonWrite()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCardTesting::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<CCard> vec_cardcontent;
	CString strSql;
	strSql.Format(_T("SELECT * FROM CardContent WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 1"), g_v.g_nProtocolType, g_v.g_nDllType, m_nCardType);
	g_v.g_ccdb.Select(strSql, vec_cardcontent);
	if (vec_cardcontent.size())
	{
		CString strItem;
		CString * p_strFile= vec_cardcontent[0].GetFileForSet();
		int nSize = m_vec_listItem.size();
		for (int i=0;i< nSize;++i)
		{
			int nRow = m_vec_listItem[i]->GetItemCount();
			for (int j = 0; j < nRow; ++j)
			{
				strItem += m_vec_listItem[i]->GetItemText(j, 1);
				strItem += _T('#');
			}
			p_strFile[i] = strItem;
			strItem.Empty();
		}
		if (g_v.g_ccdb.Update(vec_cardcontent[0]))
		{
			AfxMessageBox(_T("保存成功!"));
			return;
		}
	}
	AfxMessageBox(_T("保存失败!"));
}
