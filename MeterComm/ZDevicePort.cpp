#include "StdAfx.h"
#include "ZDevicePort.h"


ZDevicePort::ZDevicePort(void)
	: m_nOnceBytesSend(1000)
	, m_nOnceBytesRecv(1000)
	, m_nTimeOutSend(3000)
	, m_nTimeOutRecv(3000)
	, m_bIsStopSend(false)
	, m_bIsStopRecv(false)
	, m_strPort(_T(""))
	, m_hEvtStop(NULL)
	, m_hEvtExitComm(NULL)
{
	m_hEvtStop=CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hEvtExitComm=CreateEvent(NULL,TRUE,TRUE,NULL);
}


ZDevicePort::~ZDevicePort(void)
{
	if(m_hEvtStop!=NULL)
	{
		CloseHandle(m_hEvtStop);
		m_hEvtStop=NULL;
	}
	if(m_hEvtExitComm!=NULL)
	{
		CloseHandle(m_hEvtExitComm);
		m_hEvtExitComm=NULL;
	}
	m_strPort.Empty();
}

bool ZDevicePort::OpenPort(const CString & in_strPort)
{
	bool bRtn=ZSerialPort::OpenPort(in_strPort);
	if(bRtn)
	{
		SetPortTimeOut(MAXDWORD,0,0,0,0);
		SetBufferSize(m_nOnceBytesSend*2,m_nOnceBytesRecv*2);
		m_strPort=in_strPort;
	}
	return bRtn;
}

void ZDevicePort::ClosePort(void)
{
	ZSerialPort::ClosePort();
	m_strPort.Empty();
}

int ZDevicePort::SendToPort(const CString & in_strSendData)
{
	ResetEvent(m_hEvtExitComm);
	int nRtn= ERROR_OK,i=0;
	unsigned int nStart,nSpan;
	DWORD dwIn=0,dwOut=0;
	nStart=GetTickCount();
	ResetEvent(m_hEvtStop);
	m_bIsStopSend=false;
	CString strSend=in_strSendData.Mid(i,m_nOnceBytesSend*2);
	while(!m_bIsStopSend)
	{
		ClearPortError(dwIn,dwOut);
		if(dwOut)
			WaitForSingleObject(m_hEvtStop,10);
		else
		{
			nRtn=ZSerialPort::SendToPort(strSend);
			if(nRtn)
				break;
			else
			{
				i+=m_nOnceBytesSend*2;
				strSend=in_strSendData.Mid(i,m_nOnceBytesSend*2);
				if(strSend.IsEmpty())
					break;
			}
		}
		nSpan=unsigned int(GetTickCount())-nStart;
		if(nSpan>=m_nTimeOutSend)
		{
			nRtn= ERROR_SEND_TIMEOUT;
			break;
		}
	}
	if(m_bIsStopSend)
		nRtn= ERROR_SEND_STOP;
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

int ZDevicePort::RecvFromPort(CString & out_strRecvData,CheckCall checkcall)
{
	ResetEvent(m_hEvtExitComm);
	out_strRecvData.Empty();
	CString strRecvOnce;
	int nRtn= ERROR_OK;
	unsigned int nStart,nSpan;
	DWORD dwIn=0,dwOut=0;
	nStart=GetTickCount();
	ResetEvent(m_hEvtStop);
	m_bIsStopRecv=false;
	while(!m_bIsStopRecv)
	{
		ClearPortError(dwIn,dwOut);
		if(dwIn)
		{
			nRtn=ZSerialPort::RecvFromPort(strRecvOnce,m_nOnceBytesRecv);
			if(nRtn)
				break;
			else
			{
				out_strRecvData+=strRecvOnce; 
				if(checkcall(out_strRecvData))
					break;
			}
		}
		else
			WaitForSingleObject(m_hEvtStop,10);
		nSpan=unsigned int(GetTickCount())-nStart;
		if(nSpan>=m_nTimeOutRecv)
		{
			nRtn= ERROR_RECV_TIMEOUT;
			break;
		}
	}
	if(m_bIsStopRecv)
		nRtn= ERROR_RECV_STOP;
	SetEvent(m_hEvtExitComm);
	return nRtn;
}

void ZDevicePort::SetOnceBytesSend(const unsigned int & in_nBytes)
{
	m_nOnceBytesSend=in_nBytes;
	SetBufferSize(m_nOnceBytesSend*2,m_nOnceBytesRecv*2);
}

void ZDevicePort::SetOnceBytesRecv(const unsigned int & in_nBytes)
{
	m_nOnceBytesRecv=in_nBytes;
	SetBufferSize(m_nOnceBytesSend*2,m_nOnceBytesRecv*2);
}

void ZDevicePort::SetTimeOutSend(const unsigned int & in_nTimeOut)
{
	m_nTimeOutSend=in_nTimeOut;
}

void ZDevicePort::SetTimeOutRecv(const unsigned int & in_nTimeOut)
{
	m_nTimeOutRecv=in_nTimeOut;
}

void ZDevicePort::StopComm(void)
{
	m_bIsStopSend=true;
	m_bIsStopRecv=true;
	SetEvent(m_hEvtStop);
	WaitForSingleObject(m_hEvtExitComm,INFINITE);
}

CString ZDevicePort::GetPort(void)
{
	return m_strPort;
}