/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR1.c

	Group Members:	Alex Hamilton, Luis Armendariz, Sam Romano
	Version: 0.5
	Date:  09/16/10

	Purpose: Module R1 file for MPX support

		This file contains the user interface and command handler
		functions needed to support the MPX OS.

		
	??Environment: System Independent (header file only)--------------------------------
	
	
	Procedures in this file:

		commandHandler
			displayWelcome
			
			handler_help
			handler_version
			handler_set_date
			handler_get_date
			handler_display_MPX
			handler_display_history
			handler_terminate_MPX
			change_Prompt
			
			displayClosing
			commandCleanup
			keyboardInput
			keyboardInput2
			cmpP2S
			sizeOfPointer
			sizeOfArray
			copyPtr
			errorCheck
			storeHistory
			
			
************************************************************************

 Change Log:

	09/11/10  lca	original version
	09/13/10  lca	Completed commandHandler Function
	09/14/10  lca	Added errorCheck Function
	09/15/10  lca	Added display_history Function
	09/16/10  lca	Compiled Code Together
	09/17/10  lca   Code v1.0 Ready for Demonstration

************************************************************************/
#include "mpx_supt.h"
/* ANSI C Include Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Logical Constants */
#define historySize 10


/******** Prototypes ********/
/* commandHandler: Handles User Commands & Command Execution */
void commandHandler(void);

/* displayWelcome: Displays Welcome Message */
void displayWelcome(void);

/* displayClosing: Displays Closing Message */
void displayClosing(void);

/* commandCleanup: Frees Memory */
void commandCleanup(void);

/* errorCheck: Displays Error Messages */
/*	RETURNS: Integer Error Code; Zero If Ok */
int errorCheck(int);

/* storeHistory: Stores User Command to History */
void storeHistory(char*);

/* handler_help: Displays Help Information */
void handler_help(void);

/* handler_version: Displays Version Number */
/*	RETURNS: Integer Error Code; Zero If Ok */
int handler_version(void);

/* handler_get_date: Gets System Date */
/*	RETURNS: Integer Error Code; Zero If Ok */
int handler_get_date(void);

/* handler_set_date: Sets System Date */
/*	RETURNS: Integer Error Code; Zero If Ok */
int handler_set_date(void);

/* handler_display_MPX: Displays Directory of MPX Process Files */
void handler_display_MPX();

/* handler_terminate_MPX: Terminate MPX & Return to Host OS */
void handler_terminate_MPX(void);

/* handler_readme: Displays ReadMe Information */
//void handler_readme(void);

/* handler_display_history: Displays User Command History */
//void handler_display_history(void);

/* keyboardInput2: Receives Keyboard Input (StaticBuffer) */
/*	RETURNS: String of Keyboard Input */
char* keyboardInput2(void);

/* keyboardInput: Receives Keyboard Input (DynamicBuffer) */
/*	RETURNS: String of Keyboard Input */
char* keyboardInput(int);

/* change_prompt: Change Prompt Symbol */
void change_prompt(void);

/* cmpP2S: Compare Strings */
/*	RETURNS: Integer Comparison Code; Zero If Not Same */
int cmpP2S(char *, char[]);

/* sizeOfPointer: Gets Size of Pointer */
/*	RETURNS: Integer Pointer Size */
int sizeOfPointer(char*);

/* sizeOfArray: Gets Size of Array */
/*	RETURNS: Integer Array Size */
int sizeOfArray(char[]);

/* copyPtr: Copies a Pointer */
void copyPtr(char*, char*);


/******** Parameter List ********/
int version1 = 0;								//Current MPX Version # - Ones Digit
int version2 = 5; 								//Current MPX Version # - Decimal Digit
int comDone = 0;								//Command Handler Loop Indictor - Indicates whether the user is ready to terminate the program.
char *userInput;								//Current User Input
char *userCommand;								//Current UserCommand
int error = 0;									//Variable for Error Handling

int bufSize = 80;								//User Command Buffer Size
char prompt[5] = ":>";							//Current Prompt

