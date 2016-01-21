#ifndef CONTAINERS_UTILS
#define CONTAINERS_UTILS
#include <QtCore>

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

inline bool operator==(const tuple a, const tuple b){
    if(a.i==b.i && a.j==b.j) return true;
    return false;
}

void addInMap(QMap<int, QVector<tuple> > *, const int, const tuple);
int min(QMap<int, QVector<tuple> > *);
int min(int array[], unsigned int size);

#endif // CONTAINERS_UTILS

