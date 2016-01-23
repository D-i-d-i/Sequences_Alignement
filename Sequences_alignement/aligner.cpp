#include <algorithm>
#include "aligner.h"

using namespace std;

Aligner::Aligner(QList<QStringList> sequences)
{
    _sequences = sequences;
}

QList<QVector<int> > Aligner::groupByLongestCommon(){
    QList<QVector<int> > group; //returned variable
    QMap<int, int> memScores;
    int score, k;
    bool initMap = true;
    QMap<int, int>::iterator it;
    memScores.insert(0, 0);
    for(int i = 0; i < _sequences.size() - 1; i++){
        for(int j = i + 1; j < _sequences.size(); j++){
            if(initMap) memScores.insert(j, 0);
            score = Helper::longestCommon(&(_sequences[i]), &(_sequences[j]));
            if(score > memScores[i]) memScores[i] = score;
            if(score > memScores[j]) memScores[j] = score;
        }
        initMap = false;
    }
    while(!memScores.isEmpty()){
        k = memScores.firstKey();
        group.append(QVector<int>(1, k));
        score = memScores[k];
        memScores.remove(k);
        it = memScores.begin();
        while(it!=memScores.end()){
            if(it.value() == score){
                group.last().append(it.key());
                it = memScores.erase(it);
            }else it++;
        }
    }
    return group;
}

void Aligner::simpleAlignement(QStringList *a, QStringList * b, QSet<int> *posA, QSet<int> *posB, int aOffset, int bOffset){
    if(a->size()==0 && b->size()==0) return;
    int ia, ib;
    int subSize = Helper::longestCommon(a,b, &ia, &ib);
    if(subSize == 0){
        if(a->size()<b->size()) for(int i = 0; i<b->size()-a->size(); i++) posA->insert(aOffset + i);
        else if(a->size()>b->size()) for(int i = 0; i<a->size()-b->size(); i++) posB->insert(bOffset + i);
    }else{
        QStringList la,ra,lb,rb;
        for(int i = 0; i < ia; i++) la << a->at(i);
        for(int i = ia + subSize; i < a->size(); i++) ra << a->at(i);
        for(int i = 0; i < ib; i++) lb << b->at(i);
        for(int i = ib + subSize; i < b->size(); i++) rb << b->at(i);
        simpleAlignement(&la, &lb, posA, posB, aOffset, bOffset);
        simpleAlignement(&ra, &rb, posA, posB, aOffset + ia + subSize, bOffset + ib + subSize);
    }
}


void Aligner::process(){
    QList<QVector<int> > group = groupByLongestCommon();
    QList<QSet<int> > * positions;
    QSet<int> tmpa, tmpb;
    do{
        for(QList<QVector<int> >::iterator it = group.begin(); it != group.end(); it++){
            positions = new QList<QSet<int> >[it->size()];
            for(int j = 0; j < it->size() - 1; j++){
                for(int i = j + 1; i < it->size(); i++){
                    tmpa.clear();
                    tmpb.clear();
                    simpleAlignement(&(_sequences[it->at(j)]), &(_sequences[it->at(i)]),&tmpa,&tmpb);
                    if(!tmpa.isEmpty()) positions[j].append(tmpa);
                    if(!tmpb.isEmpty()) positions[i].append(tmpb);
                }
            }
            for(int i = 0; i < it->size(); i++){
                tmpa.clear();
                for(int j = 0; j < positions[i].size(); j++){
                    if(tmpa.isEmpty()) tmpa = positions[i][j];
                    else tmpa = tmpa.intersect(positions[i][j]);
                }
                for(QSet<int>::iterator sit = tmpa.begin(); sit != tmpa.end(); sit++)  _sequences[it->at(i)].insert(*sit, EMPTY);
            }
        }
        group[1] += group[0];
        group.removeFirst();
    }while(group.size()>1);
    printSequences();
}

QList<QStringList> Aligner::getSequences(){
    return _sequences;
}

void Aligner::printSequence(const int indice){
    for(QStringList::iterator sit = _sequences[indice].begin(); sit != _sequences[indice].end(); sit++) cout << sit->toStdString();
    cout << endl;
}

void Aligner::printSequences(){
    if(_sequences.isEmpty()) return;
    for(int i = 0; i < _sequences.size(); i++) printSequence(i);
    cout << endl;
}
