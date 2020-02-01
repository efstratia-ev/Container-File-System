#include <unistd.h>
#include <cstdio>
#include "cfs_elmnt.h"

#define ANSI_COLOR_MAGENTA "\x1b[35m"

#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_COLOR_RESET "\x1b[0m"

cfs_elmnt::cfs_elmnt(unsigned int fnsz, char *const filename, unsigned int size, char type, unsigned int parentNodeid,time_t time):filename(new char[fnsz+1]){
    strcpy(this->filename,filename);
    this->size=size;
    this->type=type;
    this->parent_nodeid=parentNodeid;
    this->creation_time=time;
    this->access_time=time;
    this->modification_time=time;
}
cfs_elmnt::~cfs_elmnt(){
	delete[] filename;
}

void cfs_elmnt::print(){
	char * time_str;
	cout<<"nodeid: "<<nodeid<<endl;
	cout<<"filename: "<<filename<<endl;
	cout<<"size: "<<size<<endl;
	cout<<"type: "<<type<<endl;
	cout<<"pnid: "<<parent_nodeid<<endl;
	time_str=ctime(&creation_time);
    time_str[strlen(time_str)-1] = '\0';
    cout<<"c_time: "<<time_str<<endl;
    time_str=ctime(&access_time);
    time_str[strlen(time_str)-1] = '\0';
    cout<<"a_time: "<<time_str<<endl;
    time_str=ctime(&modification_time);
    time_str[strlen(time_str)-1] = '\0';
    cout<<"m_time: "<<time_str<<endl;
}

void cfs_elmnt::readfromfile(int fd,int offset,unsigned int filename_size) {
    offset+=pread(fd,&nodeid,sizeof(unsigned int),offset);
    offset+=pread(fd,filename,filename_size,offset);
    offset+=pread(fd,&size,sizeof(unsigned int),offset);
    offset+=pread(fd,&type,sizeof(char),offset);
    offset+=pread(fd,&parent_nodeid,sizeof(unsigned int),offset);
    offset+=pread(fd,&creation_time,sizeof(time_t),offset);
    offset+=pread(fd,&access_time,sizeof(time_t),offset);
    pread(fd,&modification_time,sizeof(time_t),offset);
}

void cfs_elmnt::writetofile(int fd,int offset,unsigned int filename_size) {
    offset+=pwrite(fd,&nodeid,sizeof(unsigned int),offset);
    offset+=pwrite(fd,filename,filename_size,offset);
    offset+=pwrite(fd,&size,sizeof(unsigned int),offset);
    offset+=pwrite(fd,&type,sizeof(char),offset);
    offset+=pwrite(fd,&parent_nodeid,sizeof(unsigned int),offset);
    offset+=pwrite(fd,&creation_time,sizeof(time_t),offset);
    offset+=pwrite(fd,&access_time,sizeof(time_t),offset);
    pwrite(fd,&modification_time,sizeof(time_t),offset);
}

cfs_elmnt::cfs_elmnt(unsigned int fnsz) :filename(new char[fnsz+1]){}

void cfs_elmnt::ls() {
    if(type=='f') printf(ANSI_COLOR_RESET"%s\n",filename);
    else if(type=='d') printf(ANSI_COLOR_CYAN"%s\n",filename);
    else printf(ANSI_COLOR_MAGENTA"%s\n",filename);
    printf(ANSI_COLOR_RESET);
}

void cfs_elmnt::ls_l() {
    char * ctime_str,*atime_str,*mtime_str;
    ctime_str=ctime(&creation_time);
    ctime_str[strlen(ctime_str)-1] = '\0';
    atime_str=ctime(&access_time);
    atime_str[strlen(atime_str)-1] = '\0';
    mtime_str=ctime(&modification_time);
    mtime_str[strlen(mtime_str)-1] = '\0';
    if(type=='f') printf(ANSI_COLOR_RESET"%u %s %u %s\t%s\t%s\n",nodeid,filename,size,ctime_str,atime_str,mtime_str);
    else if(type=='d') printf(ANSI_COLOR_CYAN"%u %s %u %s\t%s\t%s\n",nodeid,filename,size,ctime_str,atime_str,mtime_str);
    else printf(ANSI_COLOR_MAGENTA"%u %s %u %s\t%s\t%s\n",nodeid,filename,size,ctime_str,atime_str,mtime_str);
    printf(ANSI_COLOR_RESET);
}
