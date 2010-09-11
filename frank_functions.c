#include <stdio.h>
#include "MPX_SUPT.h"
int handler_help(void);

void main () {
	handler_help();
}

int handler_help (){
	char Buffer [10];
	printf("This is a help file");
	sys_req(Read,Terminal,Buffer,10);
	return 0;
	}
	