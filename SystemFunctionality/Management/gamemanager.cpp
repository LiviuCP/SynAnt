#include <QFileInfo>
#include <QFile>
#include <QThread>

#include "gamemanager.h"
#include "gamefacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
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
    , m_pDataSourceProxy{nullptr}
    , m_pDataSourceAccessHelper{new DataSourceAccessHelper{this}}
    , m_pWordMixer{new WordMixer{this}}
    , m_pWordPairOwner{new WordPairOwner{this}}
    , m_pInputBuilder{new InputBuilder{this}}
    , m_pScoreItem {new ScoreItem{this}}
    , m_pDataSourceThread{nullptr}
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

        // create the data source and assign it to a separate thread
        m_pDataSource = new DataSource{dataFilePath};
        m_pDataSourceThread = new QThread{this};
        m_pDataSourceProxy = new DataSourceProxy{m_pDataSource, this};

        m_pDataSource->moveToThread(m_pDataSourceThread);

        bool connected{connect(m_pDataSourceThread, &QThread::finished, m_pDataSource, &DataSource::deleteLater)};
        Q_ASSERT(connected);
        connected = connect(this, &GameManager::readDataFromDb, m_pDataSource, &DataSource::onReadDataFromDbRequested, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(this, &GameManager::writeDataToDbIfValid, m_pDataSource, &DataSource::onWriteDataToDbRequested, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataSource, &DataSource::readDataFromDbFinished, m_pDataSourceProxy, &DataSourceProxy::readDataFromDbFinished, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataSource, &DataSource::writeDataToDbFinished, m_pDataSourceProxy, &DataSourceProxy::writeDataToDbFinished, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataSource, &DataSource::writeDataToDbErrorOccured, m_pDataSourceProxy, &DataSourceProxy::writeDataToDbErrorOccured, Qt::QueuedConnection);
        Q_ASSERT(connected);
        connected = connect(m_pDataSource, &DataSource::entryProvidedToConsumer, m_pDataSourceProxy, &DataSourceProxy::entryProvidedToConsumer, Qt::QueuedConnection);
        Q_ASSERT(connected);

        m_pDataSourceThread->start();

        Q_EMIT dataSourceSetupCompleted();
    }
}

void GameManager::loadDataFromDb()
{
    Q_EMIT readDataFromDb();
}

void GameManager::saveDataToDbIfValid(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    Q_EMIT writeDataToDbIfValid(newWordsPair, areSynonyms);
}

GameFacade* GameManager::getFacade() const
{
    return m_pGameFacade;
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
    m_pDataSourceThread->quit();
    m_pDataSourceThread->wait();
}

void GameManager::_onDataSourceSetupCompleted()
{
    Q_ASSERT(m_pDataSourceThread->isRunning());

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
