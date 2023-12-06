#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "include/unity.h"
#include "include/bplustree.h"
#include "include/disconnect.h"
#include "include/fileExists.h"
#include "include/createDatabase.h"
#include "include/deleteDatabase.h"
#include "include/connect.h"

extern BPlusNode* root; // Define the global HashMap variable
extern const char* dbname; // Define database name as a global variable

// Define a helper function to redirect stdin for testing
void redirectStdin(const char* input) 
{
    FILE* newStdin = freopen("test_input.txt", "w+", stdin);
    if (newStdin != NULL) 
    {
        fprintf(newStdin, "%s\n", input);
        rewind(newStdin);
    }
}

// Define a test function for the integrated test
void testConnectandDisconnect() 
{   
    // set_up function connects to the test database before each test, for the testing of connection we first test diconnect
    // bypass setup
    root = NULL;
    
    char dbFilePath[1024];
    snprintf(dbFilePath, sizeof(dbFilePath), "databases/%s.db", dbname);

    // Test if the database file was created
    createDatabaseWithName(dbname);

    // Copy the state of stdin because redirectStdin uses freopen and changes the stream to simulate the user input
    int fd = dup(STDIN_FILENO);

    // Connect to the database
    redirectStdin(dbname);
    connect();
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_NOT_NULL(dbname);

    // Restore the stdin after running freopen(in redirectStdin function) and close stdin to remove temporary txt file.
    // Important note here: had stdin not been restored, all the scanfs in the program will malfunction.
    close(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    remove("test_input.txt");


    // Disconnect from the database
    disconnect();
    TEST_ASSERT_NULL(root);
    TEST_ASSERT_NULL(dbname);
}