char *historyList[historySize];					//Command History - Queue
int historyQueue_Head = 0;						//Command History - Queue Head
int historyQueue_Tail = 0;						//Command History - Queue Tail
int historyCounter = 0;							//Command History Counter


/**
*	\n Procedure: Main()
*	\n Purpose: Initializes the program and runs the program
*	\n Parameters: None
*	\n Returns: None
*	\n Procedures Called: sys_init, commandHandler
*	\n Global Data Accessed: None
*/
void main(){
	sys_init(MODULE_R1);			    		//1. Initialize
	commandHandler();				    		//2. Call commandHandler function
}//end main


/******** Command Handler ********/
/**
	Procedure: commandHandler

	Purpose: Handles User Commands & Command Execution

	Parameters: None  

	Return value: None

	Calls: displayWelcome, keyboardInput, cmpP2S, handler_help, handler_version, handler_set_date, handler_get_date, handler_display_MPX, 
		handler_display_history, handler_terminate_MPX, change_prompt, sys_req, displayClosing, commandCleanup, sys_exit

	Globals: comDone
		userCommand

	Errors: None
**/
void commandHandler(){
	int userCommandSize;
	int fix = 0;
	displayWelcome();							//2.1 Display the Welcome Message

	while(comDone !=1){							//2.2 Begin While Loop for User Commands
		userCommand = NULL;
		if( fix ==0){
			printf("\nPlease enter the command to be executed(case sensitive).\n");
			userCommand = keyboardInput(0);      	//2.2.1 Request User Input & Accept Command from User
			printf("\n");
		
		
			//Decision Statement
			if(cmpP2S(userCommand, "help") == 1 || cmpP2S(userCommand, "/?") == 1){
				handler_help();
			} 
			else if(cmpP2S(userCommand, "version") == 1){
				handler_version();
			} 
			else if(cmpP2S(userCommand, "set_date") == 1){
				handler_set_date();
				fix = 1;
			} 
			else if(cmpP2S(userCommand, "get_date") == 1){
				handler_get_date();
			} 
			else if(cmpP2S(userCommand, "display_MPX") == 1){
				handler_display_MPX();
			} 
			//else if(cmpP2S(userCommand, "display_history") == 1){
			//	handler_display_history();
			//}
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
		}
		else {
			sys_req(READ,TERMINAL,userCommand,&bufSize);
			fix = 0;
		}
	}//end while
	displayClosing();						//2.2.4 Display closing message
	commandCleanup();						//2.2.5 Cleanup Allocated Memory
	keyboardInput(0);
	sys_exit();								//2.2.6 Return to host system
}



/******** Command Handler Functions ********/
/**
	Procedure: displayWelcome

	Purpose: Displays Welcome Message

	Parameters: None  

	Return value: None

	Calls: None    

	Globals: None

	Errors: None
**/
void displayWelcome(){
	printf("Welcome to the Functional Fresco MPX OS.\n");
}//end displayWelcome

/**
	Procedure: displayClosing

	Purpose: Displays Closing Message

	Parameters: None  

	Return value: None

	Calls: None    

	Globals: None

	Errors: None
**/
void displayClosing(){
	printf("Thank you for using the Functional Fresco MPX OS.\n Have a nice day! :)\n");
}//end displayClosing

/**
	Procedure: commandCleanup

	Purpose: Free Memory

	Parameters: None  

	Return value: None

	Calls: sys_free_mem   

	Globals: error

	Errors: None
**/
void commandCleanup(){
	//int error = sys_free_mem(userCommand); //!NOTE! - Display Errors
	//error = errorCheck(error);
	printf("The error number is %d.\n", error); //test3------------------------------------------
}//end commandCleanup

/**
	Procedure: errorCheck

	Purpose: Displays Error Messages

	Parameters: See Prototype  

	Return value: Error Code; Zero If Ok

	Calls: None    

	Globals: None

	Errors: None
**/
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

