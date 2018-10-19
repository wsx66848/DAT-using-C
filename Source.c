#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef unsigned int STATE;

typedef struct TrieNode {
    char key;
    struct TrieNode* next;
    struct TrieNode* right;
    int isFinal;
    STATE state;
}TrieNode;

STATE code(char c);

void createRootNode(TrieNode* root, int* node_num) {
    root->key = '#';
    root->right = 0;
    root->next = 0;
    root->state = 1;
    root->isFinal = FALSE;
    (*node_num)++;
}

void insertNode(char* str, TrieNode* parent, int* node_num) {
    int flag;
    TrieNode* child;
    while(*str) {
        flag = FALSE;
        for(child = parent->next;child;child = child->right) {
            if(child->key == *str) {
                flag = TRUE;
                break;
            }
        }
        if(!flag) {
            child = (TrieNode*) malloc(sizeof(TrieNode));
            child->right = parent->next;
            child->next = 0;
            child->key = *str;
            child->isFinal = FALSE;
            child->state = 0;
            parent->next = child;
            (*node_num)++;
        }
        parent = child;
        if(! *(++str)) {
            child->isFinal = TRUE;
        }
    }
}

void outputTrie(TrieNode* root,int num) {
	TrieNode** node = (TrieNode**) malloc(num * sizeof(TrieNode*));
	node[0] = root;
	int depth = 1;
	int currentNum = 1;
	int node_num = 0;
	printf("depth:%d,node:%c\n", depth, node[0]->key);
	while(node_num < num) {
		int tempNum = currentNum;
		TrieNode* child;
		for(child=node[node_num]->next;child;child = child->right) {
			node[currentNum++] = child;
		}
        for(tempNum;tempNum<currentNum;tempNum++) {
        	printf("parent:%c, node:%c  ",node[node_num]->key,node[tempNum]->key);
        	if(node[tempNum]->isFinal) {
        		printf("½áÊø×Ö·û");
			}
			printf("\n");
		}
		node_num++;
	}
	
}

void init(int* base, int* check) {
    base[1] = 1;
    check[1] = 0;     
}

void generateDAT(TrieNode* root, int* base, int* check, STATE* last, int num) {
    int lastNum = 0;
    int node_num = 0;
    TrieNode** nodes = (TrieNode**) calloc(num, sizeof(TrieNode*));
    nodes[0] = root;
    int currentNum = 1;
    while(node_num < num) {
        int tempNum = currentNum;
        int insert_index = 0;
        TrieNode* child;
        for(child = nodes[node_num]->next;child;child = child->right) {
            nodes[tempNum+(insert_index++)] = child;
        }
        if(insert_index > 0) {
            currentNum = tempNum + insert_index;
            if(base[nodes[node_num]->state] == 0){
                int begin = 1;
                do {
                    int flag = TRUE;
                    int child_index = 0;
                    for(child_index;child_index<insert_index;child_index++) {
                        if(check[begin+code(nodes[tempNum+child_index]->key)] != 0) {
                            flag = FALSE;
                            break;
                        }
                    }
                    if(flag) break;
                }while(begin++);
                base[nodes[node_num]->state] = begin;
            }
            int child_index =0;
            for(child_index=0;child_index<insert_index;child_index++) {
                nodes[tempNum+child_index]->state = base[nodes[node_num]->state] + code(nodes[tempNum+child_index]->key);
                check[nodes[tempNum+child_index]->state] = nodes[node_num]->state;
            }
        }
        if(nodes[node_num]->isFinal) {
            last[lastNum++] = nodes[node_num]->state;
        }
        node_num++;
    }
    int last_index = 0;
    for(last_index=0;last_index<lastNum;last_index++) {
        if(base[last[last_index]] == 0) {
            base[last[last_index]] = last[last_index] * -1;
        } else {
            base[last[last_index]] = base[last[last_index]] * -1;
        }
    }
}

int search(char *str, int *base,int *check) {
    STATE index = 1;
    while(*str) {
        STATE begin;
        if(base[index] < 0) {
            begin = base[index] * -1;
        } else {
            begin = base[index];
        }
        STATE sub =  begin + code(*str);
        if(check[sub] != index) return FALSE;
        index = sub;
        if(!*(++str)) {
            if(base[index] >= 0) return FALSE;
        }
    }
    return TRUE;
}

int main() {
	int node_num = 0;
	TrieNode* root = (TrieNode*) malloc(sizeof(TrieNode));
	createRootNode(root, &node_num);
	insertNode("a", root, &node_num);
	insertNode("b", root, &node_num);
	insertNode("c", root, &node_num);
	insertNode("ab", root, &node_num);
	insertNode("abc", root, &node_num);
	insertNode("ac", root, &node_num);
	insertNode("bc", root, &node_num);
	insertNode("bac", root, &node_num);
	insertNode("cb", root, &node_num);
	outputTrie(root, node_num);
	int base[200] = { 0 };
	int check[200] = { 0 };
	STATE last[20] = { 0 };
	init(base, check);
	generateDAT(root, base, check, last, node_num);
	//do {
		//int n = 0;
		//scanf_s("%d", &n);
		//char* str = (char*) malloc((n+1)*sizeof(char));
		//char str[2];
		//scanf_s("%s", str);
		char* res = search("ba", base, check) == TRUE ? "found" : "not found";
		printf("%s\n", res);
	//} while (1);

	return 0;
}

STATE code(char c) {
	return (STATE)c;
    //gbk2312
}