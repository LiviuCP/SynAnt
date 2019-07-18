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

void DataSourceProxy::resetDataEntryCache()
{
    GameManager::getManager()->resetDataEntryCache();
}

void DataSourceProxy::provideDataEntryToConsumer(int entryNumber)
{
    GameManager::getManager()->provideDataEntryToConsumer(entryNumber);
}

void DataSourceProxy::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    GameManager::getManager()->requestWriteToCache(newWordsPair, areSynonyms);
}

int DataSourceProxy::getNrOfValidEntries()
{
    return GameManager::getManager()->getNrOfValidEntries();
}

Game::ValidationCodes DataSourceProxy::getPairEntryValidationCode() const
{
    return GameManager::getManager()->getPairEntryValidationCode();
}