/**
	Procedure: storeHistory

	Purpose: Stores History of User Commands

	Parameters: See Prototype  

	Return value: None

	Calls: copyPtr    

	Globals: historyList, historyQueue_Tail, historyQueue_Head, historyCounter

	Errors: None
**/
void storeHistory(char *userCom){
	char *userInput;
	copyPtr(userInput, userCom);							//Copy Pointer Information from userCom to userInput
	
	if(historyCounter == 0){
		*historyList[historyQueue_Head] = *userInput;		//Store User Command to Queue Head
		historyQueue_Tail = historyQueue_Tail + 1;			//Increment Queue Tail to 1
		historyCounter = historyCounter + 1;
	} else if(historyCounter <= 9){
		*historyList[historyQueue_Tail] = *userInput;		//Store User Command to Queue Tail
		historyQueue_Tail = historyQueue_Tail + 1;			//Increment Queue Tail by 1
		historyCounter = historyCounter + 1;
	} else if(historyCounter == 10){
		if(historyQueue_Tail == 10){
			historyQueue_Tail = 0;
			historyQueue_Head = historyQueue_Head + 1;
		}//end if
		
		*historyList[historyQueue_Tail] = *userInput;
		historyQueue_Tail = historyQueue_Tail + 1;
		historyQueue_Head = historyQueue_Head + 1;
	} else if(historyCounter > 10){
		if(historyQueue_Head == 10){
			historyQueue_Head = 0;
		}//end if
		if(historyQueue_Tail == 10){
			historyQueue_Tail = 0;
		}//end if
		
		*historyList[historyQueue_Tail] = *userInput;
		historyQueue_Tail = historyQueue_Tail + 1;
		historyQueue_Head = historyQueue_Head + 1;
	}//end if
}//end storeHistory


/********* "Handler" Commands ********/
/**
	Procedure: handler_help

	Purpose: Displays Help Information

	Parameters: None 

	Return value: None

	Calls: None    

	Globals: None

	Errors: None
**/
void handler_help(){
	printf("Here is a list of Help commands.\n\n");
	printf("-----------Help-----------\n");
	printf("help                  Displays help on the current acceptable commands.\n");
	printf("version               Displays the current MPX Version Number.\n");
	printf("set_date              Allows the user to set the current date.\n");
	printf("get_date              Displays the current date.\n");
	printf("display_MPX			  Displays the available MPX Process Files.\n");
	printf("terminate_MPX		  Terminates the MPX program.\n");
	printf("display_history		  Displays a history of previous User Commands.\n");
	printf("change_prompt         Allows the user to change the prompt");
	printf("\n");
	//Add a while loop perhaps with additional command info available via help files?..........................
}//end handler_help

/**
	Procedure: handler_version

	Purpose: Displays Current MPX Version Number

	Parameters: None

	Return value: Error Code; Zero If Ok

	Calls: None    

	Globals: version1, version2

	Errors: None
**/
int handler_version(){

	//Print the current version number
	printf("MPX Version %d.%d\n", version1,version2);
	
	//Print the date current version was completed
	printf("Completed 09/16/2010\n");
	
	return 0;
}//end handler_version

/**
	Procedure: handler_get_date

	Purpose: Displays System Date

	Parameters: None  

	Return value: Error Code; Zero If Ok

	Calls: sys_get_date    

	Globals: None

	Errors: None
**/
int handler_get_date(){
	//Declare new date_rec variable
	date_rec date;
	
	//Call system function with date address passed in
	sys_get_date(&date);
	
	//Print date by accessing the attributes of the pointer to the struct
	printf("The current date is: %d/%d/%d\n", date.month, date.day, date.year);

	return 0;
}//end handler_get_Date

