#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "include/CJSON.h"
#include "include/parseSelect.h"
#include "include/arrayList.h"
#include "include/checkTable.h"
#include "include/printTable.h"
#include "include/compare.h"

#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50
extern BPlusNode* root;

void parseSelect(const char* query)
{
    char tableNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char allColumnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char selectedColumnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH];
    int numColumns = 0;
    int allNumColumns = 0;
    int numTables = 0;
    int numRows = 0;
    int selectEverythig = 0;
    char whereClause[MAX_FIELD_LENGTH] = "";
    char groupByClause[MAX_FIELD_LENGTH] = "";
    char havingClause[MAX_FIELD_LENGTH] = "";
    char orderByClause[MAX_FIELD_LENGTH] = "";
    char limitClause[MAX_FIELD_LENGTH] = "";
    char aggregateFunction[MAX_COLUMNS][MAX_FIELD_LENGTH];
    ArrayList* columnData = initArrayList();
    int haveAggFunction = 0;

    const char *cursor = query;

    // Search for SELECT
    while (*cursor && strncasecmp(cursor, "SELECT", 6)) 
    {
        cursor++;
    }
    cursor += 6;

    // Skip white spaces
    while (*cursor && isspace(*cursor)) 
    {
        cursor++;
    }

    // Get column names or check for "*"
    if (*cursor == '*') 
    {
        selectEverythig = 1;
        cursor++;
    }
    else 
    {
        while (*cursor && *cursor != '\0' && !isspace(*cursor) && strncasecmp(cursor, "FROM", 4) != 0) 
        {
            int i = 0;

            while (*cursor && *cursor != ',' && !isspace(*cursor)) 
            {
                selectedColumnNames[numColumns][i] = *cursor;
                cursor++;
                i++;
            }
            selectedColumnNames[numColumns][i] = '\0';

            // Check if it starts with aggregate functions
            if (strncmp(selectedColumnNames[numColumns], "AVG", 3) == 0 ||
            strncmp(selectedColumnNames[numColumns], "MIN", 3) == 0 ||
            strncmp(selectedColumnNames[numColumns], "MAX", 3) == 0 ||
            strncmp(selectedColumnNames[numColumns], "COUNT", 5) == 0 ||
            strncmp(selectedColumnNames[numColumns], "SUM", 3) == 0)
            {
                // Parse the aggregate function and column name
                char tempColumnName[MAX_FIELD_LENGTH];
                parseAggregateFunction(selectedColumnNames[numColumns], aggregateFunction[numColumns], tempColumnName);
                strcpy(selectedColumnNames[numColumns], tempColumnName);
                haveAggFunction = 1;
            }
            
            numColumns++;

            if (*cursor == ',') 
            {
                cursor++;
                while (isspace(*cursor)) 
                {
                    cursor++;
                }
            } 
        }        
    }

    // Skip white spaces
    while (isspace(*cursor)) 
    {
        cursor++;
    }

    // Find the "FROM" keyword
    while (*cursor && strncasecmp(cursor, "FROM", 4)) 
    {
        cursor++;
    }
    cursor += 4;

    // Skip white spaces
    while (isspace(*cursor)) 
    {
        cursor++;
    }

    // Get table names
    while (*cursor && *cursor != ' ' && *cursor != '\0') 
    {
        int i = 0;
        while (*cursor && *cursor != ',' && *cursor != ' ' && *cursor != ';') 
        {
            tableNames[numTables][i++] = *cursor;
            cursor++;
        }
        tableNames[numTables][i] = '\0';
        numTables++;

        if (*cursor == ',') 
        {
            cursor++;
            while (isspace(*cursor)) 
            {
                cursor++;
            }
        } 
        else 
        {
            break;
        }
    }

    // if the query has *, we select everything in the table
    if(selectEverythig == 1 && numTables == 1)
    {
        // Get the column info
        allNumColumns = getTableInfo(tableNames[0], allColumnNames, dataTypes);
        columnData = selectAll(root, tableNames[0], allNumColumns, &numRows, allColumnNames);
        // printf("num col: %d\n",numColumns); // debug print
        // printf("num rows: %d\n",numRows); // debug print 
        printTable(allColumnNames, columnData, numRows, allNumColumns);
    }

    allNumColumns = getTableInfo(tableNames[0], allColumnNames, dataTypes);
    columnData = selectAll(root, tableNames[0], numColumns, &numRows, selectedColumnNames);
    if(haveAggFunction == 1)
    {
        columnData = processAggregateFunction(aggregateFunction, columnData, numColumns, numRows);
    }


    // Handle other clauses (where, limit etc)
    // Search for optional clauses
    while(*cursor) 
    {
        // Skip white spaces
        while (isspace(*cursor)) 
        {
            cursor++;
        }

        // Check for WHERE clause
        if (strncasecmp(cursor, "WHERE", 5) == 0) 
        {
            cursor += 5;
            while (*cursor && isspace(*cursor)) 
            {
                cursor++;
            }
            extractClause(cursor, whereClause);
            columnData = selectAll(root, tableNames[0], allNumColumns, &numRows, allColumnNames);
            columnData = processwhere(allColumnNames, selectedColumnNames, allNumColumns, columnData, whereClause, &numRows);
        }

        // Check for ORDER BY clause
        if (strncasecmp(cursor, "ORDER BY", 8) == 0) 
        {
            cursor += 8;
            while (*cursor && isspace(*cursor)) 
            {
                cursor++;
            }
            extractClause(cursor, orderByClause);
            columnData = processOrderBy(columnData, allColumnNames, orderByClause, numColumns);
        }

        // Check for LIMIT clause
        if (strncasecmp(cursor, "LIMIT", 5) == 0) 
        {
            cursor += 5;
            while (*cursor && isspace(*cursor)) 
            {
                cursor++;
            }
            extractClause(cursor, limitClause);
        }

        // Move the cursor forward
        while (*cursor && !isspace(*cursor)) 
        {
            cursor++;
        }
    }


    numRows = columnData->size / numColumns;    
    printTable(selectedColumnNames, columnData, numRows, numColumns);



    // Print the parsed information
    if (selectEverythig) 
    {
        printf("Selected Columns: *\n");
    } 
    else 
    {
        printf("Number of Columns: %d\n", numColumns);
        for (int j = 0; j < numColumns; j++) 
        {
            printf("Column %d: %s\n", j + 1, selectedColumnNames[j]);
        }
    }

    printf("Number of Tables: %d\n", numTables);
    for (int j = 0; j < numTables; j++) 
    {
        printf("Table %d: %s\n", j + 1, tableNames[j]);
    }

    printf("Aggreate functions:\n");
    for (int j = 0; j < numColumns; j++) 
    {
        printf("Aggregate function %d: %s\n", j + 1, aggregateFunction[j]);
    }


    // Print optional clauses
    if (strlen(whereClause) > 0) 
    {
        printf("WHERE Clause: %s\n", whereClause);
    }
    if (strlen(groupByClause) > 0) 
    {
        printf("GROUP BY Clause: %s\n", groupByClause);
    }
    if (strlen(havingClause) > 0) 
    {
        printf("HAVING Clause: %s\n", havingClause);
    }
    if (strlen(orderByClause) > 0) 
    {
        printf("ORDER BY Clause: %s\n", orderByClause);
    }
    if (strlen(limitClause) > 0) 
    {
        printf("LIMIT Clause: %s\n", limitClause);
    }




    freeArrayList(columnData);
}


