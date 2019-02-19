#include "stdafx.h"
#include "CCard.h"


CCard::CCard()
{
}

CCard::CCard(int nID, int nProtocolType, int nDllType, int nCardType, int nFileNum, const CString & strFile0, const CString & strFile1, const CString & strFile2, const CString & strFile3, const CString & strFile4)
	: m_nID(nID)
	, m_nProtocolType(nProtocolType)
	, m_nDllType(nDllType)
	, m_nCardType(nCardType)
	, m_nFileNum(nFileNum)
{
	m_sz_strFile[0] = strFile0;
	m_sz_strFile[1] = strFile1;
	m_sz_strFile[2] = strFile2;
	m_sz_strFile[3] = strFile3;
	m_sz_strFile[4] = strFile4;
}

CCard::~CCard()
{
}


int CCard::GetID() const
{
	return m_nID;
}


int CCard::GetProtocolType() const
{
	return m_nProtocolType;
}


int CCard::GetDllType() const
{
	return m_nDllType;
}


int CCard::GetCardType() const
{
	return m_nCardType;
}


int CCard::GetFileNum() const
{
	return m_nFileNum;
}


const CString * CCard::GetFile() const
{
	return m_sz_strFile;
}


CString * CCard::GetFileForSet()
{
	return m_sz_strFile;
}