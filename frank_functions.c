#include "mpx_supt.h"
#include <stdio.h>
#include <string.h>

char* keyboardInput(char[]);

void main () {
	char *BufferCMP= NULL;
	BufferCMP = keyboardInput("GG");
	
}

char* keyboardInput (char Word[]){
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
		sys_req(READ,TERMINAL,Buffer,&bufSize);
	}
	else{
		printf("\nnot 0");
		sys_req(READ,TERMINAL,Buffer,&bufSize);
	}



	return BufferCMP;
}
