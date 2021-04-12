/*this program is useful to calculate 
the sine of a value x 
in the range (0,1)*/

#include<stdio.h>
#include <math.h>
#define PI 3.14159

int main(void)
{
// HERE THE VARIABLES ARE DEFINED 
 double x;
 double radiants_x;
 bool check_x;
 double sine_x;

// ask user to type a value for x
 printf("Enter value of x in the range (0-1) non inclusive: \n");
 scanf("%lf", &x);

// check if the value is in the accepted range 
 check_x =(x>0 && x<1)? true : false;
 //if not return 0 and exit
 if (!check_x) {
        printf("\a \n The entered value is outside the requested range");
        return 0;
 }
// convert the value of x in radiants and calculate the sine of x
radiants_x = 180.*x/PI;
sine_x = sin(x);

// print out the result
 printf("sine of value x is : %lf \n",  sine_x );
 return 0;
}