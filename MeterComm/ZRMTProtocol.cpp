#include "StdAfx.h"
#include "ZRMTProtocol.h"

const TCHAR tcSplitChar = _T('#');
const TCHAR tcSplitError = _T('$');
const TCHAR tcSplitValue = _T('@');

ZRMTProtocol::ZRMTProtocol(void)
	: m_strOrderType(_T(""))
	, m_strLength(_T(""))
	, m_strTgtData(_T(""))
	, m_fUb(0)
	, m_fIb(0)
	, m_fIMax(0)
	, m_fPulse(0)
	, m_nMode(0)
	, m_nPhase(0)
	, m_fPF(0)
	, m_fUcurPer(0)
	, m_fIcurPer(0)
	, m_nTurns(0)
	, m_nStabTime(0)
	, m_nClockErrorSpan(0)
{
}


ZRMTProtocol::~ZRMTProtocol(void)
{
}

int ZRMTProtocol::DataEncode(CString & strData,CString & strError)
{
	m_strOrderType.MakeUpper();
	m_strTgtData.MakeUpper();
	CString strA0(_T("A0"));
	CString strFOrderType,strFLength,strFTgtData;
	strFOrderType=m_strOrderType;
	if(m_strOrderType==_T("00"))
	{
		m_strLength.Format(_T("%02X"),0);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength;
	}
	else if(m_strOrderType==_T("03"))
	{
		m_strLength.Format(_T("%02X"),0);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength;
	}
	else if(m_strOrderType==_T("10"))
	{
		strFTgtData=m_strTgtData;
		int nRtn=Type10Pro(strFTgtData);
		if(nRtn)
		{
			switch(nRtn)
			{
			case 1:
			case 2:
			case 3:
				strError.Format(_T("error:10%c参数%d超过台体上限！"), tcSplitError,nRtn);
				break;
			case 4:
				strError.Format(_T("error:10%c参数2不能大于参数3！"),tcSplitError);
				break;
			case 5:
				strError.Format(_T("error:10%c参数%d模式不匹配！"), tcSplitError,nRtn);
				break;
			case 6:
				strError.Format(_T("error:10%c参数个数不匹配(参数列表应为：电压#基本电流#最大电流#有功常数#测量模式)"),tcSplitError);
				break;
			}
			return 1;
		}
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else if(m_strOrderType==_T("11"))
	{
		strFTgtData=m_strTgtData;
		int nRtn=Type11Pro(strFTgtData);
		if(nRtn)
		{
			switch(nRtn)
			{
			case 1:
				strError.Format(_T("error:11%c参数个数不匹配！"),tcSplitError);
				break;
			case 2:
				strError.Format(_T("error:11%c参数2不匹配！"),tcSplitError);
				break;
			}
			return 2;
		}
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else if(m_strOrderType==_T("15"))
	{
		strFTgtData=m_strTgtData;
		Type15Pro(strFTgtData);
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else if(m_strOrderType==_T("16"))
	{
		strFTgtData.Format(_T("%02X"),_ttoi(m_strTgtData));
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else if(m_strOrderType==_T("20"))
	{
		strFTgtData=m_strTgtData;
		int nRtn=Type20Pro(strFTgtData);
		if(nRtn)
		{
			switch(nRtn)
			{
			case 3:
				strError.Format(_T("error:20%c参数%d超过台体上限！"),tcSplitError,nRtn);
				break;
			case 1:
			case 2:
			case 4:
			case 5:
			case 6:
				strError.Format(_T("error:20%c参数%d不匹配！"),tcSplitError,nRtn);
				break;
			case 7:
				strError.Format(_T("error:20%c参数个数不匹配(参数列表应为：合相分相#功率因数#电压%#电流%#圈数#稳定时间)"),tcSplitError);
				break;

			}
			return 3;
		}
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else if(m_strOrderType==_T("22"))
	{
		m_strLength.Format(_T("%02X"),0);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength;
	}
	else if(m_strOrderType==_T("23"))
	{
		strFTgtData=m_strTgtData;
		int nRtn=Type23Pro(strFTgtData);
		if(nRtn)
		{
			switch(nRtn)
			{
			case 3:
				strError.Format(_T("error:23%c参数%d超过台体上限！"),tcSplitError,nRtn);
				break;
			case 1:
			case 2:
			case 4:
				strError.Format(_T("error:23%c参数%d不匹配！"),tcSplitError,nRtn);
				break;
			case 5:
				strError.Format(_T("error:23%c参数个数不匹配(参数列表应为：合相分相#功率因数#电压%#电流%#圈数#稳定时间)"),tcSplitError);
				break;

			}
			return 3;
		}
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else if(m_strOrderType==_T("33"))
	{
		m_nClockErrorSpan=_ttoi(m_strTgtData);
		strFTgtData.Format(_T("%02X"),m_nClockErrorSpan);
		m_strLength.Format(_T("%02X"),strFTgtData.GetLength()/2);
		strFLength=m_strLength;
		strData=strA0+strFOrderType+strFLength+strFTgtData;
	}
	else
	{
		strError.Format(_T("error:99%c命令类型不识别!"),tcSplitError);
		return 99;
	}
	strData+=_T("A5");
	return 0;
}

int ZRMTProtocol::Type23Pro(CString & strData)
{
	std::vector<CString> vec_strData; 
	std::vector<CString> vec_strDataTemp;
	std::vector<float> vec_fData;
	StrSplit_Z(strData,vec_strData,tcSplitChar,strData.Right(1)==tcSplitChar);
	if(int(vec_strData.size())!=4)
		return 5;
	for(int i=2;i<4;++i)
	{
		vec_strDataTemp.push_back(vec_strData[i]);
	}
	VecHexStr2VecFloat2Str(vec_strDataTemp,vec_fData,2,strData);
	m_fUcurPer=vec_fData[0];
	m_fIcurPer=vec_fData[1];
	m_nPhase=_ttoi(vec_strData[0]);
	CString strPFTemp=vec_strData[1];
	if(m_nPhase<0||m_nPhase>3)
		return 1;
	if(strPFTemp!=_T("1.0")&&strPFTemp!=_T("0.5L")&&strPFTemp!=_T("0.8C")&&strPFTemp!=_T("0"))
		return 2;
	if(m_fUcurPer*m_fUb/100>280.0)
		return 3;
	if(m_fIcurPer*m_fIb/100>m_fIMax)
		return 4;
	int nPFTemp;
	if(strPFTemp==_T("0.5L"))
		nPFTemp=1;
	else if(strPFTemp==_T("1.0"))
		nPFTemp=4;
	else if(strPFTemp==_T("0.8C"))
		nPFTemp=5;
	else
		nPFTemp=9;
	m_fPF=float(_ttof(strPFTemp));
	vec_strData[0].Format(_T("%02X"),m_nPhase);
	vec_strData[1].Format(_T("%02X"),nPFTemp);
	strData=vec_strData[0]+vec_strData[1]+strData;
	return 0;
}

int ZRMTProtocol::Type20Pro(CString & strData)
{
	std::vector<CString> vec_strData; 
	std::vector<CString> vec_strDataTemp;
	std::vector<float> vec_fData;
	StrSplit_Z(strData,vec_strData,tcSplitChar,strData.Right(1)==tcSplitChar);
	if(vec_strData.size()!=6)
		return 7;
	for(int i=2;i<4;++i)
	{
		vec_strDataTemp.push_back(vec_strData[i]);
	}
	VecHexStr2VecFloat2Str(vec_strDataTemp,vec_fData,2,strData);
	m_fUcurPer=vec_fData[0];
	m_fIcurPer=vec_fData[1];
	m_nPhase=_ttoi(vec_strData[0]);
	CString strPFTemp=vec_strData[1];
	m_nTurns=_ttoi(vec_strData[4]);
	m_nStabTime=_ttoi(vec_strData[5]);
	if(m_nPhase<0||m_nPhase>3)
		return 1;
	if(strPFTemp!=_T("1.0")&&strPFTemp!=_T("0.5L")&&strPFTemp!=_T("0.8C")&&strPFTemp!=_T("0"))
		return 2;
	if(m_fUcurPer*m_fUb/100>280.0)
		return 3;
	if(m_fIcurPer*m_fIb/100>m_fIMax)
		return 4;
	if(m_nTurns<1||m_nTurns>255)
		return 5;
	if(m_nStabTime<0||m_nStabTime>255)
		return 6;
	int nPFTemp;
	if(strPFTemp==_T("0.5L"))
		nPFTemp=1;
	else if(strPFTemp==_T("1.0"))
		nPFTemp=4;
	else if(strPFTemp==_T("0.8C"))
		nPFTemp=5;
	else
		nPFTemp=9;
	m_fPF=float(_ttof(strPFTemp));
	vec_strData[0].Format(_T("%02X"),m_nPhase);
	vec_strData[1].Format(_T("%02X"),nPFTemp);
	vec_strData[4].Format(_T("%02X"),m_nTurns);
	vec_strData[5].Format(_T("%02X"),m_nStabTime);
	strData=vec_strData[0]+vec_strData[1]+strData+vec_strData[4]+vec_strData[5];
	return 0;
}

void ZRMTProtocol::Type15Pro(CString & strData)
{
	std::vector<CString> vec_strData; 
	std::vector<float> vec_fData;
	vec_strData.push_back(strData);
	VecHexStr2VecFloat2Str(vec_strData,vec_fData,1,strData);
}

int ZRMTProtocol::Type11Pro(CString & strData)
{
	std::vector<CString> vec_strData; 
	StrSplit_Z(strData,vec_strData,tcSplitChar,strData.Right(1)==tcSplitChar);
	if(vec_strData.size()!=2)
		return 1;
	int nIsChoose=_ttoi(vec_strData[1]);
	if(nIsChoose!=0&&nIsChoose!=1)
		return 2;
	vec_strData[1].Format(_T("%02X"),nIsChoose);
	strData.Format(_T("%02X"),_ttoi(vec_strData[0]));
	for(int i=0;i<_ttoi(vec_strData[0]);++i)
	{
		strData+=vec_strData[1];
	}
	return 0;
}

int ZRMTProtocol::Type10Pro(CString & strData)
{
	std::vector<CString> vec_strData; 
	std::vector<float> vec_fData;
	StrSplit_Z(strData,vec_strData,tcSplitChar,strData.Right(1)==tcSplitChar);
	if(vec_strData.size()!=5)
		return 6;
	VecHexStr2VecFloat2Str(vec_strData,vec_fData,4,strData);
	if(vec_fData[0]>280.0)
		return 1;
	if(vec_fData[1]>120.0)
		return 2;
	if(vec_fData[2]>120.0)
		return 3;
	if(vec_fData[1]>vec_fData[2])
		return 4;
	m_fUb=vec_fData[0];
	m_fIb=vec_fData[1];
	m_fIMax=vec_fData[2];
	m_fPulse=vec_fData[3];
	m_nMode=_ttoi(vec_strData[4]);
	if(m_nMode!=0&&m_nMode!=1&&m_nMode!=3&&m_nMode!=4&&m_nMode!=7&&m_nMode!=8)
		return 5;
	vec_strData[4].Format(_T("%02X"),m_nMode);
	strData+=vec_strData[4];
	return 0;
}

bool ZRMTProtocol::VecHexStr2VecFloat2Str(const std::vector<CString> & vec_strData,std::vector<float> & vec_fData,const int & nChangeCount,CString & strOut)
{
	vec_fData.clear();
	strOut.Empty();
	if(nChangeCount>(int)vec_strData.size())
		return false;
	float fData;
	for(int i=0;i<nChangeCount;++i)
	{
		fData=float(_ttof(vec_strData[i]));
		vec_fData.push_back(fData);
	}
	BOOL nRtn=IsLittle_Endian_Z();
	CString strByte;
	BYTE * p_c=NULL;
	for(int i=0;i<nChangeCount;++i)
	{
		p_c=(BYTE *)(&vec_fData[i]);
		if(nRtn)
		{
			for(int j=0;j<sizeof(float);++j)
			{
				strByte.Format(_T("%02X"),p_c[j]);
				strOut+=strByte;
			}
		}
		else
		{
			for(int j=sizeof(float)-1;j>=0;--j)
			{
				strByte.Format(_T("%02X"),p_c[j]);
				strOut+=strByte;
			}
		}
	}
	return true;
}

bool ZRMTProtocol::DataDecode(CString strData,CString & strValue)
{
	strData.Remove(_T(' '));
	if(!CheckRMTProtocol(strData))
		return false;
	strValue.Empty(); 
	CString strOrderType=strData.Mid(2,2);
	CString strExt=_T("-")+strOrderType+_T("H");
	if(strOrderType==_T("00"))
	{
		strValue=CString(tcSplitValue)+_T("获取装置状态")+strExt;
	}
	else if(strOrderType==_T("80"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nData=_tcstol(strData.Left(2),NULL, 16);
		strValue.Format(_T("%d"),nData);
		strValue+= CString(tcSplitValue) + _T("装置状态")+strExt;
	}
	else if(strOrderType==_T("03"))
	{
		strValue= CString(tcSplitValue) + _T("获取监测数据")+strExt;
	}
	else if(strOrderType==_T("83"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		CString strTemp;
		for(int i=0;i<13;++i)
		{
			strTemp=strData.Mid(8*i,8);
			strValue+=HexStr2Float2Str(strTemp)+ CString(tcSplitChar);
		}
		strValue+=strData.Right(6)+ CString(tcSplitValue) + _T("监测数据")+strExt;
	}
	else if(strOrderType==_T("10"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		CString strTemp;
		for(int i=0;i<4;++i)
		{
			strTemp=strData.Mid(8*i,8);
			strValue+=HexStr2Float2Str(strTemp)+ CString(tcSplitChar);
		}
		strValue+=strData.Right(2)+ CString(tcSplitValue) + _T("台体参数设置")+strExt;
	}
	else if(strOrderType==_T("11"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nData=_tcstol(strData.Left(2),NULL, 16);
		strValue.Format(_T("%d%c"),nData,tcSplitChar);
		strValue+=strData.Mid(2)+ CString(tcSplitValue) + _T("表位设置")+strExt;
	}
	else if(strOrderType==_T("15"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		strValue=HexStr2Float2Str(strData);
		strValue+= CString(tcSplitValue) + _T("频率设置")+strExt;
	}
	else if(strOrderType==_T("16"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nData=_tcstol(strData.Left(2),NULL, 16);
		strValue.Format(_T("%d"),nData);
		strValue+= CString(tcSplitValue) + _T("主动上报设置")+strExt;
	}
	else if(strOrderType==_T("20"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nDatas[4]={0};
		nDatas[0]=_tcstol(strData.Mid(0,2),NULL, 16);
		nDatas[1]=_tcstol(strData.Mid(2,2),NULL, 16);
		nDatas[2]=_tcstol(strData.Mid(20,2),NULL, 16);
		nDatas[3]=_tcstol(strData.Mid(22,2),NULL, 16);
		CString strTemp;
		if(nDatas[1]==1)
			strTemp=_T("0.5L");
		else if(nDatas[1]==4)
			strTemp=_T("1.0");
		else if(nDatas[1]==5)
			strTemp=_T("0.8C");
		else
			strTemp=_T("0");
		strValue.Format(_T("%d%c"),nDatas[0],tcSplitChar);
		strValue+=strTemp+CString(tcSplitChar);
		for(int i=0;i<2;++i)
		{
			strTemp=strData.Mid(8*i+4,8);
			strValue+=HexStr2Float2Str(strTemp)+ CString(tcSplitChar);
		}
		strTemp.Format(_T("%d%c%d"),nDatas[2],tcSplitChar,nDatas[3]);
		strValue+=strTemp;
		strValue+= CString(tcSplitValue) + _T("基本误差检定")+strExt;
	}
	else if(strOrderType==_T("22"))
	{
		strValue= CString(tcSplitValue) + _T("卸载停止")+strExt;
	}
	else if(strOrderType==_T("23"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nDatas[2]={0};
		nDatas[0]=_tcstol(strData.Mid(0,2),NULL, 16);
		nDatas[1]=_tcstol(strData.Mid(2,2),NULL, 16);
		CString strTemp;
		if(nDatas[1]==4)
			strTemp=_T("1.0");
		if(nDatas[1]==1)
			strTemp=_T("0.5L");
		if(nDatas[1]==5)
			strTemp=_T("0.8C");
		if(nDatas[1]==9)
			strTemp=_T("0");
		strValue.Format(_T("%d%c"),nDatas[0], tcSplitChar);
		strValue+=strTemp+CString(tcSplitChar);
		for(int i=0;i<2;++i)
		{
			strTemp=strData.Mid(8*i+4,8);
			strValue+=HexStr2Float2Str(strTemp)+CString(tcSplitChar);
		}
		strValue=strValue.Mid(0,strValue.GetLength()-1);
		strValue+= CString(tcSplitValue) + _T("加载")+strExt;
	}
	else if(strOrderType==_T("33"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nData=_tcstol(strData.Left(2),NULL, 16);
		strValue.Format(_T("%d"),nData);
		strValue+= CString(tcSplitValue) + _T("日计时误差检定")+strExt;
	}
	else if(strOrderType==_T("FF"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		strValue=strData.Left(4);
		if(strValue==_T("0000"))
			strValue+= CString(tcSplitValue) + strData.Mid(4,2)+ CString(tcSplitChar) + _T("执行成功")+strExt;
		else
			strValue+= CString(tcSplitValue) + strData.Mid(4,2)+ CString(tcSplitChar) + _T("执行出错")+strExt;
	}
	else if(strOrderType==_T("A0"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nData=_tcstol(strData.Left(2),NULL, 16);
		strData=strData.Mid(2);
		CString strTemp;
		for(int i=0;i<nData;++i)
		{
			strTemp=strData.Mid(10*i,2);
			strTemp.Format(_T("%d"),_tcstol(strTemp,NULL, 16));
			strValue+=strTemp+CString(tcSplitChar);
			strTemp=strData.Mid(10*i+2,8);
			strValue+=HexStr2Float2Str(strTemp)+CString(tcSplitChar);
		}
		strValue=strValue.Mid(0,strValue.GetLength()-1);
		strTemp.Format(_T("%d%c"),nData, tcSplitChar);
		strValue=strTemp+strValue+ CString(tcSplitValue) + _T("基本误差结果")+strExt;
	}
	else if(strOrderType==_T("B1"))
	{
		int nDataLen=2*_tcstol(strData.Mid(4,2),NULL, 16);
		strData=strData.Mid(6,nDataLen);
		int nData=_tcstol(strData.Left(2),NULL, 16);
		strData=strData.Mid(2);
		CString strTemp;
		for(int i=0;i<nData;++i)
		{
			strTemp=strData.Mid(10*i,2);
			strTemp.Format(_T("%d"),_tcstol(strTemp,NULL, 16));
			strValue+=strTemp+CString(tcSplitChar);
			strTemp=strData.Mid(10*i+2,8);
			float fTemp=HexString2Float(strTemp);
			strTemp.Format(_T("%.4f"),(fTemp-1.0)*86400.0);
			strValue+=strTemp+CString(tcSplitChar);
		}
		strValue=strValue.Mid(0,strValue.GetLength()-1);
		strTemp.Format(_T("%d%c"),nData, tcSplitChar);
		strValue=strTemp+strValue+ CString(tcSplitValue) + _T("日计时误差结果")+strExt;
	}
	else
	{
		strValue=_T("ErrorData")+strExt;
	}
	return true;
}

float ZRMTProtocol::HexString2Float(const CString & strSrc)
{
	BYTE sz_cData[8]={0};
	float * p_fData=(float *)sz_cData;
	CStringToBYTEArrBy2(strSrc,sz_cData,sizeof(sz_cData));
	return p_fData[0];
}

CString ZRMTProtocol::HexStr2Float2Str(const CString & strSrc)
{
	CString strDest;
	BYTE szData[8]={0};
	if(CStringToBYTEArrBy2(strSrc,szData,sizeof(szData)))
	{
		float *pfData=(float *)szData;
		strDest.Format(_T("%.4f"),pfData[0]);
	}
	return strDest;
}

bool ZRMTProtocol::CheckRMTProtocol(CString str)
{
	str.Remove(_T(' '));
	BYTE szData[1000]={0};
	if(CStringToBYTEArrBy2(str,szData,sizeof(szData))==false)
		return false;
	int nLength=4+szData[2];
	if(szData[0]==0xA0&&szData[nLength-1]==0xA5&&nLength==str.GetLength()/2)
		return true;
	else if(szData[0]==0xF0&&szData[nLength-1]==0xF5&&nLength==str.GetLength()/2)
		return true;
	else
		return false;
}

bool ZRMTProtocol::CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxArrayLen)
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

void ZRMTProtocol::InitParam(const CString & strOrderType,const CString & strTgtData)
{
	if(!strOrderType.IsEmpty())
		m_strOrderType=strOrderType;
	if(!strTgtData.IsEmpty())
		m_strTgtData=strTgtData;
}

float ZRMTProtocol::GetMeterErrorWaitTime(void)
{
	float fOnceTime,fMaxTime;
	fOnceTime=3600.0f*1000.0f/(m_fIb*m_fIcurPer/100.0f)/(m_fUb*m_fUcurPer/100.0f)/m_fPF/m_fPulse;
	fMaxTime=1.3f*(fOnceTime*(m_nTurns+1)+m_nStabTime);
	return fMaxTime;
}


float ZRMTProtocol::GetClockErrorWaitTime(void)
{
	float fMaxTime;
	fMaxTime=1.3f*m_nClockErrorSpan;
	return fMaxTime;
}


CString ZRMTProtocol::GetLength(void)
{
	return m_strLength;
}


CString ZRMTProtocol::GetOrderType(void)
{
	return m_strOrderType;
}