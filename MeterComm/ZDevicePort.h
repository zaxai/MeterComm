/*
** Version  1.0.0.1
** Date     2018.12.01
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "ZSerialPort.h"
typedef bool (*CheckCall)(CString str);

class ZDevicePort :public ZSerialPort
{
private:
	unsigned int m_nOnceBytesSend;
	unsigned int m_nOnceBytesRecv;
	unsigned int m_nTimeOutSend;
	unsigned int m_nTimeOutRecv;
	bool m_bIsStopSend;
	bool m_bIsStopRecv;
	HANDLE m_hEvtStop;
	HANDLE m_hEvtExitComm;
	CString m_strPort;
public:
	ZDevicePort(void);
	virtual ~ZDevicePort(void);
	virtual bool OpenPort(const CString & in_strPort);
	virtual void ClosePort(void);
	virtual int SendToPort(const CString & in_strSendData);
	int RecvFromPort(CString & out_strRecvData,CheckCall checkcall);
	void SetOnceBytesSend(const unsigned int & in_nBytes);
	void SetOnceBytesRecv(const unsigned int & in_nBytes);
	void SetTimeOutSend(const unsigned int & in_nTimeOut);
	void SetTimeOutRecv(const unsigned int & in_nTimeOut);
	void StopComm(void);
	CString GetPort(void);
	enum ERRORINFO
	{
		ERROR_SEND_TIMEOUT = 10,//发送超时
		ERROR_SEND_STOP,//发送停止
		ERROR_RECV_TIMEOUT,//接收超时
		ERROR_RECV_STOP//接收停止
	};
};

