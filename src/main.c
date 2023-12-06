#include <stdio.h>
#include <stdlib.h>
#include "include/testParseCreate.h"
#include "include/connect.h"
#include "include/unity.h"
#include "include/parseMainQuery.h"
#include "include/createDatabase.h"
#include "include/deleteDatabase.h"
#include "include/testingConnectAndDisconnect.h"
#include "include/testingCreateAndDelete.h"
#include "include/testingFileExist.h"
#include "include/testingCheckTable.h"
#include "include/testingIsDataEntry.h"
#include "include/testingTrim.h"
#include "include/testingProcessDatabaseFile.h"
#include "include/testParseSelect.h"
#include "include/testBPlusTree.h"
#include "include/loadCSV.h"

int main() 
{
    int choice = 0;

    while (choice != 7) 
    {
        printf("Menu:\n");
        printf("1. Establish Database Connection\n");
        printf("2. SQL\n");
        printf("3. Load CSV\n");
        printf("4. Tests\n");
        printf("5. Create a new database\n");
        printf("6. Delete a existing database\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        // Checks if the input is integer
        if (scanf(" %d", &choice) != 1) 
        {   
            printf("Invalid input. Please enter a valid choice.\n");

            continue; // This will go back to the beginning of the while loop
        }

        switch (choice) 
        {
            case 1: 
            {
                printf("Connecting to database...\n");
                connect();
                break;
            }
            case 2:
            {
                parseMainQuery();
                break;
            }
            case 3:
                loadCSV();
                break;
            case 4: 
                int testChoice = 0;
                printf("Test Menu:\n");
                printf("1. Test database connection/creating functions\n");
                printf("2. Test helper functions\n");
                printf("3. Test parsing\n");
                printf("4. Test data structures\n");
                printf("Enter your test choice: ");

                if (scanf(" %d", &testChoice) != 1) 
                {   
                    printf("Invalid input.\n");
                    break;
                }

                switch (testChoice) 
                {
                    case 1: // Tests for db connection/creation                     
                        UNITY_BEGIN();
                        RUN_TEST(test_CreateAndDeleteDatabase);
                        RUN_TEST(testConnectandDisconnect);
                        RUN_TEST(testProcessDatabaseFile);
                        UNITY_END();
                        break;   
                    case 2: // Tests for helpers
                        UNITY_BEGIN();                 
                        RUN_TEST(test_FileExists_WhenFileExists);
                        RUN_TEST(test_FileExists_WhenFileDoesNotExist);
                        RUN_TEST(test_CheckTable_WhenTableExists);
                        RUN_TEST(test_CheckTable_WhenTableDoesNotExist);
                        RUN_TEST(test_GetTableInfo_WhenTableExists);
                        RUN_TEST(test_GetTableInfo_WhenTableDoesNotExist);
                        RUN_TEST(test_IsDataEntry_WhenDataEntry);                      
                        RUN_TEST(test_IsDataEntry_WhenTableEntry);
                        RUN_TEST(test_IsDataEntry_WhenInvalidEntry);                     
                        RUN_TEST(test_IsDataEntry_WhenInvalidJSON);
                        RUN_TEST(test_Trim_NoLeadingOrTrailingSpaces);
                        RUN_TEST(test_Trim_LeadingSpaces);
                        RUN_TEST(test_Trim_TrailingSpaces);
                        RUN_TEST(test_Trim_BothLeadingAndTrailingSpaces);
                        RUN_TEST(test_Trim_EmptyString);
                        RUN_TEST(test_Trim_NullString);
                        UNITY_END();
                        break;
                    case 3:
                        UNITY_BEGIN();
                        RUN_TEST(test_inOrderDataTraversal_WhenTreeExists);
                        UNITY_END();
                        break;
                    case 4:
                        printf("----------------------------------------------------------------------\n");
                        printf("# TESTING createBPlusTree and createNode functions:\n");
                        assertCreateBPlusTree();    
                        printf("----------------------------------------------------------------------\n");
                        printf("# TESTING insert functionality of B+ Tree:\n");
                        assertInsert();
                        printf("----------------------------------------------------------------------\n");
                        printf("# TESTING finding record functionality of B+ Tree:\n");
                        assertFindRecord();
                        printf("----------------------------------------------------------------------\n");
                        printf("# TESTING delete functionlity of B+ Tree:\n");
                        assertDelete();
                        printf("----------------------------------------------------------------------\n");
                        break;
                    default:
                        printf("Invalid test choice.\n");
                        break;
                } 
            break;
            case 5:
                createDatabase();
                break;
            case 6:
                deleteDatabase();
                break;
            case 7:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please select a valid option.\n"); 
                break;
        }
    }

    return 0;
}
