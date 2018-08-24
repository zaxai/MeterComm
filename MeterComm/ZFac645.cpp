#include "StdAfx.h"
#include "ZFac645.h"


ZFac645::ZFac645(void)
{
}


ZFac645::~ZFac645(void)
{
}


int ZFac645::DataEncode(CString & strData,CString & strError)
{
	m_strMeterAddr.MakeUpper();
	m_strDataItem.MakeUpper();
	m_strTgtData.MakeUpper();
	CString str68(_T("68"));
	CString strFMeterAddr,strFDataItem,strFCtrlCode,strFLength,strFGrade,strFPsWd,strFOperatorCode,strFTargetData,strFCheckSum,strFDiv,strFRand1,strFEndata1,strFEndata2;
	strFCtrlCode=m_strCtrlCode;
	strFMeterAddr=ReverseCStringBy2(m_strMeterAddr);
	strFDataItem=ReverseCStringBy2Plus33(m_strDataItem);
	if(m_strCtrlCode==_T("3A"))
	{
		m_strLength.Format(_T("%02X"),m_strDataItem.GetLength()/2);
		strFLength=m_strLength;
		strData=str68+strFMeterAddr+str68+strFCtrlCode+strFLength+strFDataItem;
	}
	else if(m_strCtrlCode==_T("3D"))
	{
		m_strLength.Format(_T("%02X"),m_strDataItem.GetLength()/2);
		strFLength=m_strLength;
		strData=str68+strFMeterAddr+str68+strFCtrlCode+strFLength+strFDataItem;
	}
	else if(m_strCtrlCode==_T("1F"))
	{
		CString strDataArea;
		strFTargetData=ReverseCStringBy2Plus33(m_strTgtData);
		strDataArea=strFDataItem+strFTargetData;
		m_strLength.Format(_T("%02X"),strDataArea.GetLength()/2);
		strFLength=m_strLength;
		strData=str68+strFMeterAddr+str68+strFCtrlCode+strFLength+strFDataItem+strFTargetData;
	}
	else
	{
		return ZDLT645_2007::DataEncode(strData,strError);
	}
	strFCheckSum=GetCheckSum(strData);
	strData=_T("FEFEFEFE")+strData+strFCheckSum+_T("16");
	return 0;
}


bool ZFac645::DataDecode(CString strData,CString & strValue)
{
	strData.Remove(_T(' '));
	strData=strData.Mid(strData.Find(_T("68")));
	if(!Check645_2007(strData))
		return false;
	strValue.Empty(); 
	CString strDataItem;
	CString strCtrlCode=strData.Mid(16,2);
	CString strExt=_T("-")+strCtrlCode+_T("H");
	if(strCtrlCode==_T("3A"))
	{
		strValue=strData.Mid(20,8);
		strValue=ReverseCStringBy2Minus33(strValue);
		strValue=strValue+strExt;
	}
	else if(strCtrlCode==_T("BA"))
		strValue=_T("设置成功")+strExt;
	else if(strCtrlCode==_T("3D"))
	{
		strValue=strData.Mid(20,8);
		strValue=ReverseCStringBy2Minus33(strValue);
		strValue=strValue+strExt;
	}
	else if(strCtrlCode==_T("BD"))
		strValue=_T("设置成功")+strExt;
	else if(strCtrlCode==_T("1F"))
	{
		switch(_ttoi(strData.Mid(18,2)))
		{
		case 3:
			strValue=strData.Mid(20,4);
			break;
		default:
			strValue=strData.Mid(20,8);
		}
		strValue=ReverseCStringBy2Minus33(strValue);
		strValue=strValue+strExt;
	}
	else if(strCtrlCode==_T("9F"))
		strValue=_T("设置成功")+strExt;
	else
		return ZDLT645_2007::DataDecode(strData,strValue);
	return true;
}


CString ZFac645::GetLength(void)
{
	return ZDLT645_2007::GetLength();
}