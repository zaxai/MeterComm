
// MeterCommView.h : CMeterCommView 类的接口
//

#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "MeterCommDoc.h"
#include "zdlt645_2007.h"

class CComPortSetting;
class CProduceTesting;
class CMeterTesting;
class CMeterCommView : public CFormView
{
protected: // 仅从序列化创建
	CMeterCommView();
	DECLARE_DYNCREATE(CMeterCommView)

public:
	enum{ IDD = IDD_METERCOMM_FORM };

// 特性
public:
	CMeterCommDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CMeterCommView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listDisplay;
	int m_nListCount;
	ZButton m_btnGetAddr;
	ZButton m_btnSend;
	ZButton m_btnGetEsamID;
	ZButton m_btnGetDataItem;
	ZButton m_btnStop;
	ZButton m_btnGetMeterNO;
	ZButton m_btnClearData;
	CComboBox m_comboProtocol;
	CComboBox m_comboFuncCode;
	CComPortSetting *m_p_comportset;
	CProduceTesting *m_p_protest;
	CMeterTesting *m_p_metertest;
	HANDLE m_hEvtExitCheckUpdate;
	HANDLE m_hEvtExitUpdateDIDB;
	bool m_bIsInitFinish;
	int m_nDllSel;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonGetaddr();
	afx_msg void OnBnClickedButtonGetdataitem();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonGetidparam();
	afx_msg void OnBnClickedButtonGetesamid();
	afx_msg void OnBnClickedButtonCleardata();
	void InitOperatorArea(void);
	void InitDisplayArea(void);
	static UINT PortCommThreadFunc(LPVOID lpParam);
	void Start(void);
	void UpdateList(const CString & strFlag, const CString & strData,CMainData * p_md);
	void UpdateEdit(const bool & bRight,const CString & strData,CMainData * p_md);
	CString DataFormat(const CString & str);
	afx_msg void OnSettingComport();
	afx_msg void OnSettingParameter();
	afx_msg void OnSettingNetwork();
	afx_msg void OnTestingProduce();
	afx_msg void OnTestingMeter();
	void AddClipSiblings(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID,const CRect & rcParent);
	void EnableBtn(const BOOL & bEnable);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNMClickListDisplay(NMHDR *pNMHDR, LRESULT *pResult);
	static UINT CheckSoftwareUpdateThreadFunc(LPVOID lpParam);
	static bool SocketCommunication(const CString & strDataIn,CString & strDataOut);
	afx_msg void OnHelpingUpdate();
	afx_msg void OnSettingDidatabase();
	static UINT UpdateDIDBThreadFunc(LPVOID lpParam);
	afx_msg void OnCbnSelchangeComboProtocol();
	void InitFuncCode(int nProtocol);
	void ReadGlobalVariable();
	void WriteGlobalVariable();
	BOOL InitGlobalVariableDB();
};

#ifndef _DEBUG  // MeterCommView.cpp 中的调试版本
inline CMeterCommDoc* CMeterCommView::GetDocument() const
   { return reinterpret_cast<CMeterCommDoc*>(m_pDocument); }
#endif

