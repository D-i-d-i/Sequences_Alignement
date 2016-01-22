#ifndef ALIGNER_H
#define ALIGNER_H
#include <QtCore>
#include <iostream>
#include"helper.h"


class Aligner
{
private:
    QList<QStringList> _sequences;

    QList<QVector<int> > groupByLongestCommon();
    void simpleAlignement(QStringList *a, QStringList * b);
public:
    Aligner(QList<QStringList> sequences);
    void process();
    QList<QStringList> getSequences();
    void printSequences();
};

#endif // ALIGNER_H
