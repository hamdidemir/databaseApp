#include "include/unity.h"
#include "include/isDataEntry.h"  

void test_IsDataEntry_WhenDataEntry() 
{
    const char* jsonString = "{\"type\":\"data\"}";
    TEST_ASSERT_EQUAL_INT(1, isDataEntry(jsonString));
}

void test_IsDataEntry_WhenTableEntry() 
{
    const char* jsonString = "{\"type\":\"table\"}";
    TEST_ASSERT_EQUAL_INT(0, isDataEntry(jsonString));
}

void test_IsDataEntry_WhenInvalidEntry() 
{
    const char* jsonString = "{\"type\":\"invalid\"}";
    TEST_ASSERT_EQUAL_INT(-1, isDataEntry(jsonString));
}

void test_IsDataEntry_WhenInvalidJSON() 
{
    const char* jsonString = "invalid JSON";
    TEST_ASSERT_EQUAL_INT(-1, isDataEntry(jsonString));
}