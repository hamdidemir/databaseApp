#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/parseCreate.h"
#include "include/parseInsert.h"
#include "include/parseSelect.h"

#define MAX_QUERY_LENGTH 1024

void parseMainQuery() 
{
    // Consume the newline character left in the input buffer
    getchar();

    // Get SQL query from user
    char query[MAX_QUERY_LENGTH];
    printf("Enter SQL query (terminate with ';'):\n");

    char line[MAX_QUERY_LENGTH];
    query[0] = '\0'; // Initialize query as an empty string

    // C version of do-while loop
    while (1) 
    {
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline

        // Concatenate the line to the query
        strcat(query, line);

        // Check if the query contains a semicolon
        if (strstr(query, ";") != NULL) {
            break;
        }
        
        printf("... "); // Continue prompt for multi-line query
    }

    // Check if the query starts with "CREATE TABLE" or "SELECT" or "INSERT" or "DELETE"
    if (strncasecmp(query, "CREATE TABLE", strlen("CREATE TABLE")) == 0) 
    {
        parseCreate(query);
    } 
    else if 
    (strncasecmp(query, "SELECT", strlen("SELECT")) == 0) 
    {
        parseSelect(query);
    } 
    else if 
    (strncasecmp(query, "INSERT", strlen("INSERT")) == 0) 
    {
        parseInsert(query);
    } 
    else if 
    (strncasecmp(query, "DELETE", strlen("DELETE")) == 0) 
    {
        // parse Delete table
    } 
    else 
    {
        printf("Invalid query.\n");
    }
}
