#include "gamefunctionalityproxy.h"
#include "../Management/gamemanager.h"

GameFunctionalityProxy::GameFunctionalityProxy(QObject *parent)
    : QObject(parent)
{
}

DataSource* GameFunctionalityProxy::getDataSource() const
{
    return GameManager::getManager()->getDataSource();
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
