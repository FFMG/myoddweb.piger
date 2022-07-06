#pragma once
#ifndef _Sqlite_H_
#define _Sqlite_H_

#include "sqlite3.h"

#include "../string/string.h"
#include <vector>

typedef std::vector<std::wstring> SQL_ROW;  //  one row
typedef std::vector<SQL_ROW> SQL_ROWS;     //  many rows.
struct SQL_DATA
{
  SQL_ROW sqlRowName;
  SQL_ROWS sqlRowsData;
  int nErrorCode;
  std::wstring szErrorMessage;
  unsigned long ulQueryTimeMS;
};

typedef std::vector<SQL_DATA> SQL_DATA_CONTAINER;     //  many rows of many queries.

typedef int(*sqlite_callback)(void*,int,char**,char**);

namespace myodd{ namespace sqlite{
  sqlite3* open( const wchar_t* dbName, int* nResult = NULL );
  bool close( sqlite3* db, int* nResult = NULL );

  bool tableExists( sqlite3* db, wchar_t* table_name );

  bool exec( sqlite3* db, const wchar_t* szSql, sqlite_callback fn = NULL, MYODD_LPARAM lparam = NULL );
  bool execWithReturn( sqlite3* db, const wchar_t *sql, SQL_DATA_CONTAINER& sqlDataContainer );

} // sqlite
} // myodd
#endif  //  _Sqlite_H_