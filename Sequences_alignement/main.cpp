#include <QtCore>
#include <iostream>
#include "aligner.h"
#include "helper.h"

#include <chrono>

using namespace std;
using namespace std::chrono;

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

    Aligner aligner(sequences);
    int size, a, b;
    size = Helper::longuestCommon(&(sequences.at(0)), &(sequences.at(1)), &a, &b);

    cout <<"Equal subsequence between 0 & 1 is : " << endl;
    cout << "a starts at " << a << " and b starts at " << b << endl;
    for(int i=0; i < size; i++){
        cout << sequences.at(0).at(a+i).toStdString();
    }
    cout << endl << endl;

    //multiAlignment(&sequences);
    //alignSequences(sequences+3, sequences + 2);
    //alignSequencesSc(sequences + 4, sequences);
    printSequences(&sequences);

    /*float seuil = 0;
    float * pw = calculateScore(&sequences, &seuil);
    cout << "Scores (seuil = " << seuil << ") : " << endl;
    for(unsigned int i = 0; i < 8; i++){
        cout << pw[i] << " | ";
    }
    cout << endl;*/
    return 0;
}
