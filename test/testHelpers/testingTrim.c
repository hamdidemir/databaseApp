#include "include/unity.h"
#include "include/trim.h" 

void test_Trim_NoLeadingOrTrailingSpaces() 
{
    char str[] = "Hello, World!";
    trim(str);
    TEST_ASSERT_EQUAL_STRING("Hello, World!", str);
}

void test_Trim_LeadingSpaces() 
{
    char str[] = "   Leading Spaces";
    trim(str);
    TEST_ASSERT_EQUAL_STRING("Leading Spaces", str);
}

void test_Trim_TrailingSpaces() 
{
    char str[] = "Trailing Spaces    ";
    trim(str);
    TEST_ASSERT_EQUAL_STRING("Trailing Spaces", str);
}

void test_Trim_BothLeadingAndTrailingSpaces() 
{
    char str[] = "   Both Leading and Trailing Spaces    ";
    trim(str);
    TEST_ASSERT_EQUAL_STRING("Both Leading and Trailing Spaces", str);
}

void test_Trim_EmptyString() 
{
    char str[] = "";
    trim(str);
    TEST_ASSERT_EQUAL_STRING("", str);
}

void test_Trim_NullString() {
    char* str = NULL;
    trim(str); // Should handle NULL gracefully
    TEST_ASSERT_NULL(str);
}
