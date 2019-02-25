/*
** Version  1.0.0.1
** Date     2019.02.23
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once


// ZListEdit

class ZListEdit : public CEdit
{
	DECLARE_DYNAMIC(ZListEdit)
public:
	class KillFocusCallBack
	{
	public:
		virtual void OnEditKillFocus(void) = 0;
	};
public:
	ZListEdit();
	ZListEdit(KillFocusCallBack * p_callback);
	virtual ~ZListEdit();

protected:
	DECLARE_MESSAGE_MAP()
private:
	KillFocusCallBack * m_p_callback;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	void SetKillFocusCallBack(KillFocusCallBack * p_callback);
};


