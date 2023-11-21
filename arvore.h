
typedef struct Node {
    int key;
    int is_deleted; // Novo campo para indicar se o nó foi removido logicamente
    struct Node* left;
    struct Node* right;
    int color; // 0 para vermelho, 1 para preto
} Node;

// Implementação da árvore LLRB como um dicionário
typedef struct Dictionary {
    Node* root;
} Dictionary;

Node* createNode(int key);
Node* insert(Node* root, int key);
Node* search(Node* root, int key);
void inOrderTraversal(Node* root);
void logicalRemove(Node* node);
Node* physicalRemove(Node* root);
Node* refreshTree(Node* root);
Dictionary* initializeDictionary();
void dictionaryInsert(Dictionary* dict, int key);
Node* dictionarySearch(Dictionary* dict, int key);
void dictionaryInOrderTraversal(Dictionary* dict);
void dictionaryLogicalRemove(Dictionary* dict, int key);
void dictionaryPhysicalRemove(Dictionary* dict);
void dictionaryRefresh(Dictionary* dict);
Node* rotateLeft(Node* h);
Node* rotateRight(Node* h);
void flipColors(Node* h);
