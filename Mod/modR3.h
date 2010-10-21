/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR3.h

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
