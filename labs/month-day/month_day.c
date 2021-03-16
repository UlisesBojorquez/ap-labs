#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);
char* month_name(int number_of_month);
int isLeap(int year);

int main(int argc, char **argv) {
    
    if(argc<3 || argc>3){
        printf("ERROR!!! Please check again the parameters are correct.\n");
        return 0;
    }

    int year=atoi(argv[1]);
    int yearday=atoi(argv[2]);

    if(isLeap(year)==1){
        if(yearday>366){
            printf("ERROR!!! Please check again the yearday\n");
            return 0;
        }
    }else{
        if(yearday>365){
            printf("ERROR!!! Please check again the yearday\n");
            return 0;
        }
    }

    int month=0, day=0;
    month_day(year,yearday,&month,&day);
    char *month_string=month_name(month);
    printf("%s %i, %i\n", month_string, day, year);

    return 0;
}

static char daytab[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, //no leap
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} //leap
};

char* month_name(int number_of_month){

    switch(number_of_month){
        case 1:
            return "Jan";
        case 2:
            return "Feb";
        case 3:
            return "Mar";
        case 4:
            return "Apr";
        case 5:
            return "May";
        case 6:
            return "Jun";
        case 7:
            return "Jul";
        case 8:
            return "Aug";
        case 9:
            return "Sep";
        case 10:
            return "Oct";
        case 11:
            return "Nov";
        case 12:
            return "Dec";
    }
    printf("entre\n");
}

//If is leap return 1
int isLeap(int year){
    if(year%4 == 0 && year%100 != 0 || year%400 == 0){
        return 1;
    }else{
        return 0;
    }
}

void month_day(int year, int yearday, int *pmonth, int *pday){
    int index_of_daytab;
    int month;

    if(isLeap(year)==1){
        index_of_daytab=1;
    }else{
        index_of_daytab=0;
    }

    for(int i=0;i<12;i++){

        if(yearday<=daytab[index_of_daytab][i]){
            month=i+1;
            *pday=yearday;
            break;
        }else{
            yearday=yearday-daytab[index_of_daytab][i];
        }

    }
    *pmonth=month;
}

