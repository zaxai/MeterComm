#pragma once
#include "MeterCommView.h"
#include "afxwin.h"

// CComPortSetting 对话框

class CComPortSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CComPortSetting)

public:
	CComPortSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComPortSetting();

// 对话框数据
	enum { IDD = IDD_COMPORTSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_comboComPort;
	CComboBox m_comboBaudRate;
	CComboBox m_comboParity;
	CComboBox m_comboByteSize;
	CComboBox m_comboStopBits;
	CComboBox m_comboComPortMD;
	CComboBox m_comboBaudRateMD;
	CComboBox m_comboParityMD;
	CComboBox m_comboByteSizeMD;
	CComboBox m_comboStopBitsMD;
	ZButton m_btnSetComPort;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	CButton m_checkIsMDComPortOpen;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetcomport();
	afx_msg void OnBnClickedCheckIsmdcomportopen();
	bool CheckMDComPortAvailable(void);
	void EnableBtn(const BOOL & bEnable);
	afx_msg void OnEnChangeEditComportnum();
	void EnableSet(const BOOL & bEnable);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
