#include <cstring>
#include "src_dest.h"

src::src(char *name, src *next) {
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->next=next;
}

src::~src() {
    delete next;
}

char *src::get_name() {
    if(next->next) return next->get_name();
    char *n=next->name;
    delete next;
    next=NULL;
    return n;
}

bool src::last_element() {
    return !next;
}

char *src::getname() {
    return name;
}

src_dest::src_dest(char *line) {
    counter=0;
    sources=NULL;
    char *temp,*cur=strtok(line," ");
    while((temp=strtok(NULL," \t"))){
        counter++;
        sources=new src(cur,sources);
        cur=temp;
    }
    dest=new char[strlen(cur)+1];
    strcpy(dest,cur);
}

src_dest::~src_dest() {
    delete sources;
    delete[] dest;
}

char *src_dest::get_dest() {
    return dest;
}

char *src_dest::get_source() {
    if(!sources) return NULL;
    if(sources->last_element()){
        char *temp=sources->getname();
        delete sources;
        sources=NULL;
        return temp;
    }
    return sources->get_name();
}

bool src_dest::rename() {
    return counter==1 && dest[0]!='/';
}
