#include <stdio.h>
#include <string.h>
#include <ctype.h>


void trim(char *str) 
{
    if (str == NULL) 
    {
        return;
    }

    size_t len = strlen(str);

    // Trim leading double-quote characters
    size_t start = 0;
    while (start < len && (isspace((unsigned char)str[start]) || str[start] == '"')) {
        start++;
    }

    // Trim trailing double-quote characters
    size_t end = len;
    while (end > start && (isspace((unsigned char)str[end - 1]) || str[end - 1] == '"')) {
        end--;
    }

    // Shift the characters to remove leading spaces and double-quote characters
    if (start > 0) {
        memmove(str, str + start, end - start);
    }

    // Null-terminate the trimmed string
    str[end - start] = '\0';
}