/**
	Procedure: handler_set_date

	Purpose: Sets System Date

	Parameters: None 

	Return value: Error Code; Zero If Ok

	Calls: keyboardInput, sys_set_date, sys_get_date, errorCheck

	Globals: None

	Errors: None
**/
int handler_set_date(){
	char *Buffer;
	int month;
	int day;
	int year;
	int flag = 0;
	int leapYear;
	int i = 0;
	
	printf("Enter the date in mmddyyyy format with no slashes or dashes: \n");
	Buffer = keyboardInput(9);
	

	while(i<8){
		if(Buffer[i] < 48 || Buffer[i] > 57){
			printf("The date you entered is invalid!");
			flag = 1;
		}
		i++;		
		
	}
	if(flag == 0){
		month = (Buffer[0] - 48)*10 + Buffer[1] - 48;
		day = (Buffer[2] - 48)*10 + Buffer[3] - 48;
		year = (Buffer[4] - 48)*1000 + (Buffer[5] - 48)*100 + (Buffer[6] - 48)*10 + Buffer[7] - 48;
		leapYear = year%4;
		if(month > 12|| day > 1 || month > 1){
			flag = 1;
		}
		if(leapYear==0){
			if(month < 13 && month == 02 && day > 29){
				flag = 1;
			}else if(month <13 && month == 1 && day > 31){
				flag = 1;
			}else if(month <13 && month == 3 && day > 31){
				flag = 1;
			}else if(month <13 && month == 5 && day > 31){
				flag = 1;
			}else if(month <13 && month == 7 && day > 31){
				flag = 1;		
			}else if(month <13 && month == 8 && day > 31){
				flag = 1;	
			}else if(month <13 && month == 10 && day > 31){
				flag = 1;	
			}else if(month <13 && month == 12 && day > 31){
				flag = 1;	
			}else if(month <13 && month == 4 && day > 30){
				flag = 1;
			}else if(month <13 && month == 6 && day > 30){
				flag = 1;	
			}else if(month <13 && month == 9 && day > 30){
				flag = 1;
			}else if(month <13 && month == 11 && day > 30){
				flag = 1;	
			}
		}else{
			if(month < 13 && month == 02 && day > 28){
				flag = 1;
			}else if(month <13 && month == 1 && day > 31){
				flag = 1;
			}else if(month <13 && month == 3 && day > 31){
				flag = 1;
			}else if(month <13 && month == 5 && day > 31){
				flag = 1;
			}else if(month <13 && month == 7 && day > 31){
				flag = 1;		
			}else if(month <13 && month == 8 && day > 31){
				flag = 1;	
			}else if(month <13 && month == 10 && day > 31){
				flag = 1;	
			}else if(month <13 && month == 12 && day > 31){
				flag = 1;	
			}else if(month <13 && month == 4 && day > 30){
				flag = 1;
			}else if(month <13 && month == 6 && day > 30){
				flag = 1;	
			}else if(month <13 && month == 9 && day > 30){
				flag = 1;
			}else if(month <13 && month == 11 && day > 30){
				flag = 1;	
			}
		}
		
	}	
	
	if(flag == 0){
		date_rec date;
		
		date.month = month;
		date.day = day;
		date.year = year;
		
		sys_set_date(&date);
		
		sys_get_date(&date);
		if(date.month != month || date.day != day || date.year != year){
			errorCheck(-109);
		}	
	}else{
		errorCheck(-108);
	}
	return 0;
}



