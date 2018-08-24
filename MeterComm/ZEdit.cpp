// ZEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "ZEdit.h"
#include "ZListCtrl.h"

// ZEdit

IMPLEMENT_DYNAMIC(ZEdit, CEdit)

ZEdit::ZEdit()
{

}

ZEdit::~ZEdit()
{

}


BEGIN_MESSAGE_MAP(ZEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// ZEdit 消息处理程序




void ZEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	GetParent()->PostMessage(WM_EDITKILLFOCUS,0,0);
}
