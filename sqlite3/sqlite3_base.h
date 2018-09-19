#ifndef SQLITE3_BASE_H
#define SQLITE3_BASE_H

#include "sqlite3.h"

#include <QTextStream>
#include <QDateTime>

class sqlite3_base
{
public:
    sqlite3 *sql_db;
    int sqlread_nrow;
	int sqlread_ncolumn;

    sqlite3_base();
    int sqlite3_base_open(const char *filename);
    int sqlite3_base_create_tab();
    int sqlite3_base_insert_data();
    char** sqlite3_base_read_data();
    int sqlite3_base_close();
    int sqlite3_base_delete_data(int id);
};

#endif // SQLITE3_BASE_H
