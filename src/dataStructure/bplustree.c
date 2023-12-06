#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "include/bplustree.h"


BPlusNode* createInternalBPlusNode()
{
    BPlusNode* new_interal_node = (BPlusNode *)malloc(sizeof(BPlusNode));
    if (new_interal_node == NULL) 
    {
        printf("Error allocating memory for B+ node\n");
        return NULL;
    }
    new_interal_node->keys = malloc((ORDER - 1) * sizeof(int));
    if (new_interal_node->keys == NULL) 
    {
        printf("Error allocating memory for keys of B+ node\n");
        return NULL;
    }
    new_interal_node->pointers = malloc(ORDER * sizeof(void *));
    if (new_interal_node->pointers == NULL) 
    {
        printf("Error allocating memory for pointers of B+ node\n");
        return NULL;
    }
    new_interal_node->is_leaf = 0;
    new_interal_node->num_keys = 0;
    new_interal_node->parent = NULL;
    new_interal_node->prev = NULL;
    new_interal_node->next = NULL;  
    return new_interal_node;
}

BPlusNode* createLeafBPlusNode()
{   
    BPlusNode* new_leaf_node = createInternalBPlusNode();
    new_leaf_node->is_leaf = 1;
    return new_leaf_node;
}

BPlusNode* createNewTree() 
{
  BPlusNode* root = createLeafBPlusNode();
  root->parent = NULL;
  root->next = NULL;
  root->prev = NULL;
  return root;
}


BPlusNode* insertIntoTree (BPlusNode* root, int key, void* value)
{
    // Check if the key already exists in the tree
    record* record_pointer = findRecord(root, key, NULL);
    if (record_pointer != NULL) 
    {
        return root;
    }

    // Key is not found in the tree, create a new record
    record_pointer = createRecord(value);

    // Check if the tree is initially empty
    if (root == NULL) 
    {
        return createNewTree();
    }

    // Find the leaf node where the key should be inserted
    BPlusNode* leafToInsert = findLeaf(root, key);

    // Insert the key and record into the leaf node
    if (leafToInsert->num_keys < ORDER - 1) 
    {
        // If there's room in the leaf node, insert directly
        leafToInsert = insertIntoLeaf(leafToInsert, key, record_pointer);
        return root;
    }

    // Leaf node is full, split it
    return insertIntoLeafAfterSplitting(root, leafToInsert, key, record_pointer);
}

record* createRecord(void* value) 
{
    record *new_record = (record *)malloc(sizeof(record));
    if (new_record == NULL) 
    {
        printf("Memory allocation error for record.\n");
        return NULL;
    } 
    else 
    {
        new_record->value = value; 
    }
    return new_record;
}

record* findRecord(BPlusNode* root, int key, BPlusNode **leaf_out) 
{
    // If the tree is empty (root is NULL), return NULL and set leaf_out to NULL
    if (root == NULL) 
    {
        if (leaf_out != NULL) 
        {
            *leaf_out = NULL;
        }
        return NULL;
    }

    int i = 0;
    BPlusNode *leaf = NULL;

    // Find the leaf node where the key should be located
    leaf = findLeaf(root, key);

    // Search for the key within the found leaf node
    for (i = 0; i < leaf->num_keys; i++)
    {
        if (leaf->keys[i] == key)
        break;
    }

    // If leaf_out is not NULL, set it to point to the leaf node where the key is located
    if (leaf_out != NULL) 
    {
        *leaf_out = leaf;
    }

    // If the key is not found in the leaf node, return NULL
    if (i == leaf->num_keys)
    {
        return NULL;
    }
    // Otherwise, return a pointer to the record associated with the key
    else
    {
        return (record *)leaf->pointers[i];
    }
}


