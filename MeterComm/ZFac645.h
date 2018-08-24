#pragma once
#include "ZDLT645_2007.h"
class ZFac645 :
	public ZDLT645_2007
{
public:
	ZFac645(void);
	virtual ~ZFac645(void);
	virtual int DataEncode(CString & strData,CString & strError);
	virtual bool DataDecode(CString strData,CString & strValue);
	virtual CString GetLength(void);
};

