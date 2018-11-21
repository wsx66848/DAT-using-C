#ifndef __DATAC_H__
#define __DATAC_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define INIT_NUM 300000

typedef unsigned int STATE;

typedef struct TrieNode {
    char key[3];//字符 
    struct TrieNode* next;
    struct TrieNode* right;// 多叉树 ，指向同级的兄弟Trie节点
    int isFinal;
    int depth; // Newly Added
    STATE state;// State对应的下标 
}TrieNode;

STATE code(const char* c);
void createRootNode(TrieNode* root);
void insertNode(char* str, TrieNode* parent);
void outputTrie(TrieNode* root);
void destroyTrie(TrieNode* root);
void init(int* base, int* check);
int generateDAT(TrieNode* root, int** base, int** check, STATE** last,int base_range, int last_range);
int search(FILE* fin, int *base, int *check, char** output, int* count);
void outputResult(FILE* fout, int* count, char** output, int max_num);
void sort(int* count, char** output, int max_num);
int isCharEmpty(char c);

#endif
