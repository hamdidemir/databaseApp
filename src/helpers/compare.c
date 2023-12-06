#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/compare.h"

// Compare strings and numbers
int compareStringsOrNumbers(const char *str1, const char *str2) 
{
    // Attempt to convert strings to numbers
    int num1 = atoi(str1);
    int num2 = atoi(str2);

    // Check if both strings are valid numbers
    if (num1 != 0 || (num1 == 0 && str1[0] == '0')) 
    {
        if (num2 != 0 || (num2 == 0 && str2[0] == '0')) 
        {
            // Both strings are valid numbers, compare them numerically
            if (num1 < num2) 
            {
                return -1;
            } 
            else if (num1 > num2) 
            {
                return 1;
            } 
            else 
            {
                return 0; // Numbers are equal
            }
        }
    }

    // If not both numbers, compare lexicographically
    return strcmp(str1, str2);
}

// Function to get the bigger value
const char* getBiggerValue(const char *str1, const char *str2) 
{
    if (compareStringsOrNumbers(str1, str2) > 0) 
    {
        return str1;
    } 
    else 
    {
        return str2;
    }
}

// Function to get the lower value
const char* getLowerValue(const char* str1, const char* str2) 
{
    if (compareStringsOrNumbers(str1, str2) < 0) 
    {
        return str1;
    } 
    else 
    {
        return str2;
    }
}