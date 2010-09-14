//Function Fresco: Alex Hamilton, Luis Armendariz, Sam Romano
//9-11-10
//CS 450 Module R1: User Interface - Command Handler   (COMHAN)

#include "mpx_supt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******** Prototypes ********/
void commandHandler(void);
void handler(char *p);

void displayWelcome(void);
void commandPrompt(void);
void displayClosing(void);
void commandCleanup(void);

void handler_Help(void);
void handler_Version(void);
void handler_displayDate(void);
void handler_setDate(void);
void handler_displayMPXDirectory(void);
void handler_terminateMPX(void);

/******** Parameter List ********/
int day = 9;
int month = 11;
int year = 2010;
int comDone = 0;
int error;
int MPXV = 0; 							//Current MPX Version #
char *userCommand;


/******** Main ********/
void main(){
	sys_init(MODULE_R1);			//1. Initialize
	commandHandler();				    //2. Call commandHandler function
}


/******** Command Handler ********/
void commandHandler(){
	displayWelcome();					//2.1 Display the Welcome Message

	while(comDone !=1){					//2.2 Begin While Loop for User Commands
		commandPrompt();				//2.2.1 Request for User Input
		scanf("%s\n", *userCommand);	//2.2.2 Accept command from User
		printf("The userCommand is: %s\n", *userCommand); //test1
		while(userCommand[0] == ' '){  //2.2.3 Parse String until First Character is not blank
			userCommand = userCommand + 1;
		}//end while
		
		printf("The parsed userCommand is: %s\n", *userCommand); //test2
		handler(userCommand);			//2.2.4 Analyze/Execute Command by calling "Handler" Function
	}//end while

	displayClosing();					//2.2.5 Display closing message
	commandCleanup();					//2.2.6 Cleanup Allocated Memory
	sys_exit();					//2.2.7 Return to host system
}

/******** "Handler" ********/
void handler(char *userCommand){

	printf("Congrats. You entered the command %s\n.", *userCommand); //test1

}//end handler





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
	int error = sys_free_mem(userCommand); //!NOTE! - Display Errors
	printf("The error number is %d.\n", error);
}//end commandCleanup



/********* "Handler" Commands ********/
//Display Help Information
void handler_Help(){
	printf("Here is a list of Help commands.\n\n");
	printf("-----------Help-----------\n");
	printf("displayMPXV           Displays the current MPX Version Number.\n");
	printf("displayDate           Displays the current Date.\n");
	//Still adding here too..........................
}//end handler_Help

//Display Current MPX Version #
void handler_Version(){
	printf("The MPX Version Number is %.3f.\n", MPXV);
}//end handler_Version

//Display Date
void handler_displayDate(){
	printf("The Date is %d/%d/%d.\n", &day, &month, &year);
}//end handler_displayDate

//Set Date
void handler_setDate(){
	//Nothing here yet either.............................
}//end handler_setDate

//Display Directory of Available MPX Process Files
void handler_displayMPXDirectory(){
	printf("This will display a directory of the available MPX Process Files.\n");
}//end handler_displayMPXDirectory

//Terminate MPX Execution
void handler_terminateMPX(){
	char userAns = 'n'; //n is No, y is Yes

	printf("Are you sure you would like to exit the system (y or n): ");
	scanf("%c\n", userAns);

	if(userAns == 'y'){
		comDone = 1;
	}//end if
}//end handler_terminateMPX

