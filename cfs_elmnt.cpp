#include <unistd.h>
#include "cfs_elmnt.h"

cfs_elmnt::cfs_elmnt(unsigned int fnsz):filename(new char[fnsz+1]){
}
cfs_elmnt::~cfs_elmnt(){
	delete filename;
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

void cfs_elmnt::readffomfile(int fd,int offset) {
    offset+=pread(fd,&nodeid,sizeof(unsigned int),offset);
    offset+=pread(fd,filename,sizeof(filename),offset);
    offset+=pread(fd,&size,sizeof(unsigned int),offset);
    offset+=pread(fd,&type,sizeof(char),offset);
    offset+=pread(fd,&parent_nodeid,sizeof(unsigned int),offset);
    offset+=pread(fd,&creation_time,sizeof(time_t),offset);
    offset+=pread(fd,&access_time,sizeof(time_t),offset);
    pread(fd,&modification_time,sizeof(time_t),offset);
}

void cfs_elmnt::writetofile(int fd,int offset) {
    offset+=pwrite(fd,&nodeid,sizeof(unsigned int),offset);
    offset+=pwrite(fd,filename,sizeof(filename),offset);
    offset+=pwrite(fd,&size,sizeof(unsigned int),offset);
    offset+=pwrite(fd,&type,sizeof(char),offset);
    offset+=pwrite(fd,&parent_nodeid,sizeof(unsigned int),offset);
    offset+=pwrite(fd,&creation_time,sizeof(time_t),offset);
    offset+=pwrite(fd,&access_time,sizeof(time_t),offset);
    pwrite(fd,&modification_time,sizeof(time_t),offset);
}
