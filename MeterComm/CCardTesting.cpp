// CCardTesting.cpp: 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "CCardTesting.h"
#include "afxdialogex.h"

const TCHAR tcSplitChar = _T('#');
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
	DDX_Control(pDX, IDC_BUTTON_READ, m_btnRead);
	DDX_Control(pDX, IDC_BUTTON_WRITE, m_btnWrite);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
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
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCardTesting::InitListBox()
{
	// TODO: 在此处添加实现代码.
	m_listCardType.AddString(_T("参数预置卡"));
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
				strSql.Format(_T("SELECT * FROM CardItem WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 3"), g_v.g_nProtocolType, g_v.g_nDllType, nCardType);
				g_v.g_cidb.Select(strSql, vec_carditem);
				strSql.Format(_T("SELECT * FROM CardContent WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 1"), g_v.g_nProtocolType, g_v.g_nDllType, nCardType);
				g_v.g_ccdb.Select(strSql, vec_cardcontent);
				if (vec_carditem.size()==3&& vec_cardcontent.size())
				{
					CString strItem = (vec_carditem[0].GetFile())[i];
					CString strContent = (vec_cardcontent[0].GetFile())[i];
					std::vector<CString> vec_strItem,vec_strContent;
					ZUtil::StrSplit(strItem, vec_strItem,tcSplitChar,strItem.Right(1)==tcSplitChar);
					ZUtil::StrSplit(strContent, vec_strContent, tcSplitChar, strContent.Right(1) == tcSplitChar);
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


void CCardTesting::InitButton(void)
{
	SetBtnColor(m_btnRead, _T(""));
	SetBtnColor(m_btnWrite, _T(""));
	SetBtnColor(m_btnSave, _T(""));
	SetBtnColor(m_btnOK, _T(""));
	SetBtnColor(m_btnCancel, _T(""));
}


void CCardTesting::SetBtnColor(ZButton & btn, const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}


BOOL CCardTesting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		UINT nKey = pMsg->wParam;
		if (nKey == VK_RETURN || nKey == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
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
	SetListBlank();
	switch (g_v.g_nProtocolType)
	{
		case CMainData::PROTOCOL_DLT645:
		{
			switch (g_v.g_nDllType)
			{
			case 5:
			{
				switch (m_nCardType)
				{
				case 0:
				{
					CString strCardNum, strAppBina, strMoney, strTimeZoneParam;
					int nRtn = ZDLT645_2007::ReadParamPresetCard18SGC(strCardNum, strAppBina, strMoney, strTimeZoneParam);
					if (!nRtn)
					{
						std::vector<CString> vec_strSrc,vec_strData;
						strAppBina.Delete(0, 8);
						strAppBina.Delete(66, 12);
						int nAmount = _tcstol(strMoney.Left(8), NULL, 16);
						int nTimes = _tcstol(strMoney.Mid(8, 8), NULL, 16);
						strMoney.Format(_T("%08d%08d"), nAmount, nTimes);
						strTimeZoneParam.Delete(0, 6);
						vec_strSrc.push_back(strAppBina);
						vec_strSrc.push_back(strMoney);
						vec_strSrc.push_back(strTimeZoneParam);
						Decode(vec_strSrc, vec_strData);
						InsertList(vec_strData);
						AfxMessageBox(_T("读取成功!"));
					}
					else
					{
						CString strShow(_T("读取失败："));
						strShow+=ZDLT645_2007::ErrorInfo18SGC(nRtn);
						AfxMessageBox(strShow);
					}
				}
				break;
				case 1:
				{
					CString strCardNum, strESAMAppBina, strESAMWorkInfo,strESAMMoney;
					int nRtn = ZDLT645_2007::ReadTestingCard18SGC(strCardNum, strESAMAppBina, strESAMWorkInfo, strESAMMoney);
					if (!nRtn)
					{
						std::vector<CString> vec_strSrc, vec_strData;
						strESAMAppBina.Delete(0, 6);
						strESAMWorkInfo.Delete(0, 6);
						strESAMMoney.Delete(0, 6);
						int nAmount = _tcstol(strESAMMoney.Left(8), NULL, 16);
						int nTimes = _tcstol(strESAMMoney.Mid(8,8), NULL, 16);
						strESAMMoney.Format(_T("%08d%08d"), nAmount, nTimes);
						vec_strSrc.push_back(strESAMAppBina);
						vec_strSrc.push_back(strESAMWorkInfo);
						vec_strSrc.push_back(strESAMMoney);
						Decode(vec_strSrc, vec_strData);
						InsertList(vec_strData);
						AfxMessageBox(_T("读取成功!"));
					}
					else
					{
						CString strShow(_T("读取失败："));
						strShow += ZDLT645_2007::ErrorInfo18SGC(nRtn);
						AfxMessageBox(strShow);
					}
				}
				break;
				case 2:
				{
					CString strCardNum, strAppBina;
					int nRtn = ZDLT645_2007::ReadIncreaseAmountCard18SGC(strCardNum, strAppBina);
					if (!nRtn)
					{
						std::vector<CString> vec_strSrc, vec_strData;
						strAppBina.Delete(0, 6);
						int nAmount = _tcstol(strAppBina.Left(8), NULL, 16);
						int nTimes = _tcstol(strAppBina.Mid(8, 8), NULL, 16);
						strAppBina.Format(_T("%08d%08d"), nAmount, nTimes);
						vec_strSrc.push_back(strAppBina);
						Decode(vec_strSrc, vec_strData);
						InsertList(vec_strData);
						AfxMessageBox(_T("读取成功!"));
					}
					else
					{
						CString strShow(_T("读取失败："));
						strShow += ZDLT645_2007::ErrorInfo18SGC(nRtn);
						AfxMessageBox(strShow);
					}
				}
				break;
				case 3:
				{
					CString strCardNum, strAppBina,strReverAppBina;
					int nRtn = ZDLT645_2007::ReadModifyMeterNumCard18SGC(strCardNum, strAppBina, strReverAppBina);
					if (!nRtn)
					{
						std::vector<CString> vec_strSrc, vec_strData;
						strAppBina.Delete(0, 6);
						strReverAppBina.Delete(0, 332);
						vec_strSrc.push_back(strAppBina);
						vec_strSrc.push_back(strReverAppBina);
						Decode(vec_strSrc, vec_strData);
						InsertList(vec_strData);
						AfxMessageBox(_T("读取成功!"));
					}
					else
					{
						CString strShow(_T("读取失败："));
						strShow += ZDLT645_2007::ErrorInfo18SGC(nRtn);
						AfxMessageBox(strShow);
					}
				}
				break;
				}
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
	switch (g_v.g_nProtocolType)
	{
	case CMainData::PROTOCOL_DLT645:
	{
		switch (g_v.g_nDllType)
		{
		case 5:
		{
			switch (m_nCardType)
			{
			case 0:
			{
				CString strAppBina, strMoney, strTimeZoneParam;
				std::vector<CString> vec_strDest;
				Encode(vec_strDest);
				if (vec_strDest.size() == 3)
				{
					vec_strDest[0] = vec_strDest[0].Left(66) + _T("000000000000") + vec_strDest[0].Mid(66);
					vec_strDest[0] = _T("08A300") + vec_strDest[0];
					vec_strDest[0] = _T("68") + vec_strDest[0] + ZDLT645_2007::GetCheckSum(vec_strDest[0]) + _T("16");
					strAppBina = vec_strDest[0];
					vec_strDest[1].Format(_T("%08X%08X"), _ttoi(vec_strDest[1].Left(8)), _ttoi(vec_strDest[1].Mid(8,8)));
					strMoney = vec_strDest[1];
					vec_strDest[2] = _T("1CDB") + vec_strDest[2];
					vec_strDest[2] = _T("68") + vec_strDest[2] + ZDLT645_2007::GetCheckSum(vec_strDest[2]) + _T("16");
					strTimeZoneParam = vec_strDest[2];
				}
				int nRtn = ZDLT645_2007::WriteParamPresetCard18SGC(g_v.g_strNetCptIP, g_v.g_strNetCptPort, g_v.g_strNetCptTimeOut, strAppBina, strMoney, strTimeZoneParam);
				if (!nRtn)
					AfxMessageBox(_T("写入成功!"));
				else
				{
					CString strShow(_T("写入失败："));
					strShow += ZDLT645_2007::ErrorInfo18SGC(nRtn);
					AfxMessageBox(strShow);
				}
			}
			break;
			case 1:
			{
				AfxMessageBox(_T("检测卡不可写入!"));
			}
			break;
			case 2:
			{
				CString strAppBina;
				std::vector<CString> vec_strDest;
				Encode(vec_strDest);
				if (vec_strDest.size() == 1)
				{
					vec_strDest[0].Format(_T("%08X%08X"), _ttoi(vec_strDest[0].Left(8)), _ttoi(vec_strDest[0].Mid(8,8)));
					vec_strDest[0] = _T("0708") + vec_strDest[0];
					vec_strDest[0] = _T("68") + vec_strDest[0] + ZDLT645_2007::GetCheckSum(vec_strDest[0]) + _T("16");
					strAppBina = vec_strDest[0];
				}
				int nRtn = ZDLT645_2007::WriteIncreaseAmountCard18SGC(g_v.g_strNetCptIP, g_v.g_strNetCptPort, g_v.g_strNetCptTimeOut, strAppBina);
				if(!nRtn)
					AfxMessageBox(_T("写入成功!"));
				else
				{
					CString strShow(_T("写入失败："));
					strShow += ZDLT645_2007::ErrorInfo18SGC(nRtn);
					AfxMessageBox(strShow);
				}
			}
			break;
			case 3:
			{
				CString strAppBina, strReverAppBina;
				std::vector<CString> vec_strDest;
				Encode(vec_strDest);
				if (vec_strDest.size() == 2)
				{
					vec_strDest[1] = vec_strDest[0].Left(12);//当前表号与起始表号同步
					vec_strDest[0] = _T("0A0C") + vec_strDest[0];
					vec_strDest[0] = _T("68") + vec_strDest[0] + ZDLT645_2007::GetCheckSum(vec_strDest[0]) + _T("16");
					strAppBina = vec_strDest[0];
					CString strZero326(_T('0'), 326);
					vec_strDest[1] = _T("1AA9") + strZero326+vec_strDest[1];
					vec_strDest[1] = _T("68") + vec_strDest[1] + ZDLT645_2007::GetCheckSum(vec_strDest[1]) + _T("16");
					strReverAppBina = vec_strDest[1];
				}
				int nRtn = ZDLT645_2007::WriteModifyMeterNumCard18SGC(g_v.g_strNetCptIP, g_v.g_strNetCptPort, g_v.g_strNetCptTimeOut, strAppBina, strReverAppBina);
				if (!nRtn)
					AfxMessageBox(_T("写入成功!"));
				else
				{
					CString strShow(_T("写入失败："));
					strShow += ZDLT645_2007::ErrorInfo18SGC(nRtn);
					AfxMessageBox(strShow);
				}
			}
			break;
			}
		}
		break;
		}
	}
	break;
	}
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
				strItem += tcSplitChar;
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


void CCardTesting::SetListBlank()
{
	for (ZListCtrl * p_list : m_vec_listItem)
	{
		int nRow = p_list->GetItemCount();
		for (int i = 0; i < nRow; ++i)
		{
			p_list->SetItemText(i, 1, _T(""));
		}
	}
}


void CCardTesting::Decode(std::vector<CString> & vec_strSrc, std::vector<CString> & vec_strData)
{
	vec_strData.clear();
	std::vector<CCard> vec_carditem;
	CString strSql;
	strSql.Format(_T("SELECT * FROM CardItem WHERE ProtocolType=%d AND DllType=%d AND CardType=%d ORDER BY ID LIMIT 3"), g_v.g_nProtocolType, g_v.g_nDllType, m_nCardType);
	g_v.g_cidb.Select(strSql, vec_carditem);
	if (vec_carditem.size() == 3)
	{
		int nFileNum = vec_carditem[0].GetFileNum();
		for (int i = 0; i < nFileNum; ++i)
		{
			CString strLength = (vec_carditem[1].GetFile())[i];
			CString strDotLen = (vec_carditem[2].GetFile())[i];
			std::vector<CString>  vec_strLength, vec_strDotLen;
			ZUtil::StrSplit(strLength, vec_strLength, tcSplitChar, strLength.Right(1) == tcSplitChar);
			ZUtil::StrSplit(strDotLen, vec_strDotLen, tcSplitChar, strDotLen.Right(1) == tcSplitChar);
			int nSize = vec_strLength.size();
			for (int j = 0; j < nSize; ++j)
			{
				CString str;
				str = vec_strSrc[i].Left(_ttoi(vec_strLength[j]));
				if (_ttoi(vec_strDotLen[j]))
					str = str.Left(str.GetLength() - _ttoi(vec_strDotLen[j])) + _T('.') + str.Right(_ttoi(vec_strDotLen[j]));
				vec_strSrc[i].Delete(0, _ttoi(vec_strLength[j]));
				vec_strData.push_back(str);
			}
		}
	}
}


void CCardTesting::InsertList(const std::vector<CString> & vec_strData)
{
	int j = 0;
	for (ZListCtrl * p_list : m_vec_listItem)
	{
		int nRow = p_list->GetItemCount();
		for (int i=0;i<nRow;++i)
		{
			p_list->SetItemText(i, 1, vec_strData[j]);
			++j;
		}
	}
}


void CCardTesting::Encode(std::vector<CString> & vec_strDest)
{
	int j = 0;
	for (ZListCtrl * p_list : m_vec_listItem)
	{
		vec_strDest.push_back(_T(""));
		int nRow = p_list->GetItemCount();
		for (int i = 0; i < nRow; ++i)
		{
			vec_strDest[j]+=p_list->GetItemText(i, 1);
		}
		++j;
	}
	for (CString & str : vec_strDest)
	{
		str.Remove(_T('.'));
	}
}