#ifndef _sequences_h_
#define _sequences_h_
#include <vector>
#include <map>
#include <iostream>
#include "containers_utils.h"

#define EMPTY char(45)
#define START char(94)
#define TOTAL char(61)

#define VERT_HOLE_PENALITY 1
#define HOR_HOLE_PENALITY 1
#define DIFF_PENALITY 1

#define CONST_K 0.1

//find the longest common subsequence
int longuestCommon(const std::vector<char>*, const std::vector<char>*, int *, int *);

//sort by longuest common
void sortByLC(std::vector<char>[], const unsigned int size);

//calc length delta with moyenne and max
unsigned int * calcDeltaLength(const std::vector<char>[], const unsigned int, float *, unsigned int *);

//list the possible path when an alignment is done
std::vector<std::vector<tuple> > possiblePath(int ** scores, const int sizeA, const int sizeB);

//align sequences perfectly
void alignSequencesP(std::vector<char>*, std::vector<char>*);

int ** calculateScore(const std::vector<char> *, const std::vector<char> *);

//align sequences by using score
void alignSequencesSc(std::vector<char>*, std::vector<char>*);

//align a set of sequences
void multiAlignment(std::vector<char>[], const unsigned int);

//calculate the counts matrix
std::map<char, int*> calcCountsMatrix(const std::vector<char>[], const unsigned int);

//calculate the frequency matrix
std::map<char, float*> calcFrequencyMatrix(const std::vector<char>[], const unsigned int);
std::map<char, float*> calcFrequencyMatrix(const std::map<char, int*>, unsigned int, unsigned int);

//calculate the PW matrix
std::map<char, float*> calcPWMatrix(const std::vector<char>[], const unsigned int);
std::map<char, float*> calcPWMatrix(const std::map<char, float*>, const unsigned int);

//calculate the weight of position
float * calculatePW(const std::map<char, float*>, const unsigned int);

//calculate the score for each sequence
float * calculateScore(const std::vector<char>[], const unsigned int, float *);

#endif // _sequences_h_
