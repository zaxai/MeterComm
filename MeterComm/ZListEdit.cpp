// ZListEdit.cpp: 实现文件
//

#include "stdafx.h"
#include "ZListEdit.h"


// ZListEdit

IMPLEMENT_DYNAMIC(ZListEdit, CEdit)

ZListEdit::ZListEdit()
	: m_p_callback(NULL)
{

}

ZListEdit::ZListEdit(KillFocusCallBack * p_callback)
	: m_p_callback(p_callback)
{

}

ZListEdit::~ZListEdit()
{
}


BEGIN_MESSAGE_MAP(ZListEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// ZListEdit 消息处理程序




void ZListEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	if (m_p_callback)
		m_p_callback->OnEditKillFocus();
}


void ZListEdit::SetKillFocusCallBack(KillFocusCallBack * p_callback)
{
	m_p_callback = p_callback;
}