// Find the leaf node where the key should be located in the B+ tree
BPlusNode* findLeaf(BPlusNode* const root, int key) 
{
    // If the tree is empty, return NULL as there are no leaf nodes
    if (root == NULL) 
    {
        return root;
    }

    int i = 0;
    BPlusNode* c = root;

    // Traverse down the tree until a leaf node is reached
    while (!c->is_leaf) 
    {
        i = 0;

        // Find the index of the child node to follow
        while (i < c->num_keys) 
        {
            if (key >= c->keys[i]) 
            {
                i++;
            } 
            else 
            {
                break;
            }
        }

        // Move to the next child node
        c = (BPlusNode *)c->pointers[i];
    }

    // Return the leaf node where the key should be located
    return c;
}


// Insert a key-value pair into a leaf node
BPlusNode* insertIntoLeaf(BPlusNode* leaf, int key, record *pointer) 
{
    int i, insertion_point;

    // Find the position where the key should be inserted in the leaf node
    insertion_point = 0;
    while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
    {
        insertion_point++;
    }

    // Shift existing keys and pointers to make space for the new key-value pair
    for (i = leaf->num_keys; i > insertion_point; i--) 
    {
        leaf->keys[i] = leaf->keys[i - 1];
        leaf->pointers[i] = leaf->pointers[i - 1];
    }

    // Insert the key and pointer at the correct position
    leaf->keys[insertion_point] = key;
    leaf->pointers[insertion_point] = pointer;

    // Increase the number of keys in the leaf node
    leaf->num_keys++;

    // Return the updated leaf node
    return leaf;
}

BPlusNode* insertIntoLeafAfterSplitting(BPlusNode* root, BPlusNode* leaf, int key, record *pointer)
{
    BPlusNode* new_leaf;       // Create a new leaf node to hold some of the split data
    int *temp_keys;            // Temporary array to hold keys during splitting
    void **temp_pointers;      // Temporary array to hold pointers during splitting
    int insertion_index, split, new_key, i, j;

    // Create a new leaf node
    new_leaf = createLeafBPlusNode();

    // Allocate memory for temporary arrays
    temp_keys = malloc(ORDER * sizeof(int));
    if (temp_keys == NULL) 
    {
        printf("Memory allocation error for temp_keys.\n");
        return NULL;
    }

    temp_pointers = malloc(ORDER * sizeof(void *));
    if (temp_pointers == NULL) 
    {
        printf("Memory allocation error for temp_pointers.\n");
        return NULL;
    }

    // Find where to insert
    insertion_index = 0;
    while (insertion_index < ORDER - 1 && leaf->keys[insertion_index] < key)
    {
        insertion_index++;
    }
    
    // Copy keys and pointers into temporary arrays, skipping the insertion index
    for (i = 0, j = 0; i < leaf->num_keys; i++, j++) 
    {
        if (j == insertion_index)
        {
            j++;
        }           
        temp_keys[j] = leaf->keys[i];
        temp_pointers[j] = leaf->pointers[i];
    }

    // Insert the new key and pointer at the insertion index
    temp_keys[insertion_index] = key;
    temp_pointers[insertion_index] = pointer;

    leaf->num_keys = 0;

    // Determine the split point
    if ((ORDER - 1) % 2 == 0)
    {
        split = (ORDER - 1) / 2;
    }    
    else
    {
        split = ((ORDER - 1) / 2) + 1;
    }

    // Copy the first half of keys and pointers back to the original leaf
    for (i = 0; i < split; i++) 
    {
        leaf->pointers[i] = temp_pointers[i];
        leaf->keys[i] = temp_keys[i];
        leaf->num_keys++;
    }

    // Copy the second half of keys and pointers to the new leaf
    for (i = split, j = 0; i < ORDER; i++, j++) 
    {
        new_leaf->pointers[j] = temp_pointers[i];
        new_leaf->keys[j] = temp_keys[i];
        new_leaf->num_keys++;
    }

    // Free the temporary arrays
    free(temp_pointers);
    free(temp_keys);

    // Update pointers and parent
    new_leaf->pointers[ORDER - 1] = leaf->pointers[ORDER - 1];
    leaf->pointers[ORDER - 1] = new_leaf;

    // Initialize unused pointers
    for (i = leaf->num_keys; i < ORDER - 1; i++)
    {
        leaf->pointers[i] = NULL;
    }    
    for (i = new_leaf->num_keys; i < ORDER - 1; i++)
    {
        new_leaf->pointers[i] = NULL;
    }

    // Update the parent and retrieve the new key for insertion into the parent
    new_leaf->parent = leaf->parent;
    new_key = new_leaf->keys[0];
    new_leaf->next = leaf;
    leaf->prev = new_leaf;

    // Insert the new key and pointer into the parent
    return insertIntoParent(root, leaf, new_key, new_leaf);
}


