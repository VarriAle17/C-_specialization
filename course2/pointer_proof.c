/* calculate area in meters*/
#include <stdio.h>
#include <math.h>

#define PI 3.16

void ale(void){
    static int proof = 5;
}

extern int proof;

int main(void)
{
    int a = 1;
    int* point_to_a = &a;
    int b = *point_to_a;
    printf("a1 %d", a);
    printf(" b1 %d", b);
    a= a+1;
    printf(" a2 %d", a);
    printf(" b2 %d", b);
    return 0;
}
