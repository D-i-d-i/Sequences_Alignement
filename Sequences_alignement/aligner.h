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
    void simpleAlignement(QStringList *a, QStringList * b, QSet<int> * posA, QSet<int> * posB, int aOffset = 0, int bOffset = 0);
public:
    Aligner(QList<QStringList> sequences);
    void process();
    QList<QStringList> getSequences();
    void printSequence(const int indice);
    void printSequences();
};

#endif // ALIGNER_H
