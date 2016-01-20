#include "sequences.h"
#include "containers_utils.h"
#include <iostream>
#include <vector>
#include <map>
#include <math.h>


using namespace std;

//Detect the longuest sequence
int longuestCommon(const vector<char> * a, const vector<char> * b, int * ia, int * ib){
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
    const vector<char> * longuest = la > lb ? a : b;
    const vector<char> * shortest = la > lb ? b : a;
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

void sortByLC(std::vector<char> sequences[], const unsigned int size){
    vector<int*>indices;
    int max;
    int tmp, a, b;
    for(unsigned int i = 0; i<size; i++){
        max = 0;
        for(unsigned int j=0; j<size; j++){
            if(i==j) continue;
            tmp = longuestCommon(sequences+i, sequences+j, &a, &b);
            if(tmp>max){
                max = tmp;
            }
        }
        unsigned int insert = 0;
        for(unsigned int j = 0; j<indices.size(); j++){
            if(indices[j][1]>max) insert = j+1;
        }
        indices.insert(indices.begin()+insert, new int[2]{i,max});
    }
    vector<char> tmpV;
    for(unsigned int i=0; i<size;i++){
        if(indices[i][0]>i){
            tmpV = sequences[indices[i][0]];
            sequences[indices[i][0]] = sequences[i];
            sequences[i] = tmpV;
        }
    }
}

void alignSequences(vector<char> * a, vector<char> * b){
    //Get the longuest common sequence
    int ia = 0;
    int ib = 0;
    int size = longuestCommon(a, b, &ia, &ib);
    if(size <= 0){
        vector<char> sa (b->size(), EMPTY);
        sa.insert(sa.begin(), a->begin(), a->end());
        vector<char> sb (a->size(), EMPTY);
        sb.insert(sb.end(), b->begin(), b->end() );
        *a = sa;
        *b = sb;
        return;
    }
    vector<char> fLeft(a->begin(), a->begin() + ia);
    vector<char> sLeft(b->begin(), b->begin() + ib);
    vector<char> fRight(a->begin() + ia + size, a->end());
    vector<char> sRight(b->begin() + ib + size, b->end());
    alignSequences(&fLeft, &sLeft);
    alignSequences(&fRight, &sRight);

    vector<char> longuest(a->begin() + ia, a->begin() + ia + size);
    *a = fLeft;
    a->insert(a->end(), longuest.begin(), longuest.end());
    a->insert(a->end(), fRight.begin(), fRight.end());
    *b = sLeft;
    b->insert(b->end(), longuest.begin(), longuest.end());
    b->insert(b->end(), sRight.begin(), sRight.end());
    return;
}

int test(const vector<char> * a, const vector<char> * b){
    int un[8] = {3,2,2,1,2,3,4,2};
    int deux[8] = {1,2,1,2,2,1,2,3};
    float ** scores = 0;
    unsigned int i;
    scores = new float * [3];
    scores[0] = new float[b->size() + 1];
    scores[1] = new float[b->size() + 1];
   // scores[2] = new float[b->size() + 1];
    scores[0][0] = 0;
    for(vector<char>::const_iterator it = a->begin(); it != a->end(); it++){
        cout << (*it) << " ";
    }
    cout << endl;
    for(vector<char>::const_iterator it = b->begin(); it != b->end(); it++){
        cout << (*it) << " ";
    }
    cout << endl;
    cout << scores[0][0] << " | ";
    for(unsigned int j = 1; j <= b->size(); j ++){
        scores[0][j] = scores[0][j-1] + deux[j-1];
        cout << scores[0][j] << " | ";
    }
    cout << endl;
    for(i = 1; i <= a->size(); i++){
        scores[i%2][0] = scores[(i-1)%2][0] + un[i-1];
        cout << scores[i%2][0] << " | ";
        for(unsigned int j = 1; j <= b->size(); j++){
            scores[i%2][j] = min( min(scores[(i-1)%2][j-1] + ( a->at(i-1)==b->at(j-1) ? 0 : (un[i-1]*deux[j-1])/2 ), scores[(i-1)%2][j] + HOR_HOLE_PENALITY), scores[i%2][j-1] + VERT_HOLE_PENALITY );
            //if(i > 1 && j > 1 && a->at(i-1) == b->at(j-2) && a->at(i-2)==b->at(j-1)) scores[i%2][j] = min(scores[i%2][j], scores[(i-2)%2][j-2] + ( a->at(i-1)==b->at(j-1) ? 0 : DIFF_PENALITY ));
            cout << scores[i%2][j] << " | ";
        }
        cout << endl;
    }
    cout << "Chouette le score des 2 c est ca :" << scores[a->size()%2][b->size()] << endl;
    return scores[i%2][b->size()];
}

int ** calculateScore(const vector<char> * a, const vector<char> * b){
    test(a,b);
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

vector<vector<tuple> > possiblePath(int ** scores, const int sizeA, const int sizeB){
    int cost = scores[sizeA-1][sizeB-1];
    tuple pos;
    tuple val;
    vector<tuple> states(1,tuple(sizeA-1, sizeB-1));
    vector<vector<tuple> > path(1, states);
    map<int, vector<tuple> > tmp;
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
        if(tmp.count(cost) > 0){
            states = tmp[cost];
            path.insert(path.begin(), tmp[cost]);
        }
    }

    return path;
}



void alignSequencesSc(vector<char> * a,vector<char> * b){
    if(a->size()*b->size()==0){
        return;
    }
    int ** scores = calculateScore(a , b);
    vector<vector<tuple> > path = possiblePath(scores, a->size()+1, b->size()+1);

    if(!isInto(path.begin(), tuple(0,0))){
        return;
    }
    tuple current = tuple(0,0);
    for(unsigned int i = 1; i < path.size(); i++){
        current.i++;
        current.j++;
        if(a->size() >= i && b->size() >= i && a->at(i - 1) == b->at(i - 1) && isInto(path.begin() + i, current)){ //if equal
            continue;
        }
        current.j--;
        if(isInto(path.begin() + i, current)){ //else first hole
            b->insert(b->begin() + i - 1, EMPTY);
            continue;
        }
        current.i--;
        current.j++;
        if(isInto(path.begin() + i, current)){ //or second hole
            a->insert(a->begin() + i - 1, EMPTY);
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

void multiAlignment(vector<char> sequences[], unsigned int size){
    /****  Before *****/
    /*for(unsigned int j = 1; j < size; j++){
        alignSequences(&sequences[0], &sequences[j]);
    }*/
    /***** Now ****/
    //sortByLC(sequences, size);
    for(unsigned int i=0; i<size-1; i++){
        for(unsigned int j=1; j<size; j++){
            alignSequencesSc(&sequences[i], &sequences[j]);
        }
    }
}

//Calculate the number of event at a position (sequences are aligned)
map<char, int*> calcCountsMatrix(const vector<char> sequences[], const unsigned int nbSequences){
    map<char, int * > matrix;
    if(nbSequences <= 0 ){
        return matrix;
    }
    unsigned int nbPos = sequences[0].size();
    for(unsigned int j = 0; j < nbPos; j++){
        for(unsigned int i = 0; i < nbSequences; i++){
            if(matrix.count(sequences[i][j])<=0){
                int * array = new int[nbPos]{};
                matrix[sequences[i][j]] = array;
            }
            matrix[sequences[i][j]][j]++;
        }
    }
    return matrix;
}

map<char, float*> calcFrequencyMatrix(const vector<char> sequences[], const unsigned int size){
    map<char, float*> frq;
    if(size <= 0 ){
        return frq;
    }
    map<char, int*> counts = calcCountsMatrix(sequences, size);
    return calcFrequencyMatrix(counts, size,sequences[0].size());
}

map<char, float*> calcFrequencyMatrix(const std::map<char, int*> counts, unsigned int size, unsigned int nbPos){
    map<char, float*> frq;
    if(size <= 0 ){
        return frq;
    }
    int * totalByPos = new int[nbPos]{};
    unsigned int i;
    for(map<char, int*>::const_iterator it = counts.begin(); it != counts.end(); it++){
        if(frq.count(it->first)<=0){
            frq[it->first] = new float[nbPos]{};
        }
        for(i = 0; i < nbPos; i++){
            frq[it->first][i] = (float) it->second[i];
            totalByPos[i] += it->second[i];
        }
    }
    for(map<char, float*>::iterator it = frq.begin(); it != frq.end(); it++){
        for(i = 0; i < nbPos; i++){
            it->second[i] += CONST_K / frq.size();
            it->second[i] /= totalByPos[i] + CONST_K;
        }
    }
    return frq;
}

map<char, float*> calcPWMatrix(const vector<char> sequences[], const unsigned int size){
    map<char, float*> pwm = calcFrequencyMatrix(sequences, size);
    if(size <= 0){
        return pwm;
    }
    for(map<char, float*>::iterator it = pwm.begin(); it != pwm.end(); it++){
        for(unsigned int i = 0; i < sequences[0].size(); i++){
            it->second[i] = log(it->second[i]/pwm.size());
        }
    }
    return pwm;
}
std::map<char, float*> calcPWMatrix(const std::map<char, float*> frq, unsigned int size){
    map<char, float*> pwm;
    char c;
    for(map<char, float*>::const_iterator it = frq.begin(); it != frq.end(); it++){
        pwm[it->first] = new float[size]{};
        cout << it->first << " : ";
        for(unsigned int i = 0; i < size; i++){
            pwm[it->first][i] = log(it->second[i]*frq.size());
            //cout << round(pwm[it->first][i]) << " | ";
            cout << ((pwm[it->first][i] > 0)?'1':'0') << " | ";
            //cout << it->second[i] << " | ";
        }
        cout << endl;
    }
    return pwm;
}

float * calculatePW(const map<char, float*> frq, const unsigned int nbPos){
    if(frq.size() == 0){
        return 0;
    }
    float * pw = new float[nbPos];
    for(map<char, float*>::const_iterator it = frq.begin(); it != frq.end(); it++){
        for(unsigned int i = 0; i < nbPos; i++){
            pw[i]+= it->second[i] * log2(it->second[i]);
        }
        cout << endl;
    }
    float t = log2(nbPos);
    for(unsigned int i = 0; i < nbPos; i++){
        pw[i] = (t + pw[i])/t;
    }
    return pw;
}

unsigned int * calcDeltaLength(const vector<char> sequences[], const unsigned int nbSequences, float * moy, unsigned int * max){
    unsigned int * length = new unsigned int[nbSequences]{};
    *max = 0;
    for(unsigned int i = 0; i < nbSequences; i++){
        for(vector<char>::const_iterator it = sequences[i].begin(); it != sequences[i].end(); it++){
            if((*it) != EMPTY) length[i]++;
        }
        if(*max < length[i]) *max = length[i];
        *moy += length[i];
    }
    *moy /= nbSequences;
    return length;
}

float * calculateScore(const vector<char> sequences[], const unsigned int nbSequences, float * seuil){
    if(nbSequences <= 1 ){
        return 0;
    }

    map<char, float*> frq = calcFrequencyMatrix(sequences, nbSequences);
    map<char, float *> pwm = calcPWMatrix(frq, sequences[0].size());
    float * pw = calculatePW(frq, sequences[0].size());
    float * scores = new float[nbSequences]{};
    float mLength = 0;
    unsigned int max = 0;
    float scoreMax = 0;
    float scoreMoy = 0;
    float scoreStd = 0;
    float * length = (float *)calcDeltaLength(sequences, nbSequences, &mLength, &max);
    for(unsigned int i = 0; i < nbSequences; i++){
        length[i]= length[i] == max ? 0.0: fabs(length[i] - mLength)/((float)(max- length[i]));
        for(unsigned j = 0; j < sequences[i].size(); j++){
            scores[i] += sequences[i][j]==EMPTY ? length[i] * pw[j]: pwm[sequences[i][j]][j];
        }
        if(scoreMax < scores[i]) scoreMax = scores[i];
        scoreMoy+= scores[i];
    }
    scoreMoy /= nbSequences;
    for(unsigned int i = 0; i < nbSequences; i++){
        scoreStd += pow(scores[i]-scoreMoy ,2);
    }
    scoreStd = sqrt(1.0/(nbSequences-1) * scoreStd);
    cout << "Moyenne : " << scoreMoy << " Ecart Type : " << scoreStd << endl;
    *seuil = (scoreMoy)-3*sqrt(scoreStd);
    return scores;
}
