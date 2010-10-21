/***********************************************************************
	MPX: The MultiProgramming eXecutive
	Project to Accompany
	A Practical Approach to Operating Systems
	Malcolm G. Lane & James D. Mooney
	Copyright 1993, P.W.S. Kent Publishing Co., Boston, MA.

	File Name:	modR2.h

	Group Members: Frank Hamilton, Luis Armendariz, Sam Romano
	Version: 1.8.1
	Date:  10/8/10

	Purpose: Module R2 file for MPX support

		This file contains the PCB and Linked List structures and handler
		functions needed to support the MPX OS.

	
	
	Prototypes in this file:

					

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

#define historySize 10
#define SYSTEM 1
#define APP 2   

#define RUNNING 3
#define READY 4
#define BLOCKED 5
#define SUSREADY 6
#define SUSBLOCKED 7
#define STACKSIZE 2048					//In Bytes (Size >= 1024)
#define bufSizeDefined 500

/******** Structures ********/
/*Process Control Block*/
struct PCB{

	char pName[20];
	int pClass;
	int priority;
	int state;
	unsigned char* stackTop;
	unsigned char* stackBase;
	
	
	int memSize;
	unsigned char* loadAddress;
	unsigned char* exeAddress;
	
	//queue pointers
	//PCB* next;
	//PCB* previous;
	
};

/*Linked List*/
struct linkedList {
	struct PCB * val;
	struct linkedList * next;
	struct linkedList * prev;
};

/*Type and Structure Definitions*/
typedef struct linkedList LLitem;
typedef struct PCB PCBitem;


//Module R2 Prototypes
/* pointer2Str: Changes a Pointer Variable to a String Variable */
/*	RETURNS: Integer Error Code; Zero If Ok */
int pointer2Str(char *,char*);

/* pointer2Int: Changes a Pointer Variable to an Integer Variable */
/*	RETURNS: Integer Error Code; Zero If Ok */
int pointer2Int(char *,int *);

/* next_ll: Sets the Current Linked List Value to the Next Linked List Value */
/*	RETURNS: Integer Error Code; Zero If Ok */
int next_ll(LLitem * curr);

/* allocate_pcb: Allocates Memory for a new PCB structure */
/*	RETURNS: Pointer to the new PCB structure */
PCBitem * allocate_pcb();

/* free_pcb: Frees the Memory Allocated to a PCB structure */
/*	RETURNS: Integer Error Code; Zero If Ok */
int free_pcb(PCBitem *);

/* setup_pcb: Creates and Initializes a new PCB Structure */
/*	RETURNS: Pointer to the new PCB structure */
PCBitem * setup_pcb(char [], int , int );

/* find_pcb: Locates a PCB Structure */
/*	RETURNS: Pointer to the located PCB structure */
PCBitem * find_pcb(char []);

/* find_ll: Locates a Linked List Structure */
/*	RETURNS: Pointer to the located Linked List structure */
LLitem* find_ll(char []);

/* insert_pcb: Inserts a PCB Structure into a chosen Queue */
/*	RETURNS: Integer Error Code; Zero If Ok */
int insert_pcb(int,PCBitem*);

/* remove_pcb: Removes a PCB Structure from its set Queue */
/*	RETURNS: Integer Error Code; Zero If Ok */
int remove_pcb(PCBitem*);

/* handler_delete_pcb: Deletes a defined PCB Structure from its assigned Queue*/
void handler_delete_pcb();

/* handler_create_pcb: Creates a new PCB Structure and inserts it into the Ready Queue*/
void handler_create_pcb();

/* handler_block: Blocks a defined PCB Structure */
void handler_block();

/* handler_unblock: Unblocks a blocked PCB Structure */
void handler_unblock();

/* handler_suspend: Suspends a defined PCB Structure */
void handler_suspend();

/* handler_resume: Resumes a suspended PCB Structure */
void handler_resume();

/* handler_set_priority: Sets a new priority for a defined PCB Structure */
void handler_set_priority();

/* handler_show_pcb: Shows all of the stored information for a defined PCB Structure */
void handler_show_pcb();

/* handler_show: Shows all of the stored information for each PCB Structure in one or all queues */
void handler_show(int);

int get_pcb_name(char * );
int get_pcb_priority(int *);
int get_pcb_class(int *);


