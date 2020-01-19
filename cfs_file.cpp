#include <cstring>
#include "cfs_file.h"
#include "cfs_elmnt.h"
//CFS FILE
cfs_file::cfs_file(int fd, unsigned int block_size, unsigned int filename_size, unsigned int max_file_size,
                   unsigned int max_dir_file_number) {
    this->fd=fd;
    this->block_size=block_size;
    this->filename_size=filename_size;
    this->max_file_size=max_file_size;
    this->max_dir_file_number=max_dir_file_number;
    this->current_dir=0;
    element_size=sizeof(cfs_elmnt)+filename_size;
}

cfs_file::~cfs_file() {
    close(fd);
}

void cfs_file::setBlockSize(unsigned int blockSize) {
    block_size = blockSize;
}

void cfs_file::setFilenameSize(unsigned int filenameSize) {
    filename_size = filenameSize;
}

void cfs_file::setMaxFileSize(unsigned int maxFileSize) {
    max_file_size = maxFileSize;
}

unsigned int cfs_file::getFilenameSize() {
	return filename_size;
}

void cfs_file::setMaxDirFileNumber(unsigned int maxDirFileNumber) {
    max_dir_file_number = maxDirFileNumber;
}

int cfs_file::get_relative_path_dir(char *rel_path, unsigned int id) {
    char *dir=strtok(rel_path,"/");
    unsigned int offset=4*sizeof(unsigned int)+id*(sizeof(cfs_elmnt)+block_size)+sizeof(cfs_elmnt);
    unsigned int size;
    int next_id=-1;
    pread(fd,&size,sizeof(unsigned int),offset);
    offset+=sizeof(unsigned int);
    char directory[filename_size];
    for(unsigned int i=0; i<size; i++){
        pread(fd,&directory,filename_size,offset);
        if(strcmp(dir,directory)==0){
            pread(fd,&next_id,sizeof(unsigned int),offset);
            break;
        }
        offset+=block_size+sizeof(unsigned int);
    }
    if(next_id==-1) return -1;
    return get_relative_path_dir(strtok(NULL,'\0'),next_id);
}

void cfs_file::info_init(){
	pwrite(fd,&block_size,sizeof(unsigned int),0);
	pwrite(fd,&filename_size,sizeof(unsigned int),sizeof(unsigned int));
	pwrite(fd,&max_file_size,sizeof(unsigned int),2*sizeof(unsigned int));
	pwrite(fd,&max_dir_file_number,sizeof(unsigned int),3*sizeof(unsigned int));
}

int cfs_file::exists(char *fn){
	//search in current directory if file exists
	return 0;
}

int cfs_file::insert(cfs_elmnt *in,int offset){
    in->writetofile(fd,offset);
    return 0;
}

void cfs_file::print(){
	cout<<"bs is "<<block_size<<endl;
	cout<<"fns is "<<filename_size<<endl;
	cout<<"cfs is "<<max_file_size<<endl;
	cout<<"mdfn is "<<max_dir_file_number<<endl;
}
