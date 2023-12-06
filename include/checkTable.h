#ifndef CHECKTABLE_H
#define CHECKTABLE_H

#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50

int checkTable(const char *tableName);
int getTableInfo(const char *tableName, char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH]);

#endif