#pragma once
#include "MeterCommView.h"
#include "afxcmn.h"
#include "zdlt645_2007.h"
#include <map>
// CProduceTesting 对话框

class CMeterPos;
class CProduceTesting : public CDialogEx
{
	DECLARE_DYNAMIC(CProduceTesting)

public:
	CProduceTesting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProduceTesting();

// 对话框数据
	enum { IDD = IDD_PRODUCETESTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ZListCtrl m_listTest;
	CTreeCtrl m_treePlan;
	std::vector<CString> m_vec_strPlanPath;
	int m_nTreeCount;
	std::vector<std::vector<CString>> m_vec2_strCurPlan;
	bool m_bIsInitFinish;
	CMeterPos *m_p_meterpos;
	ZButton m_btnImportPlan;
	ZButton m_btnUpdateNetPlan;
	ZButton m_btnMeterPos;
	ZButton m_btnSinglePro;
	ZButton m_btnContinuePro;
	ZButton m_btnAllPro;
	ZButton m_btnStopPro;
	ZButton m_btnOperatorTree;
	ZButton m_btnZoomOut;
	ZButton m_btnZoomNormal;
	ZButton m_btnZoomIn;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	double m_dScale;
	bool m_bIsShowTree;
	HANDLE m_hEvtExitUpdatePlan;
	bool m_bIsUpdatePlanRun;
	CMenu menuProTest;
	int m_nRowFirst;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonImportplan();
	afx_msg void OnBnClickedButtonUpdatenetplan();
	afx_msg void OnBnClickedButtonOperatortree();
	afx_msg void OnBnClickedButtonMeterpos();
	afx_msg void OnBnClickedButtonSinglepro();
	afx_msg void OnBnClickedButtonContinuepro();
	afx_msg void OnBnClickedButtonAllpro();
	afx_msg void OnBnClickedButtonStoppro();
	afx_msg void OnBnClickedButtonZoomout();
	afx_msg void OnBnClickedButtonZoomnormal();
	afx_msg void OnBnClickedButtonZoomin();
	void InitMenu(void);
	void InitList(void);
	void InsertList(void);
	void InitTree(void);
	void ExpandTree(CTreeCtrl & Tree,HTREEITEM hTreeItem);
	afx_msg void OnTvnSelchangedTreePlan(NMHDR *pNMHDR, LRESULT *pResult);
	void InsertPlan(const CString & strPlanPath);
	void SetListBlank(const int & nRowStart, const int & nRowEnd,const int & nColumnStart, const int & nColumnEnd);
	bool ResultCompare(CString strResultData, const CString & strCorrectData);
	bool ResultCompare645(const CString & strResult, const CString & strCorrectData);
	bool ResultCompareRMT(const CString & strResult, const CString & strCorrectData);
	bool ResultCompare698(const CString & strResult, const CString & strCorrectData, CMainData * p_md, CString & strText);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID,const CRect & rcParent);
	void CheckFirstRow(const int & nRowFirst);
	bool ExecError(CMainData * p_md,std::map<int,CString> & mapMeterError);
	void EnableBtn(const BOOL & bEnable);
	CString ExecTgtOrCorrectData(CMainData * p_md, const CString & strData);
	void ReplaceSubString(CString & strDest, const CString & strSub, const CString & strReplace);
	void LimitLength(CString & strValue, int nLen, bool bIsZeroBack);
	void ZoomList(const double & dScale);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	static UINT UpdatePlanThreadFunc(LPVOID lpParam);
	static bool SocketCommunication(const CString & strDataIn,CString & strDataOut);
	CString HexString2BinString(const CString & strHex);
	CString DecString2HexString(const CString & strDec);
	CString CharString2HexString(const CString & strChar);
};
