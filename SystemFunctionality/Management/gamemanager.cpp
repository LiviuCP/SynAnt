#include <QDir>
#include <QThread>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>

#include "gamemanager.h"
#include "gamefacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/datasourceloader.h"
#include "../DataAccess/dataentryvalidator.h"
#include "../DataAccess/dataentrycache.h"
#include "../DataAccess/dataentrystatistics.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../Proxies/datasourceproxy.h"
#include "../Utilities/scoreitem.h"
#include "../Utilities/gamestrings.h"
#include "../Utilities/exceptions.h"

GameManager* GameManager::s_pGameManager = nullptr;

GameManager *GameManager::getManager()
{
    if (!s_pGameManager)
    {
        s_pGameManager = new GameManager{};
    }

    return s_pGameManager;
}

void GameManager::releaseResources()
{
    if (s_pGameManager)
    {
        delete s_pGameManager;
        s_pGameManager = nullptr;
    }
}

GameManager::GameManager(QObject *parent)
    : QObject(parent)
    , m_pGameFacade{nullptr}
    , m_pDataSource{nullptr}
    , m_pDataSourceLoader{nullptr}
    , m_pDataEntryValidator{nullptr}
    , m_pDataEntryCache{nullptr}
    , m_pDataEntryStatistics{nullptr}
    , m_pDataSourceProxy{nullptr}
    , m_pDataSourceAccessHelper{new DataSourceAccessHelper{this}}
    , m_pWordMixer{new WordMixer{this}}
    , m_pWordPairOwner{new WordPairOwner{this}}
    , m_pInputBuilder{new InputBuilder{this}}
    , m_pScoreItem {new ScoreItem{this}}
    , m_pDataSourceLoaderThread{nullptr}
    , m_pDataEntryCacheThread{nullptr}
{
    _registerMetaTypes();

    bool connected{connect(this, &GameManager::dataSourceSetupCompleted, this, &GameManager::_onDataSourceSetupCompleted)};
    Q_ASSERT(connected);
}

void GameManager::setDataSource(const QString &dataDirPath)
{
    Q_ASSERT(QDir{dataDirPath}.exists());

    if (!m_pDataSource)
    {
        QString databasePath{dataDirPath + "/" + GameStrings::c_DatabaseName};

        _setDatabase(databasePath);

        m_pDataSource = new DataSource{databasePath, this};
        m_pDataSourceLoader = new DataSourceLoader{m_pDataSource};
        m_pDataSourceLoaderThread = new QThread{this};
        m_pDataEntryValidator = new DataEntryValidator{m_pDataSource, this};
        m_pDataEntryCache = new DataEntryCache{m_pDataSource};
        m_pDataEntryCacheThread = new QThread{this};
        m_pDataEntryStatistics = new DataEntryStatistics{this};

        // always ensure this item is created after all data source related items are initialized
        m_pDataSourceProxy = new DataSourceProxy{this};

        m_pDataSourceLoader->moveToThread(m_pDataSourceLoaderThread);
        m_pDataEntryCache->moveToThread(m_pDataEntryCacheThread);

        _makeDataConnections();

        m_pDataSourceLoaderThread->start();
        m_pDataEntryCacheThread->start();

        Q_EMIT dataSourceSetupCompleted();
    }
}

void GameManager::loadDataFromDb()
{
    Q_EMIT readDataFromDb();
}

void GameManager::saveDataToDb()
{
    Q_EMIT writeDataToDb();
}

void GameManager::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    m_pDataEntryValidator->validateWordsPair(newWordsPair, areSynonyms);
}

void GameManager::requestCacheReset()
{
    Q_EMIT resetCacheRequested();
}

void GameManager::provideDataEntryToConsumer(int entryNumber)
{
    m_pDataSource->provideDataEntryToConsumer(entryNumber);
}

int GameManager::getNrOfValidDataSourceEntries() const
{
    return m_pDataSource->getNrOfValidEntries();
}

Game::ValidationCodes GameManager::getPairEntryValidationCode() const
{
    return m_pDataEntryValidator->getValidationCode();
}

GameFacade* GameManager::getFacade() const
{
    return m_pGameFacade;
}

DataSource* GameManager::getDataSource() const
{
    return m_pDataSource;
}

DataEntryValidator* GameManager::getDataEntryValidator() const
{
    return m_pDataEntryValidator;
}

DataEntryCache* GameManager::getDataEntryCache() const
{
    return m_pDataEntryCache;
}

