#pragma once
#include "afxwin.h"

// CSoftwareUpdate 对话框

class CSoftwareUpdate : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftwareUpdate)

public:
	CSoftwareUpdate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftwareUpdate();

// 对话框数据
	enum { IDD = IDD_SOFTWAREUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hEvtExitCheckUpdate;
	ZButton m_btnOK;
	ZButton m_btnCancel;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	static UINT CheckSoftwareUpdateThreadFunc(LPVOID lpParam);
	static bool SocketCommunication(const CString & strDataIn,CString & strDataOut);
	afx_msg void OnBnClickedOk();
	void SoftWareUpdate(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
