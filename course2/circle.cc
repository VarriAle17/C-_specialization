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
    int a = 1, b = 2, c = 3;
    double r=0, area = 0;
    printf("chitebbi miett'm o ragg \n");
    scanf("%lf", &r);
    area = PI* (r*r);
    printf("l'area e': %lf", area);
    printf("HELLO\t\tWORLD %d\n",  proof+1);
    return 0;
}
