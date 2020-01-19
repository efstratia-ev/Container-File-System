#include "cfs_file.h"

//CFS FILE
cfs_file::cfs_file(int fd, unsigned int block_size, unsigned int filename_size, unsigned int max_file_size,
                   unsigned int max_dir_file_number) {
    this->fd=fd;
    this->block_size=block_size;
    this->filename_size=filename_size;
    this->max_file_size=max_file_size;
    this->max_dir_file_number=max_dir_file_number;
    this->current_dir=0;
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
    unsigned int offset=4*sizeof(unsigned int)+id*(sizeof(cfs_elmnt)+block_size);
    return 0;
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

int cfs_file::insert(cfs_elmnt *in){
	pwrite(fd,&in,sizeof(in),4*sizeof(unsigned int));	//we write in same spot_fix_
	return 0;
}

void cfs_file::print(){
	cout<<"bs is "<<block_size<<endl;
	cout<<"fns is "<<filename_size<<endl;
	cout<<"cfs is "<<max_file_size<<endl;
	cout<<"mdfn is "<<max_dir_file_number<<endl;
}
