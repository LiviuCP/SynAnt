#include <QDir>
#include <QThread>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>

#include "gamemanager.h"
#include "gamefacade.h"
#include "dataentryfacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/datasourceloader.h"
#include "../DataAccess/dataentryvalidator.h"
#include "../DataAccess/dataentrycache.h"
#include "../DataAccess/dataentrystatistics.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../Utilities/statisticsitem.h"
#include "../Utilities/chronometer.h"
#include "../Utilities/exceptions.h"
#include "../Utilities/databaseutils.h"

GameManager* GameManager::s_pGameManager = nullptr;

GameManager::GameManager(QObject *parent)
    : QObject(parent)
    , m_pGameFacade{nullptr}
    , m_pDataEntryFacade{nullptr}
    , m_pDataSource{nullptr}
    , m_pDataSourceLoader{nullptr}
    , m_pDataEntryValidator{nullptr}
    , m_pDataEntryCache{nullptr}
    , m_pDataEntryStatistics{nullptr}
    , m_pDataSourceAccessHelper{new DataSourceAccessHelper{this}}
    , m_pWordMixer{new WordMixer{this}}
    , m_pWordPairOwner{new WordPairOwner{this}}
    , m_pInputBuilder{new InputBuilder{this}}
    , m_pStatisticsItem{new StatisticsItem{this}}
    , m_pChronometer{new Chronometer{this}}
    , m_pDataSourceLoaderThread{nullptr}
    , m_pDataEntryCacheThread{nullptr}
{
    _registerMetaTypes();
}

GameManager *GameManager::getManager()
{
    if (!s_pGameManager)
    {
        s_pGameManager = new GameManager{};
    }

    return s_pGameManager;
}

void GameManager::setEnvironment(const QString &dataDirPath)
{
    Q_ASSERT(QDir{dataDirPath}.exists());

    if (!m_pDataSource)
    {
        QString databasePath{dataDirPath + "/" + Database::c_DatabaseName};

        _setDatabase(databasePath);

        m_pDataSource = new DataSource{this};
        m_pDataSourceLoader = new DataSourceLoader{m_pDataSource, databasePath};
        m_pDataSourceLoaderThread = new QThread{this};
        m_pDataEntryValidator = new DataEntryValidator{m_pDataSource, this};
        m_pDataEntryCache = new DataEntryCache{m_pDataSource, databasePath};
        m_pDataEntryCacheThread = new QThread{this};
        m_pDataEntryStatistics = new DataEntryStatistics{this};

        // always ensure these items are created after all data source related items are initialized
        m_pDataSourceLoader->moveToThread(m_pDataSourceLoaderThread);
        m_pDataEntryCache->moveToThread(m_pDataEntryCacheThread);

        _makeDataConnections();

        m_pDataSourceLoaderThread->start();
        m_pDataEntryCacheThread->start();

        if (m_pDataSourceLoaderThread->isRunning() && m_pDataEntryCacheThread->isRunning())
        {
            /* the facades are created by manager and will build the connections to the other manager provided components
                                                        (WordMixer, WordPairOwner, InputBuilder, StatisticsItem, etc) on their own */
            m_pGameFacade = new GameFacade{this};
            m_pDataEntryFacade = new DataEntryFacade{this};
        }
        else
        {
            Q_ASSERT(false);
        }
    }
}

void GameManager::fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult)
{
    Q_EMIT readDataForPrimaryLanguage(languageIndex, allowEmptyResult);
}

void GameManager::fetchDataForSecondaryLanguage(int languageIndex)
{
    Q_EMIT readDataForSecondaryLanguage(languageIndex);
}

void GameManager::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex)
{
    m_pDataEntryValidator->validateWordsPair(newWordsPair, areSynonyms, languageIndex);
}

void GameManager::requestCacheReset()
{
    Q_EMIT resetCacheRequested();
}

void GameManager::saveDataToDb()
{
    Q_EMIT writeDataToDb();
}

void GameManager::provideDataEntryToConsumer(int entryNumber)
{
    m_pDataSource->provideDataEntryToConsumer(entryNumber);
}

void GameManager::releaseResources()
{
    _deallocResources();
}

DataEntry::ValidationCodes GameManager::getPairEntryValidationCode() const
{
    return m_pDataEntryValidator->getValidationCode();
}

int GameManager::getNrOfDataSourceEntries() const
{
    return m_pDataSource->getPrimarySourceNrOfEntries();
}

int GameManager::getLastSavedTotalNrOfEntries() const
{
    return m_pDataEntryStatistics->getLastSavedTotalNrOfEntries();
}

int GameManager::getLastNrOfEntriesSavedToPrimaryLanguage() const
{
    return m_pDataEntryStatistics->getLastNrOfEntriesSavedToPrimaryLanguage();
}

int GameManager::getCurrentNrOfCachedEntries() const
{
    return m_pDataEntryStatistics->getCurrentNrOfCacheEntries();
}