ArrayList* processwhere(char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char selectedColumnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], int numColumns, ArrayList* columnData, char whereClause[MAX_FIELD_LENGTH], int* numRows)
{
    char column[MAX_FIELD_LENGTH];
    char operator[MAX_FIELD_LENGTH];
    char value[MAX_FIELD_LENGTH];
    ArrayList* newColumnData = initArrayList();

    if (sscanf(whereClause, "%s %s %s", column, operator, value) != 3) 
    {
        printf("Invalid WHERE clause format.\n");
    }

    // Find the index of the specified column
    int columnIndex = -1;
    for (int i = 0; i < numColumns; i++) 
    {
        if (strcmp(columnNames[i], column) == 0) 
        {
            columnIndex = i;
            break;
        }
    }

    // Determine the indexes of the selected columns
    int numSelectedColumns = 0;
    int selectedIndices[numColumns];
    for (int i = 0; i < numColumns; i++) 
    {
        for(int j = 0; j < numColumns; j++)
        {
            if (strcmp(selectedColumnNames[i], columnNames[j]) == 0) 
            {
                selectedIndices[numSelectedColumns] = j;
                numSelectedColumns++;
            }
        }
    }


    // Check if the column was found
    if (columnIndex == -1) 
    {
        printf("Column not found: %s\n", column);
        return NULL;
    }


    if(strcmp(operator, "=") == 0)
    {
        for(size_t i = 0; i < ((columnData->size) / numColumns); i++)
        {
            if(strcmp((char *)columnData->data[columnIndex + numColumns * i], value) == 0)
            {
                for(int j = 0; j < numSelectedColumns; j++)
                {
                    addArrayList(newColumnData, columnData->data[selectedIndices[j] + numColumns * i]);
                }            
            }
        }
    }
    else if(strcmp(operator, ">") == 0)
    {
        for(size_t i = 0; i < ((columnData->size) / numColumns); i++)
        {
            if(compareStringsOrNumbers((char *)columnData->data[columnIndex + numColumns * i], value) > 0)
            {
                for(int j = 0; j < (numSelectedColumns); j++)
                {
                    addArrayList(newColumnData, columnData->data[selectedIndices[j] + numColumns * i]);
                }            
            }
        }
    }
    else if(strcmp(operator, "<") == 0)
    {
        for(size_t i = 0; i < ((columnData->size) / numColumns); i++)
        {
            if(compareStringsOrNumbers((char *)columnData->data[columnIndex + numColumns * i], value) < 0)
            {
                for(int j = 0; j < (numSelectedColumns); j++)
                {
                    addArrayList(newColumnData, columnData->data[selectedIndices[j] + numColumns * i]);
                }            
            }
        }
    }
    else if(strcmp(operator, "<>") == 0 || strcmp(operator, "!=") == 0)
    {
        for(size_t i = 0; i < ((columnData->size) / numColumns); i++)
        {
            if(strcmp((char *)columnData->data[columnIndex + numColumns * i], value) != 0)
            {
                for(int j = 0; j < (numSelectedColumns); j++)
                {
                    addArrayList(newColumnData, columnData->data[selectedIndices[j] + numColumns * i]);
                }            
            }
        }
    }
    else if(strcmp(operator, ">=") == 0)
    {
        for(size_t i = 0; i < ((columnData->size) / numColumns); i++)
        {
            if(compareStringsOrNumbers((char *)columnData->data[columnIndex + numColumns * i], value) >= 0)
            {
                for(int j = 0; j < (numSelectedColumns); j++)
                {
                    addArrayList(newColumnData, columnData->data[selectedIndices[j] + numColumns * i]);
                }            
            }
        }
    }
    else if(strcmp(operator, "<=") == 0)
    {
        for(size_t i = 0; i < ((columnData->size) / numColumns); i++)
        {
            if(compareStringsOrNumbers((char *)columnData->data[columnIndex + numColumns * i], value) <= 0)
            {
                for(int j = 0; j < (numSelectedColumns); j++)
                {
                    addArrayList(newColumnData, columnData->data[selectedIndices[j] + numColumns * i]);
                }            
            }
        }
    }

    *(numRows) = newColumnData->size / (numSelectedColumns);

    // printTable(selectedColumnNames, newColumnData, *numRows, numSelectedColumns);

    // freeArrayList(newColumnData);

    // printf(" some data : %s\n", (char *)newColumnData->data[0]);
    // printf("size of data %lld\n", newColumnData->size);
    return newColumnData;
}




