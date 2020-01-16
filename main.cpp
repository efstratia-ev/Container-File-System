#include <iostream>
#include <cstring>
#include <cstdio>

#include "cfs_functions.h"

using namespace std;

int main() {

    cfs_file *file;

    size_t size=200;	//*temp
    char *line,*command;
    line=(char*)malloc(size*sizeof(char));
    command=(char*)malloc(size*sizeof(char));

    while (true){
        getline(&line, &size, stdin);
        command=strtok(line," \t\n");
        if(strcmp(command,"cfs_workwith")==0){
            char *filename=strtok(NULL," \t");
            if(!filename || !(file=cfs_workwith(filename))){
                cout<<"Invalid Input"<<endl;
            }
        }
        if(strcmp(command,"cfs_create")==0){
            char *arguments=strtok(NULL,"\n");
            if(!arguments || !cfs_create(arguments)){
                cout<<"Invalid Input"<<endl;
            }
        }
        if(strcmp(command,"cfs_exit")==0){
            if(file)  delete file;
            break;
        }
    }

    return 0;
}
