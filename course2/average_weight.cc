/* this program calculates 
the average weight of elephant seals
*/

#include <stdio.h>

#define SIZE 1000   //size of array hard-coded


//define a useful function for average calculation
double average(int size, double weights[]){
    double average =0;
    double sum = 0;
    for(int i =0; i< size; i++)
        sum +=weights[i];
    return average = sum/size;
}

int main(void)
{
    //define variables needed for calculation
    int i =0; 
    int weight =0;
    double average_weight = 0;
    double weights_array[SIZE] = {0};
    
    //open file with weights and check for integrity
    FILE* in_file = fopen("elephant_seal_data", "r"); // read only  mode     
    if (!in_file )
    {  
        printf("oops, file can't be read\n"); 
        return 0; 
    } 

    // attempt to read weight values from file 
    // we store the value in weights_array 
    while ( fscanf(in_file, "%d", &weight ) != EOF ){ 
        printf("We just read %d\n", weight);
        weights_array[i++]= weight; 
    } 
    //we close the file
    fclose(in_file);
   
    average_weight = average (SIZE, weights_array);

    //final printf
    printf("the final average weight is %.2lf \n", average_weight);
    return 0;
}