#include "gamefacade.h"
#include "wordpairowner.h"
#include "inputbuilder.h"
#include "wordmixer.h"
#include "scoreitem.h"
#include "gamestrings.h"

GameFacade::GameFacade(QString applicationPath, QObject *parent)
    : QObject(parent)
    , m_ApplicationPath{applicationPath}
    , m_pWordPairOwner{new WordPairOwner{this}}
    , m_pInputBuilder{new InputBuilder{this}}
    , m_pWordMixer{nullptr}
    , m_pScoreItem {new ScoreItem{this}}
    , m_pStatusUpdateTimer{new QTimer{this}}
    , m_CurrentStatusCode{Game::StatusCodes::DEFAULT}
    , m_NextStatusCode{Game::StatusCodes::DEFAULT}
{
    m_pWordMixer = new WordMixer{m_ApplicationPath + "/" + GameStrings::c_FileName, this};
    m_pWordPairOwner->connectToWordMixer(m_pWordMixer);
    m_pInputBuilder->connectToWordPairOwner(m_pWordPairOwner);
    m_pStatusUpdateTimer->setSingleShot(true);

    bool connected = connect(m_pWordPairOwner, &WordPairOwner::mixedWordsAvailable, this, &GameFacade::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::selectionChanged, this, &GameFacade::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputChanged, this, &GameFacade::inputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::completionChanged, this, &GameFacade::completionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem, &ScoreItem::statisticsUpdated, this, &GameFacade::statisticsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pStatusUpdateTimer, &QTimer::timeout, this, &GameFacade::_onStatusUpdateTimeout);
    Q_ASSERT(connected);
}

void GameFacade::startGame()
{
    Q_EMIT statisticsChanged();
    m_pWordMixer->mixWords();
    _updateStatus(Game::StatusCodes::GAME_STARTED);
}

void GameFacade::resumeGame()
{
    _updateStatus(Game::StatusCodes::GAME_RESUMED, m_pInputBuilder->isInputComplete() ? Game::StatusCodes::ALL_PIECES_SELECTED : Game::StatusCodes::DEFAULT);
}

void GameFacade::addWordPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex)
{
    if (!m_pWordPairOwner->getMixedWordsPieces().at(wordPieceIndex).isSelected)
    {
        // adding piece should always occur before updating status
        bool pieceAdded{m_pInputBuilder->addPieceToInputWord(inputWordNumber, wordPieceIndex)};

        _updateStatus(pieceAdded ? Game::StatusCodes::PIECE_SUCCESSFULLY_ADDED : Game::StatusCodes::PIECE_NOT_ADDED,
                      m_pInputBuilder->isInputComplete() ? Game::StatusCodes::ALL_PIECES_SELECTED : Game::StatusCodes::DEFAULT);
    }
}

void GameFacade::removeWordPiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart)
{
    m_pInputBuilder->removePiecesFromInputWord(inputWordNumber, inputRangeStart);
    _updateStatus(Game::StatusCodes::PIECES_REMOVED);
}

void GameFacade::clearInput()
{
    if (m_pInputBuilder->clearInput())
    {
        _updateStatus(Game::StatusCodes::USER_INPUT_CLEARED);
    }
}

void GameFacade::handleSubmitRequest()
{
    QString firstInputWord;
    QString secondInputWord;

    for (auto index : m_pInputBuilder->getFirstWordInputIndexes())
    {
        firstInputWord.append(m_pWordPairOwner->getMixedWordsPieces().at(index).content);
    }

    for (auto index : m_pInputBuilder->getSecondWordInputIndexes())
    {
        secondInputWord.append(m_pWordPairOwner->getMixedWordsPieces().at(index).content);
    }

    bool success{(firstInputWord == m_pWordPairOwner->getFirstReferenceWord() && secondInputWord == m_pWordPairOwner->getSecondReferenceWord()) ||
                 (firstInputWord == m_pWordPairOwner->getSecondReferenceWord() && secondInputWord == m_pWordPairOwner->getFirstReferenceWord())};

    Game::StatusCodes statusCode{success ? Game::StatusCodes::SUCCESS : Game::StatusCodes::INCORRECT_WORDS};

    _updateStatus(statusCode, success ? Game::StatusCodes::DEFAULT : Game::StatusCodes::ALL_PIECES_SELECTED);

    if (success)
    {
        m_pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);
        m_pWordMixer->mixWords();
    }
}

void GameFacade::provideResultsToUser()
{
    m_pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    _updateStatus(Game::StatusCodes::REQUESTED_BY_USER);
    m_pWordMixer->mixWords();
}

void GameFacade::setLevel(Game::Level level)
{
    m_pWordMixer->setWordPieceSize(level);
    m_pScoreItem->setScoreIncrement(level);
    m_pWordMixer->mixWords();

    _updateStatus(Game::StatusCodes::LEVEL_CHANGED);
}

void GameFacade::resetStatistics()
{
    m_pScoreItem->resetStatistics();
    _updateStatus(Game::StatusCodes::STATISTICS_RESET, m_pInputBuilder->isInputComplete() ? Game::StatusCodes::ALL_PIECES_SELECTED : Game::StatusCodes::DEFAULT);
}

const QVector<Game::WordPiece> GameFacade::getMixedWordsPieces() const
{
    return m_pWordPairOwner->getMixedWordsPieces();
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

bool GameFacade::areSynonyms() const
{
    return m_pWordPairOwner->areSynonyms();
}

void GameFacade::_onStatusUpdateTimeout()
{
    Q_EMIT statusChanged(m_NextStatusCode);
    m_CurrentStatusCode = m_NextStatusCode;
}

void GameFacade::_updateStatus(Game::StatusCodes tempStatusCode, Game::StatusCodes permStatusCode)
{
    bool isTempStatusCodeValid{tempStatusCode != m_CurrentStatusCode || tempStatusCode == Game::StatusCodes::REQUESTED_BY_USER};

    if (isTempStatusCodeValid || permStatusCode != m_CurrentStatusCode)
    {
        // cancel any delayed status update, no more required
        if (m_pStatusUpdateTimer->isActive())
        {
            m_pStatusUpdateTimer->stop();
        }

        if (isTempStatusCodeValid)
        {
            m_CurrentStatusCode = tempStatusCode;
            Q_EMIT statusChanged(m_CurrentStatusCode);
        }

        if (permStatusCode != m_CurrentStatusCode)
        {
            m_pStatusUpdateTimer->setInterval(m_CurrentStatusCode == Game::StatusCodes::REQUESTED_BY_USER || m_CurrentStatusCode == Game::StatusCodes::SUCCESS ?
                                              Game::c_ExtStatusUpdateTimeout : Game::c_StdStatusUpdateTimeout);
            m_NextStatusCode = permStatusCode;
            m_pStatusUpdateTimer->start();
        }
        else
        {
            m_NextStatusCode = m_CurrentStatusCode;
        }
    }
}
