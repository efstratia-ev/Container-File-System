#ifndef CFS_ELMNT_H
#define CFS_ELMNT_H

#include <time.h>

//CFS ELEMENT
typedef struct cfs_elmnt {
    unsigned int nodeid;
    char *filename;		//must be set to filename size
    unsigned int size;	//?
    char type;
    unsigned int parent_nodeid;
    time_t creation_time;
    time_t access_time;
    time_t modification_time;
    //datastream??
} MDS;


#endif
