#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "cfs_elmnt.h"

void swap(cfs_elmnt** arr,int a,int b);

int partition (cfs_elmnt** arr,int low,int high);

void quickSort(cfs_elmnt** arr, int low, int high);

#endif