GameFacade* GameManager::getGameFacade() const
{
    return m_pGameFacade;
}

DataEntryFacade* GameManager::getDataEntryFacade() const
{
    return m_pDataEntryFacade;
}

DataSourceAccessHelper *GameManager::getDataSourceAccessHelper() const
{
    return m_pDataSourceAccessHelper;
}

WordMixer* GameManager::getWordMixer() const
{
    return m_pWordMixer;
}

WordPairOwner* GameManager::getWordPairOwner() const
{
    return m_pWordPairOwner;
}

InputBuilder* GameManager::getInputBuilder() const
{
    return m_pInputBuilder;
}

StatisticsItem* GameManager::getStatisticsItem() const
{
    return m_pStatisticsItem;
}

Chronometer *GameManager::getChronometer() const
{
    return m_pChronometer;
}

GameManager::~GameManager()
{
    m_pDataSourceLoaderThread->quit();
    m_pDataSourceLoaderThread->wait();
    m_pDataEntryCacheThread->quit();
    m_pDataEntryCacheThread->wait();
}

void GameManager::_onLoadDataFromDbForPrimaryLanguageFinished(bool success, bool validEntriesLoaded)
{
    Q_EMIT fetchDataForPrimaryLanguageFinished(success, validEntriesLoaded);
    Q_EMIT dataEntryAllowed(success);
}

void GameManager::_onRequestedPrimaryLanguageAlreadyContainedInDataSource(bool entriesAvailable)
{
    Q_EMIT fetchDataForPrimaryLanguageFinished(true, entriesAvailable);
    Q_EMIT dataEntryAllowed(true);
}

void GameManager::_onLoadDataFromDbForSecondaryLanguageFinished(bool success)
{
    // keep exactly this order
    Q_EMIT fetchDataForSecondaryLanguageFinished(success);
    Q_EMIT fetchDataForDataEntryLanguageFinished(success);
}

void GameManager::_onRequestedSecondaryLanguageAlreadySetAsPrimary()
{
    // keep exactly this order
    Q_EMIT fetchDataForSecondaryLanguageFinished(true);
    Q_EMIT fetchDataForDataEntryLanguageFinished(true);
}

void GameManager::_onNewWordsPairAddedToCache()
{
    // keep exactly this execution order (statistics signal should always be executed first)
    Q_EMIT recordAddedPairRequested();
    Q_EMIT newWordsPairAddedToCache();
}

void GameManager::_onWordsPairAlreadyContainedInCache()
{
    Q_EMIT wordsPairAlreadyContainedInCache();
}

void GameManager::_onAddInvalidWordsPairRequested()
{
    Q_EMIT addInvalidWordsPairRequested();
}

void GameManager::_onCacheReset()
{
    // keep exactly this execution order (statistics signal should always be executed first)
    Q_EMIT currentEntriesStatisticsResetRequested();
    Q_EMIT cacheReset();
}

void GameManager::_onWriteDataToDbFinished(int nrOfPrimaryLanguageSavedEntries, int totalNrOfSavedEntries)
{
    // keep exactly this execution order (statistics signal should always be executed first)
    Q_EMIT dataSavedStatisticsUpdateRequested(nrOfPrimaryLanguageSavedEntries, totalNrOfSavedEntries);
    Q_EMIT primaryLanguageDataSavingFinished(nrOfPrimaryLanguageSavedEntries);
    Q_EMIT writeDataToDbFinished();
}

void GameManager::_onWriteDataToDbErrorOccured()
{
    Q_EMIT dataSavingErrorOccured();
}

void GameManager::_onEntryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    Q_EMIT entryProvidedToConsumer(newWordsPair, areSynonyms);
}

void GameManager::_deallocResources()
{
    if (s_pGameManager)
    {
        delete s_pGameManager;
        s_pGameManager = nullptr;
    }
}

