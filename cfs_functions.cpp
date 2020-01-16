#include "cfs_functions.h"

cfs_file *cfs_workwith(char *filename) {
    int len=strlen(filename);
    if(len<4) return NULL;
    if(strcmp(filename+(len-4),".cfs_file")!=0) return NULL;
    int fd;
    fd = open(filename,O_RDWR);
    if(fd==-1) return NULL;
    unsigned int bs,fns,cfs,mdfn;
    read(fd,&bs,sizeof(unsigned int));
    read(fd,&fns,sizeof(unsigned int));
    read(fd,&cfs,sizeof(unsigned int));
    read(fd,&mdfn,sizeof(unsigned int));
    return new cfs_file(fd,bs,fns,cfs,mdfn);
}

cfs_file *cfs_create(char *arguments) {
    char *word=strtok(arguments," \t");
    unsigned int bs=0,fns=0,cfs=0,mdfn=0;
    while(word){
        if(strcmp(word,"-bs")==0){
            if(bs!=0) return NULL;
            char *num=strtok(NULL," \t");
            if(!check_int(num)) return NULL;
            bs=atoi(num);
            if(bs<=0) return NULL;
        }
        else if(strcmp(word,"-fns")==0){
            if(fns!=0) return NULL;
            char *num=strtok(NULL," \t");
            if(!check_int(num)) return NULL;
            fns=atoi(num);
            if(fns<=0) return NULL;
        }
        else if(strcmp(word,"-cfs")==0){
            if(cfs!=0) return NULL;
            char *num=strtok(NULL," \t");
            if(!check_int(num)) return NULL;
            cfs=atoi(num);
            if(cfs<=0) return NULL;
        }
        else if(strcmp(word,"-mdfn")==0){
            if(mdfn!=0) return NULL;
            char *num=strtok(NULL," \t");
            if(!check_int(num)) return NULL;
            mdfn=atoi(num);
            if(mdfn<=0) return NULL;
        }
        else break;
    }
    int len=strlen(word);
    if(len<4) return NULL;
    if(strcmp(word+(len-4),".cfs_file")!=0) return NULL;
    int fd;
    fd = open(word,O_RDWR);
    if(fd==-1) return NULL;
    cfs_file *file=new cfs_file(fd);
    if(bs) file->setBlockSize(bs);
    if(fns) file->setFilenameSize(fns);
    if(cfs) file->setMaxFileSize(cfs);
    if(mdfn) file->setMaxDirFileNumber(mdfn);
    return file;
}

int cfs_touch(char *arguments) {
	return 0;
}