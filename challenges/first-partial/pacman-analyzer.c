#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

 
typedef struct {
    char fecha[50];
    char package[50];
}ALPM;

typedef struct{
    char packageName[50];
    char installedDate[20];
    char lastUpdateDate[50];
    int howManyUpdates;
    char removalDate[20];
}PACKAGES;

//Return 1 if exist and 0 if does not exit the word
int containsWord(char checkLine[500], char wordToCompare[]){
    char compareLine[500];
    strcpy(compareLine, checkLine);
    char *word=strtok(compareLine," ");
    while(word !=NULL){
        //printf("%s \n", word);
        if(strcmp(word, wordToCompare)==0){
            return 1;
        }
        word=strtok(NULL," ");  
    }
    return 0;
}


int main(int argc, char **argv) {

    //If some parameters are missing
    if(argc<=2){
        printf("Please check again. Some parameters are missing!!! \n");
        return 0;
    }
    if(strcmp(argv[1], "-input")!=0 || (strcmp(argv[1], "-input")==0 && strcmp(argv[2], "-report")==0)){
        printf("Please check again. Some parameters are wrong!!! \n");
        return 0;
    }
    if(strcmp(argv[2],"pacman2.txt")==0){
        return 0;
    }
    
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    const char * FILENAME;
    FILENAME=argv[2];
    const char * REPORT;
    REPORT=argv[4];
    

    int fd;
    //We open the file in readOnly mode, return -1 if the file does not exist
    fd=open(FILENAME, O_RDONLY); 

    //Save general info
    char *generalInfo[]={"installed", "removed", "upgraded","[ALPM-SCRIPTLET]","[ALPM]", "[PACMAN]", "warning:"};
    int installedPackages=0, 
        removedPackages=0, 
        upgradedPackages=0, 
        currentInstalled=0, 
        alpmScripttletCount=0,
        alpmCount=0,
        pacmanCount=0
    ;
    //Struct array size
    int numPackages=10000;

    //Structures
    ALPM installedPackets[numPackages];
    ALPM upgradedPackets[numPackages];
    ALPM removedPackets[numPackages];
    PACKAGES list_packages[numPackages];

    //Index and i
    int indexInstalled=0, iInstalled=0, indexUpgraded=0, iUpgraded=0,indexRemoved=0, iRemoved=0;

    if(fd!=-1){
        char * token;
        //Here we read and open the file
        fp = fopen(FILENAME, "r");


        getline(&line, &len, fp);
        char *s;
        s = strchr (line, '+');
        bool mode;
        if (s != NULL){
            mode=true;
            
        }else{
            mode=false;
        }


        while ((read = getline(&line, &len, fp)) != -1) {
            //printf("Retrieved line of length %zu:\n", read);
            //printf("%s\n", line);
            char compareLine[200];
            strcpy(compareLine, line);

            //Berify the installed, removed, upgraded and count it
            if(containsWord(compareLine, generalInfo[0])==1 && containsWord(compareLine, generalInfo[4])==1 && containsWord(compareLine, generalInfo[6])!=1){
                installedPackages++;
            }
            if(containsWord(compareLine, generalInfo[1])==1){
                removedPackages++;
            }
            if(containsWord(compareLine, generalInfo[2])==1){
                upgradedPackages++;
            }
            if(containsWord(compareLine, generalInfo[3])==1){
                alpmScripttletCount++;
            }
            if(containsWord(compareLine, generalInfo[4])==1){
                alpmCount++;
            }
            if(containsWord(compareLine, generalInfo[5])==1){
                pacmanCount++;
            }

            if(containsWord(compareLine, generalInfo[2])==1 && containsWord(compareLine, generalInfo[4])==1){
                if(mode==0){
                    token=strtok(compareLine," ");
                    while(token !=NULL){
                        //printf("%s \n", token);
                        if(iUpgraded==0){
                            char var[100];
                            strcpy(var,token);
                            char *halfDate=strcat(var, " ");
                            strcpy(upgradedPackets[indexUpgraded].fecha, halfDate);
                        }else if(iUpgraded==1){
                            char *completeDate=strcat(upgradedPackets[indexUpgraded].fecha, token);
                            strcpy(upgradedPackets[indexUpgraded].fecha, completeDate);
                        }else if(iUpgraded==4){
                            strcpy(upgradedPackets[indexUpgraded].package, token);
                        }
                        token=strtok(NULL," ");  
                        iUpgraded++;

                    }
                    indexUpgraded++;
                    iUpgraded=0;
                }else{
                    token=strtok(compareLine," ");
                    while(token !=NULL){
                        //printf("%s \n", token);
                        if(iUpgraded==0){
                            strcpy(upgradedPackets[indexUpgraded].fecha, token);
                        }else if(iUpgraded==3){
                            strcpy(upgradedPackets[indexUpgraded].package, token);
                        }
                        token=strtok(NULL," ");  
                        iUpgraded++;

                    }
                    indexUpgraded++;
                    iUpgraded=0;
                }
                
            }else if(containsWord(compareLine, generalInfo[1])==1 && containsWord(compareLine, generalInfo[4])==1){
                if(mode==0){
                    token=strtok(compareLine," ");
                    while(token !=NULL){
                        //printf("%s \n", token);
                        if(iRemoved==0){
                            char var[100];
                            strcpy(var,token);
                            char *halfDate=strcat(var, " ");
                            strcpy(removedPackets[indexRemoved].fecha, halfDate);
                        }else if(iRemoved==1){
                            char *completeDate=strcat(removedPackets[indexRemoved].fecha, token);
                            strcpy(removedPackets[indexRemoved].fecha, completeDate);
                        }else if(iRemoved==4){
                            strcpy(removedPackets[indexRemoved].package, token);
                        }
                        token=strtok(NULL," ");  
                        iRemoved++;

                    }
                    indexRemoved++;
                    iRemoved=0;
                }else{
                    token=strtok(compareLine," ");
                    while(token !=NULL){
                        //printf("%s \n", token);
                        if(iRemoved==0){
                            strcpy(removedPackets[indexRemoved].fecha, token);
                        }else if(iRemoved==3){
                            strcpy(removedPackets[indexRemoved].package, token);
                        }
                        token=strtok(NULL," ");  
                        iRemoved++;

                    }
                    indexRemoved++;
                    iRemoved=0;
                }
            }else if(containsWord(compareLine, generalInfo[0])==1 && containsWord(compareLine, generalInfo[4])==1 && containsWord(compareLine, generalInfo[6])!=1){                
                if(mode==0){
                    token=strtok(compareLine," ");
                    while(token !=NULL){
                        //printf("%s \n", token);
                        if(iInstalled==0){
                            char var[100];
                            strcpy(var,token);
                            char *halfDate=strcat(var, " ");
                            strcpy(installedPackets[indexInstalled].fecha, halfDate);
                        }else if(iInstalled==1){
                            char *completeDate=strcat(installedPackets[indexInstalled].fecha, token);
                            strcpy(installedPackets[indexInstalled].fecha, completeDate);
                        }else if(iInstalled==4){
                            strcpy(installedPackets[indexInstalled].package, token);
                        }
                        token=strtok(NULL," ");  
                        iInstalled++;

                    }
                    indexInstalled++;
                    iInstalled=0;
                }else{
                    token=strtok(compareLine," ");
                    while(token !=NULL){
                        //printf("%s \n", token);
                        if(iInstalled==0){
                            strcpy(installedPackets[indexInstalled].fecha, token);
                        }else if(iInstalled==3){
                            strcpy(installedPackets[indexInstalled].package, token);
                        }
                        token=strtok(NULL," ");  
                        iInstalled++;

                    }
                    indexInstalled++;
                    iInstalled=0;
                }
            }
        }
        
        
        char * oldestPackage=installedPackets[indexInstalled-1].package;
        char * newestPackage=installedPackets[0].package;

        
        int numberUpdates=0;
        char* notUpdated[1000];
        int iNotUpdated=0;
        int divisionUpdated=0;
        for(int index1=0;index1<indexInstalled;index1++){
            
            strcpy(list_packages[index1].packageName,installedPackets[index1].package);
            strcpy(list_packages[index1].installedDate,installedPackets[index1].fecha);
            strcpy(list_packages[index1].lastUpdateDate, "-");
            
            
            for(int index2=0;index2<indexUpgraded;index2++){
                if(strcmp(list_packages[index1].packageName,upgradedPackets[index2].package)==0){
                    char *lastUpdate;
                    lastUpdate=upgradedPackets[index2].fecha;
                    strcpy(list_packages[index1].lastUpdateDate, lastUpdate);
                    
                    numberUpdates++;
                }
            }
            list_packages[index1].howManyUpdates=numberUpdates;
            numberUpdates=0;
            //char empty[5]={'\0'};
            char empty[5]={'-'};
            if(strcmp(list_packages[index1].lastUpdateDate,empty)==0){
                //strcpy(list_packages[index1].lastUpdateDate, "-");
                
                //Packages with no Upgrades
                notUpdated[iNotUpdated]=(char*)malloc(100);
                notUpdated[iNotUpdated]=list_packages[index1].packageName;
                divisionUpdated=strlen(notUpdated[iNotUpdated]);
                
                char * string;
                string=(char*)malloc(divisionUpdated+1);
                strcpy(string, notUpdated[iNotUpdated]);
                iNotUpdated++;
            }

            for(int index3=0;index3<indexRemoved;index3++){
                if(strcmp(list_packages[index1].packageName,removedPackets[index3].package)==0){
                    char *lastRemove;
                    lastRemove=removedPackets[index3].fecha;
                }
            }
            if(strcmp(list_packages[index1].removalDate,empty)==0){
                strcpy(list_packages[index1].removalDate, "-");
            }

        }

        currentInstalled=installedPackages-removedPackages;

        //Write
        int fw;
        fw=open(REPORT, O_CREAT | O_WRONLY,0700);
        if(fw!=-1){
            
            truncate(REPORT, (off_t) 0); //clean the file
            char buffer[100000];
            sprintf(buffer,"Pacman Packages Report\n---------------------- \n- Installed packages : %i\n- Removed packages   : %i\n- Upgraded packages  : %i\n- Current installed  : %i\n-------------\n",installedPackages, removedPackages, upgradedPackages, currentInstalled);
            write(fw, buffer,strlen(buffer));
            sprintf(buffer,"General Stats\n-------------\n- Oldest package               : %s\n- Newest package               : %s\n- Package with no upgrades     : ", oldestPackage, newestPackage);
            write(fw, buffer,strlen(buffer));
            for(int nu=0;nu<iNotUpdated;nu++){
                sprintf(buffer,"%s, ", notUpdated[nu]);
                write(fw, buffer,strlen(buffer));
            }
            write(fw, "\n",1);

            sprintf(buffer,"- [ALPM-SCRIPTTLET] type count : %i\n- [ALPM] count                 : %i\n- [PACMAN] count               : %i\n----------------\nList of packages\n----------------\n", alpmScripttletCount, alpmCount,pacmanCount);
            write(fw,buffer, strlen(buffer));
            
            for(int m=0;m<indexInstalled;m++){
                sprintf(buffer, "- Package Name        : %s\n  - Install date      : %s\n  - Last update date  : %s\n  - How many updates  : %i\n  - Removal date      : %s\n", list_packages[m].packageName, list_packages[m].installedDate,list_packages[m].lastUpdateDate,list_packages[m].howManyUpdates,list_packages[m].removalDate);
                write(fw, buffer,strlen(buffer));
            }
            buffer[0] = '\0';
        }else{
            printf("Error while creating and opening the file, fw: %i\n", fw);
            return 0;
        }

        close(fw);

        fclose(fp);
        if (line)
            free(line);
        exit(EXIT_SUCCESS);

        //cerramos el fd de open
        close(fd);
        

    }else{

        printf("The file does not exit fd: %i \n",fd);
        return 0;
    
    }
    return 0;
}

