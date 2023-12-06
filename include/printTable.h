#ifndef PRINTTABLE_H
#define PRINTTABLE_H

#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50
#include "include/arrayList.h"

void printTable(char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], ArrayList* columnData, int numRows, int numCols);


#endif