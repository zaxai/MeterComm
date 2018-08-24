#pragma once
class CGlobalVariable
{
public:
	CGlobalVariable(void);
	~CGlobalVariable(void);
	int g_nProtocolType; 
	CString g_strCtrlArea;
	CString g_strDeviceAddr;
	CString g_strFuncCode;
	CString g_strLength;
	CString g_strDataItem;
	CString g_strTgtData;
	CString g_strGrade;
	CString g_strPsWd;
	CString g_strOperaCode;
	int g_nDllType;
	BOOL g_nIsRemote;
	CString g_strServerIP;
	CString g_strServerPort;
	CString g_strServerTimeOut;
	CString g_strNetCptIP;
	CString g_strNetCptPort;
	CString g_strNetCptTimeOut;
	CString Get645Param(void);
	CString Get698Param(void);
};

