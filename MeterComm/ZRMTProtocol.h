#pragma once
#include "ZProtocol.h"
class ZRMTProtocol:public ZProtocol
{
private:
	CString m_strOrderType;
	CString m_strLength;
	CString m_strTgtData;
	float m_fUb;//额定电压
	float m_fIb;//额定电流
	float m_fIMax;//最大电流
	float m_fPulse;//有功常数
	int m_nMode;//测量模式
	int m_nPhase;//合相分相
	float m_fPF;//功率因数
	float m_fUcurPer;//当前电压%
	float m_fIcurPer;//当前电流%
	int m_nTurns;//圈数
	int m_nStabTime;//稳定时间
	int m_nClockErrorSpan;//日计时误差间隔
public:
	ZRMTProtocol(void);
	virtual ~ZRMTProtocol(void);
	virtual int DataEncode(CString & strData,CString & strError);
	virtual bool DataDecode(CString strData,CString & strValue);
	virtual CString GetLength(void);
	static bool CheckRMTProtocol(CString str);
	static bool CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxArrayLen);
	int Type10Pro(CString & strData);
	bool VecHexStr2VecFloat2Str(const std::vector<CString> & vec_strData,std::vector<float> & vec_fData,const int & nChangeCount,CString & strOut);
	int Type11Pro(CString & strData);
	void Type15Pro(CString & strData);
	int Type20Pro(CString & strData);
	int Type23Pro(CString & strData);
	CString HexStr2Float2Str(const CString & strSrc);
	float HexString2Float(const CString & strSrc);
	void InitParam(const CString & strOrderType,const CString & strTgtData);
	float GetMeterErrorWaitTime(void);
	float GetClockErrorWaitTime(void);
	CString GetOrderType(void);
};

