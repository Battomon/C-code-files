/* Tanvir Ahmed
This code implements some basic operation of singly linked list like inserting in the beginning and end, delete operation, and display operation
*/

#include<stdio.h>
#include<stdlib.h>

 typedef struct node{
	int data;
	struct node *next;
}node;

//this function takes an item and insert it in the linked list pointed by root.
node*  insert_front(node *root, int item)
{
	node *temp;
	//create a new node and fill-up the node
	temp= (node *) malloc(sizeof(node));
	temp->data=item;
	temp->next=NULL;
	if(root==NULL) //if there is no node in the linked list
        root=temp;
    else //there is an existing linked list, so put existing root after temp
    {
        temp->next = root; //put the existing root after temp
        root = temp; //make the temp as the root!
    }
  return root;

}

node* reverse(node* head)
{
	if (head == NULL || head->next == NULL)
		return head;

	node* newHead = reverse(head->next);
	head->next->next = head;
	head->next = NULL;

	return newHead;
}

void insertToPlace(node* head, int val, int place)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = val;
	newNode->next = NULL;

	if (place == 1) {
		newNode->next = head;
		head = newNode;
		return;
	}

	node* current = head;
	for (int i = 1; i < place - 1; i++) {
		if (current == NULL)
			break;
		current = current->next;
	}



	newNode->next = current->next;
	current->next = newNode;
}

void display(node* t)
{
  printf("\n\nPrinting your linked list.......");

	while(t)
	{
		printf("%d ",t->data);
		t=t->next;
	}

}

int main()
{
	node *root=NULL; //very important line. Otherwise all function will fail
	node *t;
	int ch,ele,v, val, place;
	while(1)
	{
		//printf("\nMenu: 1. insert at front, 2. insert at end, 3. Delete 4. exit: ");
		printf("\nMenu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
	    scanf("%d",&ch);
		if(ch==0)
		{
			printf("\n\nGOOD BYE>>>>\n");
			break;
		}
		if(ch==1)
		{
			printf("\n\nEnter data(an integer): ");
			scanf("%d",&ele);
			root = insert_front(root, ele);

      display(root);

		}
		if(ch==2)
		{
			printf("\nList reversed. ");
			//root = insert_end(root, ele);
			root = reverse(root);

      display(root);

		}
	  if(ch==3)
	  {
			printf("\n\nEnter data (an integer) and place (>1) separated by space: ");
			scanf("%d %d",&val,&place);
			insertToPlace(root, val, place);
      display(root);

		}
	}
  return 0;
}