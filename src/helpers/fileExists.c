#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

bool fileExists(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (file) 
    {
        fclose(file);
        return true;
    }
    return false;
}