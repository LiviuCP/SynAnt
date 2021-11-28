#include "gamefunctionalityproxy.h"
#include "gamemanager.h"

GameFunctionalityProxy::GameFunctionalityProxy(QObject *parent)
    : QObject(parent)
{
    GameManager* pGameManager{GameManager::getManager()};
    Q_ASSERT(pGameManager);

    auto connected{connect(pGameManager, &GameManager::primaryLanguageDataSavingFinished, this, &GameFunctionalityProxy::primaryLanguageDataSavingFinished, Qt::DirectConnection)};
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::entryProvidedToConsumer, this, &GameFunctionalityProxy::entryProvidedToConsumer, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::dataSavingErrorOccured, this, &GameFunctionalityProxy::dataSavingErrorOccured, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::fetchDataForPrimaryLanguageFinished, this, &GameFunctionalityProxy::fetchDataForPrimaryLanguageFinished, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::fetchDataForSecondaryLanguageFinished, this, &GameFunctionalityProxy::fetchDataForSecondaryLanguageFinished, Qt::DirectConnection);
    Q_ASSERT(connected);
}

void GameFunctionalityProxy::fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult)
{
    GameManager::getManager()->fetchDataForPrimaryLanguage(languageIndex, allowEmptyResult);
}

void GameFunctionalityProxy::provideDataEntryToConsumer(int entryNumber)
{
    GameManager::getManager()->provideDataEntryToConsumer(entryNumber);
}

int GameFunctionalityProxy::getNrOfDataSourceEntries() const
{
    return GameManager::getManager()->getNrOfDataSourceEntries();
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
