#pragma once
class ZSerialPort
{
private:
	HANDLE m_hCom;
	OVERLAPPED m_ol;
	ZSerialPort(const ZSerialPort & zsp);
	ZSerialPort & operator = (const ZSerialPort & zsp);
public:
	ZSerialPort(void);
	virtual ~ZSerialPort(void);
	virtual bool OpenPort(CString in_strPort);//使用异步串口
	virtual void ClosePort(void);
	int GetPortState(DCB & out_dcb);
	int SetPortState(DCB & in_dcb);
	int SetPortState(const DWORD & in_dwBaudRate,const BYTE & in_ucParity,const BYTE & in_ucByteSize,const BYTE & in_ucStopBits);
	int SetPortState(const CString & in_strBaudRate,const CString & in_strParity,const CString & in_strByteSize,const CString & in_strStopBits);
	int SetBufferSize(const DWORD & in_dwInBuffer, const DWORD & in_dwOutBuffer);
	int GetPortTimeOut(COMMTIMEOUTS  & out_Timeout);
	int SetPortTimeOut(COMMTIMEOUTS  & in_Timeout);
	int SetPortTimeOut(const DWORD & in_dwReadIntTmout,const DWORD & in_dwReadTolTmoutMul,const DWORD & in_dwReadTolTmoutCst,const DWORD & in_dwWriteTolTmoutMul,const DWORD & in_dwWriteTolTmoutCst);
	int SetPortTimeOut(const CString & in_strReadIntTmout,const CString & in_strReadTolTmoutMul,const CString & in_strReadTolTmoutCst,const CString & in_strWriteTolTmoutMul,const CString & in_strWriteTolTmoutCst);
	int PurgePort(void);
	int SendToPort(const BYTE * in_pSendData,const DWORD & in_dwSendLen,DWORD * out_p_dwActSendLen=NULL);
	virtual int SendToPort(const CString & in_strSendData);
	bool CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxOutBuffer);
	int RecvFromPort(BYTE * out_pRecvData,const int & in_nMaxOutBuffer,DWORD & out_dwActOutBuffer);
	int RecvFromPort(CString & out_strRecvData,const int & in_nMaxOutBuffer);
	void BYTEArrToCStringAsHex(const BYTE *in_pArray,const DWORD & in_dwArrayLen,CString & out_str);
	int ClearPortError(DWORD & dwInBytesExist,DWORD & dwOutBytesExist);
	DWORD GetErrorInfo(void);
	enum ERRORINFO
	{
		ERROR_OK=0,//成功
		ERROR_INVALIDPORT,//无效端口
		ERROR_PORTSTATE,//操作端口DCB出错
		ERROR_BUFFERSIZE,//操作缓冲区出错
		ERROR_TIMEOUTS,//操作超时出错
		ERROR_PURGE,//清空缓冲区出错
		ERROR_SEND,//发送出错
		ERROR_RECV,//接收出错
		ERROR_OTHER,//其他出错
		ERROR_BUFFERSTATE//查看缓冲区状态出错
	};
};

