#pragma once
#include "ProduceTesting.h"
#define IDC_CHECK_START 3000
#define IDC_EDIT_START 4000
// CMeterPos 对话框

class CMeterPos : public CDialogEx
{
	DECLARE_DYNAMIC(CMeterPos)

public:
	CMeterPos(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterPos();

// 对话框数据
	enum { IDD = IDD_METERPOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CProduceTesting *m_p_protest;
	std::vector<CButton *> m_vec_btnCheck;
	std::vector<CEdit *> m_vec_edit;
	ZButton m_btnGetMeterPos;
	ZButton m_btnSetBarCodeInfo;
	ZButton m_btnSelectMeter;
	ZButton m_btnUnselectMeter;
	ZButton m_btnOK;
	ZButton m_btnCancel;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void SetProTest(CProduceTesting * p_protest);
	void InsertMeterPos(void);
	afx_msg void OnBnClickedCheckSelectallmeter();
	afx_msg void OnBnClickedButtonSelectmeter();
	afx_msg void OnBnClickedButtonUnselectmeter();
	afx_msg void OnBnClickedButtonGetmeterpos();
	afx_msg void OnBnClickedButtonSetbarcodeinfo();
	afx_msg void OnBnClickedCheckMeterpos(UINT nID);
	void SelectMeter(const BOOL & nIsSelect);
	void ExecVecSelect(void);
	int ChangeSize(UINT nID, const int & nTop);
	void EnableBtn(const BOOL & bEnable);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool SetBarcodeInfo(void);
};
