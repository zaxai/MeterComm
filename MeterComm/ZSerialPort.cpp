#include "StdAfx.h"
#include "ZSerialPort.h"


ZSerialPort::ZSerialPort(void)
	: m_hCom(INVALID_HANDLE_VALUE)
{
	memset(&m_ol,0,sizeof(OVERLAPPED));  
	if(m_ol.hEvent==NULL)
		m_ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL); 
}

ZSerialPort::~ZSerialPort(void)
{
	ClosePort();
	if(m_ol.hEvent)
		CloseHandle(m_ol.hEvent);
}

bool ZSerialPort::OpenPort(CString in_strPort)
{
	ClosePort();
	if(in_strPort.Find(_T("\\\\.\\"))<0)
		in_strPort=_T("\\\\.\\")+in_strPort;
	DWORD dwFlagsAndAttributes=FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED;
	m_hCom=CreateFile(in_strPort,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,dwFlagsAndAttributes,NULL); 
	if(m_hCom==INVALID_HANDLE_VALUE)  
		return false;
	return true;
}

void ZSerialPort::ClosePort(void)
{
	if(m_hCom!= INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
		m_hCom= INVALID_HANDLE_VALUE;
	}
}


int ZSerialPort::GetPortState(DCB & out_dcb)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=GetCommState(m_hCom,&out_dcb); 
	if(!bRtn)
		return ERROR_PORTSTATE;
	return ERROR_OK;
}


int ZSerialPort::SetPortState(DCB & in_dcb)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=SetCommState(m_hCom,&in_dcb); 
	if(!bRtn)  
		return ERROR_PORTSTATE;
	return ERROR_OK;
}

int ZSerialPort::SetPortState(const DWORD & in_dwBaudRate,const BYTE & in_ucParity,const BYTE & in_ucByteSize,const BYTE & in_ucStopBits)
{
	DCB dcb;
	int nRtn=GetPortState(dcb);
	if(nRtn)
		return nRtn;
	dcb.BaudRate=in_dwBaudRate;
	dcb.Parity=in_ucParity;
	dcb.ByteSize=in_ucByteSize;
	dcb.StopBits=in_ucStopBits;
	return SetPortState(dcb);
}
int ZSerialPort::SetPortState(const CString & in_strBaudRate,const CString & in_strParity,const CString & in_strByteSize,const CString & in_strStopBits)
{
	DWORD dwBaudRate;
	BYTE ucBytesize,ucParity,ucStopBits;
	dwBaudRate=_ttol(in_strBaudRate);
	ucBytesize=_ttoi(in_strByteSize);
	if(in_strParity==_T("None"))
		ucParity=NOPARITY ; 
	else if(in_strParity==_T("Even"))
		ucParity=EVENPARITY;
	else if(in_strParity==_T("Odd"))
		ucParity=ODDPARITY;
	else if(in_strParity==_T("Mark"))
		ucParity=MARKPARITY;
	else
		ucParity=SPACEPARITY;
	if(in_strStopBits==_T("1"))
		ucStopBits=ONESTOPBIT  ; 
	else if(in_strStopBits==_T("1.5"))
		ucStopBits=ONE5STOPBITS;
	else
		ucStopBits=TWOSTOPBITS;
	return SetPortState(dwBaudRate,ucParity,ucBytesize,ucStopBits);
}

int ZSerialPort::SetBufferSize(const DWORD & in_dwInBuffer, const DWORD & in_dwOutBuffer)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=SetupComm(m_hCom,in_dwInBuffer,in_dwOutBuffer);
	if(!bRtn)
		return ERROR_BUFFERSIZE;
	return ERROR_OK;
}

int ZSerialPort::GetPortTimeOut(COMMTIMEOUTS  & out_Timeout)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=GetCommTimeouts(m_hCom,&out_Timeout); 
	if(!bRtn)
		return ERROR_TIMEOUTS;
	return ERROR_OK;
}

int ZSerialPort::SetPortTimeOut(COMMTIMEOUTS  & in_Timeout)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=SetCommTimeouts(m_hCom,&in_Timeout); 
	if(!bRtn)
		return ERROR_TIMEOUTS;
	return ERROR_OK;
}

int ZSerialPort::SetPortTimeOut(const DWORD & in_dwReadIntTmout,const DWORD & in_dwReadTolTmoutMul,const DWORD & in_dwReadTolTmoutCst,const DWORD & in_dwWriteTolTmoutMul,const DWORD & in_dwWriteTolTmoutCst)
{
	COMMTIMEOUTS TimeOut;  
	TimeOut.ReadIntervalTimeout=in_dwReadIntTmout;  
	TimeOut.ReadTotalTimeoutMultiplier=in_dwReadTolTmoutMul;  
	TimeOut.ReadTotalTimeoutConstant=in_dwReadTolTmoutCst;  
	TimeOut.WriteTotalTimeoutMultiplier=in_dwWriteTolTmoutMul;  
	TimeOut.WriteTotalTimeoutConstant=in_dwWriteTolTmoutCst;  
	return SetPortTimeOut(TimeOut); 
}

