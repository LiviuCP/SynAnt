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
    void saveDataToDb();
    void resetDataEntryCache();
    void provideDataEntryToConsumer(int entryNumber);
    void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms);
    int getNrOfValidEntries();
    Game::ValidationCodes getPairEntryValidationCode() const;

signals:
    Q_SIGNAL void invalidWordsPairAddedByUser();
    Q_SIGNAL void newWordsPairValidated(bool isValid);
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void wordsPairAlreadyContainedInCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void readDataFromDbFinished(bool success);
    Q_SIGNAL void writeDataToDbFinished(int nrOfEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
};

#endif // DATASOURCEPROXY_H
