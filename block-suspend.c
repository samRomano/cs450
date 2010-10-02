void block(){
	//char tempBuff[20];												//Temporary Input Buffer
	char *tempPtr = NULL;												//Temporary Input Character Pointer for PCB Name
	PCBitem *tempPCB;													//Temporary PCB Item
	int charCount = 0;													//Number of Characters in Process Name

	// Get PCB name from the user- check that the PCB with that name exists (Make sure that name is valid & exists!)
	printf("Please enter the name of the PCB to block: ");
	tempPtr = keyboardInput(0);											//Gets process name from User
	tempPtr = pointer2Str(tempPtr);										//Convert Character Pointer to Character String
	charCount = sizeOfPointer(tempPtr);									//Get Name Size
	
	if((charCount >= 8)){												//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_PCB(tempPtr);
		if(tempPCB != NULL){											//Check if Name Exists
			if(tempPCB->state == BLOCKED){
				printf("Error: The process is already Blocked.\n");
			} else if(tempPCB->state == SUSBLOCKED){
				printf("Error: The process is already Suspended & Blocked.\n");
			} else{
				
				// Should remove the process from the ready queue (by calling remove_PCB)
				error = remove_PCB(tempPCB->state, tempPCB);
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
				error = insert_PCB(tempPCB->state, tempPCB);
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
		printf("Process Name is Invalid. Process Name must be at least 8 characters in length.\n");
	}//end if
}//end block



void suspend(){
	char *tempPtr = NULL;											//Temporary Input Character Pointer
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;												//Number of Characters in Process Name

	// Get PCB name from the user
	printf("Please enter the name of the PCB to suspend: ");
	tempPtr = keyboardInput(0);										//Gets process name from User
	tempPtr = pointer2Str(tempPtr);									//Convert Character Pointer to Character String
	charCount = sizeOfPointer(tempPtr);								//Get Name Size
	
	if((charCount >= 8)){											//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_PCB(tempPtr);
		if(tempPCB != NULL){										//Check if Name Exists
			if(tempPCB->state == RUNNING){
				printf("Error: Running Processes cannot be Suspended.\n");
			} else if(tempPCB->state == SUSREADY){
				printf("Error: The process is already Suspended & Ready.\n");
			} else if(tempPCB->state == SUSBLOCKED){
				printf("Error: The process is already Suspended & Blocked.\n");
			} else{
				
				// Should remove the process from the ready queue (by calling remove_PCB)
				error = remove_PCB(tempPCB->state, tempPCB);
				error = errorCheck(error);
				
				// Puts the PCB in the suspended state
				if(tempPCB->state == READY){
					tempPCB->state = SUSREADY;
				} else if(tempPCB->state == BLOCKED){
					tempPCB->state = SUSBLOCKED;
				}//end if
				
				// Might require changing queues (from ready to suspended ready, for example) if 4 queues are used
				error = insert_PCB(tempPCB->state, tempPCB);
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
		printf("Process Name is Invalid. Process Name must be at least 8 characters in length.\n");
	}//end if
}//end suspend