ArrayList* processAggregateFunction(char aggregateFunction[MAX_COLUMNS][MAX_FIELD_LENGTH], ArrayList* columnData, int numColumns, int numRows)
{
    ArrayList* newData = initArrayList();

    int sum[numColumns];
    for (int i = 0; i < numColumns; i++) {sum[i] = 0;}
    int count[numColumns];
    for (int i = 0; i < numColumns; i++) {count[i] = 0;}
    float avg[numColumns];
    for (int i = 0; i < numColumns; i++) {avg[i] = 0.0;}

    for(int i = 0; i < numColumns; i++)
    {
        if(strcmp(aggregateFunction[i], "SUM") == 0)
        { 
            for(int j = 0; j < numRows; j++)
            {
                int currentElement = atoi(columnData->data[numColumns * j + i]);
                // printf("current element %d", currentElement);
                sum[i] += currentElement;
                // printf("Sum: %d \n", sum[i]);
            }
            
            char *stringSum = (char *)malloc(MAX_FIELD_LENGTH * sizeof(char));

            printf("column data 1aa: %s\n", (char *)newData->data[0]);
            itoa(sum[i], stringSum, 10);
            printf("column data 1aaa: %s\n", (char *)newData->data[0]);
            
            addArrayList(newData, (void *)stringSum);
        }

        if(strcmp(aggregateFunction[i], "MIN") == 0)
        {
            void* min[numColumns];
            for (int i = 0; i < numColumns; i++) {min[i] = columnData->data[0];}
            for(int j = 0; j < numRows; j++)
            {
                void* currentElement = columnData->data[numColumns * j + i];
                // printf("current element: %s\n", (char *)currentElement);
                min[i] = (void *)getLowerValue((const char *)currentElement, (const char *)min[i]);
                // printf("min element: %s\n", (char *)min[i]);
            }
            char *stringMin = (char *)malloc(MAX_FIELD_LENGTH * sizeof(char));
            strcpy(stringMin, (void *)min[i]);
            addArrayList(newData, (void *)stringMin);
        }

        if(strcmp(aggregateFunction[i], "MAX") == 0)
        {
            void* max[numColumns];
            for (int i = 0; i < numColumns; i++) {max[i] = columnData->data[0];}
            for(int j = 0; j < numRows; j++)
            {
                void* currentElement = columnData->data[numColumns * j + i];
                // printf("current elemetn: %s\n", (char *)currentElement);
                max[i] = getBiggerValue(currentElement, max[i]);
                // printf("max element: %s\n", (char *)max[i]);
            }
            char *stringMax = (char *)malloc(MAX_FIELD_LENGTH * sizeof(char));
            strcpy(stringMax, (void *)max[i]);

            addArrayList(newData, (void *)stringMax);
        }

        if(strcmp(aggregateFunction[i], "COUNT") == 0)
        {
            for(int j = 0; j < numRows; j++)
            {
                count[i]++;
            }
            char *stringCount = (char *)malloc(MAX_FIELD_LENGTH * sizeof(char));
            sprintf(stringCount, "%d", count[i]);
            addArrayList(newData, (void *)stringCount);
        }

        if(strcmp(aggregateFunction[i], "AVG") == 0)
        {
            for(int j = 0; j < numRows; j++)
            {
                int currentElement = atoi(columnData->data[numColumns * j + i]);
                sum[i] += currentElement;
                count[i]++;
                avg[i] = (float)sum[i] / count[i];
            }
            char *stringAvg = (char *)malloc(MAX_FIELD_LENGTH * sizeof(char));
            sprintf(stringAvg, "%f", avg[i]);
            addArrayList(newData, (void *)stringAvg);
        }
    }

    return newData;
}




