#include "include/unity.h"
#include "include/checkTable.h"  // Include the header file that contains the checkTable function

extern const char* dbname; // Define database name as a global variable
#define MAX_FIELD_LENGTH 1024
#define MAX_COLUMNS 50


void test_CheckTable_WhenTableExists() 
{
    // Create a sample database file with an existing table
    const char* dbName = "sample_db.json";
    FILE* file = fopen(dbName, "w");
    fprintf(file, "{\"type\":\"table\",\"table name\":\"existing_table\"}\n");
    fclose(file);

    // Test the checkTable function
    dbname = dbName; // Set the dbname global variable
    TEST_ASSERT_EQUAL_INT(1, checkTable("existing_table"));

    // Clean up the sample database file
    remove(dbName);
}

void test_CheckTable_WhenTableDoesNotExist() 
{
    // Create a sample database file with a non-existing table
    const char* dbName = "sample_db.json";
    FILE* file = fopen(dbName, "w");
    fprintf(file, "{\"type\":\"table\",\"table name\":\"another_table\"}\n");
    fclose(file);

    // Test the checkTable function
    dbname = dbName; // Set the dbname global variable
    TEST_ASSERT_EQUAL_INT(0, checkTable("non_existing_table"));

    // Clean up the sample database file
    remove(dbName);
}

void test_GetTableInfo_WhenTableExists() 
{
    // Create a sample database file with an existing table and columns
    const char* dbName = "sample_db.json";
    FILE* file = fopen(dbName, "w");
    fprintf(file, "{\"type\":\"table\",\"table name\":\"existing_table\",\"columns\":[{\"name\":\"column1\",\"type\":\"int\"},{\"name\":\"column2\",\"type\":\"text\"}]}\n");
    fclose(file);

    // Test the getTableInfo function
    dbname = dbName; // Set the dbname global variable
    char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH];
    int columnCount = getTableInfo("existing_table", columnNames, dataTypes);
    
    TEST_ASSERT_EQUAL_INT(2, columnCount);
    TEST_ASSERT_EQUAL_STRING("column1", columnNames[0]);
    TEST_ASSERT_EQUAL_STRING("int", dataTypes[0]);
    TEST_ASSERT_EQUAL_STRING("column2", columnNames[1]);
    TEST_ASSERT_EQUAL_STRING("text", dataTypes[1]);

    // Clean up the sample database file
    remove(dbName);
}

void test_GetTableInfo_WhenTableDoesNotExist() 
{
    // Create a sample database file with a non-existing table
    const char* dbName = "sample_db.json";
    FILE* file = fopen(dbName, "w");
    fprintf(file, "{\"type\":\"table\",\"table name\":\"another_table\"}\n");
    fclose(file);

    // Test the getTableInfo function
    dbname = dbName; // Set the dbname global variable
    char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH];
    char dataTypes[MAX_COLUMNS][MAX_FIELD_LENGTH];
    int columnCount = getTableInfo("existing_table", columnNames, dataTypes);

    TEST_ASSERT_EQUAL_INT(0, columnCount);

    // Clean up the sample database file
    remove(dbName);
}


