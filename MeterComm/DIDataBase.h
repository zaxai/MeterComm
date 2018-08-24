#pragma once
#include "zlistctrl.h"
#include "zbutton.h"

// CDIDataBase 对话框

class CDIDataBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDIDataBase)

public:
	CDIDataBase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDIDataBase();

// 对话框数据
	enum { IDD = IDD_DIDATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ZListCtrl m_listItem;
	ZButton m_btnAdd;
	ZButton m_btnDelete;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	bool m_bIsInitFinish;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void InitList(void);
	void InsertList(void);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID,const CRect & rcParent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
