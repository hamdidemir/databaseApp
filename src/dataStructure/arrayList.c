#include <stdlib.h>
#include <stdio.h>
#include "include/arrayList.h"


// Initialize an ArrayList
ArrayList* initArrayList() 
{
    ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList));
    if (list == NULL) 
    {
        // Handle memory allocation failure
        return NULL;
    }

    list->data = (void**)malloc(INITIAL_CAPACITY * sizeof(void*));
    if (list->data == NULL) 
    {
        free(list); // Clean up the list structure
        // Handle memory allocation failure
        return NULL;
    }

    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

// Add an element to the ArrayList
void addArrayList(ArrayList* list, void* element) 
{
    if (list == NULL) {
        // Handle null list
        return;
    }

    // Check if the array needs to be resized
    if (list->size >= list->capacity) {
        // Double the capacity
        list->capacity *= 2;
        list->data = (void**)realloc(list->data, list->capacity * sizeof(void*));
        if (list->data == NULL) {
            // Handle memory allocation failure
            return;
        }
    }

    list->data[list->size++] = element;
}

// Retrieve an element from the ArrayList at a given index
void* getArrayList(ArrayList* list, size_t index) {
    if (list == NULL || index >= list->size) {
        // Handle invalid list or out-of-bounds index
        return NULL;
    }

    return list->data[index];
}

// Get the size of the ArrayList
size_t sizeArrayList(ArrayList* list) {
    if (list == NULL) {
        return 0;
    }
    return list->size;
}

// Free the memory used by the ArrayList
void freeArrayList(ArrayList* list) {
    if (list == NULL) {
        return;
    }

    free(list->data);
    free(list);
}


// Swap two elements in the ArrayList at given indices
void swapArrayList(ArrayList* list, size_t index1, size_t index2) 
{
    if (list == NULL || index1 >= list->size || index2 >= list->size) 
    {
        printf("Out of array size error.");
        return;
    }

    // Swap elements using a temporary variable
    void* temp = list->data[index1];
    list->data[index1] = list->data[index2];
    list->data[index2] = temp;
}
