#include "various.h"

bool check_int(char *num) {
    int i=0;
    do{
        if(num[i]<'0' || num[i]>'9') return false;
        i++;
    }while(num[i]!='\0');
    return true;
}
