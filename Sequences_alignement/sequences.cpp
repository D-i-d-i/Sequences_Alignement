#include "sequences.h"

#include "containers_utils.h"
#include <iostream>
#include <vector>
#include <map>
#include <math.h>


using namespace std;

//Detect the longuest sequence
int longuestCommon(const QStringList * a, const QStringList * b, int * ia, int * ib){
    int la = a->size();
    int lb = b->size();
    *ia = 0;
    *ib = 0;
    //If sequences are empty, return an emtpty longuest
    if(la*lb == 0){
        return 0;
    }

    int size = 0;
    int cur = size;
    int tia = 0;
    int tib = 0;
    const QStringList * longuest = la > lb ? a : b;
    const QStringList * shortest = la > lb ? b : a;
    int * til = la > lb ? &tia : &tib;
    int * tis = la > lb ? &tib : &tia;
    for(int i = (int) longuest->size(); i >= -((int) shortest->size()); i--){ //The both sequences will slide, to run on all the possibilities, we need the sum of both sequences iterations
        cur = 0;
        for(int j = 0; j < (int) shortest->size(); j++){
            if(i+j >= 0 && i+j < (int) longuest->size() && longuest->at(i+j) == shortest->at(j) && longuest->at(i+j) != EMPTY){ //while it's the same, we count up cur
                if(cur == 0){
                    *til = i+j;
                    *tis = j;
                }
                cur++;
            }else{
                if(cur > size){
                    size = cur;
                    *ia = tia;
                    *ib = tib;
                }
                cur = 0;
            }
        }
        if(cur > size){
            size = cur;
            *ia = tia;
            *ib = tib;
        }
    }
    return size;
}

void sortByLC(QList<QStringList> * sequences){
    QVector<int*> indices;
    int max;
    int tmp, a, b;
    for(unsigned int i=0; i < sequences->size(); i++){
        max = 0;
        for(unsigned int j=0; j<sequences->size(); j++){
            if(i==j) continue;
            tmp = longuestCommon(&(sequences->at(i)), &(sequences->at(j)), &a, &b);
            if(tmp>max){
                max = tmp;
            }
        }
        unsigned int insert = 0;
        for(unsigned int j = 0; j<indices.size(); j++){
            if(indices[j][1]>max) insert = j+1;
        }
        indices.insert(insert, new int[2]{i,max});
    }
    for(unsigned int i=0; i<sequences->size();i++){
        if(indices[i][0]>i){
            sequences->swap(indices[i][0],i);
        }
    }
}

void alignSequences(QStringList * a, QStringList * b){
    //Get the longuest common sequence
    int ia = 0;
    int ib = 0;
    int size = longuestCommon(a, b, &ia, &ib);
    if(size <= 0){
        QStringList sa = *a;
        for(unsigned int i = 0; i<b->size(); i++) sa.append(EMPTY);
        QStringList sb;
        for(unsigned int i = 0; i<a->size(); i++) sb.append(EMPTY);
        sb += *b;
        *a = sa;
        *b = sb;
        return;
    }
    QStringList fLeft = a->mid(0, ia);
    QStringList sLeft = b->mid(0, ib);
    QStringList fRight = a->mid(ia + size);
    QStringList sRight = b->mid(ib + size);
    alignSequences(&fLeft, &sLeft);
    alignSequences(&fRight, &sRight);

    *a = a->mid(ia, size);
    *b = sLeft + *a + sRight;
    *a = fLeft + *a + fRight;

    return;
}

int ** calculateScore(const QStringList * a, const QStringList * b){
    int ** scores = 0;
    scores = new int * [a->size() + 1];
    scores[0] = new int[b->size() + 1];
    scores[0][0] = 0;
    for(unsigned int j = 1; j <= b->size(); j ++){
        scores[0][j] = scores[0][j-1] + VERT_HOLE_PENALITY;
    }
    for(unsigned int i = 1; i <= a->size(); i++){
        scores[i] = new int[b->size() + 1];
        scores[i][0] = scores[i-1][0] + HOR_HOLE_PENALITY;
        for(unsigned int j = 1; j <= b->size(); j++){
            scores[i][j] = min( min(scores[i-1][j-1] + ( a->at(i-1)==b->at(j-1) ? 0 : DIFF_PENALITY ), scores[i-1][j] + HOR_HOLE_PENALITY), scores[i][j-1] + VERT_HOLE_PENALITY );
            //if(i > 1 && j > 1 && a->at(i-1) == b->at(j-2) && a->at(i-2)==b->at(j-1)) scores[i][j] = min(scores[i][j], scores[i-2][j-2] + ( a->at(i-1)==b->at(j-1) ? 0 : DIFF_PENALITY ));
        }
    }
    return scores;
}

