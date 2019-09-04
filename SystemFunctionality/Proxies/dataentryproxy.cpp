#include "dataentryproxy.h"
#include "../Management/gamemanager.h"

DataEntryProxy::DataEntryProxy(QObject *parent) : QObject(parent)
{
}

void DataEntryProxy::saveDataToDb()
{
    GameManager::getManager()->saveDataToDb();
}

void DataEntryProxy::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    GameManager::getManager()->requestWriteToCache(newWordsPair, areSynonyms);
}

void DataEntryProxy::requestCacheReset()
{
    GameManager::getManager()->requestCacheReset();
}

DataEntry::ValidationCodes DataEntryProxy::getPairEntryValidationCode() const
{
    return GameManager::getManager()->getPairEntryValidationCode();
}

int DataEntryProxy::getLastSavedNrOfCacheEntries() const
{
    return GameManager::getManager()->getLastSavedNrOfCacheEntries();
}

int DataEntryProxy::getCurrentNrOfCacheEntries() const
{
    return GameManager::getManager()->getCurrentNrOfCacheEntries();
}
