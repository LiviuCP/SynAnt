#include "datasourceproxy.h"
#include "../DataAccess/datasource.h"
#include "../Management/gamemanager.h"

DataSourceProxy::DataSourceProxy(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_pDataSource{pDataSource}
{
}

void DataSourceProxy::loadDataFromDb()
{
    GameManager::getManager()->loadDataFromDb();
}

void DataSourceProxy::saveDataToDbIfValid(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    GameManager::getManager()->saveDataToDbIfValid(newWordsPair, areSynonyms);
}

void DataSourceProxy::provideDataEntryToConsumer(int entryNumber)
{
    m_pDataSource->provideDataEntryToConsumer(entryNumber);
}

int DataSourceProxy::getNrOfValidEntries()
{
    return m_pDataSource->getNrOfValidEntries();
}
