#pragma once
#include "MeterCommView.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "zbutton.h"
#define IDC_LIST_START 5000

// CMeterTesting 对话框

class CMeterTesting : public CDialogEx
{
	DECLARE_DYNAMIC(CMeterTesting)

public:
	CMeterTesting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterTesting();

// 对话框数据
	enum { IDD = IDD_METERTESTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_treeItemClass;
	std::vector<CString> m_vec_strClass;
	std::vector<CListCtrl *> m_vec_listItem;
	int m_nIndexOfItem;
	bool m_bIsInitFinish;
	CComboBox m_comboPortSelected;
	ZButton m_btnSelect;
	ZButton m_btnUnselect;
	ZButton m_btnReadMeter;
	ZButton m_btnSetMeter;
	ZButton m_btnStop;
	ZButton m_btnOK;
	ZButton m_btnCancel;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void InitTree(void);
	void ExpandTree(CTreeCtrl & Tree,HTREEITEM hTreeItem);
	afx_msg void OnTvnSelchangedTreeItemclass(NMHDR *pNMHDR, LRESULT *pResult);
	void InsertList(const int & nIndex);
	CButton m_checkSelectAll;
	afx_msg void OnBnClickedCheckSelectall();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonUnselect();
	afx_msg void OnBnClickedButtonReadmeter();
	afx_msg void OnBnClickedButtonSetmeter();
	afx_msg void OnBnClickedButtonStop();
	void Select(const BOOL & nIsSelect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID,const CRect & rcParent);
	void OperatorList(const int & nIndex);
	void InsertCombo(void);
	void EnableBtn(const BOOL & bEnable);
	void SetBlank(void);
	int GetFirstRow(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg void OnNMDblclkListItem(UINT nID, NMHDR * pNMHDR, LRESULT * pResult);
	CString ExecResult(const CString & strDataItem,const CString &strData);
	CString Str2TimeFormat(const CString & str);
	void DataChange(CString & str,const CString & strFormat,const CString & strUnit);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
