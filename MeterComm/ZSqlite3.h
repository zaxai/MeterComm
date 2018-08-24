#pragma once
#include "sqlite3.h"
#include "MyFuncLib.h"
#pragma comment(lib,"sqlite3.lib")

class ZSqlite3
{
private:
	sqlite3 *m_p_sqlite3;
	ZSqlite3(const ZSqlite3 & zsql);
	ZSqlite3 & operator=(const ZSqlite3 & zsql);
public:
	ZSqlite3(void);
	~ZSqlite3(void);
	bool OpenDB(const CString & in_strPath);
	bool CloseDB(void);
	int ExecSQL(const CString & in_strSQL, CString *  out_p_strErrMsg=NULL);
	int GetTable(const CString & in_strSQL, std::vector<std::vector <CString>> & out_vec2_strData, CString *  out_p_strErrMsg=NULL);
};

