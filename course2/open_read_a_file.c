/* calculate area in meters*/
#include <stdio.h>
#include <stdlib.h>

typedef struct rational{int numerator; int denominator;}rational;

rational add_rational(rational* r1, rational* r2){
    rational result = {r1->numerator * r2->denominator + r2->numerator * r1->denominator, r1->denominator * r2->denominator};
    result= ((result.numerator)%(result.denominator) == 0)? (rational){result.numerator/result.denominator, 1} : result;
    return result;
}

rational sub_rational(rational* r1, rational* r2){
    rational result = {r1->numerator * r2->denominator - r2->numerator * r1->denominator, r1->denominator * r2->denominator};
    result= ((result.numerator)%(result.denominator) == 0)? (rational){result.numerator/result.denominator, 1} : result;
    return result;
}

rational multiply_rational(rational* r1, rational* r2){
    rational result = {r1->numerator * r2->numerator, r2->denominator * r1->denominator};
    result= ((result.numerator)%(result.denominator) == 0)? (rational){result.numerator/result.denominator, 1} : result;
    return result;
}

rational divide_rational(rational* r1, rational* r2){
    rational result = {r1->numerator * r2->denominator, r2->numerator *r1->denominator};
    result= ((result.numerator)%(result.denominator) == 0)? (rational){result.numerator/result.denominator, 1} : result;
    return result;
}

int main(int  argc, char*argv[])
{
    if (argc!=2){
        printf("please enter file name input as command line argument");
        exit(1);
    }

    FILE* ofptr= fopen("answer-hw3.txt", "w");
    FILE* ifptr = fopen(argv[1], "r");

    if (ifptr == NULL){
        printf("file not found");
        exit(1);
    }

    int rational_members= 0, i=0;
    rational sum = {};

    rewind(ifptr);

    fscanf(ifptr, "%d", &rational_members);
    
    printf("The number of rationals inside input file is: %d \n", rational_members);

    rational* ptr_to_rational_array = (rational*) calloc(rational_members,sizeof(rational));
    
    while(fscanf(ifptr, "%d %d", &(ptr_to_rational_array+i)->numerator, &(ptr_to_rational_array+i)->denominator ) != EOF){
        sum = (!i)? ptr_to_rational_array[i] : add_rational(&sum, ptr_to_rational_array+i);
        printf("rational %d \t numerator: %d \t denominator: %d \n", i, ptr_to_rational_array[i].numerator, ptr_to_rational_array[i].denominator);
        i++;

    }
    fclose(ifptr);

    double average = (double)sum.numerator/(sum.denominator*rational_members);

    printf("the average is: %f", average);

    fprintf(ofptr,"the average is: %f", average );
    fclose(ofptr);

    return 0;
}
