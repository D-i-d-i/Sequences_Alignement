#ifndef CONTAINERS_UTILS
#define CONTAINERS_UTILS
#include <vector>
#include <map>

struct tuple{
    int i;
    int j;

    tuple(){
        i = -1;
        j = -1;
    }

    tuple(int a, int b){
        i = a;
        j = b;
    }
};

void addInMap(std::map<int, std::vector<tuple> > *, const int, const tuple);
int min(std::map<int, std::vector<tuple> > *);
int min(int * array, unsigned int size);
bool isInto(std::vector< std::vector<tuple> >::iterator, tuple);

#endif // CONTAINERS_UTILS

