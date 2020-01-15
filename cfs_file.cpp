#include <unistd.h>
#include "cfs_file.h"


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

void cfs_file::setMaxDirFileNumber(unsigned int maxDirFileNumber) {
    max_dir_file_number = maxDirFileNumber;
}
