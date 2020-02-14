#ifndef DATASOURCEPROXY_H
#define DATASOURCEPROXY_H

#include <QObject>

#include "../Utilities/gameutils.h"

class GameManager;

class DataSourceProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceProxy(QObject *parent = nullptr);

    void loadDataFromDb(int languageIndex, bool allowEmptyResult);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfDataSourceEntries();

signals:
    Q_SIGNAL void loadDataFromDbFinished(bool success, bool validEntriesLoaded);
    Q_SIGNAL void writeDataToDbFinished(int nrOfEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
};

#endif // DATASOURCEPROXY_H
