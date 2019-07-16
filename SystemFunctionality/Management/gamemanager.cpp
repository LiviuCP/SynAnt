#include <QFileInfo>
#include <QFile>
#include <QThread>

#include "gamemanager.h"
#include "gamefacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/datasourceloader.h"
#include "../DataAccess/dataentryvalidator.h"
#include "../DataAccess/dataentrycache.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../Proxies/datasourceproxy.h"
#include "../Proxies/wordmixerproxy.h"
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
    , m_pDataSourceProxy{nullptr}
    , m_pDataSourceAccessHelper{new DataSourceAccessHelper{this}}
    , m_pWordMixer{new WordMixer{this}}
    , m_pWordPairOwner{new WordPairOwner{this}}
    , m_pInputBuilder{new InputBuilder{this}}
    , m_pScoreItem {new ScoreItem{this}}
    , m_pDataSourceLoaderThread{nullptr}
{
    m_pWordMixerProxy = new WordMixerProxy{m_pWordMixer, this};

    bool connected{connect(this, &GameManager::dataSourceSetupCompleted, this, &GameManager::_onDataSourceSetupCompleted)};
    Q_ASSERT(connected);
}

void GameManager::setDataSource(const QString &dataDirPath)
{
    if (!m_pDataSource)
    {
        QString dataFilePath{dataDirPath + "/" + GameStrings::c_FileName};
        QFile dataFile{dataFilePath};

        // manager only checks that that data file exists and can be opened, all other checks to be done by DataSource
        if (!QFileInfo{dataFilePath}.exists())
        {
            if (!dataFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                throw FileException{GameStrings::c_CannotCreateFileMessage, dataFilePath};
            }

            dataFile.close();
        }

        if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw FileException{GameStrings::c_CannotOpenFileMessage, dataFilePath};
        }

        dataFile.close();

        m_pDataSource = new DataSource{dataFilePath, this};
        m_pDataSourceLoader = new DataSourceLoader{m_pDataSource};
        m_pDataSourceLoaderThread = new QThread{this};
        m_pDataEntryValidator = new DataEntryValidator{m_pDataSource, this};
        m_pDataEntryCache = new DataEntryCache{m_pDataSource};
        m_pDataEntryCacheThread = new QThread{this};

        // always ensure this item is created after all data source related items are initialized
        m_pDataSourceProxy = new DataSourceProxy{this};

        m_pDataSourceLoader->moveToThread(m_pDataSourceLoaderThread);
        m_pDataEntryCache->moveToThread(m_pDataEntryCacheThread);

        int id{qRegisterMetaType<DataSource::DataEntry>()};
        Q_UNUSED(id);

        // loader
        bool connected{connect(m_pDataSourceLoaderThread, &QThread::finished, m_pDataSourceLoader, &DataSourceLoader::deleteLater)};
        Q_ASSERT(connected);
        connected = connect(m_pDataSourceLoader, &DataSourceLoader::readDataFromDbFinished, m_pDataSourceProxy, &DataSourceProxy::readDataFromDbFinished, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(this, &GameManager::readDataFromDb, m_pDataSourceLoader, &DataSourceLoader::onLoadDataFromDbRequested, Qt::QueuedConnection);
        Q_ASSERT(connected);
        // cache
        connected = connect(m_pDataEntryCacheThread, &QThread::finished, m_pDataEntryCache, &DataEntryCache::deleteLater);
        Q_ASSERT(connected);
        connected = connect(m_pDataEntryCache, &DataEntryCache::writeDataToDbFinished, m_pDataSourceProxy, &DataSourceProxy::writeDataToDbFinished, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataEntryCache, &DataEntryCache::writeDataToDbErrorOccured, m_pDataSourceProxy, &DataSourceProxy::writeDataToDbErrorOccured, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataEntryCache, &DataEntryCache::newWordsPairAddedToCache, m_pDataSourceProxy, &DataSourceProxy::newWordsPairAddedToCache, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataEntryCache, &DataEntryCache::cacheReset, m_pDataSourceProxy, &DataSourceProxy::cacheReset, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(this, &GameManager::writeDataToDb, m_pDataEntryCache, &DataEntryCache::onWriteDataToDbRequested, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(this, &GameManager::resetCacheRequested, m_pDataEntryCache, &DataEntryCache::onResetCacheRequested, Qt::QueuedConnection);
        Q_ASSERT(connected);
        // validator
        connected = connect(m_pDataEntryValidator, &DataEntryValidator::invalidWordsPairAddedByUser, m_pDataSourceProxy, &DataSourceProxy::invalidWordsPairAddedByUser);
        Q_ASSERT(connected);
        connected = connect(m_pDataEntryValidator, &DataEntryValidator::entryValidated, m_pDataEntryCache, &DataEntryCache::onValidEntryReceived, Qt::QueuedConnection);
        Q_ASSERT(connected);
        // datasource
        connected = connect(m_pDataSource, &DataSource::entryProvidedToConsumer, m_pDataSourceProxy, &DataSourceProxy::entryProvidedToConsumer, Qt::DirectConnection);
        Q_ASSERT(connected);

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

void GameManager::resetDataEntryCache()
{
    Q_EMIT resetCacheRequested();
}

void GameManager::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    m_pDataEntryValidator->validateWordsPair(newWordsPair, areSynonyms);
}

void GameManager::provideDataEntryToConsumer(int entryNumber)
{
    m_pDataSource->provideDataEntryToConsumer(entryNumber);
}

int GameManager::getNrOfValidEntries() const
{
    return m_pDataSource->getNrOfValidEntries();
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

GameManager::~GameManager()
{
    m_pDataSourceLoaderThread->quit();
    m_pDataSourceLoaderThread->wait();
    m_pDataEntryCacheThread->quit();
    m_pDataEntryCacheThread->wait();
}

void GameManager::_onDataSourceSetupCompleted()
{
    Q_ASSERT(m_pDataSourceLoaderThread->isRunning());

    // do all external backend connections except the ones to the facade (facade will build them itself)
    bool connected{connect(m_pWordPairOwner, &WordPairOwner::mixedWordsAvailable, m_pInputBuilder, &InputBuilder::onNewPiecesAvailable)};
    Q_ASSERT(connected);
    connected = connect(m_pWordMixer, &WordMixer::mixedWordsChanged, m_pWordPairOwner, &WordPairOwner::onMixedWordsAvailable);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::pieceAddedToInput, m_pWordPairOwner, &WordPairOwner::onPieceAddedToInput);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::piecesRemovedFromInput, m_pWordPairOwner, &WordPairOwner::onPiecesRemovedFromInput);
    Q_ASSERT(connected);

    m_pWordPairOwner->setWordMixerProxy(m_pWordMixerProxy);

    m_pGameFacade = new GameFacade{this};
}
