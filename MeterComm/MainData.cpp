#include "StdAfx.h"
#include "MainData.h"

int CMainData::m_nBarCodeLength=0;
int CMainData::m_nMeterAddStart=0;
int CMainData::m_nMeterAddEnd=0;
int CMainData::m_nMeterNOStart=0;
int CMainData::m_nMeterNOEnd=0;
int CMainData::m_nPropertyNOStart=0;
int CMainData::m_nPropertyNOEnd=0;

CMainData::CMainData(void)
	: m_p_zdp(NULL)
	, m_p_protl(NULL)
	, m_p_dlt645(NULL)
	, m_p_rmtProtl(NULL)
	, m_p_dlt698(NULL)
	, m_pf_checkcall(NULL)
	, m_bIsThreadRun(true)
	, m_hEvtThread(NULL)
	, m_hEvtStop(NULL)
	, m_hEvtExit(NULL)
	, m_p_MainDlg(NULL)
	, m_strDataSend(_T(""))
	, m_strDataRecv(_T(""))
	, m_nRtn(0)
	, m_p_WndOfSource(NULL)
	, m_strError(_T(""))
	, m_strDataValue(_T(""))
	, m_nRow(0)
	, m_nRowMax(0)
	, m_nColumn(0)
	, m_nProtocolType(0)
	, m_nUserDefined(0)
	, m_nFlagGetSendData(0)
	, m_nFlagEncode(0)
	, m_nFlagUpdateSendData(0)
	, m_nFlagUpdateRecvData(0)
	, m_nFlagEndOnceLoop(0)
	, m_dwThreadTimeWait(0)
	, m_strTgtMeterAddr(_T(""))
	, m_strTgtMeterNO(_T(""))
	, m_strTgtPropertyNO(_T(""))
{
}


CMainData::~CMainData(void)
{
}


void CMainData::ClearMD(void)
{
	m_bIsThreadRun=false;
	m_nRow=m_nRowMax;
	if(m_p_zdp!=NULL)
		m_p_zdp->StopComm();
	if(m_hEvtThread!=NULL)
		SetEvent(m_hEvtThread);
	WaitForSingleObject(m_hEvtExit, INFINITE);
	if(m_hEvtThread!=NULL)
	{
		CloseHandle(m_hEvtThread);
		m_hEvtThread=NULL;
	}
	if(m_hEvtStop!=NULL)
	{
		CloseHandle(m_hEvtStop);
		m_hEvtStop=NULL;
	}
	if(m_hEvtExit!=NULL)
	{
		CloseHandle(m_hEvtExit);
		m_hEvtExit=NULL;
	}
	if(m_p_zdp!=NULL)
	{
		delete m_p_zdp;
		m_p_zdp=NULL;
	}
	if(m_p_dlt645!=NULL)
	{
		delete m_p_dlt645;
		m_p_dlt645=NULL;
	}
	if(m_p_rmtProtl!=NULL)
	{
		delete m_p_rmtProtl;
		m_p_rmtProtl=NULL;
	}
	if(m_p_dlt698!=NULL)
	{
		delete m_p_dlt698;
		m_p_dlt698=NULL;
	}
}


bool CMainData::InitMD(const CString  & strPort)
{
	if(m_p_zdp==NULL)
		m_p_zdp=new ZDevicePort;
	if(m_p_dlt645==NULL)
		m_p_dlt645=new ZFac645;
	if(m_p_rmtProtl==NULL)
		m_p_rmtProtl=new ZRMTProtocol;
	if(m_p_dlt698==NULL)
		m_p_dlt698=new ZDLT698_45;
	bool bRtn=m_p_zdp->OpenPort(strPort);
	if(m_hEvtThread==NULL)
		m_hEvtThread =CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_hEvtStop==NULL)
		m_hEvtStop =CreateEvent(NULL,TRUE,TRUE,NULL);
	if(m_hEvtExit==NULL)
		m_hEvtExit =CreateEvent(NULL,TRUE,TRUE,NULL);
	m_bIsThreadRun=true;
	return bRtn;
}
