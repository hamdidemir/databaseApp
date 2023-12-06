#include "include/unity.h"
#include <stdbool.h>
#include "include/createDatabase.h"
#include "include/deleteDatabase.h"
#include "include/fileExists.h"

void test_CreateAndDeleteDatabase() 
{
    // bypass setup
    remove("test_db.db");

    const char* testDBName = "testingDatabase";
    char dbFilePath[1024];
    snprintf(dbFilePath, sizeof(dbFilePath), "databases/%s.db", testDBName);

    // Test if the database file was created
    createDatabaseWithName(testDBName);
    TEST_ASSERT_TRUE(fileExists(dbFilePath));

    // Test if the database file was deleted
    deleteDatabaseWithName(testDBName);
    TEST_ASSERT_FALSE(fileExists(dbFilePath));
}