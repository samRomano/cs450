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
#include "modR2.h"

extern int error;	


LLitem * tail4 = NULL,  *head4 = NULL,  *curr4 = NULL;
LLitem * tail5 = NULL,  *head5 = NULL,  *curr5 = NULL;
LLitem * tail6 = NULL,  *head6 = NULL,  *curr6 = NULL;
LLitem * tail7 = NULL,  *head7 = NULL,  *curr7 = NULL;



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
int pointer2Int(char * Buffer, int * inputInt){
	int BufferSize;
	int i = 0;
	int j = 0;
	int inputIntTemp = 0;
	int powInt = 1;
	int negative = 1;
	BufferSize = sizeOfPointer(Buffer);
	
	
	if(BufferSize < 1){
		return -1;
	}
	else{
		if(Buffer[0] == 45){
			negative = -1;
			i++;
		}
		
		while(i<BufferSize && Buffer[i] != 10){
			j = 0;
			powInt = 1;
			while(j<BufferSize-1-i){
				powInt = powInt*10;
				j++;
			}
			if(Buffer[i]<48 || Buffer[i] > 57){ 
				
				return -1;
			}
			else{
				inputIntTemp = inputIntTemp + (Buffer[i]-48)*powInt;
			}
			i++;
		}
		
		
	}	
	*inputInt = inputIntTemp * negative;
	printf("%d\n",*inputInt);
	return 0;
}
//			}

