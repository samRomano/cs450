/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR1.c

	Group Members: Frank Hamilton, Luis Armendariz, Sam Romano
	Version: 1.0
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
	09/15/10  fah   Added display_mpx and support functions
	09/16/10  lca	Compiled Code Together
	09/17/10  fah   Added comment blocks from a lca file
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

/* handler_help: Displays Help Information on a command */
void handler_help_function(char[]);

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
void handler_display_mpx(void);

/* handler_terminate_MPX: Terminate MPX & Return to Host OS */
void handler_terminate_mpx(void);

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

/* printInterrupt: This is an informal interrupt for pagination*/
void printInterrupt();


/******** Parameter List ********/
int version1 = 1;								//Current MPX Version # - Ones Digit
int version2 = 0; 								//Current MPX Version # - Decimal Digit
int comDone = 0;								//Command Handler Loop Indictor - Indicates whether the user is ready to terminate the program.
char *userInput;								//Current User Input
char *userCommand;								//Current UserCommand
int error = 0;									//Variable for Error Handling

int bufSize = 500;								//User Command Buffer Size
char prompt[5] = ":>";							//Current Prompt
int printConstant = 0;							//pagination constant

char *historyList[historySize];					//Command History - Queue
int historyQueue_Head = 0;						//Command History - Queue Head
int historyQueue_Tail = 0;						//Command History - Queue Tail
int historyCounter = 0;							//Command History Counter

/******** Main ********/
/**
	Procedure: main

	Purpose: Initiates mpx OS

	Parameters: None  

	Return value: None

	Calls: sys_init, commandHandler

	Globals: None

	Errors: None
**/
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
	//int fix = 0;
	displayWelcome();							//2.1 Display the Welcome Message

	while(comDone !=1){							//2.2 Begin While Loop for User Commands
		userCommand = NULL;
		//if( fix ==0){
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
			//fix = 1;
		} 
		else if(cmpP2S(userCommand, "get_date") == 1){
			handler_get_date();
		} 
		else if(cmpP2S(userCommand, "display_mpx") == 1){
			handler_display_mpx();
		} 
		//else if(cmpP2S(userCommand, "display_history") == 1){
		//	handler_display_history();
		//}
		else if(cmpP2S(userCommand, "terminate_mpx") == 1||cmpP2S(userCommand, "exit") == 1||cmpP2S(userCommand, "quit") == 1){
			handler_terminate_mpx();
		} 
		else if(cmpP2S(userCommand, "change_prompt") == 1){
			change_prompt();
			//fix = 1;
		} 
		else if(cmpP2S(userCommand, "help_version")==1){
			handler_help_function("version");
		}
		else if(cmpP2S(userCommand, "help_set_date")==1){
			handler_help_function("set_date");
		}
		else if(cmpP2S(userCommand, "help_get_date")==1){
			handler_help_function("get_date");
		}
		else if(cmpP2S(userCommand, "help_display_mpx")==1){
			handler_help_function("display_mpx");
		}
		else if(cmpP2S(userCommand, "help_terminate_mpx")==1){
			handler_help_function("terminate_mpx");
		}
		else if(cmpP2S(userCommand, "help_change_prompt")==1){
			handler_help_function("change_prompt");
		}
		/*else if(cmpP2S(userCommand, "exit")==1){
				break;
			}*/ 
		else {
			printf("Invalid Command.\n");
		}//end if - Decision
		/*}
		else {
			sys_req(READ,TERMINAL,userCommand,&bufSize);
			fix = 0;
		}*/
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
	printf("Welcome to the Functional Fresco mpx OS.\n");
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
	printf("\nThank you for using the Functional Fresco mpx OS.\n Have a nice day! :)\n");
}//end displayClosing

/**
	Procedure: commandCleanup

	Purpose: Free Memory(unused as of version 1.0)

	Parameters: None  

	Return value: None

	Calls: sys_free_mem   

	Globals: error

	Errors: None
**/
void commandCleanup(){
	//int error = sys_free_mem(userCommand); //!NOTE! - Display Errors
	//error = errorCheck(error);
	//printf("The error number is %d.\n", error); //test3------------------------------------------
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
		} else if(error == -124){
			printf("Error(-124): Invalid help_ command.\n");
		} 
		
		return -1;				//Common Error Value
	}//end if
}//end errorCheck

