#include <stdbool.h>
#include "include/unity.h"
#include "include/fileExists.h"  // Include the header file that contains the fileExists function

void test_FileExists_WhenFileExists() 
{
    // Create a sample file that exists for testing
    const char* existingFile = "testfile.txt";
    FILE* file = fopen(existingFile, "w");
    fclose(file);

    // Test the fileExists function
    TEST_ASSERT_TRUE(fileExists(existingFile));

    // Clean up the sample file
    remove(existingFile);
}

void test_FileExists_WhenFileDoesNotExist() 
{
    // Test the fileExists function with a non-existing file
    const char* nonExistingFile = "nonexistingfile.txt";
    TEST_ASSERT_FALSE(fileExists(nonExistingFile));
}
