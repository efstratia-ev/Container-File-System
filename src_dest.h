#ifndef SRC_DEST_H
#define SRC_DEST_H


class src{
    char *name;
    src *next;
public:
    src(char *name,src *next);
    ~src();
    char *get_name();
    bool last_element();
    char *getname();
};

class src_dest {
    int counter;
    char *dest;
    src* sources;
public:
    src_dest(char *line);
    ~src_dest();
    char *get_dest();
    char *get_source();
    bool rename();
};


#endif
