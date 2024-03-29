#ifndef CFS_FUNCTIONS_H
#define CFS_FUNCTIONS_H

#include <fcntl.h>
#include <cstdlib>

#include "cfs_file.h"
#include "various.h"
#include "src_dest.h"

using namespace std;

cfs_file *cfs_workwith(char *filename);

cfs_file *cfs_create(char *arguments);

int cfs_touch(cfs_file *f_info,char *arguments);

bool cfs_mkdir(cfs_file *f_info,char *arguments);

bool cfs_cd(cfs_file *f_info, char *arguments);

void cfs_pwd(cfs_file *f_info);

int cfs_ls(cfs_file *f_info,char *arguments);

int cfs_rm(cfs_file *f_info,char *arguments);

bool cfs_ln(cfs_file *f_info,char *arguments);

bool cfs_cat(cfs_file *f_info,char *arguments);

bool cfs_cp(cfs_file *f_info,char *arguments);

bool cfs_mv(cfs_file *f_info,char *arguments);

bool cfs_import(cfs_file *f_info, char *arguments);

#endif
