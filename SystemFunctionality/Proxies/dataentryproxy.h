#ifndef DATAENTRYPROXY_H
#define DATAENTRYPROXY_H

#include <QObject>

#include "../Utilities/game.h"

class GameManager;

class DataEntryProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryProxy(QObject *parent = nullptr);

    void saveDataToDb();
    void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms);
    void requestCacheReset();

    Game::ValidationCodes getPairEntryValidationCode() const;
    int getLastSavedNrOfCacheEntries() const;
    int getCurrentNrOfCacheEntries() const;

signals:
    Q_SIGNAL void dataEntryAllowed(bool allowed);
    Q_SIGNAL void newWordsPairValidated(bool isValid);
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void addInvalidWordsPairRequested();
    Q_SIGNAL void wordsPairAlreadyContainedInCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void writeDataToDbFinished(int nrOfEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
};

#endif // DATAENTRYPROXY_H
