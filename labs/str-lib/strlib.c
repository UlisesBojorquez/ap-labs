int mystrlen(char *str){

    int len;
    for(len=0;str[len]!='\0';len++);

    return len;
}

char *mystradd(char *origin, char *addition){

    char *str1=origin;
    char *str2=addition;

    int i=mystrlen(str1);
    for(int j=0;str2[j]!='\0';j++)  
    {  
        str1[i]=str2[j];  
        i++;  
    }  
    str1[i]='\0';  
    return str1;
}

int mystrfind(char *origin, char *substr){
    
    char *src=origin;
    char *sub=substr;
    
    int i=0;
    int j=0;
    int index;
 
    while (src[i] != '\0') {
        //Start searching the until the src[i] is equal to sub[0]
        while (src[i] != sub[0] && src[i] != '\0'){
            i++;
        }
        //If it does not find anything
        if (src[i] == '\0'){
            return -1;
        }
 
        index = i;
        //If match
        while (src[i] == sub[j] && src[i] != '\0' && sub[j] != '\0') {
            i++;
            j++;
        }
        //If we are at the end of the subtring
        if (sub[j] == '\0'){
            return index;
        }
        //If we read all the source and does not find the substring
        if (src[i] == '\0'){
            return -1;
        }

        i = index + 1;
        j = 0;
   }
}