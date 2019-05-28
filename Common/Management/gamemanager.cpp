#include <QFileInfo>
#include <QFile>

#include "gamemanager.h"
#include "gamefacade.h"
#include "DataAccess/datasource.h"
#include "DataAccess/datasourceaccesshelper.h"
#include "CoreFunctionality/wordmixer.h"
#include "Proxies/wordmixerproxy.h"
#include "CoreFunctionality/wordpairowner.h"
#include "CoreFunctionality/inputbuilder.h"
#include "Utilities/scoreitem.h"
#include "Utilities/gamestrings.h"
#include "Utilities/exceptions.h"

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
    , m_pDataSourceAccessHelper{new DataSourceAccessHelper{this}}
    , m_pWordMixer{new WordMixer{this}}
    , m_pWordPairOwner{new WordPairOwner{this}}
    , m_pInputBuilder{new InputBuilder{this}}
    , m_pScoreItem {new ScoreItem{this}}
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

        // manager only checks that that data file exists, can be opened and is not empty, all other checks to be done by DataSource
        if (!QFileInfo{dataFilePath}.exists())
        {
            throw FileException{GameStrings::c_FileNotFoundMessage, dataFilePath};
        }

        QFile dataFile{dataFilePath};

        if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw FileException{GameStrings::c_CannotOpenFileMessage, dataFilePath};
        }

        if (dataFile.readLine().size() == 0)
        {
            dataFile.close();
            throw FileException{GameStrings::c_EmptyFileMessage, dataFilePath};
        }

        dataFile.close();

        m_pDataSource = new DataSource{dataFilePath, this};

        Q_EMIT dataSourceSetupCompleted();
    }
}

GameFacade* GameManager::getFacade() const
{
    return m_pGameFacade;
}

DataSource* GameManager::getDataSource() const
{
    return m_pDataSource;
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

void GameManager::_onDataSourceSetupCompleted()
{
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
