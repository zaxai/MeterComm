/*
** Version  1.0.0.1
** Date     2018.12.01
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "sqlite3.h"
#include "ZUtil.h"
#pragma comment(lib,"sqlite3.lib")

class ZSqlite3
{
public:
	enum ERRORINFO
	{
		ERROR_OK=0,//成功
		ERROR_INVALIDSQLITE,//sqlite3数据成员无效
		ERROR_EXEC,//Exec执行出错
		ERROR_GETTABLE,//GetTable执行出错
		ERROR_OTHER//其他出错
	};
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

