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
    cfs_elmnt *ce=new cfs_elmnt(0,(char*)"/",0,'d',0,time(NULL));
	file->insert_directory(ce);
	delete ce;
    return file;
}

int cfs_touch(cfs_file *f_info,char *arguments) {
	char *word=strtok(arguments," \t");
	unsigned int file_id=0;
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
    	if(!f_info->exists(word,f_info->getCurrentDir(),file_id)){	//if the file doesnt exist create it
    		cfs_elmnt *ce=new cfs_elmnt(f_info->getFilenameSize(),word,0,'f',f_info->getCurrentDir(),time(NULL));
			if(!f_info->insert_file(ce)) return -1;
			delete ce;
    	}
    	else{				//if the file exists alter its timestamps
    		f_info->reset_timestamps(file_id,a,m);
    	}
    	word=strtok(NULL," \t\n");
    }
	return 0;
}

bool cfs_mkdir(cfs_file *f_info, char *arguments) {
    char *dir=strtok(arguments," \t");
    bool succes=false;
    while(dir){
        cfs_elmnt *element=new cfs_elmnt(f_info->getFilenameSize(),dir,0,'d',f_info->getCurrentDir(),time(NULL));
        succes=f_info->insert_directory(element);
        delete element;
        dir=strtok(NULL," \t");
    }
    return succes;
}

bool cfs_cd(cfs_file *f_info, char *arguments){
    unsigned int dir;
    char *next_dir;
    if(strcmp(arguments,".")==0) return true;
    else if(strcmp(arguments,"..")==0){
        f_info->move_to_parent_dir();
        return true;
    }
    if(arguments[0]=='/'){
        dir=0;
        strtok(arguments,"/");
        next_dir=strtok(NULL,"/");
    }
    else{
        dir=f_info->getCurrentDir();
        next_dir=strtok(arguments,"/");
    }
    while(next_dir){
        dir=f_info->get_directory_id(dir,next_dir);
        if(dir==0) return false;
        next_dir=strtok(NULL,"/");
    }
    f_info->setCurrentDir(dir);
    return true;
}

void cfs_pwd(cfs_file *f_info) {
    f_info->print_directory(f_info->getCurrentDir());
    cout<<endl;
}

int cfs_ls(cfs_file *f_info,char *arguments) {
    char *word=strtok(arguments," \t");
    unsigned int file_id=0;
    bool a=false,r=false,l=false,u=false,d=false,h=false,dest=false;
    while(word){
        if(strcmp(word,"-a")==0){
            a=true;
        }
        else if(strcmp(word,"-r")==0){
            r=true;
        }
        else if(strcmp(word,"-l")==0){
            l=true;
        }
        else if(strcmp(word,"-u")==0){
            u=true;
        }
        else if(strcmp(word,"-d")==0){
            d=true;
        }
        else if(strcmp(word,"-h")==0){
            h=true;
        }
        else break;
        word=strtok(NULL," \t");
    }
    while(word){
        dest=true;
        if(!f_info->exists(word,f_info->getCurrentDir(),file_id)) continue;
        else f_info->ls(0,file_id,a,r,l,u,d,h);
        word=strtok(NULL," \t\n");
        cout<<endl;
    }
    if(!dest) f_info->ls(0,f_info->getCurrentDir(),a,r,l,u,d,h);
    return 0;
}

int cfs_rm(cfs_file *f_info, char *arguments) {
    char *word=strtok(arguments," \t");
    unsigned int file_id=0;
    bool i=false,r=false;
    while(word){
        if(strcmp(word,"-i")==0){
            i=true;
        }
        else if(strcmp(word,"-r")==0){
            r=true;
        }
        else break;
        word=strtok(NULL," \t");
    }
    while(word){
        if(f_info->exists(word,f_info->getCurrentDir(),file_id)){
            if(f_info->rm(file_id,i,r)) f_info->rm_file(file_id);
        }
        word=strtok(NULL," \t\n");
    }
    return 0;
}

