#include "mpx_supt.h"
#include <stdio.h>
#include <string.h>

char* keyboardInput2(int);
char* keyboardInput();
void change_prompt();
int cmpP2S(char *, char[]);
int sizeOfPointer(char*);
void handler_readme();
void handler_display_mpx();
//char* pointerToArray(char*);
int bufSize= 80;
char prompt[5] = ":>";


void main () {
	char *Buffer= NULL;
	//int truth = 0;
	/*Buffer = keyboardInput();
	truth = cmpP2S(Buffer,"OOOKKKK");	
	printf("truth:%d", truth);
	Buffer = keyboardInput();
	truth = cmpP2S(Buffer,"OOOKKKK");	
	printf("truth:%d", truth);
	//handler_readme();*/
	handler_display_mpx();
	//change_prompt();
	//change_prompt();
	sys_req(READ,TERMINAL,Buffer,&bufSize);
}
char* keyboardInput (){
	char *Buffer= NULL;
	char *Buffer2 = NULL;
	int bufSize2 = 0;
	int WordSize;
	int Temp;
	int flag;
	int i = 0;
	int j = 0;
	if(bufSize2<1){
		printf("%s",prompt);
		sys_req(READ,TERMINAL,Buffer2,&bufSize);

		while(i<bufSize){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
					//printf("\nc:%c b:%c",Buffer[j],Temp);
					Buffer[j] = Buffer2[i];
					j++;
					//printf("%d",j);
				}
			}
			i++;
		}
		return Buffer;
	}
	else{
		printf("%s",prompt);
		sys_req(READ,TERMINAL,Buffer2,&bufSize2);

		while(i<bufSize2){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
					//printf("\nc:%c b:%c",Buffer[j],Temp);
					Buffer[j] = Buffer2[i];
					j++;
					//printf("%d",j);
				}
			}
			i++;
		}
		return Buffer;
	}
}

char* keyboardInput2 (int bufSize2){
	char *Buffer= NULL;
	char *Buffer2 = NULL;
	int WordSize;
	int Temp;
	int flag;
	int i = 0;
	int j = 0;
	if(bufSize2<1){
		printf("%s",prompt);
		sys_req(READ,TERMINAL,Buffer2,&bufSize);

		while(i<bufSize){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
					//printf("\nc:%c b:%c",Buffer[j],Temp);
					Buffer[j] = Buffer2[i];
					j++;
					//printf("%d",j);
				}
			}
			i++;
		}
		return Buffer;
	}
	else{
		printf("%s",prompt);
		sys_req(READ,TERMINAL,Buffer2,&bufSize2);

		while(i<bufSize2){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
					//printf("\nc:%c b:%c",Buffer[j],Temp);
					Buffer[j] = Buffer2[i];
					j++;
					//printf("%d",j);
				}
			}
			i++;
		}
		return Buffer;
	}
}



int cmpP2S(char *Buffer, char Word[]){
	int WordSize, BufferSize;
	int Temp;
	int flag= 0;
	int i = 0;
	BufferSize = sizeOfPointer(Buffer);
	WordSize = sizeOfArray(Word);
	//printf("\n\n%d",WordSize);
	if(WordSize <1){
		//error word to short
		printf("error 1");
		return -1;
	}
	else{
		if(BufferSize>WordSize){
			printf(">");
			return 0;
		}
		else if(BufferSize<WordSize){
			printf("<");
			return 0;
		}
		else{
			while(i< WordSize){
				//printf("\nWord:%c Buff:%c",Word[i],Buffer[i]);
				if(Word[i] != Buffer[i]){
					flag = 1;
					break;
				}
				i++;
			}
		
			//printf("%d",flag);
			if(flag == 0){
				//printf("\nzero!");
				//sys_req(READ,TERMINAL,Buffer,&bufSize);
				return 1;
			}
			else{
				//printf("\nnot 0");
				//sys_req(READ,TERMINAL,Buffer,&bufSize);
				return 0;
			}
		}
	}
}	
	

void handler_readme(){
	printf("\nReadme!");
}

void handler_display_mpx(){
	char *Buffer= NULL;
	char BufferArray[80] = {0};
	char currentFile[40];
	long j = 0;
	int error, i = 0;
	int nameSize = 40;
	printf("\nWelcome to MPX");
	printf("\nPlease enter the directory to be opened\n");
	Buffer = keyboardInput();
	while(i<sizeOfPointer(Buffer)){
		BufferArray[i] = Buffer[i];
		i++;
	}
	error = sys_open_dir(BufferArray);
	//errorCheck(error);
	printf("%s",BufferArray);
	if(error ==0){
		i = 0;
		while(error == 0){
			error = sys_get_entry(currentFile,nameSize,&j);
			if(error == 0){
				i++;
				printf("\nFilename:%s       \tBuffersize:%d\tFile Size:%ld",currentFile,nameSize,j);
			}
			//printf("\n%d",error);
		}
		if(error != -113){
			//errorCheck(error);
		}
		else{
			error = sys_close_dir();
			//errorCheck(error);
			//printf("\n%d",error);
			if(i == 0){
			printf("No MPX files are in that directory\n");
			}
		}
	}
}
int sizeOfPointer(char *Buffer){
	int i = 0;
	while(i<bufSize){
		//printf("\nBlah:%d",Buffer[i]);
		if(Buffer[i] == 10){
			return i;
		}
		i++;
	}
	return -1;
}
int sizeOfArray(char Array[]){
	int i = 0;
	while(i<80){
		//printf("Blah:%d",Array[i]);
		if(Array[i] ==0){
			return i;
		}
		i++;
	}
	return -1;
}
void change_prompt(){
	char *newPrompt;
	int i= 0, newPromptSize;
	
	printf("\nEnter New Prompt String(Max 5 characters)\n");
	newPrompt = keyboardInput2(5);
	newPromptSize = sizeOfPointer(newPrompt);
	while(i<4){
		//printf("\nWord:%c Buff:%c",prompt[i],newPrompt[i]);
		if(i<newPromptSize){
			prompt[i] = newPrompt[i];
		}
		else {
			prompt[i] = 0;
		}
		//printf("\nWord:%c Buff:%c",prompt[i],newPrompt[i]);
		i++;
	}
}