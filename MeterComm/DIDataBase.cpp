// DIDataBase.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "DIDataBase.h"
#include "afxdialogex.h"


// CDIDataBase 对话框

IMPLEMENT_DYNAMIC(CDIDataBase, CDialogEx)

CDIDataBase::CDIDataBase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDIDataBase::IDD, pParent)
	, m_bIsInitFinish(false)
{

}

CDIDataBase::~CDIDataBase()
{
}

void CDIDataBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDIDataBase, CDialogEx)
	ON_MESSAGE(WM_MSGRECVPRO, &CDIDataBase::OnMsgrecvpro)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDIDataBase::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDIDataBase::OnBnClickedButtonDelete)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDIDataBase 消息处理程序


BOOL CDIDataBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	InsertList();
	InitButton();
	m_bIsInitFinish=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDIDataBase::InitList(void)
{
	CRect rc;     
	m_listItem.GetClientRect(&rc);     
	m_listItem.SetExtendedStyle(m_listItem.GetExtendedStyle()|LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	const int nMax=29;
	m_listItem.InsertColumn(0, _T("ID"), LVCFMT_LEFT, rc.Width()*1/nMax, 0);   
	m_listItem.InsertColumn(1, _T("数据标识"), LVCFMT_LEFT, rc.Width()*3/nMax,1);  
	m_listItem.InsertColumn(2, _T("数据名称"), LVCFMT_LEFT, rc.Width()*5/nMax,2);  
	m_listItem.InsertColumn(3, _T("第一分类"), LVCFMT_LEFT, rc.Width()*2/nMax,3);  
	m_listItem.InsertColumn(4, _T("第二分类"), LVCFMT_LEFT, rc.Width()*2/nMax,4); 
	m_listItem.InsertColumn(5, _T("数据格式"), LVCFMT_LEFT, rc.Width()*4/nMax,5); 
	m_listItem.InsertColumn(6, _T("数据长度"), LVCFMT_LEFT, rc.Width()*2/nMax,6); 
	m_listItem.InsertColumn(7, _T("单位"), LVCFMT_LEFT, rc.Width()*3/nMax,7); 
	m_listItem.InsertColumn(8, _T("是否可读"), LVCFMT_LEFT, rc.Width()*2/nMax,8); 
	m_listItem.InsertColumn(9, _T("是否可写"), LVCFMT_LEFT, rc.Width()*2/nMax,9); 
	m_listItem.InsertColumn(10, _T("是否ASCII"), LVCFMT_LEFT, rc.Width()*2/nMax,10); 
	m_listItem.EnableEdit(true);
	m_listItem.SetTextDefaultColor(RGB(192,64,0));
}

void CDIDataBase::InsertList(void)
{
	ZSqlite3 zsql;
	CString strPath=ZUtil::GetExeCatalogPath()+_T("\\res\\DataItem.di");
	CStdioFile f;
	if(!f.Open(strPath, CFile::modeRead))
	{
		AfxMessageBox(_T("数据库文件丢失!"));
		return;
	}
	f.Close();
	if(!zsql.OpenDB(strPath))
	{
		AfxMessageBox(_T("数据库打开出错!"));
		return;
	}
	CString strSQL,strError;
	strSQL=_T("SELECT * FROM DataItem645 ORDER BY FstClass,SecClass");
	std::vector<std::vector <CString>> vec2_strData;
	zsql.GetTable(strSQL,vec2_strData,&strError);
	zsql.CloseDB();
	int nRow=vec2_strData.size();
	if(nRow)
	{
		int nColumn=vec2_strData[0].size();
		if(nColumn==11)
		{
			for(int i=0;i<nRow-1;++i)
			{
				m_listItem.InsertItem(i,vec2_strData[i+1][0]);
				for(int j=1;j<nColumn;++j)
				{
					m_listItem.SetItemText(i,j,vec2_strData[i+1][j]);
				}
			}
		}
	}
}

afx_msg LRESULT CDIDataBase::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case MSGUSER_SP_EDITKILLFOCUS:
		{
			if(m_listItem.m_hWnd)
			{
				ItemInfo * p_ii=(ItemInfo *)wParam;
				const CString sz_strHead[]={_T("ID"),_T("DataItem"),_T("DataName"),_T("FstClass"),_T("SecClass"),_T("DataFormat"),_T("DataLen"),_T("Unit"),_T("IsRead"),_T("IsWrite"),_T("IsASCII")};
				ZSqlite3 zsql;
				CString strPath=ZUtil::GetExeCatalogPath()+_T("\\res\\DataItem.di");
				CStdioFile f;
				if(!f.Open(strPath, CFile::modeRead))
				{
					AfxMessageBox(_T("数据库文件丢失!"));
				}
				else
				{
					f.Close();
					if(!zsql.OpenDB(strPath))
					{
						AfxMessageBox(_T("数据库打开出错!"));
					}
					else
					{
						CString strSQL,strError,strHead,strValue,strWhere;
						strHead=sz_strHead[p_ii->m_nSubItem];
						strValue=m_listItem.GetItemText(p_ii->m_nItem,p_ii->m_nSubItem);
						if(p_ii->m_nSubItem!=0)
						{
							strWhere=m_listItem.GetItemText(p_ii->m_nItem,0);
							strSQL.Format(_T("UPDATE DataItem645 SET %s='%s' WHERE ID=%d"),strHead,strValue,_ttoi(strWhere));
						}
						else
						{
							strWhere=m_listItem.GetItemText(p_ii->m_nItem,1);
							strSQL.Format(_T("UPDATE DataItem645 SET %s=%d WHERE DataItem='%s'"),strHead,_ttoi(strValue),strWhere);
						}
						if(zsql.ExecSQL(strSQL,&strError))
						{
							if(p_ii->m_nSubItem!=0)
							{
								strWhere=m_listItem.GetItemText(p_ii->m_nItem,0);
								strSQL.Format(_T("SELECT %s FROM DataItem645 WHERE ID=%d"),strHead,_ttoi(strWhere));
							}
							else
							{
								strWhere=m_listItem.GetItemText(p_ii->m_nItem,1);
								strSQL.Format(_T("SELECT %s FROM DataItem645 WHERE DataItem='%s'"),strHead,strWhere);
							}
							std::vector<std::vector <CString>> vec2_strData;
							zsql.GetTable(strSQL,vec2_strData,&strError);
							if(vec2_strData.size())
								m_listItem.SetItemText(p_ii->m_nItem,p_ii->m_nSubItem,vec2_strData[1][0]);
							AfxMessageBox(_T("执行失败 ")+strError);
						}
					}
				}
			}
		}
		break;
	}
	return 0;
}


