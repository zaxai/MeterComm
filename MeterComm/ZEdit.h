#pragma once


// ZEdit

class ZEdit : public CEdit
{
	DECLARE_DYNAMIC(ZEdit)

public:
	ZEdit();
	virtual ~ZEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


