#include "cfs_functions.h"

cfs_file *cfs_workwith(char *filename) {
    int len=strlen(filename);
    if(len<4) return NULL;
    if(strcmp(filename+(len-4),".cfs")!=0) return NULL;
    int fd;
    fd = open(filename,O_RDWR);
    if(fd==-1) return NULL;
    unsigned int bs,fns,cfs,mdfn;
    pread(fd,&bs,sizeof(unsigned int),0);
    pread(fd,&fns,sizeof(unsigned int),sizeof(unsigned int));
    pread(fd,&cfs,sizeof(unsigned int),2*sizeof(unsigned int));
    pread(fd,&mdfn,sizeof(unsigned int),3*sizeof(unsigned int));
    //test
    cfs_file *b=new cfs_file(fd,bs,fns,cfs,mdfn);
    cfs_elmnt *ce=new cfs_elmnt(b->getFilenameSize());
    ce->readffomfile(fd,4*sizeof(unsigned int));	//read '/' node
    b->setCurrentDir(ce->nodeid);
    ce->print();
    delete ce;
    return b;
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
    if(bs) file->setBlockSize(bs);
    if(fns) file->setFilenameSize(fns);
    if(cfs) file->setMaxFileSize(cfs);
    if(mdfn) file->setMaxDirFileNumber(mdfn);
    //set metadata
    file->info_init();
    //create root dir
    cfs_elmnt *ce=new cfs_elmnt(file->getFilenameSize());
    ce->nodeid=0;
	strcpy(ce->filename,"/");
    ce->filename[1]='\0';
	ce->size=0;
	ce->type='d';
	ce->parent_nodeid=0;
	ce->creation_time=time(NULL);
	ce->access_time=time(NULL);
	ce->modification_time=time(NULL);
	ce->print();
	file->insert(ce,4*sizeof(unsigned int));
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
    		cfs_elmnt *ce=new cfs_elmnt(f_info->getFilenameSize());
    		ce->nodeid=f_info->locate_next_avail();
			strcpy(ce->filename,word);
			ce->filename[strlen(word)]='\0';
			ce->size=0;
			ce->type='f';
			ce->parent_nodeid=f_info->getCurrentDir();
			ce->creation_time=time(NULL);
			ce->access_time=time(NULL);
			ce->modification_time=time(NULL);
			ce->print();
			f_info->insert(ce,4*sizeof(unsigned int));

    	}
    	else{				//if the file exists alter its timestamps

    	}
    	word=strtok(NULL," \t\n");
    }
	return 0;
}