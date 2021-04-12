/* this program prints a table value
for sine and cose functions in a range [0,1]
with a 0.1 step*/

#define STEP  0.1
#define DOWN_LIMIT  0
#define UPPER_LIMIT  1

#include<stdio.h>
#include<math.h> /* has  sin() */

int main(void)
{ 
/*variable declaration*/
double x;
double cos_x;
double sin_x;


for(int i = 0; x < UPPER_LIMIT; i++)
{
 x= DOWN_LIMIT + STEP *i;
 printf("cos( %lf ) = %lf \t", x, cos(x));
 printf("sin( %lf ) = %lf \t \n", x, sin(x));
}


printf("\n+++++++\n");
return 0;
}