/**
	Procedure: storeHistory

	Purpose: Stores History of User Commands(unused as of version 1.0)

	Parameters: See Prototype  

	Return value: None

	Calls: copyPtr    

	Globals: historyList, historyQueue_Tail, historyQueue_Head, historyCounter

	Errors: None
**/
/*void storeHistory(char *userCom){
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
*/

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
	printf("help\t\tDisplays help on the current acceptable commands.\n");
	printf("help_\t\tDisplays information on a specified function.\n");
	printf("version\t\tDisplays the current mpx OS Version Number.\n");
	printf("set_date\tAllows the user to set the current date.\n");
	printf("get_date\tDisplays the current date(MM/DD/YYYY format).\n");
	printf("display_mpx\tDisplays the available mpx Process Files.\n");
	printf("terminate_mpx\tTerminates the mpx OS.\n");
	//printf("display_history		  Displays a history of previous User Commands.\n");
	printf("change_prompt\tAllows the user to change the prompt symbol.");
	printf("\n\n");
	printf("Type \"help_\" then a command name to access addition information on that command.\n");
	//Add a while loop perhaps with additional command info available via help files?..........................
}//end handler_help

/**
	Procedure: handler_help_function

	Purpose: Displays information on specified command

	Parameters: funName(the command name that more information is desired) 

	Return value: None

	Calls: errorCheck    

	Globals: None

	Errors: Invalid help_ command
**/	
void handler_help_function(char funName[]){
	printf("\n");
	if(strcmp(funName,"version")==0){
		printf("The version command simply displays the current working version number and the date it was finalized on. This command is called by typing \"version\". Possible Error: Invalid help_ command.");
	}
	else if(strcmp(funName,"display_mpx")==0){
		printf("The display_mpx command is used to locate mpx files in a directory. The directory can be specified or the current directory can be selected by pressing enter once the prompt for directory has come up. Then all mpx files will be displayed with name, buffer size used, and file size. This command may be called by typing \"display_mpx\" in the command prompt. Possible Errors: Invalid Directory, Directory Not Open, No More Directory Entries, Read Failed, Name To Long For Buffer, No Directory Is Open.");
	}
	else if(strcmp(funName,"get_date")==0){
		printf("The get_date command grabs the set date and displays it. If there is no set date then the system date is acquired and displayed. This command can be accessed by typing \"get_date\" in the command prompt.");
	}
	else if(strcmp(funName,"set_date")==0){
		printf("The set_date command takes in user input in the form of a string of numbers(MMDDYYYY) and sets that as the current date. The set_date command can be accessed by typing \"set_date\" in the command prompt. Possible Errors: Invalid Date, Date Not Changed.");
	}
	else if(strcmp(funName,"change_prompt")==0){
		printf("The change_prompt command replaces the standard :> prompt with any prompt that is desired and is 4 characters or less. This command is initiated by typing \"change_prompt\"in the command prompt.");
	}
	else if(strcmp(funName,"terminate_mpx")==0){
		printf("The terminate_mpx command exits the mpx os after it confirms with the user that the os is desired to exit. After confirmation allocated memory is cleared and the os terminates. This command may be called by typing either \"quit\",\"exit\", or \"terminate_mpx\"in the command prompt.");
	}
	else{
		errorCheck(-124);
	}
	printf("\n");
}

