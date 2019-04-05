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
{
    m_pWordMixer = new WordMixer{m_ApplicationPath + "/" + GameStrings::c_FileName, this};
    m_pWordPairOwner->connectToWordMixer(m_pWordMixer);
    m_pInputBuilder->connectToWordPairOwner(m_pWordPairOwner);

    bool connected = connect(m_pWordPairOwner, &WordPairOwner::mixedWordsAvailable, this, &GameFacade::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputChanged, this, &GameFacade::inputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::selectionChanged, this, &GameFacade::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem, &ScoreItem::statisticsUpdated, this, &GameFacade::statisticsChanged);
    Q_ASSERT(connected);
}

void GameFacade::startGame()
{
    Q_EMIT statisticsChanged();
    m_pWordMixer->mixWords();
}

bool GameFacade::handleSubmitRequest(const QString &firstWord, const QString &secondWord)
{
    const QString firstWordRef{m_pWordPairOwner->getFirstWord()};
    const QString secondWordRef{m_pWordPairOwner->getSecondWord()};
    bool success{(firstWord == firstWordRef && secondWord == secondWordRef) || (firstWord == secondWordRef && secondWord == firstWordRef)};
    Game::StatusCodes statusCode{success ? Game::StatusCodes::SUCCESS : Game::StatusCodes::INCORRECT_WORDS};

    Q_EMIT statusChanged(statusCode);

    if (success)
    {
        updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);
        m_pWordMixer->mixWords();
    }

    return success;
}

void GameFacade::provideResultsToUser()
{
    m_pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    Q_EMIT statusChanged(Game::StatusCodes::REQUESTED_BY_USER);
    m_pWordMixer->mixWords();
}

void GameFacade::updateStatistics(Game::StatisticsUpdate updateType)
{
    m_pScoreItem->updateStatistics(updateType);
}

void GameFacade::resetStatistics()
{
    m_pScoreItem->resetStatistics();
    Q_EMIT statusChanged(Game::StatusCodes::STATISTICS_RESET);
}

void GameFacade::setLevel(Game::Level level)
{
    m_pWordMixer->setWordPieceSize(level);
    m_pScoreItem->setScoreIncrement(level);
    m_pWordMixer->mixWords();

    Q_EMIT statusChanged(Game::StatusCodes::LEVEL_CHANGED);
}

void GameFacade::selectWordPieceForFirstWord(int index)
{
    if (!m_pWordPairOwner->getMixedWordsPieces().at(index).isSelected)
    {
        qInfo("");
        qInfo("Adding piece with index [%d] and content [%s] to first word input", index, m_pWordPairOwner->getMixedWordsPieces().at(index).content.toStdString().c_str());

        Q_UNUSED(m_pInputBuilder->addPieceToFirstWordInput(index));

        qInfo("DONE");
    }
}

void GameFacade::selectWordPieceForSecondWord(int index)
{
    if (!m_pWordPairOwner->getMixedWordsPieces().at(index).isSelected)
    {
        qInfo("");
        qInfo("Adding piece with index [%d] and content [%s] to second word input", index, m_pWordPairOwner->getMixedWordsPieces().at(index).content.toStdString().c_str());

        Q_UNUSED(m_pInputBuilder->addPieceToSecondWordInput(index));

        qInfo("DONE");
    }
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

QString GameFacade::getFirstWord() const
{
    return m_pWordPairOwner->getFirstWord();
}

QString GameFacade::getSecondWord() const
{
    return m_pWordPairOwner->getSecondWord();
}

bool GameFacade::areSynonyms() const
{
    return m_pWordPairOwner->areSynonyms();
}
