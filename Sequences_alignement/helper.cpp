#include <iostream>
#include<algorithm>
#include "helper.h"

using namespace std;

Helper::Helper()
{

}

/**
 * @brief longuestCommon sub sequence betwenn two
 * @param a : first sequence
 * @param b : second sequence
 * @param ia : indice on a where the subsequence begin
 * @param ib : indice on b where the subsequence begin
 * @return
 */
int Helper::longuestCommon(const QStringList * a, const QStringList * b, int * ia, int * ib){
    //If sequences are empty, return an emtpty longuest
    if(a->size()*b->size() == 0){
        return 0;
    }
    int currentSize = 0, size = 0; //size : the returned variable
    int memA, memB; //"memory" variable
    int nbIter, ta, tb;
    for(int k = - a->size() + 1; k < b->size(); k++){ //Size(a) + Size(b) - 1 == nbIterations
        currentSize = 0;
        if(k<0){
            tb = 0;
            ta = -k;
            nbIter = min(a->size() + k, b->size());
        }else{
            tb = k;
            ta = 0;
            nbIter = min(a->size(), b->size() - k);
        }
        for(int i = 0; i < nbIter; i++){
            if(currentSize == 0){ //if it's the first time
                memA = ta + i;
                memB = tb + i;
            }
            if(a->at(ta+i) == b->at(tb+i)){ //if equal
                currentSize++;
            }else{
                if(currentSize>size){ //save when greater
                    size = currentSize;
                    if(ia != NULL) *ia = memA;
                    if(ib != NULL) *ib = memB;
                }
                currentSize = 0;
            }
        }
        if(currentSize>size){ //save when greater is over
            size = currentSize;
            if(ia != NULL) *ia = memA;
            if(ib != NULL) *ib = memB;
        }
    }
    return size;
}
