#include <stdlib.h>
#include "include/bplustree.h"

extern BPlusNode* root; // Define the global HashMap variable
extern const char* dbname; // Define database name as a global variable

void disconnect() 
{

    // This function can be called explicitly when needed to disconnect from the database
    if (root != NULL) 
    {
        freeBPlusTree(root);
        root = NULL;
    }

    if (dbname != NULL)
    {
        free((void*)dbname);
        dbname = NULL;
    }
}