void CDIDataBase::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	ZSqlite3 zsql;
	CString strPath=ZUtil::GetExeCatalogPath()+_T("\\res\\DataItem.di");
	CStdioFile f;
	if(!f.Open(strPath, CFile::modeRead))
	{
		AfxMessageBox(_T("数据库文件丢失!"));
		return;
	}
	f.Close();
	if(!zsql.OpenDB(strPath))
	{
		AfxMessageBox(_T("数据库打开出错!"));
		return;
	}
	CString strEdit,strSQL,strError,strID;
	int nID=1;
	strSQL=_T("SELECT ID FROM DataItem645 ORDER BY ID DESC LIMIT 1");
	std::vector<std::vector <CString>> vec2_strData;
	zsql.GetTable(strSQL,vec2_strData,&strError);
	if(vec2_strData.size())
	{
		nID=_ttoi(vec2_strData[1][0])+1;
	}
	GetDlgItemText(IDC_EDIT_ADD,strEdit);
	strEdit.Remove(_T('\r'));
	strEdit.Remove(_T('\n'));
	std::vector<CString>  vec_strData,vec_strRowData;
	ZUtil::StrSplit(strEdit,vec_strData,_T(';'),strEdit.Right(1)==_T(';'));
	int nSize=vec_strData.size();
	m_listItem.SetRedraw(FALSE);
	for(int i=0;i<nSize;++i)
	{
		ZUtil::StrSplit(vec_strData[i],vec_strRowData,_T(','),vec_strData[i].Right(1)==_T(','));
		if(vec_strRowData.size()!=10)
			break;
		strSQL.Format(_T("INSERT INTO DataItem645 VALUES (%d,'%s','%s',%s,%s,'%s',%s,'%s',%s,%s,%s)"),nID+i,vec_strRowData[0],vec_strRowData[1],vec_strRowData[2],vec_strRowData[3],vec_strRowData[4],vec_strRowData[5],vec_strRowData[6],vec_strRowData[7],vec_strRowData[8],vec_strRowData[9]);
		if(zsql.ExecSQL(strSQL,&strError)==0)
		{
			strID.Format(_T("%d"),nID+i);
			int nRow=m_listItem.GetItemCount();
			m_listItem.InsertItem(nRow,strID);
			for(int j=1;j<11;++j)
			{
				m_listItem.SetItemText(nRow,j,vec_strRowData[j-1]);
			}
		}
	}
	m_listItem.SetRedraw(TRUE);
}