/**
	Procedure: handler_display_MPX

	Purpose: Displays Directory of Available MPX Process Files

	Parameters: None  

	Return value: None

	Calls: sizeOfPointer, sys_open_dir, sys_close_dir  

	Globals: error

	Errors: None
**/
void handler_display_MPX(){
	char *Buffer= NULL;
	char BufferArray[80] = {0};
	char currentFile[40];
	long j = 0;
	int error, i = 0;
	int nameSize = 40;
	printf("\nWelcome to MPX");
	printf("\nPlease enter the directory to be opened it cannot contain spaces\n");
	Buffer = keyboardInput(0);
	while(i<sizeOfPointer(Buffer)){
		//printf("%c",Buffer[i]);
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

/**
	Procedure: handler_terminate_MPX

	Purpose: Terminate MPX & Return to Host OS

	Parameters: None  

	Return value: None

	Calls: keyboardInput, cmpP2S

	Globals: comDone

	Errors: None
**/
void handler_terminate_MPX(){
	int flag = 0;
	char *userAns; //n is No, y is Yes
	while(flag ==0){
		printf("\nAre you sure you would like to exit the system (y or n):\n");
		
		userAns = keyboardInput(0); 		//Chang this to sys_req-------------------------------------------------

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

/*void handler_readme(){
	printf("\nReadme!");
}*/


/********* Extra Credit Commands ********/
/*/**
	Procedure: handler_display_history

	Purpose: Display History of User Commands
	
	Parameters: None  

	Return value: None

	Calls: None

	Globals: historyDisplay_Head, historyDisplay_Tail, historyList, 

	Errors: None
**/
/*
void handler_display_history(){
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
*/

/**
	Procedure: change_prompt

	Purpose: Allows User to Change the Prompt Character

	Parameters: None  

	Return value: None

	Calls: keyboardInput, sizeOfPointer

	Globals: None

	Errors: None
**/
void change_prompt(){
	char *newPrompt;
	int i= 0, newPromptSize;
	
	printf("\nEnter New Prompt String(Max 5 characters)\n");
	newPrompt = keyboardInput(5);
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


/**
	Procedure: keyboardInput2

	Purpose: Gets User Input from Keyboard - Static Buffer Size (Buffer Size=80)

	Parameters: None  

	Return value: String of Keyboard Input

	Calls: sys_req

	Globals: bufSize, prompt

	Errors: None
**/
char* keyboardInput2 (){
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
				if(Temp>=33 && Temp <127){
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
				if(Temp>=33 && Temp <127){
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

/**
	Procedure: keyboardInput

	Purpose: Gets User Input from Keyboard - Dynamic Buffer Size

	Parameters: See Prototype  

	Return value: String of Keyboard Input

	Calls: sys_req

	Globals: bufSize, prompt

	Errors: None
**/
char* keyboardInput (int bufSize2){
	char *Buffer= NULL;
	char *Buffer2 = NULL;
	int WordSize;
	int Temp;
	int flag;
	int i = 0;
	int j = 0;
	if(bufSize2==0){
		printf("%s",prompt);
		sys_req(READ,TERMINAL,Buffer2,&bufSize);

		while(i<bufSize){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp>=33 && Temp <127){
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
	else if(bufSize2 >0){
		printf("%s",prompt);
		sys_req(READ,TERMINAL,Buffer2,&bufSize2);

		while(i<bufSize2){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp>=33 && Temp <127){
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
		sys_req(READ,TERMINAL,Buffer2,&bufSize);

		while(i<bufSize){
			Temp = Buffer2[i];
			if(Temp==10){
				break;
			}
			else{
				if(Temp>=32 && Temp <127){
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

/**
	Procedure: cmpP2S

	Purpose: Compares User Input to String (1 True, 0 False)

	Parameters: See Prototype  

	Return value: Comparison Code; Zero If Not Same

	Calls: sizeOfPointer, sizeOfArray

	Globals: None

	Errors: None
**/
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

/**
	Procedure: sizeOfPointer

	Purpose: Gets Size of a Pointer Variable

	Parameters: See Prototype  

	Return value: Integer Pointer Size

	Calls: None

	Globals: bufSize

	Errors: None
**/
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

/**
	Procedure: sizeOfArray

	Purpose: Gets Size of an Array

	Parameters: See Prototype  

	Return value: Integer Array Size

	Calls: None

	Globals: None

	Errors: None
**/
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

/**
	Procedure: copyPtr

	Purpose: Copy information in Pointer Y to Pointer X. (Pointer X, Pointer Y)

	Parameters: See Prototype  

	Return value: None

	Calls: sizeOfPointer

	Globals: None

	Errors: None
**/
void copyPtr(char *userInp, char *userCom){
	int userCom_Size;
	int i = 0;											//While Loop Variable
	userCom_Size = sizeOfPointer(userCom);			//Get Size of User Command

	while(i < userCom_Size){
		*(userInp + i) = *(userCom + i);
		i = i + 1;
	}//end while
}//end copyPtr


/* END OF FILE */

