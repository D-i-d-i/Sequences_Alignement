#include"containers_utils.h"
#include <vector>
#include <map>

using namespace std;

void addInMap(map<int, vector<tuple> > * arr, int key, const tuple value){
    if(arr->count(key)>0){
        arr->at(key).push_back(value);
    }else{
        arr->insert(pair<int, vector<tuple> >(key, vector<tuple>(1,value)));
    }
}

int min(int * array, unsigned int size){
    if(size ==0){
        return -1;
    }
    int lower = array[0];
    for(unsigned int i = 1; i < size; i++){
        lower = min(lower, array[i]);
    }
    return lower;
}

int min(std::map<int, std::vector<tuple> > *arr){
    int lower = arr->begin()->first;
    for(map<int, vector<tuple> >::iterator it = arr->begin(); it!=arr->end(); it++){
        lower = min(lower, it->first);
    }
    return lower;
}

bool isInto(vector< vector<tuple> >::iterator v, tuple t){
    for(vector<tuple>::iterator it = v->begin(); it!=v->end(); it++){
        if(it->i == t.i && it->j == t.j){
            return true;
        }
    }
    return false;
}
