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
        else if(strcmp(command,"cfs_create")==0){
            delete file;
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !(file=cfs_create(arguments))){
                cout<<">Invalid Input."<<endl;
            }
        }
        else if(strcmp(command,"cfs_mkdir")==0 && file){
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !cfs_mkdir(file,arguments)){
                cout<<">Invalid Input."<<endl;
            }
        }
        else if(strcmp(command,"cfs_cd")==0 && file){
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !cfs_cd(file,arguments)){
                cout<<">Invalid Input."<<endl;
            }
        }
        else if(strcmp(command,"cfs_pwd")==0){
            cfs_pwd(file);
        }
        else if(strcmp(command,"cfs_touch")==0){
            int error_code=0;
            char *arguments=strtok(NULL,"\n");
            if(!arguments || (error_code=cfs_touch(file,arguments))!=0){
                if(error_code==-1) cout<<"this directory is full"<<endl;
                else cout<<">Invalid Input."<<endl;
            }
        }
        else if(strcmp(command,"cfs_ls")==0){
            char *arguments=strtok(NULL,"\n");
            cfs_ls(file,arguments);
        }
        else if(strcmp(command,"cfs_rm")==0){
            char *arguments=strtok(NULL,"\n");
            cfs_rm(file,arguments);
        }
        else if(strcmp(command,"cfs_exit")==0){
            if(file)
                delete file;
            break;
        }
    }
    return 0;
}
