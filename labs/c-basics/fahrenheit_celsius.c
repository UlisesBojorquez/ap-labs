#include <stdio.h>


/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    int fahr;
    if(argc==2){
        fahr=(int) argv[1];
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }else{
        int lower=(int) argv[1];
        int upper=(int) argv[2];
        int step=(int) argv[3];
        
        for (fahr = lower; fahr <= upper; fahr = fahr + step)
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    }
    
    return 0;
}
