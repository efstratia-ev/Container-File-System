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
    cfs_elmnt(unsigned int fnsz, char *const filename, unsigned int size, char type, unsigned int parentNodeid,time_t time);
    ~cfs_elmnt();
    void print();
    void readfromfile(int fd,int offset,unsigned int filename_size);
    void writetofile(int fd,int offset,unsigned int filename_size);
    void ls();
    void ls_l();
};

struct cfs_in_dir {
	char *const filename;
	char type;
	unsigned int nodeid;
};


#endif
