#pragma once
#include "MeterCommView.h"
#include "afxcmn.h"
#include "zbutton.h"

// CDataItem 对话框

class CDataItem : public CDialogEx
{
	DECLARE_DYNAMIC(CDataItem)

public:
	CDataItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataItem();

// 对话框数据
	enum { IDD = IDD_DATAITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_treeItemClass;
	CListCtrl m_listItem;
	CListCtrl m_listSearch;
	std::vector<CString> m_vec_strClass;
	CString m_strDataItem;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	ZButton m_btnSearch;
	bool m_bIsInitFinish;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	void InitTree(void);
	void InitList(void);
	void ExpandTree(CTreeCtrl & Tree,HTREEITEM hTreeItem);
	afx_msg void OnTvnSelchangedTreeItemclass(NMHDR *pNMHDR, LRESULT *pResult);
	void InsertList(const int &nIndex);
	afx_msg void OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	CString GetDataItem(void);
	afx_msg void OnNMClickListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID,const CRect & rcParent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
