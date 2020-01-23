#include <cstring>
#include "cfs_file.h"
#include "cfs_elmnt.h"
//CFS FILE
cfs_file::cfs_file(int fd) {
    this->fd=fd;

}

void cfs_file::set_info(unsigned int block_size, unsigned int filename_size, unsigned int max_file_size,
                        unsigned int max_dir_file_number) {
    this->block_size=block_size;
    this->filename_size=filename_size;
    this->max_file_size=max_file_size;
    this->max_dir_file_number=max_dir_file_number;
    this->current_dir=0;
    this->element_number=0;
    this->empty_spot=0;
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
    element_size=sizeof(cfs_elmnt)+filename_size;
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
    return get_relative_path_dir(strtok(NULL,"\0"),next_id);
}

void cfs_file::info_init(){
    pwrite(fd,&element_number,sizeof(unsigned int),0);
    pwrite(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
	pwrite(fd,&block_size,sizeof(unsigned int),2*sizeof(unsigned int));
	pwrite(fd,&filename_size,sizeof(unsigned int),3*sizeof(unsigned int));
	pwrite(fd,&max_file_size,sizeof(unsigned int),4*sizeof(unsigned int));
	pwrite(fd,&max_dir_file_number,sizeof(unsigned int),5*sizeof(unsigned int));
}

void cfs_file::get_info(){
    pread(fd,&element_number,sizeof(unsigned int),0);
    pread(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
    pread(fd,&block_size,sizeof(unsigned int),2*sizeof(unsigned int));
    pread(fd,&filename_size,sizeof(unsigned int),3*sizeof(unsigned int));
    pread(fd,&max_file_size,sizeof(unsigned int),4*sizeof(unsigned int));
    pread(fd,&max_dir_file_number,sizeof(unsigned int),5*sizeof(unsigned int));
    element_size=sizeof(cfs_elmnt)+filename_size;
}

int cfs_file::exists(char *fn){		//TODO
	//search in current directory if file exists
	return 0;
}

unsigned int cfs_file::insert_element(cfs_elmnt *in){
    unsigned int spot;
    if(element_number==0) spot=0;
    if(empty_spot!=0) spot=get_next_empty_spot();
    else spot=element_number;
    int offset=6*sizeof(unsigned int)+spot*(element_size+block_size);
    in->nodeid=spot;
    in->writetofile(fd,offset);
    increase_element_number();
    if(spot==0) return spot;
    unsigned int elements,parent=in->parent_nodeid;
    offset=6*sizeof(unsigned int)+parent*(element_size+block_size)+element_size;
    pread(fd,&elements,sizeof(unsigned int),offset);
    int block_offset=offset+sizeof(unsigned int)+elements*(sizeof(unsigned int)+filename_size);
    pwrite(fd,&spot,sizeof(unsigned int),block_offset);
    pwrite(fd,&(in->filename),filename_size,block_offset+sizeof(unsigned int));
    elements++;
    pwrite(fd,&elements,sizeof(unsigned int),offset);
    return spot;
}

void cfs_file::print(){
	cout<<"bs is "<<block_size<<endl;
	cout<<"fns is "<<filename_size<<endl;
	cout<<"cfs is "<<max_file_size<<endl;
	cout<<"mdfn is "<<max_dir_file_number<<endl;
}

void cfs_file::setCurrentDir(unsigned int dir) {
    current_dir=dir;
}

unsigned int cfs_file::getCurrentDir() {
    return current_dir;
}

void cfs_file::increase_element_number() {
    element_number+=1;
    pwrite(fd,&element_number,sizeof(unsigned int),0);
}

unsigned int cfs_file::get_next_empty_spot() {
    unsigned int item=empty_spot;
    int offset=6*sizeof(unsigned int)+empty_spot*(element_size+block_size);
    pread(fd,&empty_spot,sizeof(unsigned int),offset);
    pwrite(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
    return item;
}

void cfs_file::insert_directory(cfs_elmnt *in) {
    unsigned int elements=0,spot=insert_element(in);
    int offset=6*sizeof(unsigned int)+spot*(element_size+block_size)+element_size;
    pwrite(fd,&elements,sizeof(unsigned int),offset);
}

void cfs_file::insert_file(cfs_elmnt *in) { //TODO::den tin teleiosa
    unsigned int spot=insert_element(in); //vazei to file stin epomeni thesi(in) kai sto proxoraei
    //spot einai i thesi pou pires
}


