#include <QtCore>
#include <iostream>
#include "aligner.h"
#include "helper.h"

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

    Aligner aligner(sequences);
    aligner.process();

    return 0;
}