/**
	Procedure: next_ll

	Purpose: Sets current Linked List structure to next Linked List structure

	Parameters: curr

	Return value: Integer Error Code; Zero If Ok

	Calls: None

	Globals: None

	Errors: Invalid Linked List Structure
**/
int next_ll(LLitem * curr){
	if(curr->next != NULL){
		printf("Snap");
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

	Errors: Invalid PCB Name, Invalid PCB Priority Value, Invalid PCB Class
**/
PCBitem *setup_pcb(char new_pName[], int new_pPriority, int new_pClass){
	int charCount = 0;												//Number of Characters in Process Name
	PCBitem *tempPCB;												//Temporary PCB Item
	PCBitem *newPCB;												//Pointer to New PCB Item
	char store_pName[20];											// Initializes a PCB’s content (name, priority, class)
	//printf("SNAP");
	//printf("name:%s p:%d c:%d",new_pName,new_pPriority,new_pClass);
	
	strncpy(store_pName, new_pName, 20);
	
	tempPCB = find_pcb(store_pName);							// Check that Name is unique (1. Check to see if name already exists)
	charCount = (sizeOfArray(store_pName));						// Check that Name is unique (2. Check to see if at least 8 chars. + Null Terminator)
	if(tempPCB != NULL || charCount >20){
		//printf("%d",charCount);
		errorCheck(-126);
		return NULL;
	}//end if
	
	else if((new_pPriority > 127) || (new_pPriority < -128)){			// Check that Priority between -128 and +127
		errorCheck(-127);
		return NULL;
		
		
	}//end if
	
	else if((new_pClass > 1) || (new_pClass < 0)){						// Check that Class is valid
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
	//printf("%s",name);
	error = 0;
	curr4 = head4;
	
	do{ 
		
		if(strcmp(curr4->val->pName ,name)==0){
			
			return curr4->val;
		}
		
		//error = next_ll(curr4);
		if(curr4->next != NULL){
			curr4 = curr4->next;
		}
		else{
			error = 1;
		}
		
	}while(curr4 != head4 && error == 0);

	error = 0;
	curr5 = head5;
	do{ 
		if(strcmp(curr5->val->pName ,name)==0){
			return curr5->val;
		}
		
		if(curr5->next != NULL){
			curr5 = curr5->next;
		}
		else{
			error = 1;
		}	
	}while(curr5 != head5 && error == 0);
	
	error = 0;
	curr6 = head6;
	do{ 
		if(strcmp(curr6->val->pName ,name)==0){
			return curr6->val;
		}
		
		if(curr6->next != NULL){
			curr6 = curr6->next;
		}
		else{
			error = 1;
		}
	}while(curr6 != head6 && error == 0);
	
	error = 0;
	curr7 = head7;
	do{ 
		if(strcmp(curr7->val->pName ,name)==0){
			return curr7->val;
		}
		
		if(curr7->next != NULL){
			curr7 = curr7->next;
		}
		else{
			error = 1;
		}
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
	error = 0;
	curr4 = head4;
	
	do{ 
		
		if(strcmp(curr4->val->pName ,name)==0){
			
			return curr4;
		}
		
		if(curr4->next != NULL){
			curr4 = curr4->next;
		}
		else{
			error = 1;
		}
	}while(curr4 != head4 && error == 0);
	
	error = 0;
	curr5 = head5;
	do{ 
		if(strcmp(curr5->val->pName ,name)==0){
			return curr5;
		}
		
		if(curr5->next != NULL){
			curr5 = curr5->next;
		}
		else{
			error = 1;
		}	
	}while(curr5 != head5 && error == 0);
	
	error = 0;
	curr6 = head6;
	do{ 
		if(strcmp(curr6->val->pName ,name)==0){
			return curr6;
		}
		
		if(curr6->next != NULL){
			curr6 = curr6->next;
		}
		else{
			error = 1;
		}	
	}while(curr6 != head6 && error == 0);
	
	error = 0;
	curr7 = head7;
	do{ 
		if(strcmp(curr7->val->pName ,name)==0){
			return curr7;
		}
		
		if(curr7->next != NULL){
			curr7 = curr7->next;
		}
		else{
			error = 1;
		}	
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

	Errors: Invalid Integer Queue Value, Invalid PCB Pointer Value
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
			printf("4");
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
			tempCurr = tempHead->next;
			while(1){
				//printf("%d %d %s",PCBtemp->priority,tempCurr->val->priority,tempCurr->val->pName);
				if(PCBtemp->priority > tempCurr->val->priority){
					//printf("howdy");
					temp->prev = tempCurr->prev;
					temp->next = tempCurr;
					temp->val = PCBtemp;
					tempCurr->prev = temp;
					temp->prev->next = temp;
					break;
				}
				//printf("");
				tempCurr = tempCurr->next;
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
		printf("Error no items\n");
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
	}
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

	Errors: None
**/
void handler_create_pcb(){
	PCBitem * temp;
	char inputName[bufSizeDefined] = {0};
	int *inputClass;
	int *inputPriority ;
	
	error = get_pcb_name(inputName); 
	if(error == 0){ 
		error = get_pcb_priority(inputPriority);
		if (error == 0){
			error = get_pcb_class(inputClass);
		}
	}
	if(error == 0){
		
		
		
		temp = setup_pcb(inputName,*inputPriority,*inputClass);
		//printf("name:%s p:%d c:%d",inputName,*inputPriority,*inputClass);
		if(temp != NULL){
			error = insert_pcb(READY, temp);
		}
	}
}

/**
	Procedure: handler_delete_pcb

	Purpose: Deletes a defined PCB Structure from its assigned Queue

	Parameters: None

	Return value: None

	Calls: keyboardInput, pointer2Str, sizeOfArray, find_pcb, remove_pcb, free_pcb

	Globals: bufSizeDefined

	Errors: None
**/
void handler_delete_pcb(){
	PCBitem * temp = NULL;
	char inputName[bufSizeDefined] = {0};
	error = get_pcb_name(inputName);
	if(error == 0){	
		temp = find_pcb(inputName);					
		if(temp == NULL){
			errorCheck(-132);
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
	char inputName[bufSizeDefined] = {0};	
	
	PCBitem *tempPCB;												//Temporary PCB Item
	

	error = get_pcb_name(inputName);
	if(error == 0){												//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(inputName);
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
	char inputName[bufSizeDefined] = {0};	
	PCBitem *tempPCB;												//Temporary PCB Item
	error = get_pcb_name(inputName);
	if(error == 0){													//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(inputName);
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
	char inputName[bufSizeDefined] = {0};	
	PCBitem *tempPCB;												//Temporary PCB Item
	error = get_pcb_name(inputName);
	if(error == 0){													
		tempPCB = find_pcb(inputName);								

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
	char inputName[bufSizeDefined] = {0};	
	PCBitem *tempPCB;												//Temporary PCB Item
	error = get_pcb_name(inputName);
	if(error == 0){													//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(inputName);
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
	char inputName[bufSizeDefined] = {0};
	int * inputPriority;
	PCBitem *tempPCB;												//Temporary PCB Item
	int tempInt;
	error = get_pcb_name(inputName);
	if(error == 0){													//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(inputName);
		if(tempPCB != NULL){										//Check that the PCB Exists
			
			//Get New Priority from User
			error = get_pcb_class(inputPriority);
			
			if(error == 0){			//Check that the new priority is valid (-128 to 127)
				
				tempInt = *inputPriority;
				tempPCB->priority = tempInt;						//Set new PCB Priority
				
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
	char inputName[bufSizeDefined] = {0};	
	
	PCBitem *tempPCB;												//Temporary PCB Item
	

	error = get_pcb_name(inputName);
	if(error == 0){												//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_pcb(inputName);
		if(tempPCB != NULL){										//Check that the PCB Exists
			//Display all information about the PCB (except pointers) in “attractive” format
			printf("\nProcess Name:\t%s\n", tempPCB->pName);
			printf("Process Class:\t%d\n", tempPCB->pClass);
			printf("Process Priority:\t%d\n", tempPCB->priority);
			printf("Process State:\t%d\n", tempPCB->state);
			printf("Process Memory Size:\t%d\n", tempPCB->memSize);
		} else{														
			errorCheck(-132);				//Display error if PCB doesn’t exist
		}//end if
	} 
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
}
int get_pcb_name(char * inputName){
	char * Buffer = NULL;
	printf("Enter the PCB name(max 20 characters): \n");
	Buffer = keyboardInput(0);
	if(sizeOfPointer(Buffer)<1){
		error = -125;
	}
	else{
		
		error = pointer2Str(Buffer,inputName);
		if (error != 0){
			error = -125;
		}
		if(sizeOfArray(inputName)<1 || sizeOfArray(inputName)>20){
			error = -125;
		}
	}
	errorCheck(error);
	return error;
}

int get_pcb_priority(int * inputPriority){
	char * Buffer = NULL;
	error = 0;
	printf("Enter the PCB Priority(-128 to 127): \n");
	Buffer = keyboardInput(0);
	if(sizeOfPointer(Buffer)<1){
		error = -127;
	}
	else{
		
		error = pointer2Int(Buffer,inputPriority);
		if (error != 0){
			error = -127;
		}
		else{
			
			if( *inputPriority <-128 || *inputPriority >127){
				
				error = -127;
			}
		}
	}
	errorCheck(error);
	return error;
}
int get_pcb_class(int * inputClass){
	char * Buffer = NULL;
	error = 0;
	printf("Enter the PCB Class(0 for System or 1 for APP): \n");
	Buffer = keyboardInput(0);
	if(sizeOfPointer(Buffer)<1){
		error = -128;
	}
	else{
		
		error = pointer2Int(Buffer,inputClass);
		if (error != 0){
			error = -128;
		}
		else{
			if( *inputClass <0 || *inputClass >1){
				error = -128;
			}
		}
	}
	errorCheck(error);
	return error;
}

