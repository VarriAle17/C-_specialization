/* read the three floats */
#include <stdio.h>

int main(){
	float a, b ,c, sum;
	printf("Input 3 floats: ");
	scanf("%f%f%f", &a,&b,&c);
	printf("a=%f, b=%f, c=%f \n", a,b,c);
	sum = a +b +c;
	printf("sum=%f", sum);
	return 0;
}
