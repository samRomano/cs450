#include <mpx_supt.h>;

//Function prototypes
int handler_version();
int handler_get_date();

void main()
{
	handler_get_date();
}


int handler_version(){

	//Print the current version number
	printf("MPX version 1.0\n");

	//Print the date current version was completed
	printf("Completed 09/17/2010\n");

	return 0;
}

int handler_get_date(){
	
	//Declare new date_rec variable
	date_rec date;
	
	//Declare a pointer of type pointer to date_rec
	date_rec *dptr;
	
	//Set pointer to point to date_rec date
	dptr = &date;
	
	//Call system function with pointer passed in
	sys_get_date(&date);
	
	//Print date by accessing the attributes of the pointer to the struct
	printf("The current date is: %d/%d/%d\n", date.month, date.day, date.year);
	
	return 0;
	
}

int handler_set_date(){

	printf("Enter the day (dd): \n");
	
