#include "datasourceproxy.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/dataentryvalidator.h"
#include "../DataAccess/dataentrycache.h"
#include "../Management/gamemanager.h"

DataSourceProxy::DataSourceProxy(QObject *parent)
    : QObject(parent)
    , m_pDataSource{nullptr}
    , m_pDataEntryValidator{nullptr}
    , m_pDataEntryCache{nullptr}
{
    if (GameManager::isManagerSetup())
    {
        m_pDataSource = GameManager::getManager()->getDataSource();
        m_pDataEntryValidator = GameManager::getManager()->getDataEntryValidator();
        m_pDataEntryCache = GameManager::getManager()->getDataEntryCache();
    }

    Q_ASSERT(m_pDataSource);
    Q_ASSERT(m_pDataEntryValidator);
    Q_ASSERT(m_pDataEntryCache);
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
    m_pDataSource->provideDataEntryToConsumer(entryNumber);
}

void DataSourceProxy::validateWordsPair(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    m_pDataEntryValidator->validateWordsPair(newWordsPair, areSynonyms);
}

int DataSourceProxy::getNrOfValidEntries()
{
    return m_pDataSource->getNrOfValidEntries();
}

int DataSourceProxy::getNrOfCachedEntries()
{
    return m_pDataEntryCache->getNrOfCachedEntries();
}
