//Function Fresco: Alex Hamilton, Luis Armendariz, Sam Romano
//9-15-10
//CS 450 Module R1: User Interface - Command Handler   (COMHAN)

#include "mpx_supt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define historySize 10


/******** Prototypes ********/
void commandHandler(void);

void displayWelcome(void);
void commandPrompt(void);
void displayClosing(void);
void commandCleanup(void);
int errorCheck(int);
void storeHistory(char*);

void handler_help(void);
int handler_version(void);
int handler_get_Date(void);
void handler_set_Date(void);
void handler_display_MPX();
void handler_terminate_MPX(void);
void handler_readme();

char* keyboardInput2(int);
char* keyboardInput();
void change_prompt();
int cmpP2S(char *, char[]);
int sizeOfPointer(char*);


/******** Parameter List ********/
int version1 = 0;
int version2 =4; 							//Current MPX Version #
int comDone = 0;							//Command Handler Loop Indictor - Indicates whether the user is ready to terminate the program.
char *userInput;							//Current User Input
char *userCommand;
int error = 0;								//Variable for Error Handling

int bufSize = 80;
char prompt[5] = ":>";

char *historyList[historySize];
int historyQueue_Head = 0;
int historyQueue_Tail = 0;
int historyCounter = 0;


/******** Main ********/
void main(){
	sys_init(MODULE_R1);			    	//1. Initialize
	commandHandler();				    	//2. Call commandHandler function
}//end main


/******** Command Handler ********/
void commandHandler(){
	int i = 0;
	int userCommandSize;
	displayWelcome();						//2.1 Display the Welcome Message

	while(comDone !=1){						//2.2 Begin While Loop for User Commands
		//commandPrompt();					//2.2.1 Request for User Input
		printf("\nPlease enter the command to be executed(case sensitive).\n");
		userCommand = keyboardInput();      //2.2.2 Accept command from User
		printf("The userCommand is: ");
		userCommandSize = sizeOfPointer(userCommand);
		//printf("%d",userCommandSize);
		i = 0;
		while(i<userCommandSize){
			printf("%c", userCommand[i]); //test1
			i++;
		}
		printf("\n");
		
		//Temporary Decision Statement
		if(cmpP2S(userCommand, "help") == 1||cmpP2S(userCommand, "?") == 1){
			handler_help();
		} 
		else if(cmpP2S(userCommand, "version") == 1){
			handler_version();
		} 
		else if(cmpP2S(userCommand, "set_Date") == 1){
			//handler_set_Date();
		} 
		else if(cmpP2S(userCommand, "get_Date") == 1){
			//handler_get_Date();
		} 
		else if(cmpP2S(userCommand, "display_MPX") == 1){
			handler_display_MPX();
		} 
		else if(cmpP2S(userCommand, "terminate_MPX") == 1){
			handler_terminate_MPX();
		} 
		else if(cmpP2S(userCommand, "change_prompt") == 1){
			change_prompt();
		} 
		else if(cmpP2S(userCommand, "exit")==1){
			break;
		} 
		else {
			printf("Invalid Command.\n");
		}//end if - Decision
		
	}//end while
	displayClosing();						//2.2.4 Display closing message
	commandCleanup();						//2.2.5 Cleanup Allocated Memory
	keyboardInput();
	sys_exit();								//2.2.6 Return to host system
}



/******** Command Handler Functions ********/
void displayWelcome(){
	printf("Welcome to the Functional Fresco MPX OS.\n");
}//end displayWelcome

void commandPrompt(){
	printf("Please enter a command to be executed: ");
	printf(">");
}//end commandPrompt

void displayClosing(){
	printf("Thank you for using the Functional Fresco MPX OS.\nHave a nice day! :)\n");
}//end displayClosing

void commandCleanup(){
	//int error = sys_free_mem(userCommand); //!NOTE! - Display Errors
	//error = errorCheck(error);
	printf("The error number is %d.\n", error); //test3------------------------------------------
}//end commandCleanup

int errorCheck(int error){
	if(error == 0){
		return 0;				//No Error
	} else{
		if(error == -101){
			printf("Error(-101): Invalid Device ID.\n");
		} else if(error == -102){
			printf("Error(-102): Invalid Op Code.\n");
		} else if(error == -103){
			printf("Error(-103): Invalid Character Position.\n");
		} else if(error == -104){
			printf("Error(-104): Read Failed.\n");
		} else if(error == -105){
			printf("Error(-105): Write Failed.\n");
		} else if(error == -106){
			printf("Error(-106): Invalid Memory Block Pointer.\n");
		} else if(error == -107){
			printf("Error(-107): Free Failed.\n");
		} else if(error == -108){
			printf("Error(-108): Invalid Date.\n");
		} else if(error == -109){
			printf("Error(-109): Date Not Changed.\n");
		} else if(error == -110){
			printf("Error(-110): Invalid Directory Name.\n");
		} else if(error == -111){
			printf("Error(-111): Directory Open Error.\n");
		} else if(error == -112){
			printf("Error(-112): No Directory Is Open.\n");
		} else if(error == -113){
			printf("Error(-113): No More Directory Entries.\n");
		} else if(error == -114){
			printf("Error(-114): Name Too Long For Buffer.\n");
		} else if(error == -115){
			printf("Error(-115): Directory Close Error.\n");
		} else if(error == -116){
			printf("Error(-116): Progam Load Failed.\n");
		} else if(error == -117){
			printf("Error(-117): File Not Found.\n");
		} else if(error == -118){
			printf("Error(-118): File Invalid.\n");
		} else if(error == -119){
			printf("Error(-119): Program Size Error.\n");
		} else if(error == -120){
			printf("Error(-120): Invalid Load Address.\n");
		} else if(error == -121){
			printf("Error(-121): Memory Allocation Error.\n");
		} else if(error == -122){
			printf("Error(-122): Memory Free Error.\n");
		} else if(error == -123){
			printf("Error(-123): Invalid Handler Address.\n");
		}
		return -1;				//Common Error Value
	}//end if
}//end errorCheck

