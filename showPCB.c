

void showPCB(){
	char *tempPtr = NULL;											//Temporary Input Character Pointer for PCB Name
	PCBitem *tempPCB;												//Temporary PCB Item
	int charCount = 0;												//Number of Characters in Process Name

	// Get PCB name from the user
	printf("Please enter the name of the PCB to Set the Priority: ");
	tempPtr = keyboardInput(0);										//Gets process name from User
	tempPtr = pointer2Str(tempPtr);									//Convert Character Pointer to Character String
	charCount = sizeOfPointer(tempPtr);								//Get Name Size
	
	if((charCount >= 8)){											//Check that the Name is Valid (Check to see if at least 8 chars. + Null Terminator)
		tempPCB = find_PCB(tempPtr);
		if(tempPCB != NULL){										//Check that the PCB Exists
			//Display all information about the PCB (except pointers) in “attractive” format
			printf("\nProcess Name:\t%s\n", tempPCB->pName);
			printf("Process Class:\t%d\n", tempPCB->pClass);
			printf("Process Priority:\t%d\n", tempPCB->priority);
			printf("Process State:\t%d\n", tempPCB->state);
			printf("Process Memory Size:\t%d\n", tempPCB->memSize);
		} else{														
			printf("Process Name does not exist.\n");				//Display error if PCB doesn’t exist
		}//end if
	} else{
		printf("Process Name is Invalid. Process Name must be at least 8 characters in length.\n");
	}//end if
}//end showPCB