#include "acdat.h"
#include <locale.h>
#include <time.h>

#define CSET_GBK    "GB2312"
#define LC_NAME_zh_CN   "Chinese_People's Republic of China"
#define LC_NAME_zh_CN_GBK  LC_NAME_zh_CN "." CSET_GBK

extern int nodeNum;

void printTime();

int main(int argc, char** argv) {
    printTime();
	printf("%d\n",setlocale(LC_ALL, LC_NAME_zh_CN_GBK));
	TrieNode* root = (TrieNode*) malloc(sizeof(TrieNode));
	createRootNode(root);
	FILE* fin_pattern = fopen(argv[1], "r");
	if(!fin_pattern){printf("%s Open Failed!\n", argv[1]);return 0;}
	char data[500];
	while(!feof(fin_pattern)) {
        fscanf(fin_pattern,"%s",data);
        insertNode(data, root);
        //printf("%s\n", data);
	}
	fclose(fin_pattern);
	printf("Trie Build\n");
    printTime();
	//outputTrie(root);
	int* base = (int*)calloc(INIT_NUM, sizeof(int));
	int* check = (int*)calloc(INIT_NUM, sizeof(int));
    STATE* last = (STATE*) calloc(INIT_NUM, sizeof(STATE));
	init(base, check);
	int max_num = generateDAT(root, &base, &check, &last,INIT_NUM,INIT_NUM);
	printf("DAT Trie Build, max_numΪ:%d\n", max_num);
    printTime();
	int* fail = (int*)calloc(max_num, sizeof(int));
	int* count = (int*)calloc(max_num, sizeof(int));
	char** output = (char**)calloc(max_num, sizeof(char*));
	constructFailureStates(root, fail, base, check, output);
	printf("DAT Fail Build\n");
    printTime();
	FILE* fin_string = fopen(argv[2],"r");
	FILE* fout = fopen(argv[3],"w");
	if(!(fin_string&&fout)) { printf("Open Files %s/%s Error!\n", argv[2], argv[3]); return 0; }
    printf("start search\n");
	search(fin_string, base, check, fail, count); 
    free(base);
    free(check);
    free(last);
    free(fail);
    //printf("start sort\n");
    //sort(count, output, max_num);
    printTime();
    printf("start output\n");
	outputResult(fout, count, output, max_num);
	fclose(fin_string);
	fclose(fout);
    printTime();
	
	//TODO free, destroy tree
	return 0;
}

void printTime() {
    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);
    printf("now datetime: %d-%d-%d %d:%d:%d\n", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec) ;
}
