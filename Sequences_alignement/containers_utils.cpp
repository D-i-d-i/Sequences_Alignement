#include <QtCore>
#include "containers_utils.h"
#include <math.h>

void addInMap(QMap<int, QVector<tuple> > * arr, int key, const tuple value){
    if(arr->contains(key)){
        arr->take(key).push_back(value);
    }else{
        arr->insert(key, QVector<tuple>(1, value));
    }
}

int min(int array[], unsigned int size){
    if(size ==0){
        return -1;
    }
    int lower = array[0];
    for(unsigned int i = 1; i < size; i++){
        lower = fmin(lower, array[i]);
    }
    return lower;
}

int min(QMap<int, QVector<tuple> > * arr){
    int lower = arr->begin().key();
    for(QMap<int, QVector<tuple> >::iterator it = arr->begin(); it!=arr->end(); it++){
        lower = fmin(lower, it.key());
    }
    return lower;
}
