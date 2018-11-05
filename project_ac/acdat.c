#include "acdat.h"

int nodeNum = 0;
//初始化Trie树根节�
void createRootNode(TrieNode* root) {
    root->key[0] = '#';
    root->key[1] = '\0';
    root->key[2] = '\0';
    root->right = 0;
    root->next = 0;
    root->state = 1;//Root的State = 1 
    root->depth = 0; //Newly Added 
    root->isFinal = FALSE;
    nodeNum++;
}

//Trie树插入新的节点 
void insertNode(char* str, TrieNode* parent) {
    int flag;
    TrieNode* child;
    while(*str) {
    	char temp[3];
    	if(*str > 0) {
    		temp[0] = *str;
    		temp[1] = '\0';
    		temp[2] = '\0';
		} else if(*str < 0) {
			temp[0] = *str;
			temp[1] = *(++str);
		    temp[2] = '\0';
		}
        flag = FALSE;
        for(child = parent->next;child;child = child->right) {
            if(child->key[0] == temp[0] && child->key[1] == temp[1]) {
        	    	flag = TRUE;
        	    	break;
			}	
        }
        if(!flag) {
            child = (TrieNode*) malloc(sizeof(TrieNode));
            child->right = parent->next; //多叉树 ，指向同级的兄弟Trie节点 
            child->next = 0;
            child->key[0] = temp[0];//字符
			child->key[1] = temp[1];
			child->key[2] = temp[2];
			//printf("%d %d %d\n", child->key[0], child->key[1], child->key[2]);
			//printf("%s\n",child->key);
			//printf("%d %d %d\n",child->key[0], (STATE)(child->key[0]), (unsigned char)(child->key[0]));
            child->isFinal = FALSE;
            child->state = 0; //构建DAT时再赋值，此处留空 
            child->depth = parent->depth + 1; //Newly Added 
            parent->next = child;
            nodeNum++;
        }
        parent = child;
        if(! *(++str)) {
            child->isFinal = TRUE;
        }
    }
}

// Debug输出Trie树: root - Trie树根节点, num - Trie树节点个数 
void outputTrie(TrieNode* root) {
	TrieNode** node = (TrieNode**) malloc(nodeNum * sizeof(TrieNode*));
	node[0] = root;
	int depth = 0;
	int currentNum = 1;
	int node_num = 0;
	printf("depth:%d,node:%s\n", depth, node[0]->key);
	while(node_num < nodeNum) {
		int tempNum = currentNum;
		TrieNode* child;
		for(child=node[node_num]->next;child;child = child->right) {
			node[currentNum++] = child;
		}
        for(tempNum;tempNum<currentNum;tempNum++) {
        	printf("parent:%s, depth:%d, node:%s  ",node[node_num]->key,node[tempNum]->depth, node[tempNum]->key);
        	if(node[tempNum]->isFinal) {
        		printf("�����ڵ�");
			}
			printf("\n");
		}
		node_num++;
	}	
    free(node);
}

//初始化Base, Check双数组 
void init(int* base, int* check) {
    base[1] = 1;
    check[1] = 0;     
}

