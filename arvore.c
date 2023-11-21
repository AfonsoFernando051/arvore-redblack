#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

int main() {
    Dictionary* dict = initializeDictionary();
    dictionaryInsert(dict, 10);
    dictionaryInsert(dict, 5);
    dictionaryInsert(dict, 15);
    dictionaryInsert(dict, 3);
    dictionaryInsert(dict, 7);

    printf("Dicionario Incial: ");
    dictionaryInOrderTraversal(dict);
    printf("\n");

    dictionaryLogicalRemove(dict, 5);
    dictionaryLogicalRemove(dict, 15);

    printf("Dicionario apos remocao logica: ");
    dictionaryInOrderTraversal(dict);
    printf("\n");

    dictionaryPhysicalRemove(dict);

    printf("Dicionario apos remocao fisica: ");
    dictionaryInOrderTraversal(dict);
    printf("\n");

    dictionaryRefresh(dict);

    printf("Apos refresh: ");
    dictionaryInOrderTraversal(dict);
    printf("\n");

    return 0;
}

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->is_deleted = 0; // Inicialmente, o nó não está marcado como removido
    newNode->left = newNode->right = NULL;
    newNode->color = 0; // Inicialmente, o nó é vermelho
    return newNode;
}

Node* insert(Node* root, int key) {
    // Caso base: se atingimos uma folha, criamos um novo nó vermelho
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    } else {
        return root;
    }

    // Correção da propriedade de cor vermelha
    if (root->right != NULL && root->right->color == 0) {
        root = rotateLeft(root);
    }
    if (root->left != NULL && root->left->color == 0 && root->left->left != NULL && root->left->left->color == 0) {
        root = rotateRight(root);
    }
    if (root->left != NULL && root->left->color == 0 && root->right != NULL && root->right->color == 0) {
        flipColors(root);
    }

    return root;
}

// Função para rotacionar um nó para a esquerda
Node* rotateLeft(Node* h) {
    Node* x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = 0; // O nó à esquerda deve ser vermelho após a rotação
    return x;
}

// Função para rotacionar um nó para a direita
Node* rotateRight(Node* h) {
    Node* x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = h->color;
    h->color = 0; // O nó à direita deve ser vermelho após a rotação
    return x;
}

// Função para inverter as cores de um nó e seus filhos
void flipColors(Node* h) {
    h->color = 0; // Torna o nó vermelho
    h->left->color = 1; // Torna o filho esquerdo preto
    h->right->color = 1; // Torna o filho direito preto
}

Node* search(Node* root, int key) {
    // Caso base: se atingimos uma folha ou encontramos a chave
    if (root == NULL || root->key == key) {
        return root;
    }

    // Chave menor, busca à esquerda
    if (key < root->key) {
        return search(root->left, key);
    }

    // Chave maior, busca à direita
    return search(root->right, key);
}

// Função para percorrer a árvore em ordem
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        if (!root->is_deleted) {
            printf("%d ", root->key);
        }
        inOrderTraversal(root->right);
    }
}

// Função para marcar um nó como removido logicamente
void logicalRemove(Node* node) {
    if (node != NULL) {
        node->is_deleted = 1;
    }
}

Node* physicalRemove(Node* root) {
    // Caso base: se atingimos uma folha, retornamos NULL
    if (root == NULL) {
        return NULL;
    }

    // Remoção recursiva nos filhos esquerdo e direito
    root->left = physicalRemove(root->left);
    root->right = physicalRemove(root->right);

    // Se o nó está marcado como removido logicamente, liberamos a memória
    if (root->is_deleted) {
        free(root);
        return NULL;
    }

    return root;
}

Node* refreshTree(Node* root) {
    // Caso base: se atingimos uma folha, retornamos NULL
    if (root == NULL) {
        return NULL;
    }

    // Recursivamente cria cópias dos filhos esquerdo e direito
    Node* newLeft = refreshTree(root->left);
    Node* newRight = refreshTree(root->right);

    // Se o nó está marcado como removido logicamente, não o incluímos na nova árvore
    if (root->is_deleted) {
        // Liberamos a memória do nó marcado como removido logicamente
        free(root);
        return newLeft ? newLeft : newRight; // Retorna o filho não nulo
    }

    // Criamos um novo nó para a nova árvore
    Node* newNode = createNode(root->key);
    newNode->left = newLeft;
    newNode->right = newRight;
    newNode->color = root->color; // Copiamos a cor do nó

    return newNode;
}

// Função para inicializar o dicionário
Dictionary* initializeDictionary() {
    Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        exit(EXIT_FAILURE);
    }
    dict->root = NULL;
    return dict;
}

// Função para inserir um elemento no dicionário
void dictionaryInsert(Dictionary* dict, int key) {
    dict->root = insert(dict->root, key);
}

// Função para pesquisar um elemento no dicionário
Node* dictionarySearch(Dictionary* dict, int key) {
    return search(dict->root, key);
}

// Função para percorrer o dicionário em ordem
void dictionaryInOrderTraversal(Dictionary* dict) {
    inOrderTraversal(dict->root);
}

// Função para remover logicamente um elemento do dicionário
void dictionaryLogicalRemove(Dictionary* dict, int key) {
    Node* node = search(dict->root, key);
    if (node != NULL) {
        logicalRemove(node);
    }
}

// Função para remover fisicamente os elementos marcados como removidos logicamente do dicionário
void dictionaryPhysicalRemove(Dictionary* dict) {
    dict->root = physicalRemove(dict->root);
}

// Função para atualizar o dicionário, removendo logicamente os elementos marcados
void dictionaryRefresh(Dictionary* dict) {
    dict->root = refreshTree(dict->root);
}