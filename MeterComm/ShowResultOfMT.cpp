// ShowResultOfMT.cpp : 实现文件
//

#include "stdafx.h"
#include "MeterComm.h"
#include "ShowResultOfMT.h"
#include "afxdialogex.h"


// CShowResultOfMT 对话框

IMPLEMENT_DYNAMIC(CShowResultOfMT, CDialogEx)

CShowResultOfMT::CShowResultOfMT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowResultOfMT::IDD, pParent)
	, m_strInfo(_T(""))
	, m_strText(_T(""))
{

}

CShowResultOfMT::~CShowResultOfMT()
{
}

void CShowResultOfMT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowResultOfMT, CDialogEx)
END_MESSAGE_MAP()


// CShowResultOfMT 消息处理程序


BOOL CShowResultOfMT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_STATIC_INFO,m_strInfo);
	SetDlgItemText(IDC_EDIT_RESULT,m_strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShowResultOfMT::SetParam(const CString & strInfo, const CString & strText)
{
	m_strInfo=strInfo;
	m_strText=strText;
}
