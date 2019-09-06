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

void DataSourceProxy::provideDataEntryToConsumer(int entryNumber)
{
    GameManager::getManager()->provideDataEntryToConsumer(entryNumber);
}

int DataSourceProxy::getNrOfDataSourceEntries()
{
    return GameManager::getManager()->getNrOfDataSourceEntries();
}
