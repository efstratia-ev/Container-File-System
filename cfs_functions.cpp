#include "cfs_functions.h"

cfs_file *cfs_workwith(char *filename) {
    int len=strlen(filename);
    if(len<4) return NULL;
    if(strcmp(filename+(len-4),".cfs")!=0) return NULL;
    int fd;
    fd = open(filename,O_RDWR);
    if(fd==-1) return NULL;
    cfs_file *current_file=new cfs_file(fd);
    current_file->get_info();
    return current_file;
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
        word=strtok(NULL," \t");
    }
    int len=strlen(word);
    if(len<4) return NULL;
    if(strcmp(word+(len-4),".cfs")!=0) return NULL;
    int fd;
    fd = open(word,O_RDWR | O_CREAT, 00755);
    if(fd==-1) return NULL;
    cfs_file *file=new cfs_file(fd);
    file->set_info();
    if(bs) file->setBlockSize(bs);
    if(fns) file->setFilenameSize(fns);
    if(cfs) file->setMaxFileSize(cfs);
    if(mdfn) file->setMaxDirFileNumber(mdfn);
    //set metadata
    file->info_init();
    //create root dir
    cfs_elmnt *ce=new cfs_elmnt(0,"/",0,'d',0,time(NULL));
	ce->print();
	file->insert_directory(ce);
	delete ce;
    return file;
}

int cfs_touch(cfs_file *f_info,char *arguments) {
	char *word=strtok(arguments," \t");
    bool a=false,m=false;
    while(word){
        if(strcmp(word,"-a")==0){
            a=true;
        }
        else if(strcmp(word,"-m")==0){
            m=true;
        }
        else break;
        word=strtok(NULL," \t");
    }
    while(word){
    	if(!f_info->exists(word)){	//if the file doesnt exist create it
    		cfs_elmnt *ce=new cfs_elmnt(f_info->getFilenameSize(),word,0,'f',f_info->getCurrentDir(),time(NULL));
			f_info->insert_file(ce); //den dineis thesi vriskei moni tis
			delete ce;
    	}
    	else{				//if the file exists alter its timestamps

    	}
    	word=strtok(NULL," \t\n");
    }
	return 0;
}