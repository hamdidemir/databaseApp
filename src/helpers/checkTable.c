#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/cJSON.h"
#include "include/checkTable.h"
#include "include/isDataEntry.h"

extern const char* dbname; // Define database name as a global variable

// Function to check if a table exists in the database
int checkTable(const char *tableName) 
{  
    FILE *databaseFile = fopen(dbname, "r");
    if (databaseFile == NULL) 
    {
        printf("Error opening database file(checkTable): %s\n", dbname);
        return 0; // Unable to open the database file
    }

    char line[1024];

    while (fgets(line, sizeof(line), databaseFile) != NULL) 
    {
        // printf("line: %s\n", line);
        cJSON *root = cJSON_Parse(line);

        if(root == NULL){printf("root is null\n");}

        if (root != NULL && !isDataEntry(line)) 
        {  
            cJSON *typeField = cJSON_GetObjectItemCaseSensitive(root, "type");
            if (cJSON_IsString(typeField) && (strcmp(typeField->valuestring, "table") == 0)) 
            {
                cJSON *tableNameField = cJSON_GetObjectItemCaseSensitive(root, "table name");
                if (cJSON_IsString(tableNameField) && (strcmp(tableNameField->valuestring, tableName) == 0))
                {
                    cJSON_Delete(root);
                    fclose(databaseFile);
                    return 1; // Table exists
                }
            }
            cJSON_Delete(root);
        }
    }

    fclose(databaseFile);

    return 0; // Table does not exist
}

// Function to retrieve table information
int getTableInfo(const char *tableName, char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH]) 
{  
    FILE *databaseFile = fopen(dbname, "r");
    if (databaseFile == NULL) 
    {
        printf("Error opening database file: %s\n", dbname);
        return 0; // Unable to open the database file
    }

    char line[1024];
    cJSON *root;

    int columnCount = 0; // To keep track of the number of columns found

    while (fgets(line, sizeof(line), databaseFile) != NULL) 
    {
        // printf("line %s\n",line);
        root = cJSON_Parse(line);
        if(root == NULL){printf("root is null(getTableInfo)\n");}

        if (root != NULL) 
        {
            cJSON *typeField = cJSON_GetObjectItemCaseSensitive(root, "type");
            if (cJSON_IsString(typeField) && (strcmp(typeField->valuestring, "table") == 0)) 
            {
                cJSON *tableNameField = cJSON_GetObjectItemCaseSensitive(root, "table name");
                if (cJSON_IsString(tableNameField) && (strcmp(tableNameField->valuestring, tableName) == 0))
                {
                    // Table exists, now retrieve column information
                    cJSON *columnsField = cJSON_GetObjectItemCaseSensitive(root, "columns");
                    if (cJSON_IsArray(columnsField)) 
                    {
                        cJSON *column;
                        cJSON_ArrayForEach(column, columnsField) 
                        {
                            cJSON *columnNameField = cJSON_GetObjectItemCaseSensitive(column, "name");
                            cJSON *dataTypeField = cJSON_GetObjectItemCaseSensitive(column, "type");

                            if (cJSON_IsString(columnNameField) && cJSON_IsString(dataTypeField)) 
                            {
                                // Store column name and data type in arrays
                                if (columnCount < MAX_COLUMNS) 
                                {
                                    strcpy(columnNames[columnCount], columnNameField->valuestring);
                                    strcpy(dataTypes[columnCount], dataTypeField->valuestring);
                                    columnCount++;
                                }
                            }
                        }
                    }

                    cJSON_Delete(root);
                    fclose(databaseFile);

                    // Return the number of columns found
                    return columnCount;
                }
            }
            cJSON_Delete(root);
        }
    }

    fclose(databaseFile);

    return 0; // Table does not exist
}