//根据Trie树，构建双数组（Base[], Check[]）- num (Trie树节点个数) ，last[] (数组存储模式串的终止状态(Trie树叶节点)) 
int generateDAT(TrieNode* root, int** base, int** check, STATE** last, int base_range,int last_range) {
    int lastNum = 0;
    int node_num = 0;
    TrieNode** nodes = (TrieNode**) calloc(nodeNum, sizeof(TrieNode*));
    nodes[0] = root;
    int currentNum = 1;
    int max_range = base_range;
    printf("nodeNum:%d\n",nodeNum); 
    //system("pause");
    while(node_num < nodeNum) {
		//printf("node_num:%d\n",node_num); 
        int tempNum = currentNum;
        int insert_index = 0;//记录nodes[node_num]子节点的个数 
        TrieNode* child;
        for(child = nodes[node_num]->next;child;child = child->right) {
            nodes[tempNum+(insert_index++)] = child;//将nodes[node_num]子节点存入nodes数组 
        }
        if(insert_index > 0) {//如果存在nodes[node_num]子节点 
            currentNum = tempNum + insert_index;
            // 相同索引下的*check[]先赋值，begin[]后赋值 
            //赋值Base数组 (nodes[node_num] 节点)
            if((*base)[nodes[node_num]->state] == 0) {
                int begin = 1;
                do {
                    int flag = TRUE;
                    int child_index = 0;
                    
                    // 找出满足*check[begin + a1...an] (�*base[begin+ a1...an])  == 0的n个空闲空间,a1...an是siblings中的n个节点
                    for(child_index;child_index<insert_index;child_index++) { // 遍历nodes[node_num]子节点 
                        STATE child_state = begin + code(nodes[tempNum+child_index]->key);
                        if(child_state > max_range-1) {
                        	*base = (int*)realloc(*base, child_state * 2 * sizeof(int));
                        	memset(*(base)+max_range,0,(child_state * 2 - max_range) * sizeof(int));
                        	*check = (int*)realloc(*check, child_state * 2 * sizeof(int));
                        	memset((*check)+max_range,0,(child_state * 2 - max_range) * sizeof(int));
                        	max_range = child_state * 2;
                        	printf("����Ϊ%d\n", max_range);
						}
                        if((*check)[child_state] != 0) { // 
                            flag = FALSE;
                            break;
                        }
                    }
                    if(flag) break; 
                } while(begin++);
                (*base)[nodes[node_num]->state] = begin; // Root节点的state=1   
            }
            
            //赋值Check数组 (nodes[node_num]子节点） 
            int child_index =0;
            for(child_index=0;child_index<insert_index;child_index++) {
                nodes[tempNum+child_index]->state = (*base)[nodes[node_num]->state] + code(nodes[tempNum+child_index]->key); //赋值子节点的state域，对应数组的index 
                (*check)[nodes[tempNum+child_index]->state] = nodes[node_num]->state; //赋值Check数组对应子节点的元素 
            }
        }
        
        //赋值Last数组 
        if(nodes[node_num]->isFinal) {
        	if(lastNum > last_range -1) {
        		*last = (int*)realloc(*last, lastNum * 2 * sizeof(int));
                //memset(last+last_range,0,(lastNum * 2-last_range) * sizeof(int));
                last_range = lastNum * 2;
			}
            (*last)[lastNum++] = nodes[node_num]->state;
        }
        node_num++;
    }
    int last_index = 0;
    for(last_index=0;last_index<lastNum;last_index++) {
        if((*base)[(*last)[last_index]] == 0) {
            (*base)[(*last)[last_index]] = (*last)[last_index] * -1;
        } else {
            (*base)[(*last)[last_index]] = (*base)[(*last)[last_index]] * -1;
        }
    }
    free(nodes);
    return max_range;
}

//构建输出表 
void constructOutput(char** output, int* base, int* check, TrieNode* node){
	char* str = (char*)calloc(50, sizeof(char));
	if(str) {
		int index = 0;
		int i;
		str = strcat(str, codetostring((base[check[node->state]] < 0) ? node->state + base[check[node->state]] : node->state - base[check[node->state]]));//Newly Modified bug unfixed
		for(i = check[node->state];check[i] != 0;i = check[i]){
			str = strcat(str, codetostring((base[check[i]] < 0) ? i + base[check[i]] : i - base[check[i]])); 
		}
		str = strrev(str);
		output[node->state] = str;
	} else printf("Output Error.\n");
	return;
}