/**
	Procedure: handler_version

	Purpose: Displays Current MPX Version Number

	Parameters: None

	Return value: 0

	Calls: None    

	Globals: version1, version2

	Errors: None
**/
int handler_version(){

	//Print the current version number
	printf("mpx Version %d.%d\n", version1,version2);
	
	//Print the date current version was completed
	printf("Completed 09/17/2010\n");
	
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

	Return value: 0

	Calls: keyboardInput, sys_set_date, sys_get_date, errorCheck

	Globals: None

	Errors: Invalid Date, Date Not Changed
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
	Buffer = keyboardInput(0);
	

	while(i<8){
		if(Buffer[i] < 48 || Buffer[i] > 57){
			//printf("The date you entered is invalid!");
			flag = 1;
		}
		i++;		
		
	}
	if(flag == 0){
		month = (Buffer[0] - 48)*10 + Buffer[1] - 48;
		day = (Buffer[2] - 48)*10 + Buffer[3] - 48;
		year = (Buffer[4] - 48)*1000 + (Buffer[5] - 48)*100 + (Buffer[6] - 48)*10 + Buffer[7] - 48;
		if(year%4 == 0 && year%100 ==0 && year% 400 == 0){
			leapYear = 1;
		}
		if(month > 12|| day < 1 || month < 1){
			flag = 1;
		}
		if(leapYear==0){
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
		}else{
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
	}
	else{
		errorCheck(-108);
	}
	return 0;
}

/**
	Procedure: handler_display_mpx

	Purpose: Displays Directory of Available MPX Process Files

	Parameters: None  

	Return value: None

	Calls: sizeOfPointer, sys_open_dir, sys_close_dir  

	Globals: error

	Errors: Invalid Directory, Directory Not Open, No More Directory Entries, Read Failed, Name To Long For Buffer, No Directory Is Open.
**/
void handler_display_mpx(){
	char *Buffer= NULL;
	char BufferArray[80] = {0};
	char currentFile[40];
	long j = 0;
	int error, i = 0;
	int nameSize = 40;
	printf("\nWelcome to mpx");
	printf("\nPlease enter the directory to be opened it cannot contain spaces\n");
	Buffer = keyboardInput(0);
	
	while(i<sizeOfPointer(Buffer)){
		//printf("%c",Buffer[i]);
		BufferArray[i] = Buffer[i];
		i++;
	}
	if(sizeOfPointer(Buffer)==0){
		//printf("success");
		getcwd(BufferArray,80);
	}
	error = sys_open_dir(BufferArray);
	errorCheck(error);
	//printf("%s",BufferArray);
	if(error ==0){
		i = 0;
		printConstant =0;
		while(error == 0){
			error = sys_get_entry(currentFile,nameSize,&j);
			if(error == 0){
				i++;
				printf("\nFilename:%s       \tBuffersize:%d\tFile Size:%ld",currentFile,nameSize,j);
				printInterrupt();
			}
			//printf("\n%d",error);
		}
		if(error != -113){
			errorCheck(error);
		}
		else{
			error = sys_close_dir();
			errorCheck(error);
			//printf("\n%d",error);
			if(i == 0){
				if(Buffer[0] == 10){
					printf("The current directory contains no mpx files\n");
				}
				else{
					printf("No mpx files are in that directory\n");
				}
			}
			
		}
	}
	printf("\n");
}

/**
	Procedure: handler_terminate_mpx

	Purpose: Terminate MPX & Return to Host OS

	Parameters: None  

	Return value: None

	Calls: keyboardInput, cmpP2S

	Globals: comDone

	Errors: None
**/
void handler_terminate_mpx(){
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
}//end handler_terminate_mpx



/********* Extra Credit Commands ********/
//Display History of User Commands
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

	Errors: Invalid New Prompt
**/
/*void change_prompt(){
	char *newPrompt;
	int newPromptSize;
	printf("\nEnter New Prompt String(1 or 2 characters)\n");
	newPrompt = keyboardInput(0);
	newPromptSize = sizeOfPointer(newPrompt);
	//printf("%d",newPromptSize);
	if(newPromptSize ==1&& newPrompt[0] > 33){
		prompt[0] = newPrompt[0];
		prompt[1] = 0;
	}
	else if(newPromptSize ==2&& newPrompt[0] > 33&& newPrompt[1] > 33){
		prompt[0] = newPrompt[0];
		prompt[1] = newPrompt[1];
	}
	else{
		errorCheck(-125);
	}
}*/
void change_prompt(){
	char *newPrompt;
	int i= 0, newPromptSize;
	
	printf("\nEnter New Prompt String(Max 4 characters)\n");
	newPrompt = keyboardInput(0);
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
	Procedure: keyboardInput

	Purpose: Gets User Input from Keyboard - Dynamic Buffer Size

	Parameters: zeroAllow(==0 no blanks's allowed in pointer, !=0 blanks's allowed)

	Return value: String of Keyboard Input

	Calls: sys_req

	Globals: bufSize, prompt

	Errors: None
**/
char* keyboardInput (int zeroAllow){
	char *Buffer= NULL;
	char *Buffer2 = NULL;
	
	int WordSize;
	int Temp;
	int flag;
	int i = 0;
	int j = 0;
	if(zeroAllow==0){
	printf("%s",prompt);
	sys_req(READ,TERMINAL,Buffer2,&bufSize);

	while(i<bufSize){
		Temp = Buffer2[i];
		//printf("\n1:j:%d d:%d b:%d",j,Buffer[j],Temp);
		if(Temp==10){
			Buffer[j] = 10;
			break;
		}
		else{
			if(Temp>=33 && Temp <127){
				//printf("\nc:%c b:%d",Buffer[j],Temp);
				Buffer[j] = Temp;
				//printf("\n2:j:%d d:%d b:%d",j,Buffer[j],Temp);
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
				Buffer[j] = 10;
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

	Parameters: Buffer(input from keyboard)
				Word(the string being compared to)

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

	Parameters: Buffer(input from keyboard)  

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

	Parameters: Array(what is being analyzed for size)  

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

	Parameters: userInp(the copy of the pointer)
				userCom(the pointer that is being copied)

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

//Set Date


void printInterrupt(){
	char * Good = NULL;
	printConstant ++;
	if(printConstant==24){
		printf("\n<Press enter to see more files>");
		sys_req(READ,TERMINAL,Good,&bufSize);
		printConstant = 0;
	}
}





