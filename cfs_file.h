#ifndef CFS_H
#define CFS_H

#include <sys/types.h>
#include <dirent.h>

#include "cfs_elmnt.h"
#include "QuickSort.h"

using namespace std;

//CFS FILE
class cfs_file{
    int fd;
    unsigned int element_size;
    unsigned int element_number;
    unsigned int empty_spot;
    unsigned int current_dir;
    unsigned int block_size;
    unsigned int filename_size;
    unsigned int max_file_size;
    unsigned int max_dir_file_number;
public:
    cfs_file(int fd);

    void set_info(unsigned int block_size=1024,unsigned int filename_size=20,unsigned int max_file_size=1024,unsigned int max_dir_file_number=20);

    ~cfs_file();

    void setBlockSize(unsigned int blockSize);

    unsigned int getCurrentDir();

    void setFilenameSize(unsigned int filenameSize);

    unsigned int getFilenameSize();

    void setMaxFileSize(unsigned int maxFileSize);

    void setMaxDirFileNumber(unsigned int maxDirFileNumber);

    unsigned int get_relative_path_dir(char *rel_path,unsigned int id);

    void info_init();

    void get_info();

    bool exists(char *fn, unsigned int dir, unsigned int &to_id);

    unsigned int insert_element(cfs_elmnt *in);

    void print();

    void increase_element_number();

    unsigned int get_next_empty_spot();

    void set_empty_spot(unsigned int spot);

    unsigned int insert_directory(cfs_elmnt *in);

    bool insert_file(cfs_elmnt *in);

    bool insert_link(cfs_elmnt *in,unsigned int file_id);

    unsigned int get_directory_id(unsigned int parent_dir,char *dir_name);

    void move_to_parent_dir();

    void setCurrentDir(unsigned int currentDir);

    char get_element_type(unsigned int id);

    bool dir_is_full(unsigned int id);

    int reset_timestamps(unsigned int id,bool a,bool m);

    void print_directory(unsigned int id);

    bool ls(int level,unsigned int dir,bool a,bool r,bool l, bool u,bool d,bool h);

    bool rm(unsigned int dir,bool i,bool r);

    void rm_file(unsigned int spot);

    void move_dir_element(unsigned int dir, unsigned int spot, unsigned int element);

    bool cp(unsigned int source,unsigned int dest_id,bool r,bool i,bool l,bool R);

    bool cp(unsigned int source, char * destination,bool r,bool i,bool l,bool R);

    bool rename(char *source,char *dest);

    bool mv(char *source,char *dest,bool i);

    bool import_file(char *path,char *directory);

    bool import_file(char *path,unsigned int dest_id);
};



#endif