//构建失效表Fail 
void constructFailureStates(TrieNode* root, int* fail, int* base, int*  check, char** output){
	TrieNode** queue = (TrieNode**) calloc(nodeNum+1, sizeof(TrieNode*));
	int front = 0;
	int tail = 0;
	fail[tail++] = 1;
	// 第一步，将深度为1的节点的failure设为根节点
	TrieNode* child; 
	for(child = root->next;child;child = child->right) {
		fail[child->state] = root->state;
		queue[tail++] = child;
        if(child->isFinal) {
           constructOutput(output, base, check, child); 
        }
	}
	front++;
	// 第二步，为深度 > 1 的节点建立failure表 (bfs)
	while(front < tail) {
		TrieNode* curNode = queue[front++];
		for(child = curNode->next;child;child = child->right) {
			STATE tmpStat = base[fail[curNode->state]] > 0 ? base[fail[curNode->state]]+code(child->key) : base[fail[curNode->state]]*(-1)+code(child->key);
			if(check[tmpStat] == fail[curNode->state]){
				fail[child->state] = tmpStat; 
			} else
				fail[child->state] = root->state; 
            if(child->isFinal) {
              constructOutput(output, base, check, child); 
            }
			queue[tail++] = child; 
		}
	}
    free(queue);
}

int search(FILE* fin, int *base, int *check, int *fail, int* count) {
    STATE stat = 1;
    char ch;
    while(EOF!=(ch= fgetc(fin))){
    	if(ch == '\n') continue;
    	char temp[3];
    	if(ch > 0) {
    		temp[0] = ch;
    		temp[1] = '\0';
		} else if(ch <0) {
			temp[0] = ch;
			do{temp[1] = fgetc(fin);
			}while(temp[1] == '\n');
			temp[2] = '\0';
		}
    	//printf("%s\n",temp);
    	//system("pause");
    	if((base[stat] > 0 ? check[base[stat]+code(temp)] : check[base[stat]*(-1)+code(temp)]) != stat) { // fail
    		while(stat != 1) {//fail
				stat = fail[stat];
    			if(stat == 0) stat = 1; //Bug Fix
    			if((base[stat] > 0 ? check[base[stat]+code(temp)] : check[base[stat]*(-1)+code(temp)])  == stat) break;
			}
		}
    	
    	if((base[stat] > 0 ? check[base[stat]+code(temp)] : check[base[stat]*(-1)+code(temp)]) == stat){ //success - goto
			stat = (base[stat] > 0) ? base[stat]+code(temp) : base[stat] * -1 +code(temp); 
			//检验是否属于终止状态、统计关键字词频
    		STATE tmpStat = stat;
    		while(tmpStat != 1){
    			if(base[tmpStat] < 0) { //isFinal
    				count[tmpStat]++; 
    				//printf("\nAddd Count\n");
				}
    			tmpStat = fail[tmpStat];
    			if(tmpStat == 0) tmpStat = 1; //Bug Fix
			}
		}
	}
}

void sort(int* count, char** output, int max_num) {
    int temp;
    char* temp_char;
    int i,j;
    for(i = 0;i< max_num;i++) {
        for(j= i+1;j< max_num;j++) {
            if(count[i] < count[j]) {
                temp = count[i];
                temp_char = output[i];
                count[i] = count[j];
                output[i] = output[j];
                count[j] = temp;
                output[j] = temp_char;
            }
        }
    }
}

void outputResult(FILE* fout, int* count, char** output, int max_num){
	int i;
	for(i = 0;i < max_num; i++){
		if(count[i] != 0 && output[i] != NULL)
			fprintf(fout,"%s %d\n",output[i],count[i]);
	}
	return;
}

inline STATE code(const char* c) {//字符c的状态编码 ，字符串的状态编码是字符状态编码的可叠加和 
	if(c[0] > 0) return c[0];
	if(c[0] < 0) return (unsigned char)c[0]*256+(unsigned char)c[1];//c[0],���ֽڣ�c[1],���ֽ� 
    //gbk2312
}

inline char* codetostring(STATE code) {
	//TODO
	char* str = (char*)malloc(3*sizeof(char));
	//english str[1] = '\0' str[2]='\0'
	//chinese str[2] = '\0' 
	if(code < 256){
		str[0] = code; str[1] = '\0'; 
	}
	else{
		str[0] = code % 256; str[1] = code/ 256; str[2] = '\0';
	} 
	return str;
}

void destroyTrie(TrieNode* root) {
	//TODO
}

char *strrev(char *str)
{
   char *p1, *p2;
   if (! str || ! *str)
      return str;
   for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
   {
       *p1 ^= *p2;
       *p2 ^= *p1;
       *p1 ^= *p2;
   }
   return str;
}
