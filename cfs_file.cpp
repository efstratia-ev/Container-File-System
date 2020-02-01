#include <cstring>
#include <cstdio>
#include "cfs_file.h"
#include "QuickSort.h"
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
    unsigned int offset=4*sizeof(unsigned int)+id*(sizeof(cfs_elmnt)+max_file_size)+sizeof(cfs_elmnt);
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
        offset+=max_file_size+sizeof(unsigned int);
    }
    if(next_id==-1) return -1;
    return get_relative_path_dir(strtok(NULL,"\0"),next_id);
}

void cfs_file::info_init(){
    pwrite(fd,&element_number,sizeof(unsigned int),0);
    pwrite(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
	pwrite(fd,&max_file_size,sizeof(unsigned int),2*sizeof(unsigned int));
	pwrite(fd,&filename_size,sizeof(unsigned int),3*sizeof(unsigned int));
	pwrite(fd,&max_file_size,sizeof(unsigned int),4*sizeof(unsigned int));
	pwrite(fd,&max_dir_file_number,sizeof(unsigned int),5*sizeof(unsigned int));
}

void cfs_file::get_info(){
    pread(fd,&element_number,sizeof(unsigned int),0);
    pread(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
    pread(fd,&max_file_size,sizeof(unsigned int),2*sizeof(unsigned int));
    pread(fd,&filename_size,sizeof(unsigned int),3*sizeof(unsigned int));
    pread(fd,&max_file_size,sizeof(unsigned int),4*sizeof(unsigned int));
    pread(fd,&max_dir_file_number,sizeof(unsigned int),5*sizeof(unsigned int));
    element_size=sizeof(cfs_elmnt)+filename_size;
}

bool cfs_file::exists(char *fn, unsigned int dir, unsigned int &to_id){
    int offset=6*sizeof(unsigned int)+dir*(element_size+max_file_size)+element_size;
    int counter;
    pread(fd,&counter,sizeof(unsigned int),offset);
    offset+=sizeof(unsigned int);
    unsigned int id;
    char *filename=new char[filename_size+1];
    for(int i=0; i<counter; i++){
        pread(fd,&id,sizeof(unsigned int),offset);
        offset+=sizeof(unsigned int);
        pread(fd,filename,filename_size,offset);
        offset+=filename_size;
        if(strcmp(fn,filename)==0){
        	to_id=id;
            delete[] filename;
            return true;
        }
    }
    delete[] filename;
	return false;
}

unsigned int cfs_file::insert_element(cfs_elmnt *in){
    unsigned int spot;
    if(element_number==0) spot=0;
    else if(empty_spot!=0) spot=get_next_empty_spot();
    else spot=element_number;
    int offset=6*sizeof(unsigned int)+spot*(element_size+max_file_size);
    in->nodeid=spot;
    in->writetofile(fd,offset,filename_size);
    increase_element_number();
    if(spot==0) return spot;
    unsigned int elements,parent=in->parent_nodeid;
    offset=6*sizeof(unsigned int)+parent*(element_size+max_file_size);
    cfs_elmnt *elmnt=new cfs_elmnt(filename_size);
    elmnt->readfromfile(fd,offset,filename_size);
    elmnt->size+=1;
    elmnt->writetofile(fd,offset,filename_size);
    delete elmnt;
    offset=6*sizeof(unsigned int)+parent*(element_size+max_file_size)+element_size;
    pread(fd,&elements,sizeof(unsigned int),offset);
    int block_offset=offset+sizeof(unsigned int)+elements*(sizeof(unsigned int)+filename_size);
    pwrite(fd,&spot,sizeof(unsigned int),block_offset);
    pwrite(fd,(in->filename),filename_size,block_offset+sizeof(unsigned int));
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

unsigned int cfs_file::getCurrentDir() {
    return current_dir;
}

void cfs_file::increase_element_number() {
    element_number+=1;
    pwrite(fd,&element_number,sizeof(unsigned int),0);
}

unsigned int cfs_file::get_next_empty_spot() {
    unsigned int item=empty_spot;
    int offset=6*sizeof(unsigned int)+empty_spot*(element_size+max_file_size);
    pread(fd,&empty_spot,sizeof(unsigned int),offset);
    pwrite(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
    return item;
}

void cfs_file::set_empty_spot(unsigned int spot) {
    int offset=6*sizeof(unsigned int)+spot*(element_size+max_file_size);
    pwrite(fd,&empty_spot,sizeof(unsigned int),offset);
    empty_spot=spot;
    pwrite(fd,&empty_spot,sizeof(unsigned int),sizeof(unsigned int));
    element_number--;
    pwrite(fd,&element_number,sizeof(unsigned int),0);
}

bool cfs_file::insert_directory(cfs_elmnt *in) {
	unsigned int none;
    if(element_number>0 && (dir_is_full(in->parent_nodeid) || exists(in->filename,in->parent_nodeid,none))) return false;
    unsigned int elements=0,spot=insert_element(in);
    int offset=6*sizeof(unsigned int)+spot*(element_size+max_file_size)+element_size;
    pwrite(fd,&elements,sizeof(unsigned int),offset);
    return true;
}

bool cfs_file::insert_file(cfs_elmnt *in) {
    if(element_number>0 && (dir_is_full(in->parent_nodeid))) return false;
    unsigned int spot=insert_element(in);
    return true;
}

unsigned int cfs_file::get_directory_id(unsigned int parent_dir, char *dir_name) {
    int offset=6*sizeof(unsigned int)+parent_dir*(element_size+max_file_size)+element_size;
    int counter;
    pread(fd,&counter,sizeof(unsigned int),offset);
    offset+=sizeof(unsigned int);
    unsigned int id;
    char *filename=new char[filename_size+1];
    for(int i=0; i<counter; i++){
        pread(fd,&id,sizeof(unsigned int),offset);
        offset+=sizeof(unsigned int);
        pread(fd,filename,filename_size,offset);
        offset+=filename_size;
        if(strcmp(dir_name,filename)==0 && get_element_type(id)=='d'){
            delete[] filename;
            return id;
        }
    }
    delete[] filename;
    return 0;
}

void cfs_file::setCurrentDir(unsigned int currentDir) {
    current_dir = currentDir;
}

void cfs_file::move_to_parent_dir() {
    if(current_dir==0) return;
    int offset=6* sizeof(unsigned int)+current_dir*(element_size+max_file_size);
    cfs_elmnt *elmnt=new cfs_elmnt(filename_size);
    elmnt->readfromfile(fd,offset,filename_size);
    current_dir=elmnt->parent_nodeid;
    delete elmnt;
}

char cfs_file::get_element_type(unsigned int id) {
    int offset=6*sizeof(unsigned int)+id*(element_size+max_file_size);
    cfs_elmnt elmnt(filename_size);
    elmnt.readfromfile(fd,offset,filename_size);
    return elmnt.type;
}

bool cfs_file::dir_is_full(unsigned int id) {
    unsigned int elements,parent=id;
    int offset=6*sizeof(unsigned int)+parent*(element_size+max_file_size)+element_size;
    pread(fd,&elements,sizeof(unsigned int),offset);
    return elements==max_dir_file_number;
}

int cfs_file::reset_timestamps(unsigned int id,bool a,bool m){
	cfs_elmnt *ce=new cfs_elmnt(filename_size);
	ce->readfromfile(fd,6*sizeof(unsigned int)+id*(element_size+max_file_size),filename_size);
	if(ce->type!='f')
		return -1;
	if(!(a||m)){
		ce->access_time=time(NULL);
		ce->modification_time=time(NULL);
	}
	else if(a)
		ce->access_time=time(NULL);
	else
		ce->modification_time=time(NULL);
	ce->writetofile(fd,6*sizeof(unsigned int)+id*(element_size+max_file_size),filename_size);
	delete ce;
	return 0;
}

void cfs_file::print_directory(unsigned int id) {
    cfs_elmnt elmnt(filename_size);
    elmnt.readfromfile(fd,6*sizeof(unsigned int)+id*(element_size+max_file_size),filename_size);
    if(id!=0){
        print_directory(elmnt.parent_nodeid);
        if(elmnt.parent_nodeid!=0) cout<<"/";
    }
    cout<<elmnt.filename;
}

bool cfs_file::ls(int level,unsigned int dir, bool a, bool r, bool l, bool u, bool d, bool h) {
    int offset=6* sizeof(unsigned int)+dir*(element_size+max_file_size);
    cfs_elmnt *directory=new cfs_elmnt(filename_size);
    directory->readfromfile(fd,offset,filename_size);
    if(directory->type!='d') return false;
    for(int lvl=0; lvl<level; lvl++) cout<<"\t";
    cout<<directory->filename<<":"<<endl;
    offset+=element_size;
    unsigned int elements;
    int elmnt_offset;
    pread(fd,&elements, sizeof(unsigned int),offset);
    offset+=sizeof(unsigned int);
    if(elements==0){
        cout<<endl;
        return true;
    }
    cfs_elmnt **files=new cfs_elmnt*[elements];
    for(int i=0; i<elements; i++){
        files[i]=new cfs_elmnt(filename_size);
        pread(fd,&elmnt_offset,sizeof(unsigned int),offset);
        elmnt_offset=6*sizeof(unsigned int)+elmnt_offset*(element_size+max_file_size);
        files[i]->readfromfile(fd,elmnt_offset,filename_size);
        offset+=sizeof(unsigned int)+filename_size;
    }
    if(!u) quickSort(files,0,elements-1);
    for(int i=0; i<elements; i++){
        if(!a && files[i]->filename[0]=='.') continue;
        if(d && h && files[i]->type=='f') continue;
        else if(d && files[i]->type!='d') continue;
        else if(h && files[i]->type!='h') continue;
        for(int lvl=0; lvl<level; lvl++) cout<<"\t";
        if(l) files[i]->ls_l();
        else files[i]->ls();
    }
    cout<<endl;
    if(r) {
        for (int i = 0; i < elements; i++) {
            if (files[i]->type == 'd') ls(level+1,files[i]->nodeid, a, r, l, u, d, h);
        }
    }
    for (int i = 0; i < elements; i++) delete files[i];
    delete[] files;
    delete directory;
    return true;
}

bool cfs_file::rm(unsigned int dir, bool i, bool r) {
    int elmnt_offset,offset=6* sizeof(unsigned int)+dir*(element_size+max_file_size);
    cfs_elmnt *directory=new cfs_elmnt(filename_size);
    directory->readfromfile(fd,offset,filename_size);
    if(directory->type!='d'){
        set_empty_spot(dir);
        return true;
    }
    offset+=element_size;
    unsigned int elements;
    pread(fd,&elements, sizeof(unsigned int),offset);
    offset+=sizeof(unsigned int);
    if(elements==0){
        if(r){
            if(i){
                char opt[4];
                while(true) {
                    cout << "Are you sure you want to delete " << directory->filename << "?(yes/no): ";
                    scanf("%3s",opt);
                    if(strcmp(opt,"yes")==0) break;
                    else if(strcmp(opt,"no")==0) break;
                    else cout<<"You have to answer with yes or no"<<endl;
                }
                if(strcmp(opt,"yes")==0){
                    set_empty_spot(dir);
                    return true;
                }
            }
            else{
                set_empty_spot(dir);
                return true;
            }
        }
        return false;
    }
    cfs_elmnt elmnt(filename_size);
    int j=0;
    while(j<elements){
        pread(fd,&elmnt_offset,sizeof(unsigned int),offset);
        elmnt_offset=6*sizeof(unsigned int)+elmnt_offset*(element_size+max_file_size);
        elmnt.readfromfile(fd,elmnt_offset,filename_size);
        if(r && elmnt.type=='d' && rm(elmnt.nodeid,i,r)){
            move_dir_element(dir,j,elements-1);
            elements--;
            continue;
        }
        else{
            if(i){
                char opt[4];
                while(true) {
                    cout << "Are you sure you want to delete " << elmnt.filename << "?(yes/no): ";
                    scanf("%3s",opt);
                    if(strcmp(opt,"yes")==0) break;
                    else if(strcmp(opt,"no")==0) break;
                    else cout<<"You have to answer with yes or no"<<endl;
                }
                if(strcmp(opt,"yes")==0){
                    set_empty_spot(elmnt.nodeid);
                    move_dir_element(dir,j,elements-1);
                    elements--;
                    continue;
                }
            }
            else{
                set_empty_spot(elmnt.nodeid);
                move_dir_element(dir,j,elements-1);
                elements--;
                continue;
            }
        }
        offset+=sizeof(unsigned int)+filename_size;
        j++;
    }
    offset=6* sizeof(unsigned int)+dir*(element_size+max_file_size);
    pwrite(fd,&elements, sizeof(unsigned int),offset);
    if(r && element_number==0){
        if(i){
            char opt[4];
            while(true) {
                cout << "Are you sure you want to delete " << directory->filename << "?(yes/no): ";
                scanf("%3s",opt);
                if(strcmp(opt,"yes")==0) break;
                else if(strcmp(opt,"no")==0) break;
                else cout<<"You have to answer with yes or no"<<endl;
            }
            if(strcmp(opt,"yes")==0){
                set_empty_spot(dir);
                return true;
            }
        }
        else{
            set_empty_spot(dir);
            return true;
        }
    }
    delete directory;
    return false;
}

void cfs_file::rm_file(unsigned int id) {
    unsigned int spot;
    int offset=6* sizeof(unsigned int)+current_dir*(element_size+max_file_size)+element_size;
    unsigned int elements;
    pread(fd,&elements, sizeof(unsigned int),offset);
    int offset_spot=offset+sizeof(unsigned int);
    for(int i=0; i<elements; i++){
        pread(fd,&spot,sizeof(unsigned int),offset_spot);
        if(spot==id){
            spot=i;
            break;
        }
        offset_spot+=sizeof(unsigned int)+filename_size;
    }
    move_dir_element(current_dir,spot,elements-1);
    elements--;
    pwrite(fd,&elements, sizeof(unsigned int),offset);
}

void cfs_file::move_dir_element(unsigned int dir, unsigned int spot, unsigned int element) {
    int dir_offset= 6*sizeof(unsigned int)+dir*(element_size+max_file_size)+element_size+sizeof(unsigned int);
    int offset=dir_offset+element*(sizeof(unsigned int)+filename_size);
    unsigned int id;
    char *filename=new char[filename_size];
    pread(fd,&id,sizeof(unsigned int),offset);
    pread(fd,filename,filename_size,offset+sizeof(unsigned int));
    offset=dir_offset+spot*(sizeof(unsigned int)+filename_size);
    pwrite(fd,&id,sizeof(unsigned int),offset);
    pwrite(fd,filename,filename_size,offset+sizeof(unsigned int));
    delete[] filename;
}




