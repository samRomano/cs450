/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR2.c

	Group Members: Frank Hamilton, Luis Armendariz, Sam Romano
	Version: 1.8.1
	Date:  10/8/10

	Purpose: Module R2 file for MPX support

		This file contains the PCB and Linked List structures and handler
		functions needed to support the MPX OS.

	
	
	Procedures in this file:

		commandHandler
			displayWelcome
			
			handler_help
			handler_version
			handler_set_date
			handler_get_date
			handler_display_mpx
			handler_display_history
			handler_terminate_mpx
			change_prompt
			handler_help_function
			
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
			printInterrupt
			

			pointer2Str
			pointer2Int
			allocate_pcb
			free_pcb
			setup_pcb
			find_pcb
			find_ll
			insert_pcb
			remove_pcb
			handler_create_pcb
			handler_delete_pcb
			handler_block
			handler_unblock
			handler_suspend
			handler_resume
			handler_set_priority
			handler_show_pcb
			handler_show
			
************************************************************************/

#include "mpx_supt.h"
/* ANSI C Include Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Logical Constants */
#define historySize 10
#define SYSTEM 1
#define APP 2   

#define RUNNING 3
#define READY 4
#define BLOCKED 5
#define SUSREADY 6
#define SUSBLOCKED 7
#define bufSizeDefined 500
#define STACKSIZE 2048					//In Bytes (Size >= 1024)


/******** Structures ********/
/*Process Control Block*/
struct PCB{

	char pName[20];
	int pClass;
	int priority;
	int state;
	unsigned char* stackTop;
	unsigned char* stackBase;
	
	
	int memSize;
	unsigned char* loadAddress;
	unsigned char* exeAddress;
	
	//queue pointers
	//PCB* next;
	//PCB* previous;
	
};

/*Linked List*/
struct linkedList {
	struct PCB * val;
	struct linkedList * next;
	struct linkedList * prev;
};

/*Type and Structure Definitions*/
typedef struct linkedList LLitem;
typedef struct PCB PCBitem;



/******** Prototypes ********/
 //Module R1 Prototypes
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

/* handler_display_history: Displays User Command History */
//void handler_display_history(void);

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


 //Module R2 Prototypes
/* pointer2Str: Changes a Pointer Variable to a String Variable */
/*	RETURNS: Integer Error Code; Zero If Ok */
int pointer2Str(char *,char*);

/* pointer2Int: Changes a Pointer Variable to an Integer Variable */
/*	RETURNS: Integer Error Code; Zero If Ok */
int pointer2Int(char *);

/* next_ll: Sets the Current Linked List Value to the Next Linked List Value */
/*	RETURNS: Integer Error Code; Zero If Ok */
int next_ll(LLitem * curr);

/* allocate_pcb: Allocates Memory for a new PCB structure */
/*	RETURNS: Pointer to the new PCB structure */
PCBitem * allocate_pcb();

/* free_pcb: Frees the Memory Allocated to a PCB structure */
/*	RETURNS: Integer Error Code; Zero If Ok */
int free_pcb(PCBitem *);

/* setup_pcb: Creates and Initializes a new PCB Structure */
/*	RETURNS: Pointer to the new PCB structure */
PCBitem * setup_pcb(char [], int , int );

/* find_pcb: Locates a PCB Structure */
/*	RETURNS: Pointer to the located PCB structure */
PCBitem * find_pcb(char []);

/* find_ll: Locates a Linked List Structure */
/*	RETURNS: Pointer to the located Linked List structure */
LLitem* find_ll(char []);

/* insert_pcb: Inserts a PCB Structure into a chosen Queue */
/*	RETURNS: Integer Error Code; Zero If Ok */
int insert_pcb(int,PCBitem*);

/* remove_pcb: Removes a PCB Structure from its set Queue */
/*	RETURNS: Integer Error Code; Zero If Ok */
int remove_pcb(PCBitem*);

/* handler_delete_pcb: Deletes a defined PCB Structure from its assigned Queue*/
void handler_delete_pcb();

/* handler_create_pcb: Creates a new PCB Structure and inserts it into the Ready Queue*/
void handler_create_pcb();

/* handler_block: Blocks a defined PCB Structure */
void handler_block();

/* handler_unblock: Unblocks a blocked PCB Structure */
void handler_unblock();

/* handler_suspend: Suspends a defined PCB Structure */
void handler_suspend();

/* handler_resume: Resumes a suspended PCB Structure */
void handler_resume();

/* handler_set_priority: Sets a new priority for a defined PCB Structure */
void handler_set_priority();

/* handler_show_pcb: Shows all of the stored information for a defined PCB Structure */
void handler_show_pcb();

/* handler_show: Shows all of the stored information for each PCB Structure in one or all queues */
void handler_show(int);





/******** Parameter List ********/
int version1 = 2;								//Current MPX Version # - Ones Digit
int version2 = 0; 								//Current MPX Version # - Decimal Digit
int comDone = 0;								//Command Handler Loop Indictor - Indicates whether the user is ready to terminate the program.
char *userInput;								//Current User Input
char *userCommand;								//Current UserCommand
int error = 0;									//Variable for Error Handling

int bufSize = bufSizeDefined;								//User Command Buffer Size
char prompt[5] = ":>";							//Current Prompt
int printConstant = 0;							//pagination constant
char bufArray[500] = {0};
char *historyList[historySize];					//Command History - Queue
int historyQueue_Head = 0;						//Command History - Queue Head
int historyQueue_Tail = 0;						//Command History - Queue Tail
int historyCounter = 0;							//Command History Counter

LLitem * tail4 = NULL,  *head4 = NULL,  *curr4 = NULL;
LLitem * tail5 = NULL,  *head5 = NULL,  *curr5 = NULL;
LLitem * tail6 = NULL,  *head6 = NULL,  *curr6 = NULL;
LLitem * tail7 = NULL,  *head7 = NULL,  *curr7 = NULL;




/******** Main ********/
/**
	Procedure: main

	Purpose: Initiates MPX OS

	Parameters: None  

	Return value: None

	Calls: sys_init, commandHandler

	Globals: None

	Errors: None
**/
void main(){
	sys_init(MODULE_R2);			    		//1. Initialize
	commandHandler();				    		//2. Call commandHandler function
}//end main