BPlusNode* insertIntoInternalNode(BPlusNode* root, BPlusNode* n, int left_index, int key, BPlusNode* right) 
{
    int i;

    // Shift pointers and keys to the right to make space for the new key and right child
    for (i = n->num_keys; i > left_index; i--) 
    {
        n->pointers[i + 1] = n->pointers[i];
        n->keys[i] = n->keys[i - 1];
    }

    // Insert the new key and right child at the appropriate positions
    n->pointers[left_index + 1] = right;
    n->keys[left_index] = key;

    // Increment the number of keys in the node
    n->num_keys++;

    // Return the root node (it might change if the insertion causes the root to split)
    return root;
}

BPlusNode* insertIntoInternalNodeAfterSplitting(BPlusNode* root, BPlusNode* old_node, int left_index, int key, BPlusNode* right) 
{
    int i, j, split, k_prime;
    BPlusNode *new_node, *child;
    int *temp_keys;
    BPlusNode **temp_pointers;

    // Allocate memory for temporary keys and pointers
    temp_pointers = malloc((ORDER + 1) * sizeof(BPlusNode *));
    if (temp_pointers == NULL) 
    {
        printf("Memory allocation error for temp_pointers.\n");
        return NULL;
    }
    temp_keys = malloc(ORDER * sizeof(int));
    if (temp_keys == NULL) 
    {
        printf("Memory allocation error for temp_keys.\n");
        return NULL;
    }

    // Copy pointers from the old_node to the temporary arrays, skipping the left_index
    for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) 
    {
        if (j == left_index + 1)
        j++;
        temp_pointers[j] = old_node->pointers[i];
    }

    // Copy keys from the old_node to the temporary array, skipping the left_index
    for (i = 0, j = 0; i < old_node->num_keys; i++, j++) 
    {
        if (j == left_index)
        j++;
        temp_keys[j] = old_node->keys[i];
    }

    // Insert the new key and right child into the appropriate positions
    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;

    // Determine the split point
    if (ORDER  % 2 == 0)
    {
        split = ORDER / 2;
    }    
    else
    {
        split = (ORDER / 2) + 1;
    }

    new_node = createInternalBPlusNode();
    old_node->num_keys = 0;

    // Copy keys and pointers to the old_node and increment num_keys
    for (i = 0; i < split - 1; i++) 
    {
        old_node->pointers[i] = temp_pointers[i];
        old_node->keys[i] = temp_keys[i];
        old_node->num_keys++;
    }

    // Handle the k_prime key
    old_node->pointers[i] = temp_pointers[i];
    k_prime = temp_keys[split - 1];

    // Copy remaining keys and pointers to the new_node
    for (++i, j = 0; i < ORDER; i++, j++) 
    {
        new_node->pointers[j] = temp_pointers[i];
        new_node->keys[j] = temp_keys[i];
        new_node->num_keys++;
    }
    new_node->pointers[j] = temp_pointers[i];

    // Free temporary memory
    free(temp_pointers);
    free(temp_keys);

    // Update parent pointers for child nodes
    new_node->parent = old_node->parent;
    for (i = 0; i <= new_node->num_keys; i++) 
    {
        child = new_node->pointers[i];
        child->parent = new_node;
    }

    // Insert the new key (k_prime) and right child (new_node) into the parent node
    return insertIntoParent(root, old_node, k_prime, new_node);
}

