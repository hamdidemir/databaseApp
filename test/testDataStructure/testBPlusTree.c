#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/testBPlusTree.h"


// Function to assert the creation of a B+ tree
void assertCreateBPlusTree() 
{
    int passes = 0;
    int failures = 0;
    // Test 3: Check if createLeafBPlusNode creates a valid leaf node.
    BPlusNode* leafNode = createLeafBPlusNode();
    if (leafNode == NULL) 
    {
        printf("✗ Test 1 FAILED: createLeafBPlusNode returned NULL.\n");
        failures++;
    } else {
        printf("✓ Test 1 passed. createLeafBPlusNode created a valid leaf node.\n");
        passes++;
    }

    // Test 4: Check if createInternalBPlusNode creates a valid internal node.
    BPlusNode* internalNode = createInternalBPlusNode();
    if (internalNode == NULL) {
        printf("✗ Test 2 FAILED: createInternalBPlusNode returned NULL.\n");
        failures++;
    } else {
        printf("✓ Test 2 passed. createInternalBPlusNode created a valid internal node.\n");
        passes++;
    }

    if (failures == 0) {
        printf("✓✓✓ 'createBPlusTree', 'createLeafBPlusNode', and 'createInternalBPlusNode' functions passed %d test(s) successfully. ✓✓✓\n", passes);
    } else {
        printf("✗✗✗ 'createBPlusTree', 'createLeafBPlusNode', and 'createInternalBPlusNode' functions failed %d of %d test(s). ✗✗✗\n", failures, failures + passes);
    }

    // Clean up (free the B+ tree and its nodes)
    // Modify this part to free your B+ tree and nodes accordingly
    free(leafNode);
    free(internalNode);
}


