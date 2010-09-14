#include "mpx_supt.h"
#include <stdio.h>
#include <string.h>

char* keyboardInput();
int CmpP2S(char *, char[]);
void handler_readme();
void main () {
	int bufSize= 10;
	char *BufferCMP= NULL;
	int truth = 0;
	BufferCMP = keyboardInput();
	truth = CmpP2S(BufferCMP,"JJ");	
	printf("\ntruth:%d", truth);
	handler_readme();
	sys_req(READ,TERMINAL,BufferCMP,&bufSize);
}

char* keyboardInput (){
	char *Buffer = NULL;
	char *BufferCMP= NULL;
	int WordSize;
	int Temp;
	int bufSize= 10;
	int flag;
	int i = 0;
	int j = 0;
	printf("This is a help file");
	sys_req(READ,TERMINAL,Buffer,&bufSize);

	while(i<bufSize){
		Temp = Buffer[i];
		if(Temp==10){
			break;
		}
		else{
			printf("\nc:%c b:%c",BufferCMP[j],Temp);
			BufferCMP[j] = Buffer[i];
			j++;
			//printf("%d",j);
		}
		i++;
	}
	return BufferCMP;
}
int CmpP2S(char *BufferCMP, char Word[]){
	int WordSize;
	int Temp;
	int bufSize= 10;
	int flag;
	int i;
	WordSize = sizeof(Word)-1;
	i= 0;
	flag = 0;
	printf("%d",WordSize);
	while(i< WordSize){
		//printf("\nWord:%c Buff:%c",Word[i],BufferCMP[i]);
		if(Word[i] != BufferCMP[i]){
			flag = 1;
			break;
		}
		i++;
	}
	//printf("%d",flag);
	if(flag == 0){
		printf("\nzero!");
		sys_req(READ,TERMINAL,BufferCMP,&bufSize);
		return 1;
	}
	else{
		printf("\nnot 0");
		sys_req(READ,TERMINAL,BufferCMP,&bufSize);
		return 0;
	}
}
void handler_readme(){
	printf("Readme");
	}
	