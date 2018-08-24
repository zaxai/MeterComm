
// MeterComm.h : MeterComm 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "MyFuncLib.h"
#include "MyClassLib.h"
#include <vector>

// CMeterCommApp:
// 有关此类的实现，请参阅 MeterComm.cpp
//

class CMeterCommApp : public CWinAppEx
{
public:
	CMeterCommApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMeterCommApp theApp;

enum WND_POINT
{
	WND_COMPORTSET = 0,
	WND_MAINFRAME,
	WND_METERCOMM,
	WND_METERPOS,
	WND_METERTEST,
	WND_PRODUCETEST,
};

extern CWnd * g_sz_p_wnd[6];
extern std::vector <CMainData> g_vec_md;
extern CMainData g_mdMeterDevice;
extern int g_nTotalPort;
extern int g_nFstPort;
extern BOOL g_nIsUsingMD;
extern std::vector<bool> g_vec_bIsSelect;
extern int g_nFstMeterIndex;
extern int g_nTotalSelectedPort;
extern bool g_bIsStop;
extern int g_nMaxTotalPort;
extern CGlobalVariable g_v;//全局变量对象
extern CString g_strVersion;

#define WM_MSGRECVPRO WM_USER+1

enum MSGUSER
{
	MSGUSER_GETSENDDATA=0,
	MSGUSER_ENCODE,
	MSGUSER_UPDATESENDDATA,
	MSGUSER_TRANSENDDATA,
	MSGUSER_UPDATERECVDATA,
	MSGUSER_TRANRECVDATA,
	MSGUSER_ENDONCELOOP,
	MSGUSER_PORTOPEN,
	MSGUSER_PORTCLOSE,
	MSGUSER_ENABLEBTN,
	MSGUSER_GETSERVERINFO,
	MSGUSER_ERROFGETSERVERINFO,
	MSGUSER_ISNEEDUPDATE,
	MSGUSER_UPDATEDIDB,
	MSGUSER_UPDATEMETERADD,
	MSGUSER_ALLPRO,
	MSGUSER_UPDATENETPLAN,
	MSGUSER_ENDUPDATENETPLAN,
	MSGUSER_UPDATELATESTVERSION,
	MSGUSER_SP_EDITKILLFOCUS=1000
};

enum THREADFLAG
{
	FLAG_NONE=0,
	FLAG_THREADWAIT,
	FLAG_GOTOENCODE,
	FLAG_GOTOSEND,
	FLAG_GOTORECV
};