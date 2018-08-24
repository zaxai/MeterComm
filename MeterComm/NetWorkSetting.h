#pragma once

// CNetWorkSetting 对话框

class CNetWorkSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CNetWorkSetting)

public:
	CNetWorkSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetWorkSetting();

// 对话框数据
	enum { IDD = IDD_NETWORKSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strServerIP;
	CString m_strServerPort;
	CString m_strServerTimeOut;
	CString m_strNetCptIP;
	CString m_strNetCptPort;
	CString m_strNetCptTimeOut;
	int m_nDllSel;
	BOOL m_nIsRemote;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	CComboBox m_comboEncryptor;
public:
	virtual BOOL OnInitDialog();
	void SetParam(const CString & strServerIP, const CString & strServerPort, const CString & strServerTimeOut,const CString & strNetCptIP, const CString & strNetCptPort, const CString & strNetCptTimeOut,const BOOL & nDllSel, const BOOL & nIsRemote);
	void GetParam(CString & strServerIP, CString & strServerPort, CString & strServerTimeOut, CString & strNetCptIP, CString & strNetCptPort, CString & strNetCptTimeOut, BOOL & nDllSel, BOOL & nIsRemote);
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckIsremote();
	afx_msg void OnCbnSelchangeComboEncryptor();
};
