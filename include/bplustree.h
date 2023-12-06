#ifndef BPLUSTREE_H
#define BPLUSTREE_H


// Default order
#define ORDER 3

typedef struct BPlusNode
{
    void** pointers; // pointers to either records (leaf node) or childnodes (internal node)
    int* keys; // pointers to keys
    int is_leaf; // flag the check if a node is leaf (leaf = 1, internal = 0)
    int num_keys; // number of keys
    struct BPlusNode* parent; // pointers to parent node
    struct BPlusNode* next; // pointers to next node
    struct BPlusNode* prev; // pointers to previous node
} BPlusNode;


typedef struct record 
{
  void* value;
} record;


BPlusNode* createInternalBPlusNode();
BPlusNode* createLeafBPlusNode();
BPlusNode* createNewTree();
BPlusNode* insertIntoTree (BPlusNode* root, int key, void *value);
record* createRecord(void* value);
record* findRecord(BPlusNode* root, int key, BPlusNode **leaf_out);
BPlusNode* findLeaf(BPlusNode* const root, int key);
BPlusNode* insertIntoLeaf(BPlusNode* leaf, int key, record *pointer);
BPlusNode* insertIntoLeafAfterSplitting(BPlusNode* root, BPlusNode* leaf, int key, record *pointer);
BPlusNode* insertIntoParent(BPlusNode* root, BPlusNode* left, int key, BPlusNode* right);
BPlusNode* insertIntoInternalNode(BPlusNode* root, BPlusNode* n, int left_index, int key, BPlusNode* right);
BPlusNode* insertIntoInternalNodeAfterSplitting(BPlusNode* root, BPlusNode* old_node, int left_index, int key, BPlusNode* right);
BPlusNode* insertIntoParent(BPlusNode* root, BPlusNode* left, int key, BPlusNode* right);
BPlusNode* insertIntoNewRoot(BPlusNode* left, int key, BPlusNode* right);
int getLeftIndex(BPlusNode* parent, BPlusNode* left);
int isKeyOnlyInLeaf(BPlusNode* root, int key);
BPlusNode* deleteFromTree(BPlusNode* root, int key);
void freeBPlusTree(BPlusNode* node);


#endif // BPLUSTREE_H