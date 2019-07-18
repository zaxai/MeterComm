#pragma once


// CDecode698Tool 对话框

class CDecode698Tool : public CDialogEx
{
	DECLARE_DYNAMIC(CDecode698Tool)

public:
	CDecode698Tool(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDecode698Tool();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DECODE698 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_treeAPDU;
	ZButton m_btnOK;
	ZButton m_btnCancel;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnEnChangeEditXml();
	void DecodeXml(const CString & strXml);
	void InitTree(const std::vector<int> & vec_nIndex, const std::vector<CString> & vec_strData);
	void ExpandTree(CTreeCtrl & tree, HTREEITEM hTreeItem);
	void ExecType(CMarkup & xml, int nIndex, std::vector<int> & vec_nIndex, std::vector<CString> & vec_strData);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
