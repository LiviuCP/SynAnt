#include "gamefacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../Proxies/gamefunctionalityproxy.h"
#include "../Proxies/datasourceproxy.h"
#include "../Utilities/scoreitem.h"
#include "../Utilities/gamestrings.h"

GameFacade::GameFacade(QObject *parent)
    : QObject(parent)
    , m_pGameFunctionalityProxy{new GameFunctionalityProxy{this}}
    , m_CurrentStatusCode{Game::StatusCodes::INVALID}
    , m_IsDataAvailable{false}
    , m_IsDataEntryAllowed{false}
    , m_IsGameStarted{false}
    , m_IsGamePaused{false}
{
    m_pDataSourceProxy = m_pGameFunctionalityProxy->getDataSourceProxy();
    m_pDataSourceAccessHelper = m_pGameFunctionalityProxy->getDataSourceAccessHelper();
    m_pWordMixer = m_pGameFunctionalityProxy->getWordMixer();
    m_pWordPairOwner = m_pGameFunctionalityProxy->getWordPairOwner();
    m_pInputBuilder = m_pGameFunctionalityProxy->getInputBuilder();
    m_pScoreItem = m_pGameFunctionalityProxy->getScoreItem();

    // all QObjects used by application (except the QML registered ones) should be parented (the non-parented ones would only be used in tests)
    Q_ASSERT(this->parent());
    Q_ASSERT(m_pDataSourceProxy->parent());
    Q_ASSERT(m_pDataSourceAccessHelper->parent());
    Q_ASSERT(m_pWordMixer->parent());
    Q_ASSERT(m_pWordPairOwner->parent());
    Q_ASSERT(m_pInputBuilder->parent());
    Q_ASSERT(m_pScoreItem->parent());

    bool connected{connect(m_pWordPairOwner, &WordPairOwner::mixedWordsAvailable, this, &GameFacade::mixedWordsChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::selectionChanged, this, &GameFacade::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputChanged, this, &GameFacade::inputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputCompletionChanged, this, &GameFacade::completionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::closeInputPermissionRequested, this, &GameFacade::_onCloseInputPermissionRequested);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem, &ScoreItem::statisticsUpdated, this, &GameFacade::statisticsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::readDataFinished, this, &GameFacade::_onReadDataFinished);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::writeDataFinished, this, &GameFacade::_onWriteDataFinished);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::dataEntrySaveError, this, &GameFacade::_onDataEntrySaveError);
    Q_ASSERT(connected);
}

void GameFacade::init()
{
    if (m_CurrentStatusCode == Game::StatusCodes::INVALID)
    {
        m_pDataSourceProxy->loadData();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::LOADING_DATA);
    }
    else
    {
        qWarning("Status codes initialization error or wrong usage of the function");
    }
}

void GameFacade::startGame()
{
    Q_ASSERT(m_IsDataAvailable);

    m_pDataSourceProxy->fetchDataEntry(m_pDataSourceAccessHelper->generateEntryNumber());

    Q_EMIT statisticsChanged();
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STARTED);

    m_IsGameStarted = true;
}

void GameFacade::resumeGame()
{
    Q_ASSERT(m_IsGamePaused);
    m_IsGamePaused = false;
    Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::GAME_RESUMED_COMPLETE_INPUT
                                                                                  : Game::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT);
}

void GameFacade::pauseGame()
{
    Q_ASSERT(m_IsGameStarted);
    m_IsGamePaused = true;
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_PAUSED);
}

void GameFacade::quitGame()
{
    Q_ASSERT(m_IsGameStarted);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STOPPED);
}

void GameFacade::startWordEntry()
{
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::WORD_ENTRY_STARTED);
}

void GameFacade::addWordPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex)
{
    if (!m_pWordPairOwner->getIsWordPieceSelected(wordPieceIndex))
    {
        // adding piece should always occur before updating status
        Game::PieceTypes pieceType{m_pWordPairOwner->getWordPieceType(wordPieceIndex)};
        bool pieceAdded{m_pInputBuilder->addPieceToInputWord(inputWordNumber, wordPieceIndex, pieceType)};

        Q_EMIT statusChanged(m_CurrentStatusCode = pieceAdded ? (m_pInputBuilder->isInputComplete() ? Game::StatusCodes::PIECE_ADDED_COMPLETE_INPUT
                                                                                                    : Game::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT)
                                                               : Game::StatusCodes::PIECE_NOT_ADDED);
    }
}

void GameFacade::removeWordPiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart)
{
    m_pInputBuilder->removePiecesFromInputWord(inputWordNumber, inputRangeStart);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
}

void GameFacade::clearInput()
{
    if (m_pInputBuilder->clearInput())
    {
        Q_EMIT statusChanged(Game::StatusCodes::USER_INPUT_CLEARED);
    }
}

