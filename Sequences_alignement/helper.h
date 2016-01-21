#ifndef HELPER_H
#define HELPER_H
#include <QtCore>

#define EMPTY QString("-")
#define START QString(">")
#define TOTAL QString("=")

#define VERT_HOLE_PENALITY 1
#define HOR_HOLE_PENALITY 1
#define DIFF_PENALITY 1

class Helper
{
public:
    Helper();
    static int longuestCommon(const QStringList * a, const QStringList * b, int * ia = NULL, int * ib = NULL);
};

#endif // HELPER_H
