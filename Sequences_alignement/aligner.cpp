#include "aligner.h"

using namespace std;

Aligner::Aligner(QList<QStringList> sequences)
{
    _sequences = sequences;
}

QList<QVector<int> > Aligner::groupByLonguestCommon(){
    QList<QVector<int> > group; //returned variable
    int ** scores = new int * [_sequences.size()]{};
    scores[_sequences.size() - 1] = new int[1]{};
    cout << "Matrix" << endl;
    for(int i = 0; i < _sequences.size() - 1; i++){
        scores[i] = new int[ (_sequences.size() - i - 1) ]{};
        for(int j = i + 1; j = _sequences.size(); j++){
            scores[i][j - i - 1] = Helper::longuestCommon(&(_sequences.at(i)),&(_sequences.at(j)));
            cout << scores[i][j - i - 1] << " | " << endl;
        }
        cout << endl;
    }
    return group;
}


void Aligner::process(){
    groupByLonguestCommon();
}

QList<QStringList> Aligner::getSequences(){
    return _sequences;
}
