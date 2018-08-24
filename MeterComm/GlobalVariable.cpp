#include "StdAfx.h"
#include "GlobalVariable.h"


CGlobalVariable::CGlobalVariable(void)
	: g_nProtocolType(0)
	, g_strCtrlArea(_T("43"))
	, g_strDeviceAddr(_T("AAAAAAAAAAAA"))
	, g_strFuncCode(_T(""))
	, g_strLength(_T(""))
	, g_strDataItem(_T(""))
	, g_strTgtData(_T(""))
	, g_strGrade(_T("02"))
	, g_strPsWd(_T("123456"))
	, g_strOperaCode(_T("00000000"))
	, g_nDllType(2)
	, g_nIsRemote(FALSE)
	, g_strServerIP(_T("zaxai.nat123.net"))
	, g_strServerPort(_T("46960"))
	, g_strServerTimeOut(_T("5000"))
	, g_strNetCptIP(_T("10.0.17.99"))
	, g_strNetCptPort(_T("6666")) 
	, g_strNetCptTimeOut(_T("5000"))
{
}


CGlobalVariable::~CGlobalVariable(void)
{
}


CString CGlobalVariable::Get645Param(void)
{
	CString strRtn,strDllType,strIsRemote;
	CString str(_T(","));
	strDllType.Format(_T("%d"),g_nDllType);
	strIsRemote.Format(_T("%d"),g_nIsRemote);
	strRtn=g_strDeviceAddr+str+g_strFuncCode+str+g_strDataItem+str+g_strTgtData+str+g_strGrade+str+g_strPsWd+str+g_strOperaCode+str
		+g_strServerIP+str+g_strServerPort+str+g_strServerTimeOut+str+g_strNetCptIP+str+g_strNetCptPort+str+g_strNetCptTimeOut+str+strDllType+str+strIsRemote+str;
	return strRtn;
}

CString CGlobalVariable::Get698Param(void)
{
	CString strRtn, strDllType, strIsRemote;
	CString str(_T(","));
	strDllType.Format(_T("%d"), g_nDllType);
	strIsRemote.Format(_T("%d"), g_nIsRemote);
	strRtn=g_strCtrlArea+str+g_strDeviceAddr+str+g_strFuncCode+str+g_strDataItem+str+g_strTgtData+str+g_strGrade+str+g_strPsWd+str+g_strOperaCode+str
		+g_strServerIP+str+g_strServerPort+str+g_strServerTimeOut+str+g_strNetCptIP+str+g_strNetCptPort+str+g_strNetCptTimeOut+str + strDllType + str + strIsRemote + str;
	return strRtn;
}