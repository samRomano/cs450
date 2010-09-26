#include<stdlib.h>
#include<stdio.h>

struct linkedList {
	int val;
	struct linkedList * next;
	struct linkedList * prev;
};

void insert(int);
void init(int);
typedef struct linkedList item1;
typedef struct linkedList item2;
typedef struct linkedList item3;
typedef struct linkedList item4;
item1 * tail1 = NULL, * head1 = NULL, * curr1 = NULL;

void main() {
	int i;
	//init(111);
	for(i=1;i<=10;i++) {
		insert(i);
	}
	curr1 = (head1->next)->next;
	for(i=1;i<=9;i++){
		delete();
	}
	printf("\n");
	for(i=1;i<=9;i++) {
		insert(i);
	}

	curr1 = head1;

	for(i=1;i<=11;i++){
		delete();
	}
	
}

void insert(int dope){
	item1 * temp;
	if(head1 == NULL){
		tail1 = (item1 *)malloc(sizeof(item1));	
		tail1->val = dope;
		tail1->next  = NULL;
		tail1->prev = NULL;
		head1 = tail1;
	}
	else{
	temp = tail1;
	tail1 = (item1 *)malloc(sizeof(item1));
	tail1->val = dope;
	tail1->next  = head1;
	tail1->prev  = temp;
	temp->next = tail1;
	head1->prev = tail1;
	}
}
void init(int dope){
	tail1 = (item1 *)malloc(sizeof(item1));	
	tail1->val = dope;
	tail1->next  = NULL;
	tail1->prev = NULL;
	head1 = tail1;
}
int delete(){
	//curr1 = head1;
	item1 *tempP,*tempN;
	if(tail1 == head1 && head1 != NULL){
		printf("%d\n", curr1->val);
		free(curr1);
		head1 = NULL;
		tail1 = NULL;
	}
	else if(head1 == NULL){
		printf("Error no items\n");
		return -1;
	}
	else{
		tempP = curr1->prev;
		tempN = curr1->next;
		printf("%d\n", curr1->val);
		if(curr1 == head1){
			head1 = tempN ;
			head1->prev = tail1;
			tail1->next = head1;
		}
		else if(curr1 == tail1){
			tail1 = tempP;
			head1->prev = tail1;
			tail1->next = head1;
		}
		else{
			tempP->next = tempN;
			tempN->prev = tempP;
		}		
	free(curr1);
	curr1 = tempN;
	}
	return 0;
}