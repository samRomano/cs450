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
void handler_version(void);
void handler_displayDate(void);
void handler_setDate(void);
void handler_display_MPX(void);
void handler_terminate_MPX(void);


/******** Parameter List ********/
int version = 0.3; 							//Current MPX Version #
int comDone = 0;							//Command Handler Loop Indictor - Indicates whether the user is ready to terminate the program.
char *userInput;							//Current User Input
char *userCommand;
int error = 0;								//Variable for Error Handling
int bufferSize = 80;						//-----------------------------------------------------
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
	displayWelcome();						//2.1 Display the Welcome Message

	while(comDone !=1){						//2.2 Begin While Loop for User Commands
		commandPrompt();					//2.2.1 Request for User Input
		
		//scanf("%s", *userCommand);		//2.2.2 Accept command from User---------------------------------------------------
		error  = sys_req(READ, TERMINAL, userCommand, &bufferSize);
		printf("\n");
		printf("The userCommand is: %s\n", *userCommand); //test1
		
		while(userCommand[0] == ' '){  		//2.2.3 Parse String until First Character is not blank
			userCommand = userCommand + 1;
		}//end while
		printf("The parsed userCommand is: %s\n", *userCommand); //test2
		
		/*//Temporary Decision Statement
		if(strcmp(*userCommand, "help") == 0){
			handler_help();
		} else if(strcmp(*userCommand, "version") == 0){
			handler_version();
		} else if(strcmp(*userCommand, "set_Date") == 0){
			handler_set_Date();
		} else if(strcmp(*userCommand, "get_Date") == 0){
			handler_get_Date();
		} else if(strcmp(*userCommand, "display_MPX") == 0){
			handler_display_MPX();
		} else if(strcmp(*userCommand, "terminate_MPX") == 0){
			handler_terminate_MPX();
		} else {
			printf("Error.\n");
		}//end if - Decision*/
		
	}//end while

	displayClosing();						//2.2.4 Display closing message
	commandCleanup();						//2.2.5 Cleanup Allocated Memory
	sys_exit();								//2.2.6 Return to host system
}



/******** Command Handler Functions ********/
void displayWelcome(){
	printf("Welcome to the Functional Fresco MPX OS.\n\n");
}//end displayWelcome

void commandPrompt(){
	printf("Please enter a command to be executed: ");
	printf(">");
}//end commandPrompt

void displayClosing(){
	printf("Thank you for using the Functional Fresco MPX OS.\n Have a nice day! :)\n");
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
	printf("\n");
	//Add a while loop perhaps with additional command info available via help files?..........................
}//end handler_help

//Display Current MPX Version #
void handler_version(){
	printf("The current MPX Version Number is %.3f.\n", version);
}//end handler_version

//Display Date
void handler_get_Date(){
	//Use built in function........................
}//end handler_get_Date

//Set Date
void handler_set_Date(){
	//Use built in function.............................
}//end handler_set_Date

//Display Directory of Available MPX Process Files
void handler_display_MPX(){
	printf("This will display a directory of the available MPX Process Files.\n");
}//end handler_display_MPX

//Terminate MPX Execution
void handler_terminate_MPX(){
	char userAns = 'n'; //n is No, y is Yes

	printf("Are you sure you would like to exit the system (y or n): ");
	scanf("%c\n", userAns); 		//Chang this to sys_req-------------------------------------------------

	if(userAns == 'y'){
		comDone = 1;
	}//end if
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