// Function to assert the insertion into a B+ tree
void assertInsert() 
{
    BPlusNode* root = createLeafBPlusNode();
    int passes = 0;
    int failures = 0;

    // Test 1: Insert a single key-value pair
    int key1 = 1;
    int value1 = 100;

    root = insertIntoTree(root, key1, (void *)value1);

    int *retrievedValue1 = (int *)root->pointers[0];
    
    // Check if the value can be indirectly validated
    if (root->keys[0] == key1 && *retrievedValue1 == value1) 
    {
        printf("✓ Test 1 passed. A single key-value pair inserted correctly.\n");
        passes++;
    } 
    else 
    {
        printf("✗ Test 1 FAILED: Key not inserted correctly. Expected key %d, Found key %d, Expected value %d, Found value %d\n", key1, root->keys[0], value1, *retrievedValue1);
        failures++;
    }

    // Test 2: Insert 1 more key-value pairs for (ORDER 3)
    int key2 = 2;
    int value2 = 200;

    root = insertIntoTree(root, key2, (void *)value2);

    int *retrievedValue2 = (int *)root->pointers[1];

    // Check if the value can be indirectly validated
    if (root->keys[0] == key1 && *retrievedValue1 == value1 &&
        root->keys[1] == key2 && *retrievedValue2 == value2) 
    {
        printf("✓ Test 2 passed. Total of 2 key-value pairs inserted successfully (Order 3).\n");
        passes++;
    } 
    else 
    {
        printf("✗ Test 2 FAILED: Keys are not inserted correctly (Order 3). \n");
        printf("     Expected values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d'\n", key1, value1, key2, value2);
        printf("     Found values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d'\n", root->keys[0], *retrievedValue1, root->keys[1], *retrievedValue2);
        failures++;
    }

    // Test 3: Insert 2 more (total of 4) key-value pairs (Order 3)

    int key3 = 3;
    int value3 = 300;
    int key4 = 4;
    int value4 = 400;
    int key5 = 5;
    int value5 = 500;

    root = insertIntoTree(root, key3, (void *)value3);
    
    BPlusNode* leaf0 = root->pointers[0];
    BPlusNode* leaf1 = root->pointers[1];

    int *newretrievedValue1= (int *)leaf0->pointers[0];
    int *newretrievedValue2 = (int *)leaf1->pointers[0];
    int *newretrievedValue3 = (int *)leaf1->pointers[1];


    // Check if the values can be indirectly validated
    if (leaf0->keys[0] == key1 && *newretrievedValue1 == value1 &&
        leaf1->keys[0] == key2 && *newretrievedValue2 == value2 &&
        leaf1->keys[1] == key3 && *newretrievedValue3 == value3)
    {
        printf("✓ Test 3 passed. Total of 3 key-value pairs inserted successfully (Order 3).\n");
        passes++;
    } 
    else 
    {
        printf("✗ Test 3 FAILED: Keys are not inserted correctly.\n");
        printf("     Expected values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d', key 3: '%d', value 3: '%d\n", key1, value1, key2, value2, key2, value2);
        printf("     Found values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d', key 3: '%d', value 3: '%d'\n", leaf0->keys[0], *newretrievedValue1, leaf1->keys[0], *newretrievedValue2, leaf1->keys[1], *newretrievedValue3);
        failures++;
    }

    // Test 4: Insert 7 more (total of 10) key-value pairs (Order 3) to chech if splitting is done right
    int keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int values[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

    for (int i = 3; i < 10; i++) 
    {
        root = insertIntoTree(root, keys[i], values[i]);
    }

    // Determine the structure of the tree by hand (since it should be done without any search functionality, yet), (for ORDER 3)

    BPlusNode* internal0 = root->pointers[0];
    BPlusNode* internal1 = root->pointers[1];
    BPlusNode* internal2 = internal0->pointers[0];
    BPlusNode* internal3 = internal0->pointers[1];
    BPlusNode* internal4 = internal1->pointers[0];
    BPlusNode* internal5 = internal1->pointers[1];
    
    BPlusNode* newleaf0 = internal2->pointers[0];
    BPlusNode* newleaf1 = internal2->pointers[1];
    BPlusNode* newleaf2 = internal3->pointers[0];
    BPlusNode* newleaf3 = internal3->pointers[1];
    BPlusNode* newleaf4 = internal4->pointers[0];
    BPlusNode* newleaf5 = internal4->pointers[1];
    BPlusNode* newleaf6 = internal5->pointers[0];
    BPlusNode* newleaf7 = internal5->pointers[1];
    BPlusNode* newleaf8 = internal5->pointers[2];
    
    int *retValue1 = newleaf0->pointers[0];
    int *retValue2 = newleaf1->pointers[0];
    int *retValue3 = newleaf2->pointers[0];
    int *retValue4 = newleaf3->pointers[0];
    int *retValue5 = newleaf4->pointers[0];
    int *retValue6 = newleaf5->pointers[0];
    int *retValue7 = newleaf6->pointers[0];
    int *retValue8 = newleaf7->pointers[0];
    int *retValue9 = newleaf8->pointers[0];
    int *retValue10 = newleaf8->pointers[1];

    // Check if the values are inserted at correct places
    if (newleaf0->keys[0] == keys[0] && *retValue1 == values[0] &&
        newleaf1->keys[0] == keys[1] && *retValue2 == values[1] &&
        newleaf2->keys[0] == keys[2] && *retValue3 == values[2] &&
        newleaf3->keys[0] == keys[3] && *retValue4 == values[3] &&
        newleaf4->keys[0] == keys[4] && *retValue5 == values[4] &&
        newleaf5->keys[0] == keys[5] && *retValue6 == values[5] &&
        newleaf6->keys[0] == keys[6] && *retValue7 == values[6] &&
        newleaf7->keys[0] == keys[7] && *retValue8 == values[7] &&
        newleaf8->keys[0] == keys[8] && *retValue9 == values[8] &&
        newleaf8->keys[1] == keys[9] && *retValue10 == values[9] 
        ) 
    {
        printf("✓ Test 4 passed. Total of 10 key-value pairs inserted successfully at the correct branches (Order 3).\n");
        passes++;
    } 
    else 
    {
        printf("✗ Test 3 FAILED: Keys are not inserted correctly.\n");
        printf("     Expected values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d', key 3: '%d', value 3: '%d\n", keys[0], values[0], keys[1], values[1], keys[2], values[2]);
        printf("     Found values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d', key 3: '%d', value 3: '%d'\n", newleaf0->keys[0], *retValue1, newleaf1->keys[0], *retValue2, newleaf2->keys[0], *retValue3);
        printf("     Expected values: key 4: '%d', value 4: '%d', key 5: '%d', value 5: '%d', key 6: '%d', value 6: '%d\n", keys[3], values[3], keys[4], values[4], keys[5], values[5]);
        printf("     Found values: key 4: '%d', value 4: '%d', key 5: '%d', value 5: '%d', key 6: '%d', value 6: '%d'\n", newleaf3->keys[0], *retValue4, newleaf4->keys[0], *retValue5, newleaf5->keys[0], *retValue6);
        printf("     Expected values: key 7: '%d', value 7: '%d', key 8: '%d', value 8: '%d', key 9: '%d', value 9: '%d\n", keys[6], values[6], keys[7], values[7], keys[8], values[8]);
        printf("     Found values: key 7: '%d', value 7: '%d', key 8: '%d', value 8: '%d', key 9: '%d', value 9: '%d'\n", newleaf6->keys[0], *retValue7, newleaf7->keys[0], *retValue8, newleaf8->keys[0], *retValue9);
        printf("     Expected values: key 10: '%d', value 10: '%d'\n", keys[9], values[9]);
        printf("     Found values: key 10: '%d', value 10: '%d'\n", newleaf8->keys[1], *retValue10);
        failures++;
    }



    if (failures == 0) 
    {
        printf("✓✓✓ 'insert' function passed %d test(s) successfully. ✓✓✓\n", passes);
    } 
    else 
    {
        printf("✗✗✗ 'insert' function failed %d of %d test(s). ✗✗✗\n", failures, failures + passes);
    }

}


// Function to assert the insertion into a B+ tree
void assertFindRecord() 
{
    BPlusNode* root = createLeafBPlusNode();
    int passes = 0;
    int failures = 0;

    // Test 1: Insert key-value pairs
    int keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int values[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

    for (int i = 0; i < 10; i++) 
    {
        root = insertIntoTree(root, keys[i], (void *)values[i]);
    }

    record* foundRecord = findRecord(root, keys[3], NULL);


    // Check if the values can be indirectly validated
    if (*(int *)foundRecord == values[3])
    {
        printf("✓ Test 1 passed. The inserted value is retrieved succesfully.\n");
        passes++;
    } 
    else 
    {
        printf("✗ Test 1 FAILED: The inserted value can not be retrieved.\n");
        printf("     Expected value: %d\n", values[3]);
        printf("     Found value: %d\n", *(int *)foundRecord);
        failures++;
    }

}

void assertDelete()
{
    BPlusNode* root = createLeafBPlusNode();
    int passes = 0;
    int failures = 0;

    // Test 1: Case 1 deletion of the keys only exist in the leafs
    int keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int values[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

    for (int i = 0; i < 10; i++) 
    {
        root = insertIntoTree(root, keys[i], (void *)values[i]);
    }

    root = deleteFromTree(root, keys[9]);

    // Determine the structure of the tree by hand (since it should be done without any search functionality, yet), (for ORDER 3)
    BPlusNode* internal0 = root->pointers[0];
    BPlusNode* internal1 = root->pointers[1];
    BPlusNode* internal2 = internal0->pointers[0];
    BPlusNode* internal3 = internal0->pointers[1];
    BPlusNode* internal4 = internal1->pointers[0];
    BPlusNode* internal5 = internal1->pointers[1];
    
    BPlusNode* newleaf0 = internal2->pointers[0];
    BPlusNode* newleaf1 = internal2->pointers[1];
    BPlusNode* newleaf2 = internal3->pointers[0];
    BPlusNode* newleaf3 = internal3->pointers[1];
    BPlusNode* newleaf4 = internal4->pointers[0];
    BPlusNode* newleaf5 = internal4->pointers[1];
    BPlusNode* newleaf6 = internal5->pointers[0];
    BPlusNode* newleaf7 = internal5->pointers[1];
    BPlusNode* newleaf8 = internal5->pointers[2];
    
    int *retValue1 = newleaf0->pointers[0];
    int *retValue2 = newleaf1->pointers[0];
    int *retValue3 = newleaf2->pointers[0];
    int *retValue4 = newleaf3->pointers[0];
    int *retValue5 = newleaf4->pointers[0];
    int *retValue6 = newleaf5->pointers[0];
    int *retValue7 = newleaf6->pointers[0];
    int *retValue8 = newleaf7->pointers[0];
    int *retValue9 = newleaf8->pointers[0];

    // Check if the values are inserted at correct places
    if (newleaf0->keys[0] == keys[0] && *retValue1 == values[0] &&
        newleaf1->keys[0] == keys[1] && *retValue2 == values[1] &&
        newleaf2->keys[0] == keys[2] && *retValue3 == values[2] &&
        newleaf3->keys[0] == keys[3] && *retValue4 == values[3] &&
        newleaf4->keys[0] == keys[4] && *retValue5 == values[4] &&
        newleaf5->keys[0] == keys[5] && *retValue6 == values[5] &&
        newleaf6->keys[0] == keys[6] && *retValue7 == values[6] &&
        newleaf7->keys[0] == keys[7] && *retValue8 == values[7] &&
        newleaf8->keys[0] == keys[8] && *retValue9 == values[8] &&
        newleaf8->keys[1] == NULL
        ) 
    {
        printf("✓ Test 4 passed. Total of 10 key-value pairs inserted correctly and then one which ony exist in leaf deleted succesfully (Order 3).\n");
        passes++;
    } 
    else 
    {
        printf("✗ Test 3 FAILED: Keys are not inserted andor deleted correctly (Order 3).\n");
        printf("     Expected values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d', key 3: '%d', value 3: '%d\n", keys[0], values[0], keys[1], values[1], keys[2], values[2]);
        printf("     Found values: key 1: '%d', value 1: '%d', key 2: '%d', value 2: '%d', key 3: '%d', value 3: '%d'\n", newleaf0->keys[0], *retValue1, newleaf1->keys[0], *retValue2, newleaf2->keys[0], *retValue3);
        printf("     Expected values: key 4: '%d', value 4: '%d', key 5: '%d', value 5: '%d', key 6: '%d', value 6: '%d\n", keys[3], values[3], keys[4], values[4], keys[5], values[5]);
        printf("     Found values: key 4: '%d', value 4: '%d', key 5: '%d', value 5: '%d', key 6: '%d', value 6: '%d'\n", newleaf3->keys[0], *retValue4, newleaf4->keys[0], *retValue5, newleaf5->keys[0], *retValue6);
        printf("     Expected values: key 7: '%d', value 7: '%d', key 8: '%d', value 8: '%d', key 9: '%d', value 9: '%d\n", keys[6], values[6], keys[7], values[7], keys[8], values[8]);
        printf("     Found values: key 7: '%d', value 7: '%d', key 8: '%d', value 8: '%d', key 9: '%d', value 9: '%d'\n", newleaf6->keys[0], *retValue7, newleaf7->keys[0], *retValue8, newleaf8->keys[0], *retValue9);
        printf("     Expected values: key 10: NULL (because it should be deleted consequently no corresponding value should exist).\n");
        printf("     Found values: key 10: '%d'\n", newleaf8->keys[1]);
        failures++;
    }

    root = deleteFromTree(root, keys[0]);


}