void CDIDataBase::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	ZSqlite3 zsql;
	CString strPath=ZUtil::GetExeCatalogPath()+_T("\\res\\DataItem.di");
	CStdioFile f;
	if(!f.Open(strPath, CFile::modeRead))
	{
		AfxMessageBox(_T("数据库文件丢失!"));
		return;
	}
	f.Close();
	if(!zsql.OpenDB(strPath))
	{
		AfxMessageBox(_T("数据库打开出错!"));
		return;
	}
	CString strSQL,strError,strIDStart,strIDEnd;
	GetDlgItemText(IDC_EDIT_IDSTART,strIDStart);
	GetDlgItemText(IDC_EDIT_IDEND,strIDEnd);
	strSQL.Format(_T("DELETE FROM DataItem645 WHERE ID>=%d AND ID<=%d"),_ttoi(strIDStart),_ttoi(strIDEnd));
	if(zsql.ExecSQL(strSQL,&strError)==0)
	{
		int nRow=m_listItem.GetItemCount();
		int nIDStart,nIDEnd,nIDCur;
		nIDStart=_ttoi(strIDStart);
		nIDEnd=_ttoi(strIDEnd);
		std::vector<int> vec_nItem;
		for(int i=0;i<nRow;++i)
		{
			nIDCur=_ttoi(m_listItem.GetItemText(i,0));
			if(nIDCur>=nIDStart&&nIDCur<=nIDEnd)
				vec_nItem.push_back(i);
		}
		int nSize=vec_nItem.size();
		m_listItem.SetRedraw(FALSE);
		for(int i=nSize-1;i>=0;--i)
		{
			m_listItem.DeleteItem(vec_nItem[i]);
		}
		m_listItem.SetRedraw(TRUE);
	}
}


BOOL CDIDataBase::OnEraseBkgnd(CDC* pDC)
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

void CDIDataBase::InitButton(void)
{
	SetBtnColor(m_btnAdd,_T(""));
	SetBtnColor(m_btnDelete,_T(""));
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
}

void CDIDataBase::SetBtnColor(ZButton & btn,const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN,RGB(230,248,226));
	btn.SetColor(ZButton::COLOR_BK_OUT,RGB(230,248,226));
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

void CDIDataBase::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_bIsInitFinish&&nType!=SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		ChangeSize(IDCANCEL,rc); 
		ChangeSize(IDOK,rc); 
		ChangeSize(IDC_STATIC_TO,rc); 
		ChangeSize(IDC_STATIC_ID,rc); 
		ChangeSize(IDC_EDIT_IDEND,rc); 
		ChangeSize(IDC_EDIT_IDSTART,rc); 
		ChangeSize(IDC_BUTTON_DELETE,rc);
		ChangeSize(IDC_EDIT_ADD,rc); 
		ChangeSize(IDC_BUTTON_ADD,rc); 
		ChangeSize(IDC_STATIC_SAMPLE,rc);
		ChangeSize(IDC_LIST_ITEM,rc); 
		CRect rcItem;     
		m_listItem.GetClientRect(&rcItem);      
		const int nMax=29;
		m_listItem.SetRedraw(FALSE);
		m_listItem.SetColumnWidth(0,rcItem.Width()*1/nMax);   
		m_listItem.SetColumnWidth(1,rcItem.Width()*3/nMax);  
		m_listItem.SetColumnWidth(2,rcItem.Width()*5/nMax);  
		m_listItem.SetColumnWidth(3,rcItem.Width()*2/nMax);  
		m_listItem.SetColumnWidth(4,rcItem.Width()*2/nMax); 
		m_listItem.SetColumnWidth(5,rcItem.Width()*4/nMax); 
		m_listItem.SetColumnWidth(6,rcItem.Width()*2/nMax); 
		m_listItem.SetColumnWidth(7,rcItem.Width()*3/nMax); 
		m_listItem.SetColumnWidth(8,rcItem.Width()*2/nMax); 
		m_listItem.SetColumnWidth(9,rcItem.Width()*2/nMax); 
		m_listItem.SetColumnWidth(10,rcItem.Width()*2/nMax); 
		m_listItem.SetRedraw(TRUE);
		InitButton();
		Invalidate();
	}
}

void CDIDataBase::ChangeSize(UINT nID,const CRect & rcParent)
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
		case IDC_STATIC_TO:
		case IDC_STATIC_ID:
			{
				rc.bottom=rcParent.bottom-15;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_EDIT_IDEND:
		case IDC_EDIT_IDSTART:
		case IDC_BUTTON_DELETE:
			{
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_EDIT_ADD:
			{
				rc.right=rcParent.right-10;
				rc.bottom=rcLast.top-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_BUTTON_ADD:
			{
				rc.top=rcLast.top;
				rc.bottom=rc.top+nHeight;		
			}
			break;
		case IDC_STATIC_SAMPLE:
			{
				rc.right=rcParent.right-10;
				rc.bottom=rcLast.top-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_LIST_ITEM:
			{
				rc.right=rcParent.right-10;
				rc.bottom=rcLast.top-10;
			}
			break;
		}
		rcLast=rc;
		pWnd->MoveWindow(rc,FALSE);
	}
}

HBRUSH CDIDataBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr =(HBRUSH)GetStockObject(NULL_BRUSH);
		}
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
