#include "gamefunctionalityproxy.h"
#include "../Management/gamemanager.h"
#include "../Proxies/datasourceproxy.h"

GameFunctionalityProxy::GameFunctionalityProxy(QObject *parent)
    : QObject(parent)
{
}

DataSourceProxy* GameFunctionalityProxy::getDataSourceProxy() const
{
    return GameManager::getManager()->getDataSourceProxy();
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

ScoreItem* GameFunctionalityProxy::getScoreItem() const
{
    return GameManager::getManager()->getScoreItem();
}
