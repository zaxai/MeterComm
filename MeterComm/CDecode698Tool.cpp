// CDecode698Tool.cpp: 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "CDecode698Tool.h"
#include "afxdialogex.h"


// CDecode698Tool 对话框

IMPLEMENT_DYNAMIC(CDecode698Tool, CDialogEx)

CDecode698Tool::CDecode698Tool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DECODE698, pParent)
{

}

CDecode698Tool::~CDecode698Tool()
{
}

void CDecode698Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_APDU, m_treeAPDU);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDecode698Tool, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_EDIT_DATA, &CDecode698Tool::OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_XML, &CDecode698Tool::OnEnChangeEditXml)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDecode698Tool 消息处理程序


BOOL CDecode698Tool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDecode698Tool::OnEnChangeEditData()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strData,strXml;
	GetDlgItemText(IDC_EDIT_DATA, strData);
	ZDLT698_45 dlt698;
	ZDLT698_45 * p_dlt698 = NULL;
	if (g_vec_md.empty())
		p_dlt698 = &dlt698;
	else
		p_dlt698 = g_vec_md[0].m_p_dlt698;
	p_dlt698->DataDecode(strData, strXml);
	SetDlgItemText(IDC_EDIT_XML, strXml);
	DecodeXml(strXml);
}


void CDecode698Tool::OnEnChangeEditXml()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strXml;
	GetDlgItemText(IDC_EDIT_XML, strXml);
	DecodeXml(strXml);
}


void CDecode698Tool::DecodeXml(const CString & strXml)
{
	CMarkup xml;
	int nIndex = 0;
	std::vector<int> vec_nIndex;
	std::vector<CString> vec_strData;
	vec_nIndex.push_back(nIndex++);
	vec_strData.push_back(strXml.Right(3));
	if (strXml.Right(3) == _T("90H"))
	{
		vec_nIndex.push_back(nIndex++);
		vec_strData.push_back(strXml.Right(7).Left(3));
	}
	m_treeAPDU.DeleteAllItems();
	if (xml.SetDoc(strXml))
	{
		if (xml.FindElem(_T("N")))
		{
			vec_nIndex.push_back(nIndex++);
			vec_strData.push_back(_T("Num_")+xml.GetAttrib(_T("num")));
			xml.IntoElem();
		}
		while (xml.FindElem(_T("O")))
		{
			vec_nIndex.push_back(nIndex);
			vec_strData.push_back(_T("Obj_") + xml.GetAttrib(_T("obj")));
			xml.IntoElem();
			while (xml.FindElem(_T("Rsd")))
			{
				vec_nIndex.push_back(nIndex + 1);
				vec_strData.push_back(_T("Rsd_") + xml.GetAttrib(_T("choice")));
			}
			while (xml.FindElem(_T("Rcsd")))
			{
				vec_nIndex.push_back(nIndex + 1);
				vec_strData.push_back(_T("Rcsd"));
				xml.IntoElem();
				while (xml.FindElem(_T("Csd")))
				{
					vec_nIndex.push_back(nIndex + 2);
					vec_strData.push_back(_T("Csd_") + xml.GetData());
				}
				xml.OutOfElem();
			}
			while (xml.FindElem(_T("D")))
			{
				vec_nIndex.push_back(nIndex+1);
				vec_strData.push_back(_T("Dar_") + xml.GetAttrib(_T("dar"))+_T("_")+xml.GetData());
			}
			while (xml.FindElem(_T("T")))
				ExecType(xml, nIndex + 1, vec_nIndex, vec_strData);
			xml.OutOfElem();
		}
		InitTree(vec_nIndex, vec_strData);
	}
}


void CDecode698Tool::ExecType(CMarkup & xml,int nIndex,std::vector<int> & vec_nIndex,std::vector<CString> & vec_strData)
{
	CString strType = xml.GetAttrib(_T("type"));
	if (strType == _T("1") || strType == _T("2"))
	{
		vec_nIndex.push_back(nIndex);
		vec_strData.push_back(_T("Type_") + strType);
		xml.IntoElem();
		if (xml.FindElem(_T("T")))
			ExecType(xml,nIndex+1,vec_nIndex,vec_strData);
		xml.OutOfElem();
	}
	else
	{
		vec_nIndex.push_back(nIndex);
		vec_strData.push_back(_T("Type_") + strType + _T("_") + xml.GetData());
	}
	if(xml.FindElem(_T("T")))
		ExecType(xml, nIndex , vec_nIndex, vec_strData);
}


void CDecode698Tool::InitTree(const std::vector<int> & vec_nIndex, const std::vector<CString> & vec_strData)
{
	HTREEITEM hItem,hParentItem;
	int nSize = vec_nIndex.size();
	for (int i=0;i<nSize;++i)
	{
		if (i == 0)
			hItem = m_treeAPDU.InsertItem(vec_strData[i]);
		else
		{
			if (vec_nIndex[i] > vec_nIndex[i - 1])
				hItem = m_treeAPDU.InsertItem(vec_strData[i], hItem);
			else if (vec_nIndex[i] == vec_nIndex[i - 1])
			{
				hParentItem = m_treeAPDU.GetParentItem(hItem);
				hItem = m_treeAPDU.InsertItem(vec_strData[i], hParentItem);
			}
			else
			{
				int n = vec_nIndex[i - 1] - vec_nIndex[i];
				hParentItem = m_treeAPDU.GetParentItem(hItem);
				while (n--)
				{
					hParentItem = m_treeAPDU.GetParentItem(hParentItem);
				}
				hItem = m_treeAPDU.InsertItem(vec_strData[i], hParentItem);
			}
		}
	}	
	ExpandTree(m_treeAPDU, m_treeAPDU.GetRootItem());
}


void CDecode698Tool::ExpandTree(CTreeCtrl & tree, HTREEITEM hTreeItem)
{
	if (!tree.ItemHasChildren(hTreeItem))
	{
		return;
	}
	HTREEITEM hNextItem = tree.GetChildItem(hTreeItem);
	while (hNextItem != NULL)
	{
		ExpandTree(tree, hNextItem);
		hNextItem = tree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	tree.Expand(hTreeItem, TVE_EXPAND);
}


void CDecode698Tool::InitButton(void)
{
	SetBtnColor(m_btnOK, _T(""));
	SetBtnColor(m_btnCancel, _T(""));
}


void CDecode698Tool::SetBtnColor(ZButton & btn, const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}


BOOL CDecode698Tool::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CDecode698Tool::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
