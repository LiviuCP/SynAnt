#ifndef DATASOURCEPROXY_H
#define DATASOURCEPROXY_H

#include <QObject>

#include "../Utilities/game.h"

class GameManager;

class DataSourceProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceProxy(QObject *parent = nullptr);

    void loadDataFromDb();
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfValidDataSourceEntries();

signals:
    Q_SIGNAL void loadDataFromDbFinished(bool success);
    Q_SIGNAL void writeDataToDbFinished(int nrOfEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
};

#endif // DATASOURCEPROXY_H
