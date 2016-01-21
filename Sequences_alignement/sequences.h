#ifndef _sequences_h_
#define _sequences_h_
#include <QtCore>
#include <iostream>
#include "containers_utils.h"

#define EMPTY QString("-")
#define START QString(">")
#define TOTAL QString("=")

#define VERT_HOLE_PENALITY 1
#define HOR_HOLE_PENALITY 1
#define DIFF_PENALITY 1

#define CONST_K 0.1

//find the longest common subsequence
int longuestCommon(const QStringList *, const QStringList*, int *, int *);

//sort by longuest common
void sortByLC(QStringList);

//calc length delta with moyenne and max
unsigned int * calcDeltaLength(const QList<QStringList> *, float *, unsigned int *);

//list the possible path when an alignment is done
QList<QVector<tuple> > possiblePath(int ** scores, const int sizeA, const int sizeB);

//align sequences perfectly
void alignSequencesP(QStringList*, QStringList*);

int ** calculateScore(const QStringList *, const QStringList *);

//align sequences by using score
void alignSequencesSc(QStringList *, QStringList *);

//align a set of sequences
void multiAlignment(QList<QStringList>*);

//calculate the counts matrix
QMap<QString, int *> calcCountsMatrix(const QList<QStringList> *);

//calculate the frequency matrix
QMap<QString, float *>  calcFrequencyMatrix(const QList<QStringList> *);
QMap<QString, float *> calcFrequencyMatrix(const QMap<QString, int *> , unsigned int);

//calculate the PW matrix
QMap<QString, float *>  calcPWMatrix(const QList<QStringList> *);
QMap<QString, float *> calcPWMatrix(const QMap<QString, float *>, unsigned int nbPos);

//calculate the weight of position
float * calculatePW(const QMap<QString, float *>, const unsigned int);

//calculate the score for each sequence
float * calculateScore(const QList<QStringList> *, float *);

#endif // _sequences_h_