// Function to parse the aggregate function and column name
void parseAggregateFunction(char *input, char *aggregateFunction, char *columnName) 
{
    // Find the opening and closing parentheses
    char *openParenthesis = strchr(input, '(');
    char *closeParenthesis = strchr(input, ')');

    // If both parentheses are found and the closing parenthesis is after the opening parenthesis
    if (openParenthesis != NULL && closeParenthesis != NULL && closeParenthesis > openParenthesis) 
    {
        // Extract the aggregate function
        strncpy(aggregateFunction, input, openParenthesis - input);

        // Null-terminate the aggregate function string
        aggregateFunction[openParenthesis - input] = '\0';

        // Extract the column name
        strncpy(columnName, openParenthesis + 1, closeParenthesis - openParenthesis - 1);

        // Null-terminate the column name string
        columnName[closeParenthesis - openParenthesis - 1] = '\0';

        // Overwrite the original variable with the parsed column name
        strcpy(input, columnName);
    }
}





void extractClause(const char *input, char *output) 
{
    const char *start = input;
    const char *end = input;

    // Find the end of the clause
    while (*end && *end != ';' && *end != '\0') 
    {
        if (strncasecmp(end, "WHERE", 5) == 0 ||
            strncasecmp(end, "GROUP BY", 8) == 0 ||
            strncasecmp(end, "HAVING", 6) == 0 ||
            strncasecmp(end, "ORDER BY", 8) == 0 ||
            strncasecmp(end, "LIMIT", 5) == 0) 
            {
            break;  // Stop when a new clause begins
            }
        end++;
    }

    // Copy the clause to the output
    strncpy(output, start, end - start);
    output[end - start] = '\0';
}





