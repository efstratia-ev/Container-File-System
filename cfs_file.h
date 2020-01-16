#ifndef CFS_H
#define CFS_H

#include <unistd.h>
#include <time.h>

using namespace std;

//CFS FILE
class cfs_file {
    int fd;
    unsigned int current_dir;
    unsigned int block_size;
    unsigned int filename_size;
    unsigned int max_file_size;
    unsigned int max_dir_file_number;
public:
    cfs_file(int fd,unsigned int block_size=1024,unsigned int filename_size=20,unsigned int max_file_size=1024,unsigned int max_dir_file_number=50);

    void setBlockSize(unsigned int blockSize);

    void setFilenameSize(unsigned int filenameSize);

    void setMaxFileSize(unsigned int maxFileSize);

    void setMaxDirFileNumber(unsigned int maxDirFileNumber);

    ~cfs_file();
};

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
