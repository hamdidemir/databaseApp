#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "include/createDatabase.h"
#include "include/fileExists.h"

#define MAX_PATH_LENGTH 1024

void createDatabase() 
{
    char dbname[MAX_PATH_LENGTH];

    // Ask the user for the database name
    printf("Enter the name of the new database: ");
    scanf("%s", dbname);

    createDatabaseWithName(dbname);
}

void createDatabaseWithName(const char *dbname) 
{
    char path[MAX_PATH_LENGTH];
    const char *dbExtension = ".db";

    // Check if the provided dbname ends with the .db extension
    if (strcmp(dbname + strlen(dbname) - strlen(dbExtension), dbExtension) == 0) 
    {
        // The provided name already ends with .db, no need to modify it
        snprintf(path, MAX_PATH_LENGTH, "databases/%s", dbname);
    } 
    else 
    {
        // The provided name does not end with .db, so add the extension
        snprintf(path, MAX_PATH_LENGTH, "databases/%s.db", dbname);
    }

    // Check if the file already exists
    if (fileExists(path)) 
    {
        printf("Database file '%s' already exists.\n", dbname);
        return;  // Do nothing and return, as the database already exists
    }

    // Create the new database file
    FILE *file = fopen(path, "w");
    if (!file) 
    {
        printf("Error creating the database file.\n");
        return;
    }

    fclose(file);
    printf("Database file '%s' created successfully.\n", dbname);
}