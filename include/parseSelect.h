#ifndef PARSESELECT_H
#define PARSESELECT_H

#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50
#include "include/bplustree.h"
#include "include/arrayList.h"
#include "include/compare.h"

void parseSelect(const char* query);
ArrayList* selectAll(BPlusNode* node, const char* tableName, int numColumns, int* numRows, char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH]);
void extractClause(const char *input, char *output);
void parseAggregateFunction(char *input, char *aggregateFunction, char *columnName);
ArrayList* processwhere(char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char selectedColumnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], int numColumns, ArrayList* columnData, char whereClause[MAX_FIELD_LENGTH], int* numRows);
ArrayList* processAggregateFunction(char aggregateFunction[MAX_COLUMNS][MAX_FIELD_LENGTH], ArrayList* columnData, int allNumColumns, int numRows);
ArrayList* processOrderBy(ArrayList* columnData, char allColumnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char orderByClause[MAX_FIELD_LENGTH], int numColumns);


#endif