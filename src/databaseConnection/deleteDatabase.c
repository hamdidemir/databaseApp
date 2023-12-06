#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "include/deleteDatabase.h"
#include "include/fileExists.h"

#define MAX_PATH_LENGTH 1024

void deleteDatabase() 
{
    char dbname[MAX_PATH_LENGTH];

    // Ask the user for the database name
    printf("Enter the name of the database to delete: ");
    scanf("%s", dbname);

    deleteDatabaseWithName(dbname);
}

void deleteDatabaseWithName(const char *dbname) 
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

    if (!fileExists(path)) 
    {
        printf("Database file '%s' does not exist.\n", dbname);
        return;  // Do nothing and return, as the database doesn't exist
    }

    if (remove(path) == 0) 
    {
        printf("Database file '%s' deleted successfully.\n", dbname);
    } 
    else 
    {
        printf("Error deleting the database file.\n");
    }
}