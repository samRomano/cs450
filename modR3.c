/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR3.c

	Group Members: Frank Hamilton, Luis Armendariz, Sam Romano
	Version: 3.0
	Date:  10/20/10

	Purpose: Module R3 file for MPX support

		This file contains the simple round-robin dispatcher for short-term scheduling of
		MPX processes...  PCB and Linked List structures and handler
		functions needed to support the MPX OS.

	
	
	Procedures in this file:

		dispatch
		sys_call
			
************************************************************************/

#include "mpx_supt.h"
/* ANSI C Include Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SYS_STACK_SIZE 200

/******** Prototypes ********/
void interrupt sys_call (void);




/******** Parameter List ********/
byte sys_stack[SYS_STACK_SIZE];
unsigned short ss_save;
unsigned short sp_save;
unsigned short new_ss;
unsigned short new_sp;



/***** Temporary Command *****/
void tempCom(){
// Add 1 temporary command to your command handler:
// This command exists to allow the testing of your “dispatcher” and “system call handler”.
// This operation will be responsible for creating and initializing 5 PCB’s to represent 5 processes which will be dispatched.

// During initialization of your dispatcher, you must create five test processes. Each process
// should be established as an application process with default priority. Suggested names for these
// processes are test1 through test5. All processes should be set to the ready, non-suspended
// state.

// Your initialization procedure creates five processes and installs them on the ready queue. The action begins when you call dispatch.

// The outline of each test procedure is as follows:
// repeat N times
// display progress message
// sys_req(IDLE, NO_DEV, NULL, NULL);
// end loop
// display completion message
// sys_req(EXIT, NO_DEV, NULL, NULL);
// display error message

}//end tempCom



/***** Interrupt Handlers *****/
/**
	Procedure: dispatch

	Purpose: Responsible for locating the next ready process and performing a context 
			 switch to begin executing that process

	Parameters: None  

	Return value: None

	Calls: None

	Globals: None

	Errors: None
**/
void dispatch(){

if(sp_save == NULL){
ss_save = _SS;
sp_save = _SP;

// remove the PCB at the head of the ready queue 

// if a PCB was found set cop to this PCB 
// set _SS and _SP to the PCB's stack pointer 
//-----saving and resting of SP
// new_ss = FP_SEG(sys_stack);
// new_sp = FP_OFF(sys_stack);
// new_sp += SYS_STACK_SIZE;
// _SS = new_ss;
// _SP = new_sp;

} else {
// set cop to NULL 
_SS = ss_save;
_SP = sp_save;
}//end if 

// "return from interrupt"

}//end dispatcher




/**
	Procedure: sys_call

	Purpose: Responsible for handling “system” calls issued by the running processes.  
			 These system calls will trigger an interrupt that will result in either the current process 
			 leaving the running state, and another being dispatched, or the current process being 
			 terminated.

	Parameters: None  

	Return value: None

	Calls: None

	Globals: None

	Errors: None
**/
void interrupt sys_call(){


}//end system_call_handler