DataSourceProxy* GameManager::getDataSourceProxy() const
{
    return m_pDataSourceProxy;
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

ScoreItem* GameManager::getScoreItem() const
{
    return m_pScoreItem;
}

int GameManager::getLastSavedNrOfCacheEntries() const
{
    return m_pDataEntryStatistics->getLastSavedNrOfCacheEntries();
}

int GameManager::getCurrentNrOfCacheEntries() const
{
    return m_pDataEntryStatistics->getCurrentNrOfCacheEntries();
}

GameManager::~GameManager()
{
    m_pDataSourceLoaderThread->quit();
    m_pDataSourceLoaderThread->wait();
    m_pDataEntryCacheThread->quit();
    m_pDataEntryCacheThread->wait();
}

void GameManager::_onDataSourceSetupCompleted()
{
    Q_ASSERT(m_pDataSourceLoaderThread->isRunning() && m_pDataEntryCacheThread->isRunning());

    // the facade is created by manager and will built it's own connections to the other manager provided components: WordMixer, WordPairOwner, InputBuilder, ScoreItem, data proxy
    m_pGameFacade = new GameFacade{this};
}

void GameManager::_setDatabase(const QString& databasePath)
{
    if (QSqlDatabase::isDriverAvailable(GameStrings::c_DbDriverName))
    {
        QSqlDatabase db{QSqlDatabase::addDatabase(GameStrings::c_DbDriverName)};
        db.setDatabaseName(databasePath);

        if (db.open())
        {
            if (!db.tables().contains(GameStrings::c_TableName))
            {
                QSqlQuery createTableQuery;

                createTableQuery.prepare(GameStrings::c_CreateTableQuery);

                if (!createTableQuery.exec())
                {
                    throw GameException{GameStrings::c_CannotCreateTable};
                }
            }
            else
            {
                bool isValidTable{true};

                if (db.record(GameStrings::c_TableName).count() != Game::c_RequiredNrOfDbTableFields)
                {
                    isValidTable = false;
                }
                else if (db.record(GameStrings::c_TableName).field(0).name() != GameStrings::c_IdFieldName ||
                         db.record(GameStrings::c_TableName).field(1).name() != GameStrings::c_FirstWordFieldName ||
                         db.record(GameStrings::c_TableName).field(2).name() != GameStrings::c_SecondWordFieldName ||
                         db.record(GameStrings::c_TableName).field(3).name() != GameStrings::c_AreSynonymsFieldName ||
                         db.record(GameStrings::c_TableName).field(4).name() != GameStrings::c_LanguageFieldName)
                {
                    isValidTable = false;
                }
                else if (db.record(GameStrings::c_TableName).field(0).type() != QVariant::Int ||
                         db.record(GameStrings::c_TableName).field(1).type() != QVariant::String ||
                         db.record(GameStrings::c_TableName).field(2).type() != QVariant::String ||
                         db.record(GameStrings::c_TableName).field(3).type() != QVariant::Int ||
                         db.record(GameStrings::c_TableName).field(4).type() != QVariant::String)
                {
                    isValidTable = false;
                }

                if (!isValidTable)
                {
                    throw GameException{GameStrings::c_TableIsInvalid};
                }
            }
            db.close();
        }
        else
        {
            throw FileException{GameStrings::c_CannotOpenDatabase, databasePath};
        }
    }
    else
    {
        throw GameException{GameStrings::c_DatabaseDriverNotAvailable};
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
    connected = connect(m_pDataSourceLoader, &DataSourceLoader::loadDataFromDbFinished, m_pDataSourceProxy, &DataSourceProxy::loadDataFromDbFinished, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::readDataFromDb, m_pDataSourceLoader, &DataSourceLoader::onLoadDataFromDbRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    // cache
    connected = connect(m_pDataEntryCacheThread, &QThread::finished, m_pDataEntryCache, &DataEntryCache::deleteLater);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::writeDataToDb, m_pDataEntryCache, &DataEntryCache::onWriteDataToDbRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::resetCacheRequested, m_pDataEntryCache, &DataEntryCache::onResetCacheRequested, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::newWordsPairAddedToCache, this, &GameManager::newWordsPairAddedToCache, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::writeDataToDbFinished, this, &GameManager::writeDataToDbFinished, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::cacheReset, this, &GameManager::cacheReset, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::writeDataToDbErrorOccured, m_pDataSourceProxy, &DataSourceProxy::writeDataToDbErrorOccured, Qt::QueuedConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryCache, &DataEntryCache::wordsPairAlreadyContainedInCache, m_pDataSourceProxy, &DataSourceProxy::wordsPairAlreadyContainedInCache, Qt::QueuedConnection);
    Q_ASSERT(connected);
    // validator
    connected = connect(m_pDataEntryValidator, &DataEntryValidator::addInvalidWordsPairRequested, m_pDataSourceProxy, &DataSourceProxy::addInvalidWordsPairRequested, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryValidator, &DataEntryValidator::entryValidated, m_pDataEntryCache, &DataEntryCache::onValidEntryReceived, Qt::QueuedConnection);
    Q_ASSERT(connected);
    // datasource
    connected = connect(m_pDataSource, &DataSource::entryProvidedToConsumer, m_pDataSourceProxy, &DataSourceProxy::entryProvidedToConsumer, Qt::DirectConnection);
    Q_ASSERT(connected);
    // data entry statistics
    connected = connect(this, &GameManager::newWordsPairAddedToCache, m_pDataEntryStatistics, &DataEntryStatistics::onNewWordsPairAddedToCache, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::writeDataToDbFinished, m_pDataEntryStatistics, &DataEntryStatistics::onWriteDataToDbFinished, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::cacheReset, m_pDataEntryStatistics, &DataEntryStatistics::onCacheReset, Qt::DirectConnection);
    Q_ASSERT(connected);
    // game manager to proxy
    connected = connect(this, &GameManager::newWordsPairAddedToCache, m_pDataSourceProxy, &DataSourceProxy::newWordsPairAddedToCache, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::writeDataToDbFinished, m_pDataSourceProxy, &DataSourceProxy::writeDataToDbFinished, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(this, &GameManager::cacheReset, m_pDataSourceProxy, &DataSourceProxy::cacheReset, Qt::DirectConnection);
    Q_ASSERT(connected);
}

void GameManager::_registerMetaTypes()
{
    Q_UNUSED(qRegisterMetaType<DataSource::DataEntry>());
}
