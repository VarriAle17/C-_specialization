/* calculate area in meters*/
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MEMBERS_NUMBER 10
#define INCOME_RANGE 5000
#define EMPTY -1

typedef enum business_dpt {HR, Sales, Research, Software, Executive} bsn_dpt;

typedef struct{
    bsn_dpt business_dpt;
    int annual_salary =-1;
    unsigned int social_security_number=0;
    }member;

typedef struct stack{
    member array_members[MEMBERS_NUMBER];
    int top; 
}members_stack;

void reset_stack(struct stack* stk){
    stk->top = EMPTY;
}

bsn_dpt rdn_dpt(){
 return (bsn_dpt)(rand()%4);
}

void salary_generator(member* mbr)
{   
    int salary;
    switch(mbr->business_dpt){
    case 0:salary = 10000+(rand()%INCOME_RANGE); break;
    case 1:salary = 15000+(rand()%INCOME_RANGE); break;
    case 2 :salary = 20000+(rand()%INCOME_RANGE); break;
    case 3 :salary = 25000+(rand()%INCOME_RANGE); break;
    case 4 :salary = 30000+(rand()%INCOME_RANGE); break;
    default: salary = -1;
    }
    mbr->annual_salary = salary;
    return;
}

void push(members_stack* mbr_stk, member* mbr){
    mbr_stk->top++;
    mbr_stk->array_members[mbr_stk->top] = *mbr;
}

void print_member(int i, member* mbr){
    const char* department[] = {"HR", "Sales", "Research", "Software", "Executive"};
    printf("member %d \t unit department: %s \t annual salary: %d \t social_security_number: %u\n", 
    i, department[mbr->business_dpt], mbr->annual_salary, mbr->social_security_number);
}
int main(void)
{
    int i;
    srand(time(0));
    members_stack my_members_stack;
    reset_stack(&my_members_stack);
    for(i=0;i<MEMBERS_NUMBER; i++){
        member my_member;    
        my_member= {rdn_dpt(),0,(unsigned)rand()};
        salary_generator(&my_member);
        push(&my_members_stack, &my_member);
        print_member(i,&my_member);
    }

    return 0;
}
