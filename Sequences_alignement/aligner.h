#ifndef ALIGNER_H
#define ALIGNER_H
#include <QtCore>
#include <iostream>
#include"helper.h"


class Aligner
{
private:
    QList<QStringList> _sequences;

    QList<QVector<int> > groupByLonguestCommon();
public:
    Aligner(QList<QStringList> sequences);
    void process();
    QList<QStringList> getSequences();
};

#endif // ALIGNER_H
