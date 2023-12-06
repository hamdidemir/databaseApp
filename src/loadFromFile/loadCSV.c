#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "include/trim.h"
#include "include/checkTable.h"
#include "include/loadCSV.h"
#include "include/fileExists.h"
#include "include/bplustree.h"
#include "include/processDatabaseFile.h"

extern const char* dbname; 
extern BPlusNode* root; // Define the global HashMap variable

void loadCSV()
{
    char csvFile[MAX_FIELD_LENGTH]; // Variable to store the filename
    char tableName[MAX_FIELD_LENGTH]; // Variable to store the table name
    char line[MAX_FIELD_LENGTH];
    char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char dbColumnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char values[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH];
    int numColumns = 0;
    int dbNumColumns = 0;
    int rows = 0;
    FILE *file;

    printf("Enter the csv filename: ");
    scanf("%s", csvFile);

    printf("Enter the table name: ");
    scanf("%s", tableName);

    // check if the table exist or not
    if (!checkTable(tableName))
    {
        printf("Table %s does not exist.\n", tableName);
        return;
    }

    if(fileExists(csvFile))
    {
        file = fopen(csvFile, "r"); // Open the CSV file for reading
    }
    else
    {   
        printf("cannot find the file %s\n", csvFile);
        file = NULL;
    }

    if (file == NULL) 
    {
        printf("Failed to open the csv file.\n");
        return;
    }
    
    // Read the header line to get the column names
    if (fgets(line, sizeof(line), file)) 
    {
        // Parse the column names
        char *token = strtok(line, ",");
        while (token != NULL && numColumns < MAX_COLUMNS) 
        {
            strcpy(columnNames[numColumns], token);
            token = strtok(NULL, ",");
            numColumns++;
        }
    }

    trim(tableName);
    for(int i = 0; i < numColumns; i++)
    {
        trim(columnNames[i]);
        trim(values[i]);
    }

    dbNumColumns = getTableInfo(tableName, dbColumnNames, dataTypes);

    if(numColumns == dbNumColumns)
    {
        for (int i = 0; i < numColumns; i++) 
        {
            if (strcmp(columnNames[i], dbColumnNames[i]) != 0)
            {
                printf("The names of the columns does not macthes: from csv: %s and from db: %s\n",columnNames[i], dbColumnNames[i]);
                return;
            }
        }
    }

    // Read and parse each data line
    while (fgets(line, sizeof(line), file)) 
    {
        // Parse the values
        char *token = strtok(line, ",");
        int column = 0;
        while (token != NULL && column < numColumns) 
        {
            strcpy(values[column], token);
            token = strtok(NULL, ",");
            column++;
        }

        // Call the write function with the parsed data
        writeToDatabase(tableName, numColumns, columnNames, values);
        rows++;
    }

    fclose(file);

    // Process the new data into memory after resetting the tree
    freeBPlusTree(root);
    root = NULL;
    processDatabaseFile();
}

void writeToDatabase(char tableName[MAX_FIELD_LENGTH], int numColumns, char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH], char values[MAX_COLUMNS][MAX_FIELD_LENGTH])
{
    // Open the database file
    FILE *databaseFile = fopen(dbname, "a");
    
    if (databaseFile == NULL)
    {
        printf("Error opening database file(writeToDatabase): %s\n", dbname);
        return;
    }

    trim(tableName);
    for(int i = 0; i < numColumns; i++)
    {
        trim(columnNames[i]);
        trim(values[i]);
    }

    // Start writing JSON data
    fprintf(databaseFile, "{ \"type\": \"data\", \"table name\": \"%s\", \"data details\": {", tableName);

    for (int i = 0; i < numColumns; i++)
    {
        fprintf(databaseFile, "\"%s\": \"%s\"", columnNames[i], values[i]);
        
        if (i < numColumns - 1)
        {
            fprintf(databaseFile, ",");
        }
    }
    fprintf(databaseFile, "}}\n");

    // Close the database file
    fclose(databaseFile);
}