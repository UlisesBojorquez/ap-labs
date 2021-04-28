#define _XOPEN_SOURCE 500
#define MAX_EVENTS 1024 //Max number of Events
#define LEN_NAME 16 //}The length of the file do not exceed this LEN
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) //size of one event
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

#include <ftw.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include "logger.h"

int wd=0; //watch descriptor
int inotifyFd; //inotify dile descriptor

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    if(tflag==FTW_D){
        wd = inotify_add_watch(inotifyFd, fpath, IN_MOVE | IN_CREATE | IN_DELETE);
        if(wd==-1){
            errorf("Error while adding watch to inotify");
        }  
    }
    return 0;
}

int main(int argc, char **argv) {
    initLogger("stdout");
    if(argc!=2){
        errorf("You are missing required parameters!!!\n");
        return 0;
    }
    char buf[BUF_LEN]; //buffer
    char *dir; //directory
    ssize_t numRead; //numread for the read function
    int flags=0;
    dir=argv[1]; //assigning the directory
    inotifyFd=inotify_init(); //init the inotify
    if(inotifyFd<0){
        errorf("Error in the inotify fd!!!\n");
        return 0;
    }
    if (nftw(dir, display_info, 20, flags) == -1) {
       errorf("Error in the nftw creation.");
       return 0;
    }else{
        warnf("Starting File/Directory Monitor on %s\n", dir);
        warnf("-----------------------------------------------------\n");
    }
    while(1){
        int i=0;
        numRead=read(inotifyFd, buf, BUF_LEN);
        if(numRead==-1){
            errorf("Error in the read\n");
            return 0;
        }
        if(numRead==0){
            panicf("read() from inotify fd returned 0!\n");
            return 0;
        }
        while (i < numRead ) {
            struct inotify_event *event=(struct inotify_event*)&buf[i];
            if(event->len){
                if(event-> mask & IN_CREATE){
                    if(event-> mask & IN_ISDIR){
                        infof("- [Directory - Create] - %s\n", event->name);
                    }    
                    else{
                        infof("- [File - Create] - %s\n", event->name);
                    }
                }
                else if(event-> mask & IN_DELETE){
                    if(event-> mask & IN_ISDIR){
                        infof("- [Directory - Removal] - %s\n", event->name);
                    }    
                    else{
                        infof("- [File - Removal] - %s\n", event->name);
                    }
                }
                else if (event->cookie!=0){
                    if(event->mask & IN_MOVED_FROM){
                        if(event-> mask & IN_ISDIR){
                            infof("- [Directory - Rename] - %s -> ", event->name);
                        }    
                        else{
                            infof("- [File - Rename] - %s -> ", event->name);
                        }
                    }
                    else infof("%s\n",event->name);
                }else{
                    infof("unknown action");
                }
                i+=EVENT_SIZE+event->len;
            }
        }
    }
    inotify_rm_watch( inotifyFd, wd );
    close( inotifyFd );
    return 0;
}