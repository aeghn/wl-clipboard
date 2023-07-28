#include <sqlite3.h>

typedef unsigned char byte;

sqlite3 *cdb_maybe_create_table(const char* filename);

void cdb_insert_text(sqlite3 *db, byte *content, sqlite3_uint64 len, const char *mimes);

void cdb_insert_blob(sqlite3 *db, byte *content, sqlite3_uint64 len, const char *mimes);
