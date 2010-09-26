/* PCB commands

*/

int handler_create_PCB(char [], int, int);
int handler_delete_PCB(char []);




int handler_create_PCB(char name[], int class, int priority){
	
	setup_PCB(name, class, priority);
	insert_PCB(Queue q, PCB* pcb);
	
}

int handler_delete_PCB(char name[]){
	
	remove_PCB(name);
}

int handler_block(char name[]);

	if(find_PCB(name == NULL)){
		printf("This process does not exist!");
	}else{
		pcb.state = BLOCKED;
		
	