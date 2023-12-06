#include <stdio.h>
#include <string.h>
#include "include/parseCreate.h"
#include "include/parseInsert.h"
#include "include/bplustree.h"

extern BPlusNode* root; // Define the global HashMap variable
extern const char* dbname; // Define database name as a global variable

void testParseCreate() 
{
    // Test query
    const char *testCreateQuery = "CREATE TABLE people (id INT, name TEXT, age INT);";
    const char *testInsertQuery = "INSERT INTO people (id, name, age) VALUES (1, Hamdi, 25);";

    // Call the parsing function
    parseCreate(testCreateQuery);
    parseInsert(testInsertQuery);
    

    record* foundRecord = findRecord(root, 1, NULL);

    if (foundRecord != NULL) 
    {
        // Assuming 'value' is a string (char*) in this example
        char* value = (char*)foundRecord->value;

        // Check if the value is not NULL to avoid issues
        if (value != NULL) 
        {
            printf("record: %s\n", value);
        } 
        else 
        {
            printf("record is NULL\n");
        }
    } 
    else 
    {
        printf("Record not found\n");
    }
}