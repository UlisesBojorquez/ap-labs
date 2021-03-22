#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(char *arr[], int l, int m, int r,int (*comp) (void *, void *))
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    char* L[n1];
    char* R[n2];

    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1 + j];
    }
    i = 0; // index of the first array
    j = 0; // index os the second array
    k = l; // index of the merged array
    while (i < n1 && j < n2) {
        if ((*comp)(L[i],R[j])<=0) { //(L[i] <= R[j])
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
    }
 
    while (i < n1) {
        arr[k++] = L[i++];
    }
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(char *arr[], int l, int r, int (*comp) (void *, void *))
{
    if (l < r) {
        //obtain the mid
        int m = l + (r - l) / 2;
        //merge the first and second half
        mergeSort(arr, l, m, comp);
        mergeSort(arr, m + 1, r, comp);
        //Do the merge
        merge(arr, l, m, r,comp);
    }
}

void printArray(char *A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%s ", A[i]);
    printf("\n");
}


int numcmp(char *num1, char *num2){
    int number1=atoi(num1);
    int number2=atoi(num2);

    if(number1==number2){
        return 0;
    }else if(number1<number2){
        return -1;
    }else{
        return 1;
    } 
}

int stringcmp(char *string1, char *string2){
    return strcmp(string1, string2);
}


int main(int argc, char **argv)
{

    FILE *fo;
    const char * FILENAME;
    size_t len = 0;
    ssize_t read;
    char * line = NULL;
    int numeric=0;
    if(argc<2){
        printf("Missing parameters.\n");
        return 0;
    }else if(argc<=2 && strcmp(argv[1],"-n")==0){
        printf("Incomplete parameters.\n");
        return 0;
    }else if(argc<4 && strcmp(argv[1],"-n")==0){
        FILENAME=argv[2];
        numeric=1;
        
    }else{
        FILENAME=argv[1];
        numeric=0;
    }

    fo = fopen(FILENAME, "r");
    
    char *array[100];
    char **otro = array;
    
    if(fo!=NULL){
        char var[40];
        char str[40];
        char allLines[10000]="";
        while( fgets (str, 60, fo)) {
            strcpy(var,str);
            int len = strlen(var);
            //le borre el \n
            if( var[len-1] == '\n' ){
                var[len-1] = 0;
                len = strlen(var);
            }
            strcat(allLines,var);
            strcat(allLines, " ");
        }

        char * token;
        token=strtok(allLines," ");
        int index=0;
        
        while(token !=NULL){
            char *individual;
            individual=token;
            otro[index]=individual;
            index++;
            token=strtok(NULL," "); 
        }

        /*for ( int i = 0; i < 100; i++ ){
            printf("String %d : %s\n", i+1, array[i] );
        }*/

        fclose(fo);
        if (line)
            free(line);

    }else{
        printf("The file does not exist\n");
        return 0;
    }

    int arr_size = sizeof(array) / sizeof(array[0]);
    printf("Given array is \n");
    printArray(array, arr_size);
        
    mergeSort(array, 0, arr_size - 1, (int (*)(void*,void*))(numeric==1 ? numcmp : stringcmp));
    
    printf("\nSorted array is \n");
    printArray(array, arr_size);

    return 0;
}