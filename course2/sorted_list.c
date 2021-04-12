// this program creates 100 random integers, 
//puts them in a list and sorts their value
// with a bubble sort algorithm

//useful include 
#include <stdio.h>
#include <math.h>
#include <time.h>

//define array size
#define ELEMENTS 100

//define our data structure
typedef struct list {int value; struct list* next;} list;

//create element list
list* create_list(int value){
    list* head = (list*) malloc (sizeof(list));
    head->value = value;
    head->next = NULL;
    return head;
}

//verify if list is null
int is_Empty(const list* head){
    return (head == NULL);
}

//add element in front of list, generating a new head
list* add_to_front(int new_value, list* old_head){
    list* new_head = create_list(new_value);
    new_head->next = old_head;
    return new_head;
}

//print out list element
void print_list(list* h, const char title[]){
    int count =0;
    printf("%s\n", title);
    while(h != NULL){
       count++; 
       printf("data%i: %d \t", count, h->value);
       h = h->next;
       if(count%5 ==0)printf("\n");
    }
}

//int bubble_sort_call = 0;

//algorithm to sort the list
void bubble_sort(list* head, int n){
    int i;
    list* l1 = head;
    list* l2 = head->next;
    
    if (head == NULL || n==1){
        print_list(head, "\nFINAL SORTED LIST");   
        return;
    }
    //printf("\nbubble_sort_call: %i", ++bubble_sort_call);

    for(i=0; i<n-1; i++){
        int temp = l2->value;
        if(l1->value > l2-> value){
            l2->value = l1->value;
            l1->value = temp;
        }
        l1=l2;
        l2= l2->next;
        
    }
   // print_list(head, " - unsorted list");

    return bubble_sort(head,n-1); 
}



int main(void)
{
    int i;
    list* head = NULL;
        
    srand(time(0));

    //generate random number
    for(i=0;i<ELEMENTS; i++){
        head = add_to_front(rand(), head);
    }
   // print_list(head, " - unsorted list");

    bubble_sort(head, ELEMENTS);

    return 0;
}
