/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR1.h

	Group Members: Frank Hamilton, Luis Armendariz, Sam Romano
	Version: 1.8.1
	Date:  10/8/10

	Purpose: Module R1 header files

		

	
	
	Prototypes in this file:

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
			

			
			
************************************************************************/


/* Logical Constants */

#if !defined( bufSizeDefined)
#define bufSizeDefined 500
#endif





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

