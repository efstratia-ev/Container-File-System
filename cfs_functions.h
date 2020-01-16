#ifndef CFS_FUNCTIONS_H
#define CFS_FUNCTIONS_H
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

#include "cfs_file.h"
#include "various.h"

using namespace std;

cfs_file *cfs_workwith(char *filename);

cfs_file *cfs_create(char *arguments);

int cfs_touch(char *arguments);

#endif
