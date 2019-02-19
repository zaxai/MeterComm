#include "stdafx.h"
#include "CCardDB.h"


CCardDB::CCardDB(const CString & strTableName)
	: m_strTableName(strTableName)
	, m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\res\\Data.db3");
	CreateTable();
}


CCardDB::~CCardDB()
{
}


void CCardDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql.Format(_T("CREATE TABLE %s (\
				ID INT PRIMARY KEY NOT NULL,\
				ProtocolType INT,\
				DllType INT,\
				CardType INT,\
				FileNum INT,\
				File0 NVARCHAR(2000),\
				File1 NVARCHAR(2000),\
				File2 NVARCHAR(2000),\
				File3 NVARCHAR(2000),\
				File4 NVARCHAR(2000)\
				)"), m_strTableName);
	zsql.ExecSQL(strSql);
}


bool CCardDB::Insert(const CCard & card)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	const CString * p_strFile = card.GetFile();
	strSql.Format(_T("INSERT INTO %s VALUES (%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s')"), m_strTableName, card.GetID(), card.GetProtocolType(), card.GetDllType(),card.GetFileNum(), p_strFile[0], p_strFile[1], p_strFile[2], p_strFile[3], p_strFile[4]);
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CCardDB::Update(const CCard & card)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	const CString * p_strFile = card.GetFile();
	strSql.Format(_T("UPDATE %s SET File0='%s',File1='%s',File2='%s',File3='%s',File4='%s' WHERE ID=%d"), m_strTableName,p_strFile[0], p_strFile[1], p_strFile[2], p_strFile[3], p_strFile[4], card.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CCardDB::Select(const CString & strSql, std::vector<CCard> & vec_card)
{
	vec_card.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 10)
		{
			for (int i = 1; i < nRow; ++i)
			{
				CCard card(_ttoi(vec2_strData[i][0]), _ttoi(vec2_strData[i][1]), _ttoi(vec2_strData[i][2]), _ttoi(vec2_strData[i][3]), _ttoi(vec2_strData[i][4]), vec2_strData[i][5], vec2_strData[i][6], vec2_strData[i][7], vec2_strData[i][8], vec2_strData[i][9]);
				vec_card.push_back(card);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}