#include <iostream>
#include <cstring>
#include <cstdio>

#include "cfs_functions.h"

using namespace std;

int main() {

    cfs_file *file=NULL;

    size_t size=200;	//_temp_
    char *line,*command;
    line=(char*)malloc(size*sizeof(char));
    command=(char*)malloc(size*sizeof(char));

    while (true){
        getline(&line, &size, stdin);
        command=strtok(line," \t\n");
        if(!command) continue;
        if(strcmp(command,"cfs_workwith")==0){
            delete file;
            char *filename=strtok(NULL," \n");
            if(!filename || !(file=cfs_workwith(filename))){
                cout<<">Invalid Input."<<endl;
            }
            else
            	file->print();
        }
        if(strcmp(command,"cfs_create")==0){
            delete file;
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !(file=cfs_create(arguments))){
                cout<<">Invalid Input."<<endl;
            }
        }
        if(strcmp(command,"cfs_mkdir")==0 && file){
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !cfs_mkdir(file,arguments)){
                cout<<">Invalid Input."<<endl;
            }
        }
        if(strcmp(command,"cfs_cd")==0 && file){
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !cfs_cd(file,arguments)){
                cout<<">Invalid Input."<<endl;
            }
        }
        if(strcmp(command,"cfs_exit")==0){
            if(file)
                delete file;
            break;
        }
    }
    return 0;
}
