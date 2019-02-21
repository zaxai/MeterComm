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
	ZButton m_btnRead;
	ZButton m_btnWrite;
	ZButton m_btnSave;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	int m_nCardType;
	std::vector<ZListCtrl *> m_vec_listItem;
public:
	virtual BOOL OnInitDialog();
	void InitListBox();
	void InitTab();
	void InsertTab(int nCardType);
	void InitButton(void);
	void SetBtnColor(ZButton & btn, const CString & strTip);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLbnSelchangeListCardtype();
	afx_msg void OnTcnSelchangeTabCardcontent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonSave();
	void SetListBlank();
	void Decode(std::vector<CString> & vec_strSrc, std::vector<CString> & vec_strData);
	void InsertList(const std::vector<CString> & vec_strData);
	void Encode(std::vector<CString> & vec_strDest);
};
