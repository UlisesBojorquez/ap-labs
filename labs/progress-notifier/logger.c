/*
REFERENCES
https://sites.google.com/site/itmyshare/system-admin-tips-and-tools/write-to-syslog
https://drive.google.com/file/d/1lithSbEKl2EdZg1Of92LBwdi890hk609/view
*/

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include "logger.h"

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

char isSyslog=0;

void textcolor(int attr, int fg)
{	char command[13];
	sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
	printf("%s", command);
}

void resetcolor(){
    printf("%c[0m",0x1B);
}

int initLogger(char *logType) {
    textcolor(RESET,GREEN);

    if(strcmp(logType, "stdout") == 0) {
        isSyslog=0;
    } else if(strcmp(logType, "syslog") == 0) {
        isSyslog=1;
    }
    printf("Initializing Logger on: %s\n", logType);
    resetcolor();
    return 0;
}

void printLog(int priority,const char *format,va_list args){
    if(isSyslog){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(priority, format, args);
        closelog();
    }
    else{
        vprintf(format, args);
    }
}

int infof(const char *format, ...){
    textcolor(RESET,BLUE);
    va_list args;
    va_start(args,format);
    printLog(LOG_INFO,format,args);
    va_end(args);
    resetcolor();
}

int warnf(const char *format, ...){
    textcolor(RESET,YELLOW);
    va_list args;
    va_start(args,format);
    printLog(LOG_WARNING,format,args);
    va_end(args);
    resetcolor();
}

int errorf(const char *format, ...){
    textcolor(RESET,RED);
    va_list args;
    va_start(args,format);
    printLog(LOG_ERR,format,args);
    va_end(args);
    resetcolor();
}

int panicf(const char *format, ...){
    textcolor(RESET,MAGENTA);
    va_list args;
    va_start(args,format);
    printLog(LOG_EMERG,format,args);
    va_end(args);
    resetcolor();
    raise(SIGABRT);
}