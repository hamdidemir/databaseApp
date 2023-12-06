#include <stdio.h>
#include <stdlib.h>
#include "include/cJSON.h" 
#include "include/bplustree.h"
#include "include/isDataEntry.h"

extern const char* dbname; 
extern BPlusNode* root; 

void processDatabaseFile() 
{
    // Open the database file
    FILE *dbFile = fopen(dbname, "r");
    if (dbFile == NULL) 
    {
        printf("Error opening database file(processDatabaseFile): %s\n", dbname);
        return;
    }

    // Initialize the B+ tree
    int uniqueKey = 1;

    char line[2048];
    while (fgets(line, sizeof(line), dbFile)) 
    {
        if (isDataEntry(line)) 
        {
            // Parse the line into a JSON string
            cJSON *json = cJSON_Parse(line);
            char *jsonString = cJSON_PrintUnformatted(json);

            // Insert the JSON string into the B+ tree with the unique key
            root = insertIntoTree(root, uniqueKey, jsonString);
            uniqueKey++;
            cJSON_Delete(json);
        }
    }
    
    // Close the database file
    fclose(dbFile);
}