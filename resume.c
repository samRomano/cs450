void resume(void);

void resume(){

	char *tempPtr = NULL;											//Temporary Input Character Pointer
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;	
	
	// Get PCB name from the user
	printf("Please enter the name of the PCB to resume: ");
	tempPtr = keyboardInput(0);										//Gets process name from User
	tempPtr = pointer2Str(tempPtr);									//Convert Character Pointer to Character String
	charCount = sizeOfPointer(tempPtr);								//Get Name Size
	
	if((charCount >= 8)){											//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_PCB(tempPtr);
		if(tempPCB != NULL){										//Check if Name Exists
			if(tempPCB->state == RUNNING){
				printf("Error: There is no need to resume an already running process.\n");
			} else if(tempPCB->state == READY){
				printf("Error: This process is READY but not suspended.\n");
			} else if(tempPCB->state == BLOCKED){
				printf("Error: This process is Blocked but not suspended.\n");
			} else{
			
				// Should remove the process from the SUSREADY or SUSBLOCKED queue (by calling remove_PCB)
				error = remove_PCB(tempPCB->state, tempPCB);
				error = errorCheck(error);
				
				// Puts the PCB in the unsuspended state
				if(tempPCB->state == SUSREADY){
					tempPCB->state = READY;
				} else if(tempPCB->state == SUSBLOCKED){
					tempPCB->state = BLOCKED;
				}//end if
	
			    // Might require changing queues (from ready to suspended ready, for example) if 4 queues are used
				error = insert_PCB(tempPCB->state, tempPCB);
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
		printf("Process Name is Invalid. Process Name must be at least 8 characters in length.\n");
	}//end if
}//end resume