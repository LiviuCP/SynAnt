#include "datasourceproxy.h"
#include "../Management/gamemanager.h"

DataSourceProxy::DataSourceProxy(QObject *parent)
    : QObject(parent)
{
}

void DataSourceProxy::loadDataFromDb()
{
    GameManager::getManager()->loadDataFromDb();
}

void DataSourceProxy::saveDataToDb()
{
    GameManager::getManager()->saveDataToDb();
}

void DataSourceProxy::requestCacheReset()
{
    GameManager::getManager()->requestCacheReset();
}

void DataSourceProxy::provideDataEntryToConsumer(int entryNumber)
{
    GameManager::getManager()->provideDataEntryToConsumer(entryNumber);
}

void DataSourceProxy::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    GameManager::getManager()->requestWriteToCache(newWordsPair, areSynonyms);
}

int DataSourceProxy::getNrOfValidDataSourceEntries()
{
    return GameManager::getManager()->getNrOfValidDataSourceEntries();
}

Game::ValidationCodes DataSourceProxy::getPairEntryValidationCode() const
{
    return GameManager::getManager()->getPairEntryValidationCode();
}

int DataSourceProxy::getLastSavedNrOfCacheEntries() const
{
    return GameManager::getManager()->getLastSavedNrOfCacheEntries();
}

int DataSourceProxy::getCurrentNrOfCacheEntries() const
{
    return GameManager::getManager()->getCurrentNrOfCacheEntries();
}