BPlusNode* insertIntoParent(BPlusNode* root, BPlusNode* left, int key, BPlusNode* right) 
{
    int left_index;
    BPlusNode* parent;

    // Get the parent of the left node
    parent = (BPlusNode *)left->parent;
    left->next = right;
    right->prev =left;

    if (parent == NULL)
    {
        // If there is no parent (root node), insert into a new root
        return insertIntoNewRoot(left, key, right);
    }

    // Find the index at which the left node is located within the parent's pointers
    left_index = getLeftIndex(parent, left);

    if (parent->num_keys < ORDER - 1)
    {
        // If the parent has room for another key, insert into the internal node
        return insertIntoInternalNode(root, parent, left_index, key, right);
    }

    // If the parent is full, split it and insert into the internal node after splitting
    return insertIntoInternalNodeAfterSplitting(root, parent, left_index, key, right);
}


BPlusNode* insertIntoNewRoot(BPlusNode* left, int key, BPlusNode* right) 
{
    BPlusNode* root = createInternalBPlusNode();
    root->keys[0] = key;
    root->pointers[0] = left;
    root->pointers[1] = right;
    root->num_keys++;
    root->parent = NULL;
    left->parent = root;
    right->parent = root;
    left->next = right;
    right->prev =left;
    return root;
}

int getLeftIndex(BPlusNode* parent, BPlusNode* left) 
{
    int left_index = 0;
    while (left_index <= parent->num_keys && parent->pointers[left_index] != left)
    {
        left_index++;
    }
    return left_index;
}


