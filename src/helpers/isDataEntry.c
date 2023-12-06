#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/cJSON.h"

int isDataEntry(const char *jsonString) 
{
    cJSON *root = cJSON_Parse(jsonString);
    if (root == NULL) 
    {
        printf("Error parsing JSON.\n");
        return -1; // Unable to parse the JSON string
    }

    cJSON *typeField = cJSON_GetObjectItemCaseSensitive(root, "type");
    if (cJSON_IsString(typeField)) 
    {
        if (strcmp(typeField->valuestring, "data") == 0) 
        {
            cJSON_Delete(root);
            return 1; // Entry is of type "data"
        } 
        else if (strcmp(typeField->valuestring, "table") == 0) 
        {
            cJSON_Delete(root);
            return 0; // Entry is of type "table"
        }
    }

    cJSON_Delete(root);

    return -1; // No valid entry found
}