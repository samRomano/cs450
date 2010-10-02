

void setPriority(){
	char *tempPtr = NULL;											//Temporary Input Character Pointer for PCB Name
	char *tempPtr2 = NULL;											//Temporary Input Character Pointer for New Priority
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;												//Number of Characters in Process Name
	int newPriority = 0;											//New Priority value
	
	//Get PCB name
	printf("Please enter the name of the PCB to Set the Priority: ");
	tempPtr = keyboardInput(0);										//Gets process name from User
	tempPtr = pointer2Str(tempPtr);									//Convert Character Pointer to Character String
	charCount = sizeOfPointer(tempPtr);								//Get Name Size
	
	
	if((charCount >= 8)){											//Check that the Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_PCB(tempPtr);
		if(tempPCB != NULL){										//Check that the PCB Exists
			
			//Get New Priority from User
			printf("Please enter a number between -128 & +127 to set the new Priority: ");
			tempPtr = keyboardInput(0);									//Gets process name from User
			newPriority = pointer2Int(tempPtr);							//Convert Character Pointer to Int
			
			
			if((new_pPriority >= -128) || (new_pPriority <= 127)){		//Check that the new priority is valid (-128 to 127)
				tempPCB->pPriority = new_pPriority;						//Set new PCB Priority
				
				//If the PCB is in the ready state, you will need to change the position of the PCB in the queue based upon its new priority
				if(tempPCB->state == READY){
					error = remove_PCB(tempPCB);						//Remove PCB from Ready Queue
					error = errorCheck(error);							//Perform Error Check
					error = insert_PCB(tempPCB);						//Insert PCB into Ready Queue (so as to adjust for new Priority)
					error = errorCheck(error);							//Perform Error Check
				}//end if
			}//end if
			
			//Display appropriate error or success message.
			if(error == 0){
				printf("The PCB Process has been successfully blocked.\n");
			}//end if
			
		} else{
			printf("Process Name does not exist.\n");
		}//end if
	} else{
		printf("Process Name is Invalid. Process Name must be at least 8 characters in length.\n");
	}//end if
}//end setPriority