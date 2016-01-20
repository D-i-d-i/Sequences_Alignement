#include <QtCore>
#include <iostream>
#include <vector>
#include <map>
#include "sequences.h"

using namespace std;

void printSequences(QList<QStringList> * sequences){
    for(QList<QStringList>::iterator it = sequences->begin(); it != sequences->end(); it++){
        for(QStringList::iterator sit = it->begin(); sit != it->end(); sit++){
            cout << sit->toStdString();
        }
        cout << endl;
    }
    cout << endl;
}

void loadCsv(QList<QStringList > * sequences, QString fileName){
   sequences->clear();
   QFile file(fileName);
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
       while (!file.atEnd()){
           QString line = file.readLine();
            sequences->append(line.trimmed().split(QChar(','), QString::SkipEmptyParts));
       }
       file.close();
   }
}

int main(){
    QList<QStringList > sequences;
    loadCsv(&sequences, "../data/data.csv");
    printSequences(&sequences);

    //multiAlignment(sequences, 8);
    //alignSequences(sequences+3, sequences + 2);
    //alignSequencesSc(sequences + 4, sequences);
    printSequences(&sequences);

    /*float seuil = 0;
    float * pw = calculateScore(sequences, 8, &seuil);
    cout << "Scores (seuil = " << seuil << ") : " << endl;
    for(unsigned int i = 0; i < 8; i++){
        cout << pw[i] << " | ";
    }
    cout << endl;*/
}