void GameManager::_setDatabase(const QString& databasePath)
{
    if (QSqlDatabase::isDriverAvailable(Database::c_DbDriverName))
    {
        QSqlDatabase db{QSqlDatabase::addDatabase(Database::c_DbDriverName)};
        db.setDatabaseName(databasePath);

        if (db.open())
        {
            if (!db.tables().contains(Database::c_TableName))
            {
                QSqlQuery createTableQuery;

                createTableQuery.prepare(Database::c_CreateTableQuery);

                if (!createTableQuery.exec())
                {
                    throw GameException{Game::Error::c_CannotCreateTable};
                }
            }
            else
            {
                bool isValidTable{true};

                if (db.record(Database::c_TableName).count() != Game::Misc::c_RequiredNrOfDbTableFields)
                {
                    isValidTable = false;
                }
                else if (db.record(Database::c_TableName).field(0).name() != Database::c_IdFieldName ||
                         db.record(Database::c_TableName).field(1).name() != Database::c_FirstWordFieldName ||
                         db.record(Database::c_TableName).field(2).name() != Database::c_SecondWordFieldName ||
                         db.record(Database::c_TableName).field(3).name() != Database::c_AreSynonymsFieldName ||
                         db.record(Database::c_TableName).field(4).name() != Database::c_LanguageFieldName)
                {
                    isValidTable = false;
                }
                else if (db.record(Database::c_TableName).field(0).type() != QVariant::Int ||
                         db.record(Database::c_TableName).field(1).type() != QVariant::String ||
                         db.record(Database::c_TableName).field(2).type() != QVariant::String ||
                         db.record(Database::c_TableName).field(3).type() != QVariant::Int ||
                         db.record(Database::c_TableName).field(4).type() != QVariant::String)
                {
                    isValidTable = false;
                }

                if (!isValidTable)
                {
                    throw GameException{Game::Error::c_TableIsInvalid};
                }
            }
            db.close();
        }
        else
        {
            throw FileException{Game::Error::c_CannotOpenDatabase, databasePath};
        }
    }
    else
    {
        throw GameException{Game::Error::c_DatabaseDriverNotAvailable};
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void GameManager::_makeDataConnections()
{
    Q_ASSERT(m_pDataSourceLoader);
    Q_ASSERT(m_pDataSourceLoaderThread);
    Q_ASSERT(m_pDataEntryCache);
    Q_ASSERT(m_pDataEntryCacheThread);
    Q_ASSERT(m_pDataEntryStatistics);
    Q_ASSERT(m_pDataEntryValidator);
    Q_ASSERT(m_pDataSource);

    // loader
    bool connected{connect(m_pDataSourceLoaderThread, &QThread::finished, m_pDataSourceLoader, &DataSourceLoader::deleteLater)};
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::readDataForPrimaryLanguage, m_pDataSourceLoader, &DataSourceLoader::onLoadDataFromDbForPrimaryLanguageRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::readDataForSecondaryLanguage, m_pDataSourceLoader, &DataSourceLoader::onLoadDataFromDbForSecondaryLanguageRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceLoader, &DataSourceLoader::loadDataFromDbForPrimaryLanguageFinished, this, &GameManager::_onLoadDataFromDbForPrimaryLanguageFinished, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceLoader, &DataSourceLoader::requestedPrimaryLanguageAlreadyContainedInDataSource, this, &GameManager::_onRequestedPrimaryLanguageAlreadyContainedInDataSource, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceLoader, &DataSourceLoader::loadDataFromDbForSecondaryLanguageFinished, this, &GameManager::_onLoadDataFromDbForSecondaryLanguageFinished, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceLoader, &DataSourceLoader::requestedSecondaryLanguageAlreadySetAsPrimary, this, &GameManager::_onRequestedSecondaryLanguageAlreadySetAsPrimary, Qt::QueuedConnection);
    Q_ASSERT(connected);

    // cache
    connected = connect(m_pDataEntryCacheThread, &QThread::finished, m_pDataEntryCache, &DataEntryCache::deleteLater);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::writeDataToDb, m_pDataEntryCache, &DataEntryCache::onWriteDataToDbRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::resetCacheRequested, m_pDataEntryCache, &DataEntryCache::onResetCacheRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::newWordsPairAddedToCache, this, &GameManager::_onNewWordsPairAddedToCache, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::writeDataToDbFinished, this, &GameManager::_onWriteDataToDbFinished, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::cacheReset, this, &GameManager::_onCacheReset, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::writeDataToDbErrorOccured, this, &GameManager::_onWriteDataToDbErrorOccured, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::wordsPairAlreadyContainedInCache, this, &GameManager::_onWordsPairAlreadyContainedInCache, Qt::QueuedConnection);
    Q_ASSERT(connected);

    // validator
    connected = connect(m_pDataEntryValidator, &DataEntryValidator::addInvalidWordsPairRequested, this, &GameManager::_onAddInvalidWordsPairRequested, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryValidator, &DataEntryValidator::entryValidated, m_pDataEntryCache, &DataEntryCache::onValidEntryReceived, Qt::QueuedConnection);
    Q_ASSERT(connected);

    // datasource
    connected = connect(m_pDataSource, &DataSource::entryProvidedToConsumer, this, &GameManager::_onEntryProvidedToConsumer, Qt::DirectConnection);
    Q_ASSERT(connected);

    // data entry statistics
    connected = connect(this, &GameManager::recordAddedPairRequested, m_pDataEntryStatistics, &DataEntryStatistics::onRecordAddedPairRequested, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::dataSavedStatisticsUpdateRequested, m_pDataEntryStatistics, &DataEntryStatistics::onDataSavedStatisticsUpdateRequested, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::currentEntriesStatisticsResetRequested, m_pDataEntryStatistics, &DataEntryStatistics::onCurrentEntriesStatisticsResetRequested, Qt::DirectConnection);
    Q_ASSERT(connected);
}

void GameManager::_registerMetaTypes()
{
    Q_UNUSED(qRegisterMetaType<DataSource::DataEntry>());
}
