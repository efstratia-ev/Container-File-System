#ifndef CFS_ELMNT_H
#define CFS_ELMNT_H

#include <iostream>
#include <cstring>
#include <time.h>

using namespace std;

//CFS ELEMENT
struct cfs_elmnt {
    unsigned int nodeid;
    char *const filename;		//must be set to filename size
    unsigned int size;	//bytes if file,items inside if dir
    char type;
    unsigned int parent_nodeid;
    time_t creation_time;
    time_t access_time;
    time_t modification_time;

    cfs_elmnt(unsigned int fnsz);
    ~cfs_elmnt();
    void print();
    void readffomfile(int fd,int offset);
    void writetofile(int fd,int offset);
};

#endif
