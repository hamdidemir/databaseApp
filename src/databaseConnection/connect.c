#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "include/bplustree.h"
#include "include/processDatabaseFile.h"
#include "include/disconnect.h"
#include "include/fileExists.h"

#define MAX_PATH_LENGTH 1024

// Define the global variables here
BPlusNode *root = NULL;
const char *dbname = NULL;

void connect() 
{
    if (root != NULL) 
    {
        // Already connected to a database
        printf("Already connected to database '%s'\n", dbname);
        printf("Do you want to disconnect? (y/n): ");
        char response;
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y') 
        {
            disconnect();
            printf("Disconnected from database.\n");
        }
    } 
    else 
    {
        // Not connected to a database, prompt for connection
        printf("Enter the database filename: ");
        char filename[MAX_PATH_LENGTH];  // Define a buffer for the full file path
        scanf(" %s", filename);  // Read the user input

        // Check if the filename ends with ".db" and append it if not
        if (strncmp(filename + strlen(filename) - 3, ".db", 3) != 0) 
        {
            strcat(filename, ".db");
        }

        // Construct the full path to the database file in the "databases" directory
        char path[MAX_PATH_LENGTH];
        snprintf(path, MAX_PATH_LENGTH, "databases/%s", filename);

        if (fileExists(path)) 
        {
            dbname = strdup(path);  // Allocate memory for dbname and set it to the full path

            root = createNewTree();

            processDatabaseFile();

            if (root != NULL)
            {
                printf("Connection to %s is successful.\n", dbname);
            }
            else
            {
                printf("Connection failed.\n");
            }

            // Make sure to free memories when quitting
            atexit(disconnect);
        } 
        else 
        {
            printf("Database file '%s' does not exist.\n", path);
        }
    }
}
