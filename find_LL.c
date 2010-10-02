item1* find_LL(char []);

item1* find_LL(char name []){

	curr1 = head1;
	do{ 
		if(curr1->pName == name){
			return curr1->val;
		}
	}while(curr1 != tail1);

	curr2 = head2;
	do{ 
		if(curr2->pName == name){
			return curr2->val;
		}
	}while(curr2 != tail2);
	
	curr3 = head3;
	do{ 
		if(curr3->pName == name){
			return curr3->val;
		}
	}while(curr3 != tail3);
	
	curr4 = head4;
	do{ 
		if(curr4->pName == name){
			return curr4->val;
		}
	}while(curr4 != tail4);
	
	return NULL;
		
		
}//end find_LL