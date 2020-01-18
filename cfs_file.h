#ifndef CFS_H
#define CFS_H

#include <sys/types.h>
#include <unistd.h>

#include "cfs_elmnt.h"

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

    ~cfs_file();

    void setBlockSize(unsigned int blockSize);

    void setFilenameSize(unsigned int filenameSize);

    void setMaxFileSize(unsigned int maxFileSize);

    void setMaxDirFileNumber(unsigned int maxDirFileNumber);

    int get_relative_path_dir(char *rel_path,unsigned int id);

    void info_init();

    int exists(char *fn);

    void print();

};



#endif
