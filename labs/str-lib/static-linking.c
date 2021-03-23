#include <stdio.h>
#include <string.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char **argv) {

    if(argc<4){
        printf("Check your parameters\n");
        return 0;
    }else if(strcmp(argv[1],"-add")==0){
        char *len=argv[2];
        char* strOrigin=argv[2];
        char* strAddition=argv[3];
        printf("Initial Lenght      :%i\n", mystrlen(len));
        char* strComplete=mystradd(strOrigin,strAddition);
        printf("New String          :%s\n",strComplete);
        printf("New length          :%i\n",mystrlen(strComplete));
    }else if(strcmp(argv[1],"-find")==0){
        char* strOrigin=argv[2];
        char* strSubstring=argv[3];
        if(mystrfind(strOrigin,strSubstring)<0){
            printf("This substring does not match in the origin string\n");
            return 0;
        }
        printf("['%s'] string was found at [%i] position\n", strSubstring, mystrfind(strOrigin,strSubstring));
    }

    return 0;
}