void GameFacade::handleSubmitRequest()
{
    QVector<QString> mixedWordPiecesContent{m_pWordPairOwner->getMixedWordsPiecesContent()};

    QString firstInputWord;
    QString secondInputWord;

    for (auto index : m_pInputBuilder->getFirstWordInputIndexes())
    {
        firstInputWord.append(mixedWordPiecesContent.at(index));
    }

    for (auto index : m_pInputBuilder->getSecondWordInputIndexes())
    {
        secondInputWord.append(mixedWordPiecesContent.at(index));
    }

    bool success{(firstInputWord == m_pWordPairOwner->getFirstReferenceWord() && secondInputWord == m_pWordPairOwner->getSecondReferenceWord()) ||
                 (firstInputWord == m_pWordPairOwner->getSecondReferenceWord() && secondInputWord == m_pWordPairOwner->getFirstReferenceWord())};

    Q_EMIT statusChanged(m_CurrentStatusCode = success ? Game::StatusCodes::SUCCESS : Game::StatusCodes::INCORRECT_WORDS);

    if (success)
    {
        m_pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);
        m_pDataSourceProxy->fetchDataEntry(m_pDataSourceAccessHelper->generateEntryNumber());
    }
}

void GameFacade::requestAddPairToData(const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    m_pDataSourceProxy->requestAddPairToDataSource(QPair<QString, QString>{firstWord, secondWord}, areSynonyms);
}

void GameFacade::provideResultsToUser()
{
    m_pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::REQUESTED_BY_USER);
    m_pDataSourceProxy->fetchDataEntry(m_pDataSourceAccessHelper->generateEntryNumber());
}

void GameFacade::setLevel(Game::Level level)
{
    m_pWordMixer->setWordPieceSize(level);
    m_pScoreItem->setScoreIncrement(level);
    m_pDataSourceProxy->fetchDataEntry(m_pDataSourceAccessHelper->generateEntryNumber());

    Q_EMIT statusChanged(Game::StatusCodes::LEVEL_CHANGED);
}

void GameFacade::resetStatistics()
{
    m_pScoreItem->resetStatistics();
    Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT
                                                                                  : Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT);
}

QVector<QString> GameFacade::getMixedWordsPiecesContent() const
{
    return m_pWordPairOwner->getMixedWordsPiecesContent();
}

QVector<Game::PieceTypes> GameFacade::getMixedWordsPiecesTypes() const
{
    return m_pWordPairOwner->getMixedWordsPiecesTypes();
}

QVector<bool> GameFacade::getAreMixedWordsPiecesSelected() const
{
    return m_pWordPairOwner->getAreMixedWordsPiecesAddedToInput();
}

const QVector<int> GameFacade::getFirstWordInputIndexes() const
{
    return m_pInputBuilder->getFirstWordInputIndexes();
}

const QVector<int> GameFacade::getSecondWordInputIndexes() const
{
    return m_pInputBuilder->getSecondWordInputIndexes();
}

bool GameFacade::isInputComplete() const
{
    return m_pInputBuilder->isInputComplete();
}

QString GameFacade::getFirstReferenceWord() const
{
    return m_pWordPairOwner->getFirstReferenceWord();
}

QString GameFacade::getSecondReferenceWord() const
{
    return m_pWordPairOwner->getSecondReferenceWord();
}

int GameFacade::getObtainedScore() const
{
    return m_pScoreItem->getObtainedScore();
}

int GameFacade::getTotalAvailableScore() const
{
    return m_pScoreItem->getTotalAvailableScore();
}

int GameFacade::getGuessedWordPairs() const
{
    return m_pScoreItem->getGuessedWordPairs();
}

int GameFacade::getTotalWordPairs() const
{
    return m_pScoreItem->getTotalWordPairs();
}

bool GameFacade::isDataAvailable() const
{
    return m_IsDataAvailable;
}

bool GameFacade::isDataEntryAllowed() const
{
    return m_IsDataEntryAllowed;
}

bool GameFacade::areSynonyms() const
{
    return m_pWordPairOwner->areSynonyms();
}

void GameFacade::_onCloseInputPermissionRequested()
{
    m_pInputBuilder->setCloseInputPermission(m_pWordPairOwner->isLastAvailableWordPiece());
}

void GameFacade::_onReadDataFinished(bool success)
{
    if (success)
    {
        int nrOfEntries{m_pDataSourceProxy->getNrOfEntries()};

        if (nrOfEntries != 0)
        {
            m_pDataSourceAccessHelper->setEntriesTable(nrOfEntries);

            bool connected{connect(m_pDataSourceProxy, &DataSourceProxy::entryFetched, m_pWordMixer, &WordMixer::mixWords)};
            Q_ASSERT(connected);

            m_IsDataAvailable = true;

            Q_EMIT dataAvailableChanged();
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_LOAD_COMPLETE);
        }
        else
        {
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_LOAD_NO_VALID_ENTRIES);
        }

        m_IsDataEntryAllowed = true;
        Q_EMIT dataEntryAllowed();
    }
    else
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_LOAD_ERROR);
    }
}

void GameFacade::_onWriteDataFinished(bool success)
{
    if (success)
    {
        m_pDataSourceAccessHelper->appendNewEntry();
    }

    Q_EMIT statusChanged(success ? Game::StatusCodes::DATA_ENTRY_SUCCESS : Game::StatusCodes::INVALID_DATA_ENTRY);
}

void GameFacade::_onDataEntrySaveError()
{
    Q_EMIT statusChanged(Game::StatusCodes::DATA_ENTRY_SAVE_ERROR);
}
