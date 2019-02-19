#pragma once
class CCard
{
private:
	int m_nID;
	int m_nProtocolType;
	int m_nDllType;
	int m_nCardType;
	int m_nFileNum;
	CString m_sz_strFile[5];
public:
	CCard();
	CCard(int nID, int nProtocolType, int nDllType, int nCardType, int nFileNum,const CString & strFile0, const CString & strFile1, const CString & strFile2, const CString & strFile3, const CString & strFile4);
	virtual ~CCard();
	int GetID() const;
	int GetProtocolType() const;
	int GetDllType() const;
	int GetCardType() const;
	int GetFileNum() const;
	const CString * GetFile() const;
	CString * GetFileForSet();
};

