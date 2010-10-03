void unblock_PCB(char []);

void unblock_PCB(char name[]){

	char *tempPtr = NULL;											//Temporary Input Character Pointer
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;

	// Get PCB name from the user- check that the PCB with that name exists (Make sure that name is valid & exists!)
	printf("Please enter the name of the PCB to unblock: ");
	tempPtr = keyboardInput(0);									//Gets process name from User
	tempPtr = pointer2Str(tempPtr);								//Convert Character Pointer to Character String
	charCount = sizeOfPointer(tempPtr);							//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
	
	if((charCount >= 8)){											//Check that Name is Valid (Check to see if at least 8 chars. + Null Terminator)
	tempPCB = find_PCB(tempPtr);
	if(tempPCB != NULL){											//Check if Name Exists
		if(tempPCB->state == READY){
			printf("Error: The process is ready and already unblocked.\n");
		}else if(tempPCB->state == SUSREADY){
			printf("Error: The process is suspended and already unblocked.\n");
		}else if(tempPCB->state == RUNNING){
			printf("Error: The process is running and already unblocked.\n");
		}else{
		
			error = removePCB(tempPCB);
			error = errorCheck(error);
		
			if(tempPCB->state == BLOCKED){
				tempPCB->state = READY;
			}else if(tempPCB->state == SUSBLOCKED){
				tempPCB->state = SUSREADY;
			}
			
			error = insert_PCB(tempPCB->state, tempPCB);
			error = errorCheck(error);
			
			// Display appropriate error or success message
			if(error == 0){
				printf("The PCB Process has been successfully unblocked.\n");
			}//end if
			
		}
		
		
		}else{
			printf("Process Name is Invalid. Process Name must be at least 8 characters in length.\n");
	}
}//end unblock
			

		
			
	
}