BPlusNode* deleteFromTree(BPlusNode* root, int key)
{
    // Check if the key exists in tree
    int keyExist = 0;
    if(findRecord(root, key, NULL) != NULL)
    {
        keyExist = 1;
    }

    // The case 1: delete from leaf
    if(keyExist)
    {
        // Find the leaf node where the key is located
        BPlusNode* leafNode = findLeaf(root, key);

        // Detemine the minumum number of keys
        int minKeys = ORDER / 2 - 1;


        // if the node has more keys than mininum number of keys requiered for node simply delete the key 
        if (leafNode->num_keys > minKeys)
        {
            for(int i=0; i < leafNode->num_keys; i++)
            {   
                // Found the key in the leaf
                if(leafNode->keys[i] == key)
                {
                    // Shift the next keys one spot to left
                    for(int j = i; j<leafNode->num_keys; j++)
                    {
                        leafNode->keys[j] = leafNode->keys[j+1];
                        leafNode->pointers[j] = leafNode->pointers[j+1];
                    }
                    // Update the number of keys and remove the last element 
                    leafNode->num_keys--;                   
                    leafNode->keys[leafNode->num_keys] = NULL;
                    leafNode->pointers[leafNode->num_keys] = NULL;
                }
            }
        }
        // if the number of keys are same with the minumum required
        else if (leafNode->num_keys == minKeys) 
        {
            // Check the next node for possible borrowing 
            if(leafNode->next->num_keys > minKeys && leafNode->next != NULL)
            {   
                // Add the first one from the next node to the end of the current node
                leafNode->keys[leafNode->num_keys] = leafNode->next->keys[0];
                leafNode->pointers[leafNode->num_keys] = leafNode->next->pointers[0];
                leafNode->num_keys++;
                // Shift the rest of the keys to the one place left to cover up the hole
                for(int i = 0; i < leafNode->next->num_keys; i++)
                {
                    leafNode->next->keys[i] = leafNode->next->keys[i+1];
                    leafNode->next->pointers[i] = leafNode->next->pointers[i+1];
                }
                leafNode->next->keys[leafNode->next->num_keys] == NULL;
                leafNode->next->pointers[leafNode->next->num_keys] == NULL;
                leafNode->next->num_keys--;
            }
            // Check the previous node for possible borrowing 
            if(leafNode->prev->num_keys > minKeys && leafNode->prev != NULL)
            {
                // Add last one from the previous node to the beginning of the current one
                // First we must shift the current pointers to make room for the new key-value pair
                for(int i = 0; i < leafNode->prev->num_keys; i++)
                {
                    leafNode->keys[i] = leafNode->keys[i+1];
                    leafNode->pointers[i] = leafNode->pointers[i+1];
                }
                // Fill the first one with last one from the previous node
                leafNode->keys[0] = leafNode->prev->keys[leafNode->prev->num_keys];
                leafNode->pointers[0] = leafNode->prev->pointers[leafNode->prev->num_keys];
                // Updates number of keys
                leafNode->prev->num_keys--;
                leafNode->num_keys++;
                // remove the transferred key value
                leafNode->prev->keys[leafNode->prev->num_keys] = NULL;
                leafNode->prev->pointers[leafNode->prev->num_keys] = NULL;
            }
            // If neither next or previous nodes has any keys to borrow then we should merge two nodes, in this case we're merging current one with the next one
            else
            {
                // Carry the next node to current one
                for(int i = 0; leafNode->next->num_keys; i++)
                {
                    leafNode->keys[leafNode->num_keys+i] = leafNode->next->keys[i];
                    leafNode->pointers[leafNode->num_keys+i] = leafNode->next->pointers[i];   
                    leafNode->num_keys++;                                    
                }
                // Set up the next and previous relations;
                leafNode->next = leafNode->next->next;
                leafNode->next->prev = leafNode;
                // remove the next node from thee tree
                for(int i=0; i < leafNode->next->parent->num_keys; i++)
                {
                    // Find the pointer index of next pointers parent
                    if(leafNode->next->parent->pointers[i] == leafNode->next)
                    {
                        // Cary the pointers coming after "next node" (to be merged with the current one) one point to left to fill the gap
                        for(int j=i; j < leafNode->next->parent->num_keys ; j++)
                        {
                            leafNode->next->parent->pointers[j] = leafNode->next->parent->pointers[j+1];
                            // Set the last one to null and update the number
                            leafNode->next->parent->pointers[leafNode->next->parent->num_keys] = NULL;
                            leafNode->next->parent->num_keys--;
                        }
                    }
                }
            }
        }
    }

    //The other cases

    return root;
}



int isKeyOnlyInLeaf(BPlusNode* root, int key) 
{
    // If the tree is empty (root is NULL), return 0.
    if (root == NULL) {
        return 0;
    }

    // Flag to track if the key is found in any internal node
    int foundInInternal = 0;

    // Check if the key is present in the internal nodes
    for (int i = 0; i < root->num_keys + 1; i++) {
        if (key == root->keys[i] && !root->is_leaf) {
            // The key is found in an internal node.
            foundInInternal = 1;
        }
        if (key < root->keys[i] && !root->is_leaf) {
            // Recursively search the appropriate child node.
            int result = isKeyOnlyInLeaf((BPlusNode*)root->pointers[i], key);
            if (result == 0) {
                foundInInternal = 1;
            }
        }
    }

    // If the key was found in any internal node, return 0; otherwise, it's only in the leaf.
    return foundInInternal ? 0 : 1;
}


void freeBPlusTree(BPlusNode* node) 
{
    if (node == NULL) 
    {
        return;
    }

    // Recursively free child nodes (internal or leaf nodes)
    if (!node->is_leaf) 
    {
        for (int i = 0; i <= node->num_keys; i++) 
        {
            freeBPlusTree((BPlusNode*)node->pointers[i]);
            node->pointers[i] = NULL;
        }
    }

    // Free keys and pointers in the node
    free(node->keys);
    free(node->pointers);

    // Set keys and pointers to NULL
    node->keys = NULL;
    node->pointers = NULL;

    // Free the node itself
    free(node);
}