ArrayList* selectAll(BPlusNode* node, const char* tableName, int numColumns, int* numRows, char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH]) 
{
    if (node == NULL) 
    {
        printf("The node is null.\n");
        return NULL;
    }

    ArrayList* columnData = initArrayList(); // Initialize the ArrayList

    if (node->is_leaf) 
    {      
        for (int i = 0; i < node->num_keys; i++) 
        {                   
            (*numRows)++;
            cJSON* jsonString = cJSON_Parse((const char*)findRecord(node, node->keys[i], NULL)->value);
            // printf("row: %s\n", cJSON_PrintUnformatted(jsonString)); // debug print
            if (jsonString != NULL) 
            {
                for (int j = 0; j < numColumns; j++) 
                {
                    const cJSON* tableField = cJSON_GetObjectItemCaseSensitive(jsonString, "table name");
                    if (cJSON_IsString(tableField) && (strcmp(tableField->valuestring, tableName) == 0)) 
                    {
                        const cJSON* dataDetails = cJSON_GetObjectItemCaseSensitive(jsonString, "data details");
                        // printf("dataDetails %s:\n", cJSON_PrintUnformatted(dataDetails));
                        const cJSON* columnValue = cJSON_GetObjectItemCaseSensitive(dataDetails, columnNames[j]);
                        // printf("columnValue %s:\n", cJSON_PrintUnformatted(columnValue));
                        if (cJSON_IsString(columnValue)) 
                        {
                            // printf("Column Values(before): %s\n", columnValue->valuestring);
    
                            // Allocate memory for the new string and copy its content
                            char* valueCopy = strdup(columnValue->valuestring);
                            
                            if (valueCopy != NULL) 
                            {
                                // Allocation and copying were successful
                                addArrayList(columnData, valueCopy); // Add the copied string to the ArrayList
                            } 
                            else 
                            {
                                // Allocation failed, handle the error
                                fprintf(stderr, "Memory allocation failed\n");
                            }        
   
                            // printf("Column Values(after): %s\n", (char*)getArrayList(columnData, sizeArrayList(columnData) - 1));
                        }
                    }                      
                }
                cJSON_Delete(jsonString);    
            }
        }   
    } 
    else 
    {
        for (int i = 0; i < node->num_keys + 1; i++) 
        {
            ArrayList* childData = selectAll((BPlusNode*)node->pointers[i], tableName, numColumns, numRows, columnNames);
            
            if (childData != NULL) {
                // Merge child data with current columnData
                for (size_t j = 0; j < sizeArrayList(childData); j++) {
                    addArrayList(columnData, getArrayList(childData, j));
                }
                freeArrayList(childData); // Free the memory used by the child ArrayList, 
            }
        }
    }
    return columnData;
}
