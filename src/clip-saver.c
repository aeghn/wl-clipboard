#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#include "clip-saver.h"

/**
 * clipboard_table
 * - id
 * - content
 * - mime
 * - add_time
 **/
static void try_create_db(const char *db_path, sqlite3 **db) {
  int rc;

  rc = sqlite3_open(db_path, db);

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
    exit(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }
}

static void create_clipboard_table(sqlite3 *db) {
  char *err_msg = 0;
  int  rc;
  char *sql;

  if (!db) {
    printf("unable to open database.\n");
  }

  sql = "CREATE TABLE clipboard("
    "CONTENT0 TEXT PRIMARY KEY, "
    "MIMES TEXT DEFAULT '',"
    "COUNT NUMBER default 0, "
    "INSERT_TIME DATETIME default CURRENT_TIMESTAMP,"
    "UPDATE_TIME TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
    ");";

  rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s, sql is: %s\n", err_msg, sql);
    sqlite3_free(err_msg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }
}

void cdb_insert_text(sqlite3 *db, byte *content, sqlite3_uint64 len, const char *mimes) {
  sqlite3_stmt *pstmt = NULL;
  const char* sql = "INSERT INTO clipboard (content0, mimes) values ($1, $2) ON CONFLICT(content0) DO UPDATE SET count=count+1, UPDATE_TIME = CURRENT_TIMESTAMP;";
  sqlite3_prepare_v2(db, sql, -1, &pstmt, NULL);
  sqlite3_bind_text(pstmt, 1, (char *) content, -1, SQLITE_STATIC);
  sqlite3_bind_text(pstmt, 2, mimes, -1, SQLITE_STATIC);
  sqlite3_step(pstmt);
  sqlite3_finalize(pstmt);
}

void cdb_insert_blob(sqlite3 *db, byte *content, sqlite3_uint64 len, const char *mimes) {
  sqlite3_stmt *pstmt = NULL;
  const char* sql = "insert into clipboard (content1, mimes) values ($1, $2);";
  sqlite3_prepare_v2(db, sql, -1, &pstmt, NULL);
  sqlite3_bind_blob64(pstmt, 1, content, len, SQLITE_STATIC);
  sqlite3_bind_text(pstmt, 2, mimes, -1, SQLITE_STATIC);
  sqlite3_step(pstmt);
  sqlite3_finalize(pstmt);
}


sqlite3 *cdb_maybe_create_table(const char* filename) {
  sqlite3 *db;
  try_create_db(filename, &db);
  create_clipboard_table(db);
  return db;
}
