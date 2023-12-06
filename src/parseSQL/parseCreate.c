#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "include/trim.h"
#include "include/checkTable.h"

#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50
extern const char* dbname; // Define database name as a global variable

void parseCreate(const char *query) 
{
    char tableName[MAX_FIELD_LENGTH];
    char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH];
    int numColumns = 0;

    const char *cursor = query;

    // Find the CREATE TABLE statement
    while (*cursor && !strncasecmp(cursor, "CREATE TABLE", 12)) 
    {
        cursor += 12;
    }

    // Find the table name
    while (*cursor && isspace(*cursor)) 
    {
        cursor++;
    }

    int i = 0;
    while (*cursor && *cursor != '(' && i < MAX_FIELD_LENGTH - 1) 
    {
        tableName[i++] = *cursor;
        cursor++;
    }
    tableName[i] = '\0';

    // Find column names and data types
    while (*cursor && *cursor != '(') 
    {
        cursor++;
    }

    while (*cursor && *cursor != ')' && numColumns < MAX_COLUMNS) 
    {
        while (*cursor && (*cursor == ' ' || *cursor == ',' || *cursor == '(')) 
        {
            cursor++;
        }

        if (*cursor == ')') 
        {
            break;
        }

        i = 0;
        while (*cursor && *cursor != ' ' && *cursor != ',' && *cursor != ')' && i < MAX_FIELD_LENGTH - 1) 
        {
            columnNames[numColumns][i++] = *cursor;
            cursor++;
        }
        columnNames[numColumns][i] = '\0';

        while (*cursor && (*cursor == ' ')) {
            cursor++;
        }

        i = 0;
        while (*cursor && *cursor != ',' && *cursor != ')' && i < MAX_FIELD_LENGTH - 1) 
        {
            dataTypes[numColumns][i++] = *cursor;
            cursor++;
        }
        dataTypes[numColumns][i] = '\0';

        numColumns++;
    }

    // Remove leading and ending white spaces
    trim(tableName);   
    for ( i=0; i < numColumns; i++)
    {
        trim(columnNames[i]);
        trim(dataTypes[i]);
    }

    // Check if the table is already exist
    if (checkTable(tableName))
    {
        printf("The table %s is already exist!\n", tableName);
        return;
    }

    // Open the database file
    FILE *databaseFile = fopen(dbname, "a");
    if (databaseFile == NULL)
    {
        printf("Error opening database file: %s\n", dbname);
        return;
    }
   
    // Start writing JSON data
    fprintf(databaseFile, "{\"type\": \"table\",  \"table name\": \"%s\", \"columns\": [", tableName);

    // Write column names and data types in JSON array
    for (int i = 0; i < numColumns; i++)
    {
        fprintf(databaseFile, "{\"name\": \"%s\", \"type\": \"%s\"}", columnNames[i], dataTypes[i]);
        
        if (i < numColumns - 1)
            fprintf(databaseFile, ",");
        
        fprintf(databaseFile, " ");
    }
    fprintf(databaseFile, "]}\n");

    // Close the database file
    fclose(databaseFile);
}
