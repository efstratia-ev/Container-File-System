#include "QuickSort.h"

void swap(cfs_elmnt** arr,int a,int b){
    cfs_elmnt* temp=arr[a];
    arr[a]=arr[b];
    arr[b]=temp;
}

int partition (cfs_elmnt** arr,int low,int high){
    cfs_elmnt* pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high- 1; j++){
        if (strcmp(arr[j]->filename,pivot->filename)<0){
            i++;
            swap(arr,i,j);
        }
    }
    swap(arr,i+1,high);
    return i + 1;
}

void quickSort(cfs_elmnt** arr, int low, int high){
    if(low >= high) return;
    int pi = partition(arr, low, high);

    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
}
