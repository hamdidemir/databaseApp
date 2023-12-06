#ifndef LOADCSV_H
#define LOADCSV_H

#define MAX_COLUMNS 50
#define MAX_FIELD_LENGTH 1024


void loadCSV();
void writeToDatabase(char tableName[MAX_FIELD_LENGTH], int numColumns, char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char values[MAX_COLUMNS][MAX_FIELD_LENGTH]);

#endif
