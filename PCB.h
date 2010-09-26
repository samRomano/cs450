/* Header file PCB.h
   
*/
   
#define SYSTEM 1
#define APP 2   

#define RUNNING 3
#define READY 4
#define BLOCKED 5
#define SUSREADY 6
#define SUSBLOCKED 7
   
struct PCB{

	char pName[20];
	int pClass;
	int priority;
	int state;
	unsigned char* stackTop;
	unsigned char* stackBase;
	
	//memory descriptors
	int memSize;
	unsigned char* loadAddress;
	unsigned char* exeAddress;
	
	//queue pointers
	PCB* next;
	PCB* previous;
}

typedef struct PCB PCB;

	
	
	
	