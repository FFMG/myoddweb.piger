#pragma once
#ifndef _Sqlite_H_
#define _Sqlite_H_

#include "sqlite3.h"

#include "../string/string.h"
#include <vector>

typedef std::vector<MYODD_STRING> SQL_ROW;  //  one row
typedef std::vector<SQL_ROW> SQL_ROWS;     //  many rows.
struct SQL_DATA
{
  SQL_ROW sqlRowName;
  SQL_ROWS sqlRowsData;
  int nErrorCode;
  MYODD_STRING szErrorMessage;
  unsigned long ulQueryTimeMS;
};

typedef std::vector<SQL_DATA> SQL_DATA_CONTAINER;     //  many rows of many queries.

typedef int(*sqlite_callback)(void*,int,char**,char**);

namespace myodd{ namespace sqlite{
  sqlite3* open( const TCHAR* dbName, int* nResult = NULL );
  bool close( sqlite3* db, int* nResult = NULL );

  bool tableExists( sqlite3* db, TCHAR* table_name );

  bool exec( sqlite3* db, const TCHAR* szSql, sqlite_callback fn = NULL, LPARAM lparam = NULL );
  bool execWithReturn( sqlite3* db, const TCHAR *sql, SQL_DATA_CONTAINER& sqlDataContainer );

} // sqlite
} // myodd
#endif  //  _Sqlite_H_