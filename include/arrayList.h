#ifndef ARRAYLIST_H
#define ARRAYLIST_H

// Define the initial capacity for the ArrayList
#define INITIAL_CAPACITY 10

// Define the ArrayList structure
typedef struct ArrayList {
    void** data;          // Array of data
    size_t size;          // Number of elements in the ArrayList
    size_t capacity;      // Total capacity of the ArrayList
} ArrayList;

ArrayList* initArrayList();
void addArrayList(ArrayList* list, void* element);
void* getArrayList(ArrayList* list, size_t index);
size_t sizeArrayList(ArrayList* list);
void freeArrayList(ArrayList* list);
void swapArrayList(ArrayList* list, size_t index1, size_t index2);


#endif // ARRAYLIST_H
