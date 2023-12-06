#include <stdio.h>
#include <string.h>
#include "include/printTable.h"

void printTable(char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], ArrayList* columnData, int numRows, int numCols) 
{
    printf("rows: %d, cols: %d\n",numRows, numCols);

    // Error checking for non-positive numRows and numCols
    if (numRows <= 0 || numCols <= 0) 
    {
        fprintf(stderr, "Error: Invalid number of rows or columns.\n");
        return;
    }

    // Disable buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // Calculate the maximum width for each column
    int columnWidths[numCols];
    for (int i = 0; i < numCols; i++) 
    {
        columnWidths[i] = strlen(columnNames[i]);
        for (int j = 0; j < numRows; j++) 
        {
            char* data = (char*)getArrayList(columnData, j * numCols + i);
            int len = (data != NULL) ? strlen(data) : 0;
            if (len > columnWidths[i]) 
            {
                columnWidths[i] = len;
            }
        }
    }

    // Print the top horizontal line
    printf("+");
    for (int i = 0; i < numCols; i++) 
    {
        for (int j = 0; j < columnWidths[i] + 2; j++) 
        {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    // Print the table header with column names and separators
    printf("|");
    for (int i = 0; i < numCols; i++) 
    {
        printf(" %-*s |", columnWidths[i], columnNames[i]);
    }
    printf("\n");

    // Print horizontal lines between header and data
    printf("+");
    for (int i = 0; i < numCols; i++) 
    {
        for (int j = 0; j < columnWidths[i] + 2; j++) 
        {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    // Print the table data with separators
    for (int i = 0; i < numRows; i++) 
    {
        printf("|");
        for (int j = 0; j < numCols; j++) 
        {
            char* data = (char*)getArrayList(columnData, i * numCols + j);
            printf(" %-*s |", columnWidths[j], (data != NULL) ? data : "");
        }
        printf("\n");

        // Print horizontal lines between data rows
        printf("+");
        for (int j = 0; j < numCols; j++) 
        {
            for (int k = 0; k < columnWidths[j] + 2; k++) 
            {
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }
}