QList<QVector<tuple> > possiblePath(int ** scores, const int sizeA, const int sizeB){
    int cost = scores[sizeA-1][sizeB-1];
    tuple pos, val;
    QVector<tuple> states(1,tuple(sizeA-1, sizeB-1));
    QList<QVector<tuple> > path;
    path.append(states);
    QMap<int, QVector<tuple> > tmp;
    while(states.size() > 0){
        tmp.clear();
        for(unsigned int i = 0; i < states.size(); i++){
            pos = states[i];
            if(pos.i*pos.j > 0){
                val = tuple(pos.i-1, pos.j-1);
                addInMap(&tmp, scores[val.i][val.j], val);
            }
            if(pos.i>0){
                val = tuple(pos.i-1, pos.j);
                addInMap(&tmp, scores[val.i][val.j], val);
            }
            if(pos.j>0){
                val = tuple(pos.i, pos.j-1);
                addInMap(&tmp, scores[val.i][val.j], val);
            }
            if(tmp.size()>0){
                cost = min(&tmp);
            }
        }
        states.clear();
        if(tmp.contains(cost)){
            states = tmp[cost];
            path.insert(path.begin(), tmp[cost]);
        }
    }

    return path;
}



void alignSequencesSc(QStringList * a,QStringList * b){
    if(a->size()*b->size()==0){
        return;
    }
    int ** scores = calculateScore(a , b);
    QList<QVector<tuple> > path = possiblePath(scores, a->size()+1, b->size()+1);

    if(!path.first().contains(tuple(0,0))){
        return;
    }
    tuple current = tuple(0,0);
    for(unsigned int i = 1; i < path.size(); i++){
        current.i++;
        current.j++;
        if(a->size() >= i && b->size() >= i && a->at(i - 1) == b->at(i - 1) && path[i].contains(current)){ //if equal
            continue;
        }
        current.j--;
        if(path[i].contains(current)){ //else first hole
            b->insert(i-1, EMPTY);
            continue;
        }
        current.i--;
        current.j++;
        if(path[i].contains(current)){ //or second hole
            a->insert(i - 1, EMPTY);
            continue;
        }
        current.i++; //if no hole, same position but different
    }
    while(a->size() < b->size()){
        a->push_back(EMPTY);
    }
    while(b->size() < a->size()){
        b->push_back(EMPTY);
    }
}

void multiAlignment(QList<QStringList> * sequences){
    /****  Before *****/
    /*for(unsigned int j = 1; j < size; j++){
        alignSequences(&sequences[0], &sequences[j]);
    }*/
    /***** Now ****/
    sortByLC(sequences);
    for(unsigned int i=0; i<sequences->size()-1; i++){
        for(unsigned int j=1; j<sequences->size(); j++){
            cout << " i : " << i << " j : " << j <<endl;
            alignSequencesSc(&(*(sequences->begin() + i)), &(*(sequences->begin() + j)));
        }
    }
}

//Calculate the number of event at a position (sequences are aligned)
QMap<QString, int*> calcCountsMatrix(const QList<QStringList> * sequences){
    QMap<QString, int * > matrix;
    unsigned int nbPos = sequences[0].size();
    for(unsigned int j = 0; j < nbPos; j++){
        for(unsigned int i = 0; i < sequences->size(); i++){
            if(!matrix.contains(sequences->at(i).at(j))){
                matrix[sequences->at(i).at(j)] = new int[nbPos]{};
            }
            matrix[sequences->at(i).at(j)][j]++;
        }
    }
    return matrix;
}

QMap<QString, float*> calcFrequencyMatrix(const QList<QStringList> * sequences){
    QMap<QString, int*> counts = calcCountsMatrix(sequences);
    return calcFrequencyMatrix(counts, sequences->first().size());
}

