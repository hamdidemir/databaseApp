#include <stdio.h>
#include "include/unity.h"
#include "include/bplustree.h"
#include "include/processDatabaseFile.h"

// Declare the global variables used in the main program
extern BPlusNode* root;
extern const char* dbname;

void testProcessDatabaseFile(void) 
{
    FILE* dbFile = fopen(dbname, "w");
    fprintf(dbFile, "{\"type\":\"data\",\"field\":\"value1\"}\n");
    fprintf(dbFile, "{\"type\":\"data\",\"field\":\"value2\"}\n");
    fclose(dbFile);

    // Call the function to process the test database file
    processDatabaseFile();

    // Implement assertions to verify the behavior of the function
    // You can use findRecord to retrieve data from the B+ tree and assert the results.
    record* record1 = findRecord(root, 1, NULL);
    record* record2 = findRecord(root, 2, NULL);

    TEST_ASSERT_NOT_NULL(record1);
    TEST_ASSERT_NOT_NULL(record2);
    

    // Add assertions to compare the retrieved data with expected values
    TEST_ASSERT_EQUAL_STRING("{\"type\":\"data\",\"field\":\"value1\"}", record1->value);
    TEST_ASSERT_EQUAL_STRING("{\"type\":\"data\",\"field\":\"value2\"}", record2->value);

    remove(dbname);
}

