#include "stdafx.h"
#include "sqlite.h"

#ifdef UNICODE
#define myodd_sqlite_prepare_v2   sqlite3_prepare16_v2
#define myodd_sqlite_errmsg       sqlite3_errmsg16
#define myodd_sqlite_column_name  sqlite3_column_name16
#define myodd_sqlite_column_text  sqlite3_column_text16
#else
#define myodd_sqlite_prepare_v2   sqlite3_prepare_v2
#define myodd_sqlite_errmsg       sqlite3_errmsg
#define myodd_sqlite_column_name  sqlite3_column_name
#define myodd_sqlite_column_text  sqlite3_column_text
#endif

namespace myodd{ namespace sqlite{

/**
 * Close a database connection
 * @param const char* the name of the database we want to open.
 * @param int if the pointer is not NULL the result will be returned.
 * @return sqlite3* the value of the opened db or NULL if there was a problem.
 */
sqlite3* open( const MYODD_CHAR* dbName, int* nResult /*= NULL*/ )
{
  sqlite3* db = NULL;
  int rc = SQLITE_OK;
  rc = sqlite3_open16( dbName, &db );
  if( nResult )
  {
    *nResult = rc;
  }
  if(rc != SQLITE_OK)
  {
    close( db );
    return NULL;
  }
  return db;
}

/**
 * Close a database connection
 * @param sqlite3* the db we are closing.
 * @param int if the pointer is not NULL the result will be returned.
 * @return bool success or not
 */
bool close(sqlite3* db, int* nResult /*= NULL*/ )
{
  // close the db.
  int rc = SQLITE_OK;
  rc = sqlite3_close( db );
  if( nResult )
  {
    *nResult = rc;
  }
  return (rc == SQLITE_OK);
}

/**
 * Check if a table already exists.
 * @param sqlite3* an open database
 * @param MYODD_CHAR* the name of the table we are looking for.
 * @return bool if the table exists or not.
 */
bool tableExists
  ( 
  sqlite3* db, 
    MYODD_CHAR* table_name
  )
{
  // sanity check.
  if( !table_name )
  {
    return false;
  }

  USES_CONVERSION;

  // assume that the table does not exit.
  bool bExist = false;
  int nCol    = 0;
  int nRow    = 0;
  char** szResult;

  // create the select that we will call.
  static const MYODD_CHAR* pszSelect = _T("SELECT name FROM sqlite_master WHERE type='table' and name='%s';");
  size_t nLen = (_tcslen( pszSelect ) + _tcslen( table_name )) * sizeof(MYODD_CHAR);
  MYODD_CHAR* pszSelectFull = new MYODD_CHAR[ nLen+ sizeof(MYODD_CHAR)];
  _stprintf_s( pszSelectFull, nLen, pszSelect, table_name );

  // look for the table, if anything is found then the table exits.
  // remember that if nothing is found then row count == 0
  // but if something is found then there is the col name given in the first row.
  int rc = sqlite3_get_table( db, 
                              T2A( pszSelectFull ), 
                              &szResult, 
                              &nRow,
                              &nCol, 
                              0 
                            );
  if( rc == SQLITE_OK )
  {
    bExist = ( nRow != 0 );
  }
  sqlite3_free_table(szResult);

  delete [] pszSelectFull;
  return bExist;
}

/**
 * Execute one or more query with return values.
 * @param sqlite3* an open database
 * @param const MYODD_CHAR* the queries we want to run
 * @param SQL_DATA_CONTAINER& a container that will have all the info about each queries.
 * @return bool success or not if there was an eorror, (all errors are in SQL_DATA_CONTAINER);
 */
bool execWithReturn
( 
  sqlite3* db, 
  const MYODD_CHAR* szSql,
  SQL_DATA_CONTAINER& sqlDataContainer 
)
{
  // reset the data
  sqlDataContainer.clear();

  sqlite3_stmt *pStmt = NULL;
  const MYODD_CHAR* szLeftover;
  int rc = SQLITE_ERROR;
  while( szSql && _tcslen(szSql) > 0 )
  {
    // start timing the query.
    unsigned long ulTime = GetTickCount();

    // prepare the return value.
    #ifdef UNICODE
      rc = myodd_sqlite_prepare_v2( db, (const void*)szSql, -1, &pStmt, (const void**)&szLeftover );
    #else
      rc = myodd_sqlite_prepare_v2( db, (const char*)szSql, -1, &pStmt, (const char**)&szLeftover );
    #endif

    if( pStmt == NULL )
    {
      // this can happen with comments.
      szSql = szLeftover;
      continue;
    }

    // add one more row of data
    // even if we have an error we will accommodate for it and then break out.
    // if we have no return value, (if the item is not a select), then we will return an empty row.
    sqlDataContainer.resize( sqlDataContainer.size() + 1 );
    SQL_DATA& sqlData = sqlDataContainer.end()[-1];

    // how long did this query take?
    // note that loading the data in the container can also take a fair amount of time
    // this only shows how long one query took to run
    sqlData.ulQueryTimeMS = (GetTickCount() - ulTime);

    // get any errors we might have found.
    // we don't get the error message if we return SQLITE_OK
    sqlData.nErrorCode = sqlite3_errcode(db);

    // if there was an error, we need to break out of the while() statement.
    if( SQLITE_OK != rc )
    {
      // get the error message.
      sqlData.szErrorMessage = (const MYODD_CHAR*)myodd_sqlite_errmsg( db );

      //  there was an error, we cannot go any further.
      sqlite3_finalize(pStmt);

      break;
    }

    // number of cols.
    int nCol = sqlite3_column_count( pStmt );

    // get the cols names for the query we ran.
    for( int i = 0; i < nCol; ++i )
    {
      const MYODD_CHAR* szName = (MYODD_CHAR*)myodd_sqlite_column_name( pStmt, i );
      sqlData.sqlRowName.push_back( szName );
    }

    // get the data for each rows.
    while(sqlite3_step(pStmt) == SQLITE_ROW) 
    {
      // prepare a container that will receive all the data.
      sqlData.sqlRowsData.resize( sqlData.sqlRowsData.size() +1 );
      SQL_ROW& sqlRow = sqlData.sqlRowsData.end()[-1];
      sqlRow.reserve( nCol );
      for( int i = 0; i < nCol; ++i )
      {
        const MYODD_CHAR* sz = (MYODD_CHAR *)myodd_sqlite_column_text(pStmt, i);
        sqlRow.push_back( sz ? sz : _T("NULL") );
      }
    };
    sqlite3_finalize(pStmt); 

    // move on to the next query
    pStmt = NULL;
    szSql = szLeftover;
  }

  // success or not
  return (rc == SQLITE_OK);
}

/**
 * Execute a query directly
 * @param sqlite3* an open database
 * @param const MYODD_CHAR* the query we want to run
 * @param sqlite_callback a call back function for every returned row.
 * @param MYODD_LPARAM a param that will be passed as first param to our call back function.
 * @return bool success or not if there was an error
 */
bool exec
( 
  sqlite3* db, 
  const MYODD_CHAR* sql,
  sqlite_callback fn /*= NULL*/, 
  MYODD_LPARAM lparam /*= NULL*/
)
{
  USES_CONVERSION;

  char* zErrMsg = 0;
  int rc = sqlite3_exec( db, T2A(sql), fn, (void*)lparam, &zErrMsg );
  sqlite3_free( zErrMsg ); 
  zErrMsg = 0;

  return (rc == SQLITE_OK);
}

}
}