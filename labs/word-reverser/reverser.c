#include <stdio.h>
#include <string.h>
#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */

int main()

{
    int i, state;
    char c, word[100], reverse[100];
    state = OUT;

    i = 0;
    int len=0;
    while ((c = getchar()) != EOF) {    
        if (c == '\n' || c == '\t') {
            state = OUT;
            
            //printf("%s \n", word);
            
            //REVERSE
            len=i-1;
            for(int k=0;k<i;k++){
                reverse[k]=word[len];
                len--;
            }
            printf("%s \n", reverse);
            
            memset(&word[0], 0, sizeof(word));
            memset(&reverse[0], 0, sizeof(reverse));

            i=0;
            
        }
        else if (state == OUT) {
            state = IN;
            word[i]=c;
            i++;
            
        }else{
            word[i]=c;
            i++;
        }
    }

    return 0;
}