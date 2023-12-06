#include "include/unity.h"
#include "include/bplustree.h"
#include "include/arrayList.h"
#include "include/parseSelect.h"
#include <stdlib.h>

extern BPlusNode* root;

void test_inOrderDataTraversal_WhenTreeExists() 
{
    char* entries[] = 
    {
        "{ \"type\": \"data\", \"table name\": \"people\", \"data details\": {\"id\": \"1\",\"name\": \"Hamdi\",\"age\": \"25\"} }",
        "{ \"type\": \"data\", \"table name\": \"people\", \"data details\": {\"id\": \"2\",\"name\": \"Fatih\",\"age\": \"35\"} }",
        "{ \"type\": \"data\", \"table name\": \"people\", \"data details\": {\"id\": \"3\",\"name\": \"Mahmut\",\"age\": \"45\"} }",
        "{ \"type\": \"data\", \"table name\": \"people\", \"data details\": {\"id\": \"4\",\"name\": \"Mahmut\",\"age\": \"45\"} }",
        "{ \"type\": \"data\", \"table name\": \"people\", \"data details\": {\"id\": \"5\",\"name\": \"Mahmut\",\"age\": \"45\"} }",
        "{ \"type\": \"data\", \"table name\": \"people\", \"data details\": {\"id\": \"6\",\"name\": \"Mahmut\",\"age\": \"45\"} }"
    };

    for (int i = 0; i < 6; i++) 
    {
        root = insertIntoTree(root, i + 1, entries[i]);
    }

    const char *tableName = "people";
    char columnNames[MAX_COLUMNS][MAX_FIELD_LENGTH] = {"id", "name", "age"};
    ArrayList* columnData = initArrayList();
    int numColumns = 3;
    int numRows = 0;

    // Test the inOrderDataTraversal function
    columnData = selectAll(root, tableName, numColumns, &numRows, columnNames);


    /*
    for(int i=0; i<9;i++)
    {printf("column data: %s\n", (char *)columnData->data[i]);}
    */

    TEST_ASSERT_EQUAL_INT(6, numRows);

    // Assuming you want to check specific data in columnData, you can use Unity assertions like this:
    TEST_ASSERT_EQUAL_STRING("1", columnData->data[0]);
    TEST_ASSERT_EQUAL_STRING("Hamdi", columnData->data[1]);
    TEST_ASSERT_EQUAL_STRING("25", columnData->data[2]);

    TEST_ASSERT_EQUAL_STRING("2", columnData->data[3]);
    TEST_ASSERT_EQUAL_STRING("Fatih", columnData->data[4]);
    TEST_ASSERT_EQUAL_STRING("35", columnData->data[5]);

    TEST_ASSERT_EQUAL_STRING("3", columnData->data[6]);
    TEST_ASSERT_EQUAL_STRING("Mahmut", columnData->data[7]);
    TEST_ASSERT_EQUAL_STRING("45", columnData->data[8]);
    

    freeArrayList(columnData);
}
