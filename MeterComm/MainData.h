#pragma once
#include "ZDevicePort.h"
#include "ZFac645.h"
#include "ZRMTProtocol.h"
#include "ZDLT698_45.h"

class CMainData
{
public:
	CMainData(void);
	~CMainData(void);
	ZDevicePort * m_p_zdp;
	ZProtocol * m_p_protl;
	ZDLT645_2007 * m_p_dlt645;
	ZRMTProtocol * m_p_rmtProtl;
	ZDLT698_45 * m_p_dlt698;
	CheckCall m_pf_checkcall;
	bool m_bIsThreadRun;
	HANDLE m_hEvtThread;
	HANDLE m_hEvtStop;
	HANDLE m_hEvtExit;
	CWnd *m_p_MainDlg;
	void ClearMD(void);
	bool InitMD(const CString  & strPort);
	CString m_strDataSend;
	CString m_strDataRecv;
	int m_nRtn;
	CWnd *m_p_WndOfSource;
	CString m_strError;
	CString m_strDataValue;
	int m_nRow;
	int m_nRowMax;
	int m_nColumn;
	int m_nProtocolType;
	int m_nUserDefined;
	int m_nFlagGetSendData;
	int m_nFlagEncode;
	int m_nFlagUpdateSendData;
	int m_nFlagUpdateRecvData;
	int m_nFlagEndOnceLoop;
	DWORD m_dwThreadTimeWait;
	static int m_nBarCodeLength;
	static int m_nMeterAddStart;
	static int m_nMeterAddEnd;
	static int m_nMeterNOStart;
	static int m_nMeterNOEnd;
	static int m_nPropertyNOStart;
	static int m_nPropertyNOEnd;
	CString m_strTgtMeterAddr;
	CString m_strTgtMeterNO;
	CString m_strTgtPropertyNO;
	enum TYPE
	{
		TYPE_METER=0, //表位串口
		TYPE_METERDEVICE //台体串口
	};
	enum PROTOCOL
	{
		PROTOCOL_DLT645=0, //645协议
		PROTOCOL_RMT, //瑞科台体协议
		PROTOCOL_DLT698
	};
	enum USERDEFINED
	{
		UD_MULTI=0, //串口并发
		UD_ONEBYONE, //逐个执行
		UD_ROWTOGETHER, //单行同步执行
		UD_GOTOFIRST //回到选择的首行
	};
};

