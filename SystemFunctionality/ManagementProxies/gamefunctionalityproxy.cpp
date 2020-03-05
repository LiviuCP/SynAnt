#include "gamefunctionalityproxy.h"
#include "../Management/gamemanager.h"
#include "../ManagementProxies/dataaccessproxy.h"

GameFunctionalityProxy::GameFunctionalityProxy(QObject *parent)
    : QObject(parent)
{
}

DataAccessProxy* GameFunctionalityProxy::getDataAccessProxy() const
{
    return GameManager::getManager()->getDataAccessProxy();
}

DataSourceAccessHelper* GameFunctionalityProxy::getDataSourceAccessHelper() const
{
    return GameManager::getManager()->getDataSourceAccessHelper();
}

WordMixer* GameFunctionalityProxy::getWordMixer() const
{
    return GameManager::getManager()->getWordMixer();
}

WordPairOwner* GameFunctionalityProxy::getWordPairOwner() const
{
    return GameManager::getManager()->getWordPairOwner();
}

InputBuilder* GameFunctionalityProxy::getInputBuilder() const
{
    return GameManager::getManager()->getInputBuilder();
}

StatisticsItem* GameFunctionalityProxy::getStatisticsItem() const
{
    return GameManager::getManager()->getStatisticsItem();
}

Chronometer *GameFunctionalityProxy::getChronometer() const
{
    return GameManager::getManager()->getChronometer();
}
