#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "include/trim.h"
#include "include/checkTable.h"

#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50
extern const char* dbname; // Define database name as a global variable

void parseInsert(const char *query) 
{
    char tableName[MAX_FIELD_LENGTH];
    char columns[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char values[MAX_COLUMNS][MAX_FIELD_LENGTH];
    int numColumns = 0;
    int numValues = 0;

    const char *cursor = query;

    // Search for INSERT INTO
    while(*cursor && !strncasecmp(cursor, "INSERT INTO", 11)) {
        cursor += 11;
    }

    // Get table name
    while (*cursor && isspace(*cursor)) {
        cursor++;
    }

    int i = 0;
    while (*cursor && *cursor != '(' && i < MAX_FIELD_LENGTH - 1) 
    {
        tableName[i++] = *cursor;
        cursor++;
    }
    tableName[i] = '\0';

    // Find the opening parenthesis '('
    while (*cursor && *cursor != '(') 
    {
        cursor++;
    }

    // Parse column names between '(' and ')'
    while (*cursor && *cursor != ')' && numColumns < MAX_COLUMNS) {
        i = 0;
        cursor++; // Move past '(' or ','

        // Read characters until a comma or closing parenthesis is encountered
        while (*cursor && *cursor != ',' && *cursor != ')') {
            columns[numColumns][i++] = *cursor;
            cursor++;
        }
        columns[numColumns][i] = '\0';
        numColumns++;

        // Move past ',' if there's one, or ')' if it's the last column
        if (*cursor == ',') 
        {
            cursor++;
        } else if (*cursor == ')') 
        {
            cursor++;
            break; // Exit the loop when ')' is encountered
        }
    }

    // Find the "VALUES" keyword
    while (*cursor && !strncasecmp(cursor, "VALUES", 6)) {
        cursor += 6;
    }

    while (*cursor && *cursor != '(') {
        cursor++;
    }

    // Parse values between '(' and ')'
    while (*cursor && *cursor != ')' && numValues < MAX_COLUMNS) {
        i = 0;
        cursor++; // Move past '(' or ','

        // Read characters until a comma or closing parenthesis is encountered
        while (*cursor && *cursor != ',' && *cursor != ')') {
            values[numValues][i++] = *cursor;
            cursor++;
        }
        values[numValues][i] = '\0';
        numValues++;

        // Move past ',' if there's one, or ')' if it's the last value
        if (*cursor == ',') {
            cursor++;
        } else if (*cursor == ')') {
            cursor++;
            break; // Exit the loop when ')' is encountered
        }
    }

    
    // check if the table exist or not
    if (!checkTable(tableName))
    {
        printf("Table %s does not exist.\n", tableName);
        return;
    }

    // Open the database file
    FILE *databaseFile = fopen(dbname, "a");
    
    if (databaseFile == NULL)
    {
        printf("Error opening database file: %s\n", dbname);
        return;
    }

    trim(tableName);
    for(int i = 0; i < numColumns; i++)
    {
        trim(columns[i]);
        trim(values[i]);
    }

    // Start writing JSON data
    fprintf(databaseFile, "{ \"type\": \"data\", \"table name\": \"%s\", \"data details\": {", tableName);

    for (int i = 0; i < numColumns; i++)
    {
        fprintf(databaseFile, "\"%s\": \"%s\"", columns[i], values[i]);
        
        if (i < numColumns - 1)
        {
            fprintf(databaseFile, ",");
        }
    }
    fprintf(databaseFile, "}}\n");

    // Close the database file
    fclose(databaseFile);
}

// INSERT INTO employees (ID, FirstName, LastName) VALUES (1, 'John', 'Smith');
// INSERT INTO people (name, age) VALUES (Hamdi, 25);


// CREATE TABLE people (id INT, Name TEXT, Surname TEXT, Counrty Text, Age INT);
// INSERT INTO people (id, Name, Surname, Counrty, Age) VALUES (0, Hamdi, Demir, Turkiye, 29);