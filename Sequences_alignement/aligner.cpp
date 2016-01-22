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
        if(a->size()<b->size()){
            for(int i = 0; i<a->size(); i++){
                posA->insert(aOffset + i);
            }
        }else{
            for(int i = 0; i<b->size(); i++){
                posB->insert(bOffset + i);
            }
        }
    }else{
        QStringList la,ra,lb,rb;
        for(int i = 0; i < ia; i++){
            la << a->at(i);
        }
        for(int i = ia + subSize; i < a->size(); i++){
            ra << a->at(i);
        }
        for(int i = 0; i < ib; i++){
            lb << b->at(i);
        }
        for(int i = ib + subSize; i < b->size(); i++){
            rb << b->at(i);
        }
        simpleAlignement(&la, &lb, posA, posB);
        simpleAlignement(&ra, &rb, posA, posB, ia + subSize, ib + subSize);
    }
}


void Aligner::process(){
    QList<QVector<int> > group = groupByLongestCommon();
    QList<QSet<int> > * positions;
    QSet<int> tmpa, tmpb;
    for(QList<QVector<int> >::iterator it = group.begin(); it != group.end(); it++){
        tmpa.clear();
        tmpb.clear();
        positions = new QList<QSet<int> >[it->size()];
        for(int j = 0; j < it->size() - 1; j++){
            for(int i = 1; i < it->size(); i++){
                simpleAlignement(&(_sequences[it->at(j)]), &(_sequences[it->at(i)]),&tmpa,&tmpb);
                positions[j].append(tmpa);
                positions[i].append(tmpb);
            }
        }
        for(int i = 0; i < it->size(); i++){
            cout << it->at(i) << " " << endl;
            if(positions[i].isEmpty()) continue;
            tmpa = positions[i].first();
            for(int j = 1; j < positions[i].size(); j++){
                tmpa = tmpa.intersect(positions[i][j]);
            }
            for(QSet<int>::iterator sit = tmpa.begin(); sit != tmpa.end(); sit++){
                cout << "insert empty for " << it->at(i) << " the value " << *sit<< endl;
                _sequences[it->at(i)].insert(*sit, EMPTY);
                cout << endl;
            }
        }
        cout <<"next it" << endl;
    }
    printSequences();
}

QList<QStringList> Aligner::getSequences(){
    return _sequences;
}

void Aligner::printSequences(){
    if(_sequences.isEmpty()) return;
    for(QList<QStringList>::iterator it = _sequences.begin(); it != _sequences.end(); it++){
        for(QStringList::iterator sit = it->begin(); sit != it->end(); sit++){
            cout << sit->toStdString();
        }
        cout << endl;
    }
    cout << endl;
}
