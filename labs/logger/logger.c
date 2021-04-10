/*
REFERENCES
https://www.tutorialspoint.com/cprogramming/c_variable_arguments.htm
https://www.tutorialspoint.com/c_standard_library/stdarg_h.htm
https://stackoverflow.com/questions/5977326/call-printf-using-va-list
https://www.linuxjournal.com/article/8603
https://www.theurbanpenguin.com/4184-2/
https://www.systutorials.com/ascii-table-and-ascii-code/
*/

#include <stdio.h>
#include <stdarg.h>

//attr
#define RESET 0
#define BRIGHT 1
#define DIM 2
#define UNDERLINE 3
#define BLINK 4
#define REVERSE 7
#define HIDDEN 8

//fg, bg
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define	WHITE 7

void textcolor(int attr, int fg, int bg)
{	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

void resetcolor(){
    printf("%c[0m",0x1B);
}

int infof(const char *format, ...){
    textcolor(RESET,BLUE,BLACK);
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    resetcolor();
    return 0;
}

int warnf(const char *format, ...){
    textcolor(RESET,YELLOW,BLACK);
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    resetcolor();
    return 0;
}

int errorf(const char *format, ...){
    textcolor(RESET,RED,BLACK);
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    resetcolor();
    return 0;
}

int panicf(const char *format, ...){
    textcolor(RESET,MAGENTA,BLACK);
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
    resetcolor();
    return 0;
}
