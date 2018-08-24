#pragma once


// CShowResultOfMT 对话框

class CShowResultOfMT : public CDialogEx
{
	DECLARE_DYNAMIC(CShowResultOfMT)

public:
	CShowResultOfMT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowResultOfMT();

// 对话框数据
	enum { IDD = IDD_SHOWRESULTOFMT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strInfo;
	CString m_strText;
public:
	virtual BOOL OnInitDialog();
	void SetParam(const CString & strInfo, const CString & strText);
};
