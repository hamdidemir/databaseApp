#include <stdio.h>
#include "include/unity.h"
#include "include/bplustree.h"

// Handles the things before and after every tests

// Define a temporary test file
#define TEST_DB_FILENAME "test_db.db"

// Declare the global variables used in the main program
extern BPlusNode* root;
extern const char* dbname;

BPlusNode* tempNode; // Temporary storage for the original root
BPlusNode* testTree;
const char* tempDbName;

void setUp(void) 
{
    // Store the original root for teardown
    tempNode = root;
    // Create a new B+ tree for testing
    testTree = createNewTree();
    // Overwrite the tree with test tree
    root = testTree;

    // Set the global database filename to the test file
    tempDbName = dbname;
    dbname = TEST_DB_FILENAME;
}

void tearDown(void) 
{
    // Remove the test database file after each test
    char dbFilePath[1024];
    snprintf(dbFilePath, sizeof(dbFilePath), "databases/%s", TEST_DB_FILENAME);
    remove(dbFilePath);
    // Free the B+ tree created during setup
    freeBPlusTree(testTree);
    // Restore the original tree
    root = tempNode;
    // Restore the original dbName
    dbname = tempDbName;
}