int ZSerialPort::SetPortTimeOut(const CString & in_strReadIntTmout,const CString & in_strReadTolTmoutMul,const CString & in_strReadTolTmoutCst,const CString & in_strWriteTolTmoutMul,const CString & in_strWriteTolTmoutCst)
{
	DWORD dwReadIntTmout,dwReadTolTmoutMul,dwReadTolTmoutCst,dwWriteTolTmoutMul,dwWriteTolTmoutCst;
	dwReadIntTmout=_ttol(in_strReadIntTmout);
	dwReadTolTmoutMul=_ttol(in_strReadTolTmoutMul);
	dwReadTolTmoutCst=_ttol(in_strReadTolTmoutCst);
	dwWriteTolTmoutMul=_ttol(in_strWriteTolTmoutMul);
	dwWriteTolTmoutCst=_ttol(in_strWriteTolTmoutCst);
	return SetPortTimeOut(dwReadIntTmout,dwReadTolTmoutMul,dwReadTolTmoutCst,dwWriteTolTmoutMul,dwWriteTolTmoutCst); 
}

int ZSerialPort::PurgePort(void)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=PurgeComm(m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);  
	if(!bRtn)
		return ERROR_PURGE;
	return ERROR_OK;
}


int ZSerialPort::SendToPort(const BYTE * in_pSendData,const DWORD & in_dwSendLen,DWORD * out_p_dwActSendLen)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	DWORD dwActSendLen=0;
	BOOL bRtn=WriteFile(m_hCom,in_pSendData,in_dwSendLen,&dwActSendLen,&m_ol); 
	if(!bRtn)
	{
		if(GetLastError()!=ERROR_IO_PENDING)
			return ERROR_SEND;
	}
	if(out_p_dwActSendLen)
		*out_p_dwActSendLen=dwActSendLen;
	return ERROR_OK;
}

int ZSerialPort::SendToPort(const CString & in_strSendData)
{
	CString strSendData(in_strSendData);
	strSendData.Remove(_T(' '));
	int nMaxLen=strSendData.GetLength();
	if(nMaxLen==0)
		return ERROR_OK;
	BYTE *pSendData=new BYTE[nMaxLen];
	if(pSendData==NULL)
		return ERROR_OTHER;
	if(!CStringToBYTEArrBy2(strSendData, pSendData, nMaxLen))
	{
		delete []pSendData;
		return ERROR_OTHER;
	}
	DWORD dwSendLen=in_strSendData.GetLength()/2;
	int nRtn=SendToPort(pSendData,dwSendLen);
	delete []pSendData;
	return nRtn;
}

bool ZSerialPort::CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxArrayLen)
{
	CString strByte;
	int i=0,j=0,nLen;
	nLen=in_str.GetLength()/2;
	if(nLen>=in_nMaxArrayLen)
		return false;
	for(;i<nLen;++i,j=j+2)
	{
		strByte=in_str.Mid(j,2);
		out_pArray[i]=BYTE(_tcstol(strByte,NULL ,16));
	}
	out_pArray[nLen]='\0';
	return true;
}

int ZSerialPort::RecvFromPort(BYTE * out_pRecvData,const int & in_nMaxOutBuffer,DWORD & out_dwRecvLen)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	BOOL bRtn=ReadFile(m_hCom,out_pRecvData,in_nMaxOutBuffer,&out_dwRecvLen,&m_ol); 
	if(!bRtn)
	{
		if(GetLastError()!=ERROR_IO_PENDING)
			return ERROR_RECV;
	}
	return ERROR_OK;
}

int ZSerialPort::RecvFromPort(CString & out_strRecvData,const int & in_nMaxOutBuffer)
{
	BYTE *pOutArray=new BYTE[in_nMaxOutBuffer];
	if(pOutArray==NULL)
		return ERROR_OTHER;
	DWORD dwRecvLen=0;
	int nRtn=RecvFromPort(pOutArray,in_nMaxOutBuffer,dwRecvLen);
	if(nRtn)
	{
		delete []pOutArray;
		return nRtn;
	}
	BYTEArrToCStringAsHex(pOutArray,dwRecvLen,out_strRecvData);
	delete []pOutArray;
	return ERROR_OK;
}

void ZSerialPort::BYTEArrToCStringAsHex(const BYTE *in_pArray,const DWORD & in_dwArrayLen,CString & out_str)
{
	out_str.Empty();
	CString strByte;
	for(DWORD i=0;i<in_dwArrayLen;++i)
	{
		strByte.Format(_T("%02X"),in_pArray[i]);
		out_str+=strByte;
	}
}

int ZSerialPort::ClearPortError(DWORD & dwInBytesExist,DWORD & dwOutBytesExist)
{
	if(m_hCom== INVALID_HANDLE_VALUE)
		return ERROR_INVALIDPORT;
	COMSTAT ComStat; 
	DWORD dwErrorFlags;
	BOOL bRtn=ClearCommError(m_hCom,&dwErrorFlags,&ComStat);   
	if(!bRtn)
		return ERROR_BUFFERSTATE;
	dwInBytesExist=ComStat.cbInQue;
	dwOutBytesExist=ComStat.cbOutQue;
	return ERROR_OK;
}

DWORD ZSerialPort::GetErrorInfo(void)
{
	return GetLastError();
}