#pragma once
#define IDC_LIST_START 5000

// CCardTesting 对话框

class CCardTesting : public CDialogEx
{
	DECLARE_DYNAMIC(CCardTesting)

public:
	CCardTesting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCardTesting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARDTESTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_listCardType;
	CTabCtrl m_tabCardContent;
	int m_nCardType;
	std::vector<ZListCtrl *> m_vec_listItem;
public:
	virtual BOOL OnInitDialog();
	void InitListBox();
	void InitTab();
	void InsertTab(int nCardType);
	afx_msg void OnLbnSelchangeListCardtype();
	afx_msg void OnTcnSelchangeTabCardcontent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonSave();
};