/******** Command Handler ********/
/**
	Procedure: commandHandler

	Purpose: Handles User Commands & Command Execution

	Parameters: None  

	Return value: None

	Calls: displayWelcome, keyboardInput, cmpP2S, handler_help, handler_version, handler_set_date, handler_get_date, handler_display_mpx, 
		handler_display_history, handler_terminate_mpx, change_prompt, handler_help_function, handler_create_pcb, handler_delete_pcb, 
		handler_block, handler_unblock, handler_suspend, handler_resume, handler_set_priority, handler_show_pcb, handler_show, displayClosing, 
		commandCleanup, sys_exit

	Globals: comDone, userCommand

	Errors: None
**/
void commandHandler(){
	int userCommandSize;
	displayWelcome();							//2.1 Display the Welcome Message

	while(comDone !=1){							//2.2 Begin While Loop for User Commands
		userCommand = NULL;
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
		else if(cmpP2S(userCommand, "help_create_pcb")==1){
			handler_help_function("create_pcb");
		}
		else if(cmpP2S(userCommand, "create_pcb")==1){
			handler_create_pcb();
		}
		else if(cmpP2S(userCommand, "help_delete_pcb")==1){
			handler_help_function("delete_pcb");
		}
		else if(cmpP2S(userCommand, "delete_pcb")==1){
			handler_delete_pcb();
		}
		else if(cmpP2S(userCommand, "help_block")==1){
			handler_help_function("block");
		}
		else if(cmpP2S(userCommand, "block")==1){
			handler_block();
		}
		else if(cmpP2S(userCommand, "help_unblock")==1){
			handler_help_function("unblock");
		}
		else if(cmpP2S(userCommand, "unblock")==1){
			handler_unblock();
		}
		else if(cmpP2S(userCommand, "help_suspend")==1){
			handler_help_function("suspend");
		}
		else if(cmpP2S(userCommand, "suspend")==1){
			handler_suspend();
		}
		else if(cmpP2S(userCommand, "help_resume")==1){
			handler_help_function("resume");
		}
		else if(cmpP2S(userCommand, "resume")==1){
			handler_resume();
		}
		else if(cmpP2S(userCommand, "help_set_priority")==1){
			handler_help_function("set_priority");
		}
		else if(cmpP2S(userCommand, "set_priority")==1){
			handler_set_priority();
		}
		else if(cmpP2S(userCommand, "help_show_pcb")==1){
			handler_help_function("show_pcb");
		}
		else if(cmpP2S(userCommand, "show_pcb")==1){
			handler_show_pcb();
		}
		else if(cmpP2S(userCommand, "help_show_all")==1){
			handler_help_function("show_all");
		}
		else if(cmpP2S(userCommand, "show_all")==1){
			handler_show(0);
		}
		else if(cmpP2S(userCommand, "help_show_ready")==1){
			handler_help_function("show_ready");
		}
		else if(cmpP2S(userCommand, "show_ready")==1){
			handler_show(4);
			handler_show(6);
		}
		else if(cmpP2S(userCommand, "help_show_blocked")==1){
			handler_help_function("show_blocked");
		}
		else if(cmpP2S(userCommand, "show_blocked")==1){
			handler_show(5);
			handler_show(7);
		}
		else {
			printf("Invalid Command.\n");
		}//end if - Decision
		
		
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
	printf("\n\nWelcome to the Functional Fresco mpx OS.\n");
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

	Purpose: Free Memory that is still allocated

	Parameters: None  

	Return value: None

	Calls: sys_free_mem, errorCheck  

	Globals: error

	Errors: None
**/
void commandCleanup(){
	//int error = sys_free_mem(userCommand); //!NOTE! - Display Errors
	//error = errorCheck(error);
	//printf("The error number is %d.\n", error); //test3------------------------------------------
	LLitem * tempCurr;
	int queueInt= 0;
	if(queueInt == 0 || queueInt == 4){
		tempCurr = head4;
		if(tempCurr != NULL){
			do{
				error = remove_pcb(tempCurr->val);
				if(error !=0){
					break;
				}
				else{
					error = free_pcb(tempCurr->val);
					if(error != 0){
						break;
					}
					else{
						tempCurr = tempCurr->next;	
					}
				}
			}while(tempCurr != head4 && tempCurr != NULL);
		}
	}
	if(queueInt == 0 || queueInt == 5){
		tempCurr = head5;
		if(tempCurr != NULL){
			do{
				error = remove_pcb(tempCurr->val);
				if(error !=0){
					break;
				}
				else{
					error = free_pcb(tempCurr->val);
					if(error != 0){
						break;
					}
					else{
						tempCurr = tempCurr->next;	
					}
				}
			}while(tempCurr != head5&& tempCurr != NULL);
		}
	}
	if(queueInt == 0 || queueInt == 6){
		tempCurr = head6;
		if(tempCurr != NULL){
			do{
				error = remove_pcb(tempCurr->val);
				if(error !=0){
					break;
				}
				else{
					error = free_pcb(tempCurr->val);
					if(error != 0){
						break;
					}
					else{
						tempCurr = tempCurr->next;	
					}
				}
			}while(tempCurr != head6&& tempCurr != NULL);
		}
	}
	if(queueInt == 0 || queueInt == 7){
		tempCurr = head7;
		if(tempCurr != NULL){
			do{
				error = remove_pcb(tempCurr->val);
				if(error !=0){
					break;
				}
				else{
					error = free_pcb(tempCurr->val);
					if(error != 0){
						break;
					}
					else{
						tempCurr = tempCurr->next;	
					}
				}
			}while(tempCurr != head7&& tempCurr != NULL);
		}
	}
}//end commandCleanup

/**
	Procedure: errorCheck

	Purpose: Displays Error Messages

	Parameters: See Prototype  

	Return value: Error Code; Zero If Ok

	Calls: None    

	Globals: error

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
		 else if(error == -125){
			printf("Error(-125): Invalid input length.\n");
		} 
		else if(error == -126){
			printf("Error(-126): PCB Name already exists.\n");
		} 
		else if(error == -127){
			printf("Error(-127): Invalid PCB Priority\n");
		} 
		else if(error == -128){
			printf("Error(-128): Invalid PCB Class\n");
		} 	
		else if(error == -129){
			printf("Error(-129): Next Linked List Node is NULL.\n");
		} 
		else if(error == -130){
			printf("Error(-130): Insert with Invalid State.\n");
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
	printf("help\t\tDisplays help on the current acceptable commands.\n");				//Beginning of Module R1 Commands
	printf("help_\t\tDisplays information on a specified function.\n");
	printf("version\t\tDisplays the current mpx OS Version Number.\n");
	printf("set_date\tAllows the user to set the current date.\n");
	printf("get_date\tDisplays the current date(MM/DD/YYYY format).\n");
	printf("display_mpx\tDisplays the available mpx Process Files.\n");
	printf("terminate_mpx\tTerminates the mpx OS.\n");
	//printf("display_history		  Displays a history of previous User Commands.\n");
	printf("change_prompt\tAllows the user to change the prompt symbol.\n");
	
	printf("create_pcb\tAllows the user to create a PCB structure.\n");					//Beginning of Module R2 Commands
	printf("delete_pcb\tAllows the user to delete a PCB structure.\n");
	printf("block\tAllows the user to block a PCB structure.\n");
	printf("unblock\tAllows the user to unblock a PCB structure.\n");
	printf("suspend\tAllows the user to suspend a PCB structure.\n");
	printf("resume\tAllows the user to resume a PCB structure.\n");
	printf("set_priority\tAllows the user to set the priority of a PCB structure.\n");
	printf("show_pcb\tAllows the user to show all the information in a PCB structure.\n");
	printf("show_all\tAllows the user to show all the inforation for all PCB structures in every queue.\n");
	printf("show_ready\tAllows the user to show all information for the PCB structures in the Ready queues.\n");
	printf("show_blocked\tAllows the user to show all information for the PCB structures in the Blocked queues.\n");
	
	printf("\n");
	printf("Type \"help_\" then a command name to access addition information on that command.\n");
	//Add a while loop perhaps with additional command info available via help files?..........................
}//end handler_help


/**
	Procedure: handler_help_function

	Purpose: Displays information on specified command

	Parameters: funName(The command name that more information is desired on.) 

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
	else if(strcmp(funName,"create_pcb")==0){
		printf("The create_pcb command initializes a new PCB structure. After the command is entered, the user is prompted to input in a unique PCB name, the PCB class, and the PCB priority level. This command is called by typing \"create_pcb\" in the command prompt.");
	}
	else if(strcmp(funName,"delete_pcb")==0){
		printf("The delete_pcb command deletes a defined PCB structure. Initially, the user is prompted to input the name of a current PCB structure. If the PCB exists, it is removed from its current queue, and its assigned memory is deallocated. This command is called by typing \"delete_pcb\" in the command prompt.");
	}
	else if(strcmp(funName,"block")==0){
		printf("The block command blocks a defined PCB structure. Initially, the user is prompted to input the name of a current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"block\" in the command prompt.");
	}
	else if(strcmp(funName,"unblock")==0){
		printf("The unblock command unblocks a blocked PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"unblock\" in the command prompt.");
	}
	else if(strcmp(funName,"suspend")==0){
		printf("The suspend command suspends a defined PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"suspend\" in the command prompt.");
	}
	else if(strcmp(funName,"resume")==0){
		printf("The resume command suspends a defined PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"resume\" in the command prompt.");
	}
	else if(strcmp(funName,"set_priority")==0){
		printf("The set_priority command changes the priority of a defined PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the user is prompted to enter in the new PCB priority. If the new priority is valid, the PCB priority is updated, and the PCB position is adjusted, if the PCB is in the Ready queue. This command is called by typing \"set_priority\" in the command prompt.");
	}
	else if(strcmp(funName,"show_pcb")==0){
		printf("The show_pcb command shows all the stored information within a PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB Name, Class, Priority, State, and Memory Size is displayed. This command is called by typing \"show_pcb\" in the command prompt.");
	}
	else if(strcmp(funName,"show_all")==0){
		printf("The show_all command shows all the stored information for all PCB structures. This command is initiated by typing \"show_all\"in the command prompt.");
	}
	else if(strcmp(funName,"show_ready")==0){
		printf("The show_ready command shows all the stored information for each PCB structure in the Ready queues. This command is initiated by typing \"show_ready\"in the command prompt.");
	}
	else if(strcmp(funName,"show_blocked")==0){
		printf("The show_blocked command shows all the stored information for each PCB structure in the Blocked queues. This command is initiated by typing \"show_blocked\"in the command prompt.");
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
	printf("Completed 10/8/2010\n");
	
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

	Calls: sizeOfPointer, sys_open_dir, sys_close_dir, sys_get_entry

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
/**
	Procedure: handler_display_history

	Purpose: Display History of Last 10 User Commands

	Parameters: None  

	Return value: None

	Calls: None

	Globals: historyQueue_Head, historyQueue_Tail, historyList

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

	Globals: prompt

	Errors: Invalid New Prompt
**/
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

/********* Module R2 Commands ********/
/**
	Procedure: printInterrupt

	Purpose: Paginates large outputs

	Parameters: None

	Return value: None

	Calls: sys_req

	Globals: bufSize,printConstant

	Errors: None
**/
void printInterrupt(){
	char * Good = NULL;
	printConstant ++;
	if(printConstant==24){
		printf("\n<Press enter to see more files>");
		sys_req(READ,TERMINAL,Good,&bufSize);
		printConstant = 0;
	}
}

/**
	Procedure: pointer2Str

	Purpose: Changes a Pointer Variable to a String Variable

	Parameters: Buffer, inputStr

	Return value: Integer Error Code; Zero If Ok

	Calls: sizeOfPointer

	Globals: bufSize

	Errors: Invalid Pointer Value, Invalid String Value
**/
int pointer2Str(char * Buffer,char * inputStr){
	int BufferSize;
	int i = 0;
	int j = 0;
	//inputStr = NULL;
	
	BufferSize = sizeOfPointer(Buffer);
	//printf("\n\n%d",WordSize);
	if(BufferSize ==0){
		errorCheck(-125);
		return -1;
	}
	else{
		while(i<BufferSize && Buffer[i] != 10){
			inputStr[i] = Buffer[i];
			i++;
		}
		//inputStr[i] = 10;
		while(j<BufferSize-i){
			inputStr[j] = 0;
		}
		return 0;
	}	
}

/**
	Procedure: pointer2Int

	Purpose: Changes a Pointer Variable to an Integer Variable

	Parameters: Buffer

	Return value: Integer Value

	Calls: sizeOfPointer

	Globals: None

	Errors: Invalid Pointer Value
**/
int pointer2Int(char * Buffer){
	int BufferSize;
	int i = 0;
	int inputInt = 0;
	int j = 0;
	int powInt = 1;
	BufferSize = sizeOfPointer(Buffer);
	
	
	if(BufferSize ==0){
		errorCheck(-125);
		return -9999;
	}
	else{
		while(i<BufferSize && Buffer[i] != 10){
			j = 0;
			powInt = 1;
			while(j<BufferSize-1-i){
				powInt = powInt*10;
				j++;
			}
			inputInt = inputInt + (Buffer[i]-48)*powInt;
			i++;
		}
		return inputInt;
	}	
}
//			}

/**
	Procedure: next_ll

	Purpose: Sets current Linked List structure to next Linked List structure

	Parameters: curr

	Return value: Integer Error Code; Zero If Ok

	Calls: None

	Globals: None

	Errors: -129
**/
int next_ll(LLitem * curr){
	if(curr->next != NULL){
		curr = curr->next;
		return 0;
	}
	return -129;
}

/**
	Procedure: allocate_pcb

	Purpose: Allocates Memory for a new PCB structure

	Parameters: None

	Return value: PCB Pointer Value

	Calls: sys_alloc_mem

	Globals: STACKSIZE

	Errors: None
**/
PCBitem * allocate_pcb(){
	PCBitem * newPCB = NULL;											 //Creates New PCB Item & Initializes to NULL for Error Checking
	newPCB =(PCBitem *) sys_alloc_mem(sizeof(PCBitem));   			 //Allocates Memory for New PCB Item
	//PCBitem *ptrPCB;										         //Creates New PCB Pointer
	//*ptrPCB = newPCB;												 //Assigns PCB to PCB Pointer
	
	newPCB->stackBase = sys_alloc_mem(STACKSIZE);					 //Allocates Memory for Stack (Looks at First Location)
	newPCB->stackTop = (newPCB->stackBase) + STACKSIZE;				 //Sets Stack Top to top of Stack
	
	return newPCB;													 //Returns PCB for PCB Pointer
}//end allocate_PCB

/**
	Procedure: free_pcb

	Purpose: Frees the Memory Allocated to a PCB structure
	
	Parameters: newPCB

	Return value: Integer Error Code; Zero If Ok

	Calls: sys_free_mem, errorCheck

	Globals: None

	Errors: Invalid PCB Pointer Value
**/
int free_pcb(PCBitem *newPCB){
	error = sys_free_mem(newPCB->stackBase);					 //Need to free Stack (a.k.a. Memory associated with base pointer)
	error = errorCheck(error);

	error = sys_free_mem(newPCB);							 	 //Frees Memory Allocated to PCB
	error = errorCheck(error);
	
	

	return error;												 //Returns any error code found
}//end free_PCBULL

/**
	Procedure: setup_pcb

	Purpose: Creates and Initializes a new PCB Structure

	Parameters: new_pName, new_pPriority, new_pClass

	Return value: PCB Pointer Value

	Calls: find_pcb, sizeOfArray, allocate_pcb

	Globals: READY

	Errors: Invalid PCB Name, Invalid PCB Priority Value, Invalid PCB Class, -126 through -128
**/
PCBitem *setup_pcb(char new_pName[], int new_pPriority, int new_pClass){
	int charCount = 0;												//Number of Characters in Process Name
	PCBitem *tempPCB;												//Temporary PCB Item
	PCBitem *newPCB;												//Pointer to New PCB Item
	char store_pName[20];											// Initializes a PCB�s content (name, priority, class)
	
	
	strncpy(store_pName, new_pName, 20);
	
	tempPCB = find_pcb(new_pName);									// Check that Name is unique (1. Check to see if name already exists)
	charCount = (sizeOfArray(new_pName));						// Check that Name is unique (2. Check to see if at least 8 chars. + Null Terminator)
	if((tempPCB != NULL) || (charCount < 8)){
		errorCheck(-126);
		return NULL;
	}//end if
	
	else if((new_pPriority > 127) || (new_pPriority < -128)){			// Check that Priority between -128 and +127
		errorCheck(-127);
		return NULL;
		
		
	}//end if
	
	else if((new_pClass > 2) || (new_pClass < 1)){						// Check that Class is valid
		errorCheck(-128);
		return NULL;
	}//end if
	else{
	newPCB = allocate_pcb();										// Calls the Allocate_PCB function to allocate the memory for a new PCB
	
	strncpy(newPCB->pName,store_pName,20);							// Sets the name, priority, and class based on the parameters
	newPCB->pClass = new_pClass;
	newPCB->priority = new_pPriority;
	
	newPCB->state = READY;											// Sets state to ready, not suspended (this will change in later modules)
	
	newPCB->memSize = NULL;											// Sets remaining fields to defaults (Memory Descriptors)
	newPCB->loadAddress = NULL;
	newPCB->exeAddress = NULL;
	
	// Does not insert the PCB into a queue
	return newPCB;													// Returns PCB Pointer if successful, NULL if not successful (including if one of the parameters are not valid)
	}
}//end setup_PCB

/**
	Procedure: find_pcb

	Purpose: Locates a PCB Structure

	Parameters: name

	Return value: PCB Pointer Value

	Calls: next_ll

	Globals: curr4, head4, curr5, head5, curr6, head6, curr7, head7, error

	Errors: Invalid PCB Name
**/
PCBitem* find_pcb(char name []){
	
	curr4 = head4;
	
	do{ 
		
		if(strcmp(curr4->val->pName ,name)==0){
			
			return curr4->val;
		}
		
		error = next_ll(curr4);
	}while(curr4 != head4 && error == 0);

	curr5 = head5;
	do{ 
		if(strcmp(curr5->val->pName ,name)==0){
			return curr5->val;
		}
		
		error = next_ll(curr5);	
	}while(curr5 != head5 && error == 0);
	
	curr6 = head6;
	do{ 
		if(strcmp(curr6->val->pName ,name)==0){
			return curr6->val;
		}
		
		error = next_ll(curr6);	
	}while(curr6 != head6 && error == 0);
	
	curr7 = head7;
	do{ 
		if(strcmp(curr7->val->pName ,name)==0){
			return curr7->val;
		}
		
		error = next_ll(curr7);	
	}while(curr7 != head7 && error == 0);
	
	return NULL;
	
	
}//end find_PCB

/**
	Procedure: find_ll

	Purpose: Locates a Linked List Structure

	Parameters: name

	Return value: Linked List Pointer Value

	Calls: next_ll

	Globals: curr4, head4, curr5, head5, curr6, head6, curr7, head7

	Errors: Invalid PCB Name
**/
LLitem* find_ll(char name []){
	
	curr4 = head4;
	
	do{ 
		
		if(strcmp(curr4->val->pName ,name)==0){
			
			return curr4;
		}
		
		error = next_ll(curr4);
	}while(curr4 != head4 && error == 0);

	curr5 = head5;
	do{ 
		if(strcmp(curr5->val->pName ,name)==0){
			return curr5;
		}
		
		error = next_ll(curr5);	
	}while(curr5 != head5 && error == 0);
	
	curr6 = head6;
	do{ 
		if(strcmp(curr6->val->pName ,name)==0){
			return curr6;
		}
		
		error = next_ll(curr6);	
	}while(curr6 != head6 && error == 0);
	
	curr7 = head7;
	do{ 
		if(strcmp(curr7->val->pName ,name)==0){
			return curr7;
		}
		
		error = next_ll(curr7);	
	}while(curr7 != head7 && error == 0);
	
	return NULL;
	
	
}//end find_PCB

/**
	Procedure: insert_pcb

	Purpose: Inserts a PCB Structure into a chosen Queue

	Parameters: queueInt, PCBtemp

	Return value: Integer Error Code; Zero If Ok

	Calls: next_ll, sys_alloc_mem

	Globals: curr4, head4, tail4, curr5, head5, tail5, curr6, head6, tail6, curr7, head7, tail7, READY, BLOCKED, SUSREADY, SUSBLOCKED

	Errors: Invalid Integer Queue Value, Invalid PCB Pointer Value, -130
**/
int insert_pcb(int queueInt,PCBitem * PCBtemp){
	LLitem * tempHead, *tempTail, *tempCurr,*temp;
	if(queueInt == READY){
		tempHead = head4;
		tempTail = tail4;
		tempCurr = curr4;
	}else if(queueInt == BLOCKED){
		tempHead = head5;
		tempTail = tail5;
		tempCurr = curr5;
	}else if(queueInt == SUSREADY){
		tempHead = head6;
		tempTail = tail6;
		tempCurr = curr6;
	}else if(queueInt == SUSBLOCKED){
		tempHead = head7;
		tempTail = tail7;
		tempCurr = curr7;
	}
	else{
		errorCheck(-130);
		return -1;
	}
	
	if(queueInt ==5 || queueInt == 7){
		temp = (LLitem *)sys_alloc_mem(sizeof(LLitem));
		if(tempHead == NULL){
			temp->next = NULL;				
			temp->prev = NULL;
			temp->val = PCBtemp;
			tempHead = temp;
		}
		else if(tempTail == NULL){
			temp->val = PCBtemp;
			tempHead->next = temp;
			tempHead->prev = temp;
			temp->next = tempHead;
			temp->prev = tempHead;
			tempTail = temp;
		}
		else{
			temp->val = PCBtemp;
			temp->prev  = tempTail;
			temp->next = tempHead;
			tempTail->next  = temp;
			tempHead->prev = temp;
			tempTail = temp;
		}
	}
	else if (queueInt ==6 || queueInt == 4){
		temp = (LLitem *)sys_alloc_mem(sizeof(LLitem));
		if(tempHead == NULL){
			temp->next = NULL;
			temp->prev = NULL;
			temp->val = PCBtemp;
			tempHead = temp;
		}
		else if(PCBtemp->priority <= tempHead->val->priority && tempTail == NULL){
			temp->next = tempHead;
			temp->prev = tempHead;
			temp->val = PCBtemp;
			tempHead->prev = temp;
			tempHead->next = temp;
			tempTail = temp;
		}
		else if(PCBtemp->priority > tempHead->val->priority && tempTail == NULL){
			temp->next = tempHead;
			temp->prev = tempHead;
			temp->val = PCBtemp;
			tempHead->prev = temp;
			tempHead->next = temp;
			tempTail = tempHead;
			tempHead = temp;
		}
		else if(PCBtemp->priority > tempHead->val->priority){
			temp->next = tempHead;
			temp->prev = tempTail;
			temp->val = PCBtemp;
			tempHead->prev = temp;
			tempTail->next = temp;
			tempHead = temp;
		}
		
		else if(PCBtemp->priority <= tempTail->val->priority){
			temp->val = PCBtemp;
			temp->prev  = tempTail;
			temp->next = tempHead;
			tempTail->next  = temp;
			tempHead->prev = temp;
			tempTail = temp;
		}
		else{
			tempCurr = tempHead;
			next_ll(tempCurr);
			while(tempCurr != tempTail){
				if(PCBtemp->priority > tempCurr->val->priority){
					temp = tempCurr;
					temp->prev = tempCurr->prev;
					temp->next = tempCurr;
					temp->val = PCBtemp;
					tempCurr->prev = temp;
					break;
				}
				next_ll(tempCurr);
			}		
		}	
	}
	if(queueInt == READY){
		head4 = tempHead;
		tail4 =  tempTail;
		curr4 = tempCurr;
	}else if(queueInt == BLOCKED){
		head5 = tempHead;
		tail5 =  tempTail;
		curr5 = tempCurr;
	}else if(queueInt == SUSREADY){
		head6 = tempHead;
		tail6 =  tempTail;
		curr6 = tempCurr;
	}else if(queueInt == SUSBLOCKED){
		head7 = tempHead;
		tail7 =  tempTail;
		curr7 = tempCurr;
	}
	else{
		errorCheck(-130);
		return -1;
	}
	return 0;
}

/**
	Procedure: remove_pcb

	Purpose: Removes a PCB Structure from its set Queue

	Parameters: PCBtemp

	Return value: Integer Error Code; Zero If Ok

	Calls: find_ll, sys_free_mem

	Globals: curr4, head4, tail4, curr5, head5, tail5, curr6, head6, tail6, curr7, head7, tail7, READY, BLOCKED, SUSREADY, SUSBLOCKED

	Errors: Invalid PCB Pointer Value
**/
int remove_pcb(PCBitem * PCBtemp){
	LLitem * tempHead, *tempTail, *tempCurr;
	LLitem *tempP,*tempN;
	if(PCBtemp->state == READY){
		tempHead = head4;
		tempTail = tail4;
		tempCurr = curr4;
	}else if(PCBtemp->state == BLOCKED){
		tempHead = head5;
		tempTail = tail5;
		tempCurr = curr5;
	}else if(PCBtemp->state == SUSREADY){
		tempHead = head6;
		tempTail = tail6;
		tempCurr = curr6;
	}else if(PCBtemp->state == SUSBLOCKED){
		tempHead = head7;
		tempTail = tail7;
		tempCurr = curr7;
	}
	tempCurr = find_ll(PCBtemp->pName);
	if(tempTail == tempHead && tempHead != NULL){
		sys_free_mem(tempCurr);
		tempHead = NULL;
		tempTail = NULL;
	}
	else if(tempHead == NULL){
		//printf("Error no items\n");
		return -1;
	}
	else{
		tempP = tempCurr->prev;
		tempN = tempCurr->next;
		if(tempCurr == tempHead){
			tempHead = tempN ;
			tempHead->prev = tempTail;
			tempTail->next = tempHead;
		}
		else if(tempCurr == tempTail){
			tempTail = tempP;
			tempHead->prev = tempTail;
			tempTail->next = tempHead;
		}
		else{
			tempP->next = tempN;
			tempN->prev = tempP;
		}		
		
		sys_free_mem(tempCurr);
		tempCurr = tempN;
	}
	if(PCBtemp->state == READY){
		head4 = tempHead;
		tail4 =  tempTail;
		curr4 = tempCurr;
	}else if(PCBtemp->state == BLOCKED){
		head5 = tempHead;
		tail5 =  tempTail;
		curr5 = tempCurr;
	}else if(PCBtemp->state == SUSREADY){
		head6 = tempHead;
		tail6 =  tempTail;
		curr6 = tempCurr;
	}else if(PCBtemp->state == SUSBLOCKED){
		head7 = tempHead;
		tail7 =  tempTail;
		curr7 = tempCurr;
	}else if (PCBtemp == NULL){}
	else{
		printf("ERROR");
		return -1;
	}
	return 0;
}

/**
	Procedure: handler_create_pcb

	Purpose: Creates a new PCB Structure and inserts it into the Ready Queue

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, pointer2Int, sizeOfArray, find_pcb, setup_pcb, insert_pcb

	Globals: READY

	Errors: -125 through -128
**/
void handler_create_pcb(){
	PCBitem * temp;
	char * Buffer = NULL;
	char inputName[bufSizeDefined] = {0};
	int inputClass = 0;
	int inputPriority = 0;
	printf("Enter the PCB name(max 20 characters): \n");
	Buffer = keyboardInput(0);
	pointer2Str(Buffer,inputName);
	printf("Enter the PCB class value(1 through 2): \n");
	Buffer = keyboardInput(0);
	inputClass =pointer2Int(Buffer);
	printf("Please enter a number between -128 & +127 for Priority:\n");
	Buffer = keyboardInput(0);
	inputPriority =pointer2Int(Buffer);
	
	
	if(sizeOfArray(inputName) < 8){
		error = errorCheck(-125);
	}//end if
	else if(sizeOfArray(inputName) >20){
		error = errorCheck(-125);
	}
	else if(find_pcb(inputName) != NULL){
		error = errorCheck(-126);
	}
	else if((inputPriority > 127) || (inputPriority < -128)){			// Check that Priority between -128 and +127
		error = errorCheck(-127);
	}//end if
	
	else if((inputClass > 2) || (inputClass < 1)){						// Check that Class is valid
		error = errorCheck(-128);
	}//end if
	else if(error ==0){
	temp = setup_pcb(inputName,inputPriority,inputClass);
	error = insert_pcb(READY, temp);
	}
	
}

/**
	Procedure: handler_delete_pcb

	Purpose: Deletes a defined PCB Structure from its assigned Queue

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, free_pcb

	Globals: bufSizeDefined

	Errors: -125
**/
void handler_delete_pcb(){
	PCBitem * temp = NULL;
	char * Buffer = NULL;
	char inputName[bufSizeDefined] = {0};
	printf("Enter the PCB name(max 20 characters): \n");
	Buffer = keyboardInput(0);
	pointer2Str(Buffer,inputName);
	if(inputName== NULL){
		errorCheck(-125);
		
	}
	else if(sizeOfArray(inputName)>20){
		errorCheck(-125);
	}
	else if(sizeOfArray(inputName)<8){
		errorCheck(-125);
	}
	else{
		temp = find_pcb(inputName);					
		if(temp == NULL){
			printf("PCB Name is Invalid. PCB Name doesn't exist.\n");
		}
		else{
			error = remove_pcb(temp);
			if(error == 0){
				error = free_pcb(temp);
				if(error == 0){
				printf("\n Successfully Deleted: %s",inputName);
				}
				else{
					printf("\n Failed to Delete: %s",inputName);
				}
			}
			else{
				printf("\n Failed to Delete: %s",inputName);
			}
		}
	}
}

/**
	Procedure: handler_block

	Purpose: Blocks a defined PCB Structure

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, errorCheck, insert_pcb

	Globals: bufSizeDefined, BLOCKED, SUSBLOCKED, RUNNING, READY, SUSREADY, error

	Errors: None
**/
void handler_block(){
	char tempBuff[bufSizeDefined] = {0};												//Temporary Input Buffer
	char *tempPtr = NULL;												//Temporary Input Character Pointer for PCB Name
	PCBitem *tempPCB;													//Temporary PCB Item
	int charCount = 0;													//Number of Characters in Process Name

	// Get PCB name from the user- check that the PCB with that name exists (Make sure that name is valid & exists!)
	printf("Please enter the name of the PCB to block(max 20, min 8 characters):\n");
	tempPtr = keyboardInput(0);											//Gets process name from User
	pointer2Str(tempPtr,tempBuff);										//Convert Character Pointer to Character String
	charCount = sizeOfArray(tempBuff);									//Get Name Size
	
	if((charCount >= 8 && charCount <=20)){												//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(tempBuff);
		if(tempPCB != NULL){											//Check if Name Exists
			if(tempPCB->state == BLOCKED){
				printf("Error: The process is already Blocked.\n");
			} else if(tempPCB->state == SUSBLOCKED){
				printf("Error: The process is already Suspended & Blocked.\n");
			} else{
				
				// Should remove the process from the ready queue (by calling remove_PCB)
				error = remove_pcb(tempPCB);
				error = errorCheck(error);
				
				// Places the process (PCB) in the blocked state
				// Does not change its suspended status
				
				if(tempPCB->state == RUNNING){
					tempPCB->state = BLOCKED;
				} else if(tempPCB->state == READY){
					tempPCB->state = BLOCKED;
				} else if(tempPCB->state == SUSREADY){
					tempPCB->state = SUSBLOCKED;
				}//end if
				
				// And place the process in the blocked queue (by calling insert_PCB)
				error = insert_pcb(tempPCB->state, tempPCB);
				error = errorCheck(error);
				
				// Display appropriate error or success message
				if(error == 0){
					printf("The PCB Process has been successfully blocked.\n");
				}//end if
				
			}//end if
		} else{
			printf("Process Name does not exist.\n");
		}//end if
	} else{
		printf("Process Name is Invalid. Process Name is not between 8 and 20 characters.\n");
	}//end if
}//end block

/**
	Procedure: handler_unblock

	Purpose: Unblocks a blocked PCB Structure

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, errorCheck, insert_pcb

	Globals: bufSizeDefined, BLOCKED, SUSBLOCKED, RUNNING, READY, SUSREADY, error

	Errors: None
**/
void handler_unblock(){
	char tempBuff[bufSizeDefined] = {0};												//Temporary Input Buffer
	char *tempPtr = NULL;												//Temporary Input Character Pointer for PCB Name
	PCBitem *tempPCB;													//Temporary PCB Item
	int charCount = 0;													//Number of Characters in Process Name

	// Get PCB name from the user- check that the PCB with that name exists (Make sure that name is valid & exists!)
	printf("Please enter the name of the PCB to unblock(max 20, min 8 characters):\n");
	tempPtr = keyboardInput(0);											//Gets process name from User
	pointer2Str(tempPtr,tempBuff);										//Convert Character Pointer to Character String
	charCount = sizeOfArray(tempBuff);									//Get Name Size
	
	if((charCount >= 8 && charCount <=20)){												//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(tempBuff);
	if(tempPCB != NULL){											//Check if Name Exists
		if(tempPCB->state == READY){
			printf("Error: The process is ready and already unblocked.\n");
		}else if(tempPCB->state == SUSREADY){
			printf("Error: The process is suspended and already unblocked.\n");
		}else if(tempPCB->state == RUNNING){
			printf("Error: The process is running and already unblocked.\n");
		}else{
		
			error = remove_pcb(tempPCB);
			error = errorCheck(error);
		
			if(tempPCB->state == BLOCKED){
				tempPCB->state = READY;
			}else if(tempPCB->state == SUSBLOCKED){
				tempPCB->state = SUSREADY;
			}
			
			error = insert_pcb(tempPCB->state, tempPCB);
			error = errorCheck(error);
			
			// Display appropriate error or success message
			if(error == 0){
				printf("The PCB Process has been successfully unblocked.\n");
			}//end if
			
		}
		
		
		}else{
			printf("Process Name is Invalid. No process by that Name was found.\n");
		}
	}else{
			printf("Process Name is Invalid. Process Name is not between 8 and 20 characters.\n");
		}	
		}

/**
	Procedure: handler_suspend

	Purpose: Suspends a defined PCB Structure

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, errorCheck, insert_pcb

	Globals: bufSizeDefined, BLOCKED, SUSBLOCKED, RUNNING, READY, SUSREADY, error

	Errors: None
**/
void handler_suspend(){
	char tempBuff[bufSizeDefined] = {0};												//Temporary Input Buffer
	char *tempPtr = NULL;												//Temporary Input Character Pointer for PCB Name
	PCBitem *tempPCB;													//Temporary PCB Item
	int charCount = 0;													//Number of Characters in Process Name

	// Get PCB name from the user- check that the PCB with that name exists (Make sure that name is valid & exists!)
	printf("Please enter the name of the PCB to suspend(max 20, min 8 characters):\n ");
	tempPtr = keyboardInput(0);											//Gets process name from User
	pointer2Str(tempPtr,tempBuff);										//Convert Character Pointer to Character String
	charCount = sizeOfArray(tempBuff);									//Get Name Size
	
	if((charCount >= 8 && charCount <=20)){											//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(tempBuff);
		if(tempPCB != NULL){										//Check if Name Exists
			if(tempPCB->state == RUNNING){
				printf("Error: Running Processes cannot be Suspended.\n");
			} else if(tempPCB->state == SUSREADY){
				printf("Error: The process is already Suspended & Ready.\n");
			} else if(tempPCB->state == SUSBLOCKED){
				printf("Error: The process is already Suspended & Blocked.\n");
			} else{
				
				// Should remove the process from the ready queue (by calling remove_PCB)
				error = remove_pcb(tempPCB);
				error = errorCheck(error);
				
				// Puts the PCB in the suspended state
				if(tempPCB->state == READY){
					tempPCB->state = SUSREADY;
				} else if(tempPCB->state == BLOCKED){
					tempPCB->state = SUSBLOCKED;
				}//end if
				
				// Might require changing queues (from ready to suspended ready, for example) if 4 queues are used
				error = insert_pcb(tempPCB->state, tempPCB);
				error = errorCheck(error);
				
				// Display appropriate error or success message
				if(error == 0){
					printf("The PCB Process has been successfully suspended.\n");
				}//end if
			}//end if
		} else{
			printf("Process Name does not exist.\n");
		}//end if
	} else{
		printf("Process Name is Invalid. Process Name is not between 8 and 20 characters.\n");
	}//end if
}//end suspend

/**
	Procedure: handler_resume

	Purpose: Resumes a suspended PCB Structure

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, errorCheck, insert_pcb

	Globals: bufSizeDefined, BLOCKED, SUSBLOCKED, RUNNING, READY, SUSREADY, error

	Errors: None
**/
void handler_resume(){
char tempBuff[bufSizeDefined] = {0};												//Temporary Input Buffer
	char *tempPtr = NULL;												//Temporary Input Character Pointer for PCB Name
	PCBitem *tempPCB;													//Temporary PCB Item
	int charCount = 0;													//Number of Characters in Process Name

	// Get PCB name from the user- check that the PCB with that name exists (Make sure that name is valid & exists!)
	printf("Please enter the name of the PCB to resume(max 20, min 8 characters):\n");
	tempPtr = keyboardInput(0);											//Gets process name from User
	pointer2Str(tempPtr,tempBuff);										//Convert Character Pointer to Character String
	charCount = sizeOfArray(tempBuff);									//Get Name Size
	
	if((charCount >= 8 && charCount <=20)){							//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(tempBuff);
		if(tempPCB != NULL){										//Check if Name Exists
			if(tempPCB->state == RUNNING){
				printf("Error: There is no need to resume an already running process.\n");
			} else if(tempPCB->state == READY){
				printf("Error: This process is READY but not suspended.\n");
			} else if(tempPCB->state == BLOCKED){
				printf("Error: This process is Blocked but not suspended.\n");
			} else{
			
				// Should remove the process from the SUSREADY or SUSBLOCKED queue (by calling remove_PCB)
				error = remove_pcb(tempPCB);
				error = errorCheck(error);
				
				// Puts the PCB in the unsuspended state
				if(tempPCB->state == SUSREADY){
					tempPCB->state = READY;
				} else if(tempPCB->state == SUSBLOCKED){
					tempPCB->state = BLOCKED;
				}//end if
	
			    // Might require changing queues (from ready to suspended ready, for example) if 4 queues are used
				error = insert_pcb(tempPCB->state, tempPCB);
				error = errorCheck(error);
				
				// Display appropriate error or success message
				if(error == 0){
					printf("The PCB Process has been successfully resumed.\n");
				}//end if
			}//end if
		}else{
			printf("Process Name does not exist.\n");
		}//end if
	}else{
		printf("Process Name is Invalid. Process Name is not between 8 and 20 characters.\n");
	}//end if
}//end resume

/**
	Procedure: handler_set_priority

	Purpose: Sets a new priority for a defined PCB Structure

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, errorCheck, insert_pcb, pointer2Int

	Globals: bufSizeDefined, READY, error

	Errors: None
**/
void handler_set_priority(){
	char tempBuff[bufSizeDefined] = {0};	
	char *tempPtr = NULL;											//Temporary Input Character Pointer for PCB Name										//Temporary Input Character Pointer for New Priority
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;												//Number of Characters in Process Name
	int newPriority = 0;											//New Priority value
	
	//Get PCB name
	printf("Please enter the name of the PCB to Set the Priority(max 20, min 8 characters):\n");
	tempPtr = keyboardInput(0);										//Gets process name from User
	pointer2Str(tempPtr,tempBuff);									//Convert Character Pointer to Character String
	charCount = sizeOfArray(tempBuff);									//Get Name Size
	
	if((charCount >= 8 && charCount <=20)){													//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(tempBuff);
		if(tempPCB != NULL){										//Check that the PCB Exists
			
			//Get New Priority from User
			printf("Please enter a number between -128 & +127 to set the new Priority:\n");
			tempPtr = keyboardInput(0);									//Gets process name from User
			newPriority = pointer2Int(tempPtr);							//Convert Character Pointer to Int
			
			
			if((newPriority >= -128) || (newPriority <= 127)){		//Check that the new priority is valid (-128 to 127)
				tempPCB->priority = newPriority;						//Set new PCB Priority
				
				//If the PCB is in the ready state, you will need to change the position of the PCB in the queue based upon its new priority
				if(tempPCB->state == READY){
					error = remove_pcb(tempPCB);						//Remove PCB from Ready Queue
					error = errorCheck(error);							//Perform Error Check
					error = insert_pcb(tempPCB->state, tempPCB);						//Insert PCB into Ready Queue (so as to adjust for new Priority)
					error = errorCheck(error);							//Perform Error Check
				}//end if
			}//end if
			
			//Display appropriate error or success message.
			if(error == 0){
				printf("The PCB Process has been successfully changed.\n");
			}//end if
			
		} else{
			printf("Process Name does not exist.\n");
		}//end if
	} else{
		printf("Process Name is Invalid. Process Name is not between 8 and 20 characters.\n");
	}//end if
}//end setPriority

/**
	Procedure: handler_show_pcb

	Purpose: Shows all of the stored information for a defined PCB Structure

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb

	Globals: bufSizeDefined

	Errors: None
**/
void handler_show_pcb(){
	char tempBuff[bufSizeDefined] = {0};	
	char *tempPtr = NULL;											//Temporary Input Character Pointer for PCB Name											//Temporary Input Character Pointer for New Priority
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;												//Number of Characters in Process Name
											//New Priority value
	
	//Get PCB name
	printf("Enter the PCB name(max 20 characters): \n");
	tempPtr = keyboardInput(0);										//Gets process name from User
	pointer2Str(tempPtr,tempBuff);									//Convert Character Pointer to Character String
	charCount = sizeOfArray(tempBuff);									//Get Name Size
	if((charCount >= 8 && charCount <=20)){													//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(tempBuff);
		if(tempPCB != NULL){										//Check that the PCB Exists
			//Display all information about the PCB (except pointers) in �attractive� format
			printf("\nProcess Name:\t%s\n", tempPCB->pName);
			printf("Process Class:\t%d\n", tempPCB->pClass);
			printf("Process Priority:\t%d\n", tempPCB->priority);
			printf("Process State:\t%d\n", tempPCB->state);
			printf("Process Memory Size:\t%d\n", tempPCB->memSize);
		} else{														
			printf("Process Name does not exist.\n");				//Display error if PCB doesn�t exist
		}//end if
	} else{
		printf("Process Name is Invalid. Process Name is not between 8 and 20 characters.\n");
	}//end if
}//end showPCB

/**
	Procedure: handler_show

	Purpose: Shows all of the stored information for each PCB Structure in one or all queues

	Parameters: queueInt

	Return value: None

	Calls: printInterrupt

	Globals: head4, tail4, head5, tail5, head6, tail6, head7, tail7

	Errors: Invalid Integer Value
**/
void handler_show(int queueInt){
	LLitem * tempCurr;
	if(queueInt == 0 || queueInt == 4){
		tempCurr = head4;
		if(tempCurr != NULL){
			do{
				printf("\nName:%s\tState:%d\tPriority:%d",tempCurr->val->pName,tempCurr->val->state,tempCurr->val->priority);
				//printf("\tNext:%d\tPrev:%d",tempCurr->next,tempCurr->prev);
				printInterrupt();	
				tempCurr = tempCurr->next;	
			}while(tempCurr != head4 && tempCurr != NULL);
		}
	}
	if(queueInt == 0 || queueInt == 5){
		tempCurr = head5;
		if(tempCurr != NULL){
			do{
				printf("\nName:%s\tState:%d\tPriority:%d",tempCurr->val->pName,tempCurr->val->state,tempCurr->val->priority);
				//printf("\tNext:%d\tPrev:%d",tempCurr->next,tempCurr->prev);
				printInterrupt();	
				tempCurr = tempCurr->next;	
			}while(tempCurr != head5&& tempCurr != NULL);
		}
	}
	if(queueInt == 0 || queueInt == 6){
		tempCurr = head6;
		if(tempCurr != NULL){
			do{
				printf("\nName:%s\tState:%d\tPriority:%d",tempCurr->val->pName,tempCurr->val->state,tempCurr->val->priority);
				//printf("\tNext:%d\tPrev:%d",tempCurr->next,tempCurr->prev);
				printInterrupt();	
				tempCurr = tempCurr->next;	
			}while(tempCurr != head6&& tempCurr != NULL);
		}
	}
	if(queueInt == 0 || queueInt == 7){
		tempCurr = head7;
		if(tempCurr != NULL){
			do{
				printf("\nName:%s\tState:%d\tPriority:%d",tempCurr->val->pName,tempCurr->val->state,tempCurr->val->priority);
				//printf("\tNext:%d\tPrev:%d",tempCurr->next,tempCurr->prev);
				printInterrupt();	
				tempCurr = tempCurr->next;	
			}while(tempCurr != head7&& tempCurr != NULL);
		}
	}
	if(queueInt >7 ||queueInt <0 || queueInt ==1|| queueInt ==2|| queueInt ==3){
		printf("ERROR in show");
	}
	printf("\n");
}