bool cfs_ln(cfs_file *f_info,char *arguments){
    char *org=strtok(arguments," \t");
    char *lnk=strtok(NULL," \t");
    unsigned int file_id;
    bool success=false;
    if(org && lnk){
        if(!f_info->exists(org,f_info->getCurrentDir(),file_id))    //if the file doesnt exist there can be no link
            cout<<">File doesnt exist thus there can be no link."<<endl;
        else if(f_info->get_element_type(file_id)=='d')             //cant link a directory
            cout<<">There can not be a link to a directory."<<endl;
        else{               //if the file exists create link
            cfs_elmnt *element=new cfs_elmnt(f_info->getFilenameSize(),lnk,0,'l',f_info->getCurrentDir(),time(NULL));
            success=f_info->insert_link(element,file_id);
            delete element;
        }
    }
    return success;
}

bool cfs_cat(cfs_file *f_info,char *arguments){
    char *or_fl=strtok(arguments," \t");
    bool success=false;
    unsigned int file_id;
    while(or_fl && !strcmp(or_fl,"-o")){    //source files
        //keep files in a list to retrieve their contents
        or_fl=strtok(NULL," \t");
    }
    char *dest=strtok(arguments," \t\n");
     cout<<dest<<endl;
    if(!f_info->exists(dest,f_info->getCurrentDir(),file_id)){
        cfs_elmnt *ce=new cfs_elmnt(f_info->getFilenameSize(),dest,0,'f',f_info->getCurrentDir(),time(NULL));
        if(!f_info->insert_file(ce)){
            cout<<">File could not be created."<<endl;
            delete ce;
            return false;
        }
        else success=true;
    }
    //copy data to dest
    return success;
}

bool cfs_cp(cfs_file *f_info, char *arguments) {
    char *word;
    if(arguments[0]=='-') word=strtok(arguments," \t");
    else word=arguments;
    unsigned int file_id=0;
    bool i=false,r=false,l=false,R=false;
    while(word){
        if(strcmp(word,"-i")==0){
            i=true;
        }
        else if(strcmp(word,"-r")==0){
            r=true;
        }
        else if(strcmp(word,"-l")==0){
            l=true;
        }
        else if(strcmp(word,"-R")==0){
            R=true;
        }
        else break;
        arguments=strtok(NULL,"\n");
        if(arguments[0]=='-') word=strtok(NULL," \t");
        else word=arguments;
    }
    src_dest *INFO=new src_dest(word);
    char *dest=INFO->get_dest(),*source;
    unsigned int id;
    while((source=INFO->get_source())){
        if(!f_info->exists(source,f_info->getCurrentDir(),id)) continue;
        f_info->cp(id,dest,r,i,l,R);
        delete[] source;
    }
    return true;
}

bool cfs_mv(cfs_file *f_info, char *arguments) {
    char *word;
    if(arguments[0]=='-') word=strtok(arguments," \t");
    else word=arguments;
    bool i=false;
    while(word){
        if(strcmp(word,"-i")==0){
            i=true;
        }
        else break;
        arguments=strtok(NULL,"\n");
        if(arguments[0]=='-') word=strtok(NULL," \t");
        else word=arguments;
    }
    src_dest *INFO=new src_dest(word);
    char *dest=INFO->get_dest(),*source;
    unsigned int id;
    if(INFO->rename())
        return f_info->rename(INFO->get_source(),dest);
    while((source=INFO->get_source())){
        f_info->mv(source,dest,i);
        delete[] source;
    }
    return true;
}

bool cfs_import(cfs_file *f_info, char *arguments) {
    char *word;
    if(arguments[0]=='-') word=strtok(arguments," \t");
    else word=arguments;
    unsigned int file_id=0;

    src_dest *INFO=new src_dest(word);
    char *dest=INFO->get_dest(),*source;
    unsigned int id;
    while((source=INFO->get_source())){
        f_info->import_file(source,dest);
        delete[] source;
    }
    return true;
}
