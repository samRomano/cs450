

/**
	Procedure: handler_help

	Purpose: Displays Help Information

	Parameters: None 

	Return value: None

	Calls: None    

	Globals: None

	Errors: None
**/
void handler_help(){
	printf("Here is a list of Help commands.\n\n");
	printf("-----------Help-----------\n");
	printf("help\t\tDisplays help on the current acceptable commands.\n");				//Beginning of Module R1 Commands
	printf("help_\t\tDisplays information on a specified function.\n");
	printf("version\t\tDisplays the current mpx OS Version Number.\n");
	printf("set_date\tAllows the user to set the current date.\n");
	printf("get_date\tDisplays the current date(MM/DD/YYYY format).\n");
	printf("display_mpx\tDisplays the available mpx Process Files.\n");
	printf("terminate_mpx\tTerminates the mpx OS.\n");
	//printf("display_history		  Displays a history of previous User Commands.\n");
	printf("change_prompt\tAllows the user to change the prompt symbol.\n");
	
	printf("create_pcb\tAllows the user to create a PCB structure.\n");					//Beginning of Module R2 Commands
	printf("delete_pcb\tAllows the user to delete a PCB structure.\n");
	printf("block\tAllows the user to block a PCB structure.\n");
	printf("unblock\tAllows the user to unblock a PCB structure.\n");
	printf("suspend\tAllows the user to suspend a PCB structure.\n");
	printf("resume\tAllows the user to resume a PCB structure.\n");
	printf("set_priority\tAllows the user to set the priority of a PCB structure.\n");
	printf("show_pcb\tAllows the user to show all the information in a PCB structure.\n");
	printf("show_all\tAllows the user to show all the inforation for all PCB structures in every queue.\n");
	printf("show_ready\tAllows the user to show all information for the PCB structures in the Ready queues.\n");
	printf("show_blocked\tAllows the user to show all information for the PCB structures in the Blocked queues.\n");
	
	printf("\n");
	printf("Type \"help_\" then a command name to access addition information on that command.\n");
	//Add a while loop perhaps with additional command info available via help files?..........................
}//end handler_help

/**
	Procedure: handler_help_function

	Purpose: Displays information on specified command

	Parameters: funName(the command name that more information is desired) 

	Return value: None

	Calls: errorCheck    

	Globals: None

	Errors: Invalid help_ command
**/	
void handler_help_function(char funName[]){
	printf("\n");
	if(strcmp(funName,"version")==0){
		printf("The version command simply displays the current working version number and the date it was finalized on. This command is called by typing \"version\". Possible Error: Invalid help_ command.");
	}
	else if(strcmp(funName,"display_mpx")==0){
		printf("The display_mpx command is used to locate mpx files in a directory. The directory can be specified or the current directory can be selected by pressing enter once the prompt for directory has come up. Then all mpx files will be displayed with name, buffer size used, and file size. This command may be called by typing \"display_mpx\" in the command prompt. Possible Errors: Invalid Directory, Directory Not Open, No More Directory Entries, Read Failed, Name To Long For Buffer, No Directory Is Open.");
	}
	else if(strcmp(funName,"get_date")==0){
		printf("The get_date command grabs the set date and displays it. If there is no set date then the system date is acquired and displayed. This command can be accessed by typing \"get_date\" in the command prompt.");
	}
	else if(strcmp(funName,"set_date")==0){
		printf("The set_date command takes in user input in the form of a string of numbers(MMDDYYYY) and sets that as the current date. The set_date command can be accessed by typing \"set_date\" in the command prompt. Possible Errors: Invalid Date, Date Not Changed.");
	}
	else if(strcmp(funName,"change_prompt")==0){
		printf("The change_prompt command replaces the standard :> prompt with any prompt that is desired and is 4 characters or less. This command is initiated by typing \"change_prompt\"in the command prompt.");
	}
	else if(strcmp(funName,"terminate_mpx")==0){
		printf("The terminate_mpx command exits the mpx os after it confirms with the user that the os is desired to exit. After confirmation allocated memory is cleared and the os terminates. This command may be called by typing either \"quit\",\"exit\", or \"terminate_mpx\"in the command prompt.");
	}
	else if(strcmp(funName,"create_pcb")==0){
		printf("The create_pcb command initializes a new PCB structure. After the command is entered, the user is prompted to input in a unique PCB name, the PCB class, and the PCB priority level. This command is called by typing \"create_pcb\" in the command prompt.");
	}
	else if(strcmp(funName,"delete_pcb")==0){
		printf("The delete_pcb command deletes a defined PCB structure. Initially, the user is prompted to input the name of a current PCB structure. If the PCB exists, it is removed from its current queue, and its assigned memory is deallocated. This command is called by typing \"delete_pcb\" in the command prompt.");
	}
	else if(strcmp(funName,"block")==0){
		printf("The block command blocks a defined PCB structure. Initially, the user is prompted to input the name of a current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"block\" in the command prompt.");
	}
	else if(strcmp(funName,"unblock")==0){
		printf("The unblock command unblocks a blocked PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"unblock\" in the command prompt.");
	}
	else if(strcmp(funName,"suspend")==0){
		printf("The suspend command suspends a defined PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"suspend\" in the command prompt.");
	}
	else if(strcmp(funName,"resume")==0){
		printf("The resume command suspends a defined PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB is removed from its current queue, updates its current state, and inserted into its new queue. This command is called by typing \"resume\" in the command prompt.");
	}
	else if(strcmp(funName,"set_priority")==0){
		printf("The set_priority command changes the priority of a defined PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the user is prompted to enter in the new PCB priority. If the new priority is valid, the PCB priority is updated, and the PCB position is adjusted, if the PCB is in the Ready queue. This command is called by typing \"set_priority\" in the command prompt.");
	}
	else if(strcmp(funName,"show_pcb")==0){
		printf("The show_pcb command shows all the stored information within a PCB structure. Initially, the user is prompted to input the name of the current PCB structure. If the name is valid and the PCB exists, the PCB Name, Class, Priority, State, and Memory Size is displayed. This command is called by typing \"show_pcb\" in the command prompt.");");
	}
	else if(strcmp(funName,"show_all")==0){
		printf("The show_all command shows all the stored information for all PCB structures. This command is initiated by typing \"show_all\"in the command prompt.");
	}
	else if(strcmp(funName,"show_ready")==0){
		printf("The show_ready command shows all the stored information for each PCB structure in the Ready queues. This command is initiated by typing \"show_ready\"in the command prompt.");
	}
	else if(strcmp(funName,"show_blocked")==0){
		printf("The show_blocked command shows all the stored information for each PCB structure in the Blocked queues. This command is initiated by typing \"show_blocked\"in the command prompt.");
	}
	else{
		errorCheck(-124);
	}
	printf("\n");
}