void storeHistory(char *userCommand){
	
	if(historyCounter == 0){
		*historyList[historyQueue_Head] = *userCommand;		//Store User Command to Queue Head
		historyQueue_Tail = historyQueue_Tail + 1;			//Increment Queue Tail to 1
		historyCounter = historyCounter + 1;
	} else if(historyCounter <= 9){
		*historyList[historyQueue_Tail] = *userCommand;		//Store User Command to Queue Tail
		historyQueue_Tail = historyQueue_Tail + 1;			//Increment Queue Tail by 1
		historyCounter = historyCounter + 1;
	} else if(historyCounter == 10){
		if(historyQueue_Tail == 10){
			historyQueue_Tail = 0;
			historyQueue_Head = historyQueue_Head + 1;
		}//end if
		
		*historyList[historyQueue_Tail] = *userCommand;
		historyQueue_Tail = historyQueue_Tail + 1;
		historyQueue_Head = historyQueue_Head + 1;
	} else if(historyCounter > 10){
		if(historyQueue_Head == 10){
			historyQueue_Head = 0;
		}//end if
		if(historyQueue_Tail == 10){
			historyQueue_Tail = 0;
		}//end if
		
		*historyList[historyQueue_Tail] = *userCommand;
		historyQueue_Tail = historyQueue_Tail + 1;
		historyQueue_Head = historyQueue_Head + 1;
	}//end if
}//end storeHistory


/********* "Handler" Commands ********/
//Display Help Information
void handler_help(){
	printf("Here is a list of Help commands.\n\n");
	printf("-----------Help-----------\n");
	printf("help                  Displays help on the current acceptable commands.\n");
	printf("version               Displays the current MPX Version Number.\n");
	printf("set_Date              Allows the user to set the current date.\n");
	printf("get_Date              Displays the current date.\n");
	printf("display_MPX			  Displays the available MPX Process Files.\n");
	printf("terminate_MPX		  Terminates the MPX program.\n");
	printf("display_History		  Displays a history of previous User Commands.\n");
	printf("change_prompt         Allows the user to change the prompt");
	printf("\n");
	//Add a while loop perhaps with additional command info available via help files?..........................
}//end handler_help

//Display Current MPX Version #
int handler_version(){

	//Print the current version number
	printf("MPX Version %d.%d\n", version1,version2);
	
	//Print the date current version was completed
	printf("Completed 09/15/2010\n");
	
	return 0;
}//end handler_version

//Display Date
int handler_get_Date(){
	//Declare new date_rec variable
	date_rec date;
	
	//Declare a pointer of type pointer to date_rec
	date_rec *dptr;
	
	//Set pointer to point to date_rec date
	dptr = &date;
	
	//Call system function with pointer passed in
	sys_get_date(&date);
	
	//Print date by accessing the attributes of the pointer to the struct
	printf("The current date is: %d/%d/%d\n", date.month, date.day, date.year);
	//&date = dptr; // ERASE ME LATER
	printf("%c",dptr[0]);
	return 0;
}//end handler_get_Date

//Set Date
void handler_set_Date(){
	//Use built in function.............................
	printf("Enter the day (dd): \n");
}//end handler_set_Date

//Display Directory of Available MPX Process Files
void handler_display_MPX(){
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

//Terminate MPX Execution
void handler_terminate_MPX(){
	int flag = 0;
	char *userAns; //n is No, y is Yes
	while(flag ==0){
		printf("\nAre you sure you would like to exit the system (y or n):\n");
		
		userAns = keyboardInput(); 		//Chang this to sys_req-------------------------------------------------

		if(cmpP2S(userAns,"y")==1){
			comDone = 1;
			flag = 1;
		}
		else if(cmpP2S(userAns,"n") !=1){
			printf("Invalid input. Input is case sensitive.\n");
		}
		else{
			flag = 1;
		}//end if
	}
}//end handler_terminate_MPX

//Display History of User Commands
void handler_display_History(){
	int historyDisplay_Head = historyQueue_Head;
	int historyDisplay_Tail = historyQueue_Tail;

	printf("List of Previous User Commands\n");
	printf("------------------------------\n");
	
	while(historyDisplay_Head != historyDisplay_Tail){
		printf("%s\n", *historyList[historyDisplay_Head]);
		historyDisplay_Head = historyDisplay_Head + 1;
		
		if(historyDisplay_Head >= 10){
			historyDisplay_Head = 0;
		}//end if
	}//end while
	
	printf("%s\n", *historyList[historyDisplay_Tail]);
}//end handler_display_History

void handler_readme(){
	printf("\nReadme!");
}


//Gets input from the Keyboard
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
				if(Temp ==95 ||Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
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
				if(Temp ==95 ||Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
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
				if(Temp ==95 ||Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
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
				if(Temp ==95 ||Temp >=47 && Temp <59|| Temp >=65 && Temp<91 || Temp >= 92&& Temp < 93 || Temp >=97 && Temp <123){
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

//Compares Keyboard Output to String
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
			//printf(">");
			return 0;
		}
		else if(BufferSize<WordSize){
			//printf("<");
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
