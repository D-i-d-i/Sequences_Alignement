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

void Aligner::simpleAlignement(QStringList *a, QStringList * b){
    if(a->size()==0 && b->size()==0) return;
    int ia, ib;
    int subSize = Helper::longestCommon(a,b, &ia, &ib);

    if(subSize == 0){
        if(a->size()<b->size()){
            for(int i = a->size(); i < b->size(); i++){
                a->append(EMPTY);
            }
        }else{
            for(int i = b->size(); i < a->size(); i++){
                b->append(EMPTY);
            }
        }
    }else{
        QStringList la,ra,lb,rb, common;
        for(int i = ia; i < ia + subSize; i++){
            common << a->at(i);
        }
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
        simpleAlignement(&la, &lb);
        simpleAlignement(&ra, &rb);
        *a = la + common + ra;
        *b = lb + common + rb;
    }
}


void Aligner::process(){
    QList<QVector<int> > group = groupByLongestCommon();
    for(QList<QVector<int> >::iterator it = group.begin(); it != group.end(); it++){
        for(int i = 1; i < it->size(); i++){
            simpleAlignement(&(_sequences[it->at(0)]), &(_sequences[it->at(i)]));
        }
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