QMap<QString, float*> calcFrequencyMatrix(const QMap<QString, int*> counts, unsigned int nbPos){
    QMap<QString, float*> frq;
    int * totalByPos = new int[nbPos]{};
    unsigned int i;
    for(QMap<QString, int*>::const_iterator it = counts.begin(); it != counts.end(); it++){
        if(!frq.contains(it.key())){
            frq[it.key()] = new float[nbPos]{};
        }
        for(i = 0; i < nbPos; i++){
            frq[it.key()][i] = (float) (it.value())[i];
            totalByPos[i] += (it.value())[i];
        }
    }
    for(QMap<QString, float*>::iterator it = frq.begin(); it != frq.end(); it++){
        for(i = 0; i < nbPos; i++){
           (it.value())[i] += CONST_K / frq.size();
            (it.value())[i] /= totalByPos[i] + CONST_K;
        }
    }
    return frq;
}

QMap<QString, float*> calcPWMatrix(const QList<QStringList> * sequences){
    QMap<QString, float*> pwm = calcFrequencyMatrix(sequences);
    for(QMap<QString, float*>::iterator it = pwm.begin(); it != pwm.end(); it++){
        for(unsigned int i = 0; i < sequences[0].size(); i++){
            (it.value())[i] = log((it.value())[i]/pwm.size());
        }
    }
    return pwm;
}

QMap<QString, float*> calcPWMatrix(const QMap<QString, float*> frq, unsigned int nbPos){
    QMap<QString, float*> pwm;
    for(QMap<QString, float*>::const_iterator it = frq.begin(); it != frq.end(); it++){
        pwm[it.key()] = new float[nbPos]{};
        cout << it.key().toStdString() << " : ";
        for(unsigned int i = 0; i < nbPos; i++){
            pwm[it.key()][i] = log((it.value())[i]*frq.size());
            //cout << round(pwm[it.key()][i]) << " | ";
            cout << ((pwm[it.key()][i] > 0)?'1':'0') << " | ";
            //cout << (it.value())[i] << " | ";
        }
        cout << endl;
    }
    return pwm;
}

float * calculatePW(const QMap<QString, float*> frq, const unsigned int nbPos){
    if(frq.size() == 0){
        return 0;
    }
    float * pw = new float[nbPos];
    for(QMap<QString, float*>::const_iterator it = frq.begin(); it != frq.end(); it++){
        for(unsigned int i = 0; i < nbPos; i++){
            pw[i]+= (it.value())[i] * log2((it.value())[i]);
        }
        cout << endl;
    }
    float t = log2(nbPos);
    for(unsigned int i = 0; i < nbPos; i++){
        pw[i] = (t + pw[i])/t;
    }
    return pw;
}

unsigned int * calcDeltaLength(const QList<QStringList> * sequences, float * moy, unsigned int * max){
    unsigned int * length = new unsigned int[sequences->size()]{};
    *max = 0;
    for(unsigned int i = 0; i < sequences->size(); i++){
        for(QStringList::const_iterator it = sequences->at(i).begin(); it != sequences->at(i).end(); it++){
            if((*it) != EMPTY) length[i]++;
        }
        if(*max < length[i]) *max = length[i];
        *moy += length[i];
    }
    *moy /= sequences->size();
    return length;
}

float * calculateScore(const QList<QStringList> * sequences, float * seuil){
    QMap<QString, float*> frq = calcFrequencyMatrix(sequences);
    QMap<QString, float *> pwm = calcPWMatrix(frq, sequences[0].size());
    float * pw = calculatePW(frq, sequences[0].size());
    float * scores = new float[sequences->size()]{};
    float mLength = 0;
    unsigned int max = 0;
    float scoreMax = 0;
    float scoreMoy = 0;
    float scoreStd = 0;
    float * length = (float *)calcDeltaLength(sequences, &mLength, &max);
    for(unsigned int i = 0; i < sequences->size(); i++){
        length[i]= length[i] == max ? 0.0: fabs(length[i] - mLength)/((float)(max- length[i]));
        for(unsigned j = 0; j < sequences[i].size(); j++){
            scores[i] += sequences->at(i).at(j)==EMPTY ? length[i] * pw[j]: pwm[sequences->at(i).at(j)][j];
        }
        if(scoreMax < scores[i]) scoreMax = scores[i];
        scoreMoy+= scores[i];
    }
    scoreMoy /= sequences->size();
    for(unsigned int i = 0; i < sequences->size(); i++){
        scoreStd += pow(scores[i]-scoreMoy ,2);
    }
    scoreStd = sqrt(1.0/(sequences->size()-1) * scoreStd);
    cout << "Moyenne : " << scoreMoy << " Ecart Type : " << scoreStd << endl;
    *seuil = (scoreMoy)-3*sqrt(scoreStd);
    return scores;
}
