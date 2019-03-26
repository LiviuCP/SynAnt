#include "gamefacade.h"
#include "wordmixer.h"
#include "scoreitem.h"
#include "gamestrings.h"

GameFacade::GameFacade(QString applicationPath, QObject *parent)
    : QObject(parent)
    , m_ApplicationPath{applicationPath}
    , m_pWordMixer{nullptr}
    , m_pScoreItem {new ScoreItem{this}}
{
    m_pWordMixer = new WordMixer{m_ApplicationPath + "/" + GameStrings::c_FileName, this};

    bool connected = connect(m_pWordMixer, &WordMixer::mixedWordsChanged, this, &GameFacade::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem, &ScoreItem::statisticsUpdated, this, &GameFacade::statisticsChanged);
    Q_ASSERT(connected);
}

void GameFacade::startGame()
{
    Q_EMIT statisticsChanged();
    m_pWordMixer->mixWords();
}

bool GameFacade::checkWords(const QString &firstWord, const QString &secondWord)
{
    const QString firstWordRef{m_pWordMixer->getFirstWord()};
    const QString secondWordRef{m_pWordMixer->getSecondWord()};
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

QVector<QString> GameFacade::getMixedWordsStringArray() const
{
    return m_pWordMixer->getMixedWordsStringArray();
}

int GameFacade::getFirstWordFirstPieceIndex() const
{
    return m_pWordMixer->getFirstWordFirstPieceIndex();
}

int GameFacade::getFirstWordLastPieceIndex() const
{
    return m_pWordMixer->getFirstWordLastPieceIndex();
}

int GameFacade::getSecondWordFirstPieceIndex() const
{
    return m_pWordMixer->getSecondWordFirstPieceIndex();
}

int GameFacade::getSecondWordLastPieceIndex() const
{
    return m_pWordMixer->getSecondWordLastPieceIndex();
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
    return m_pWordMixer->getFirstWord();
}

QString GameFacade::getSecondWord() const
{
    return m_pWordMixer->getSecondWord();
}

bool GameFacade::areSynonyms() const
{
    return m_pWordMixer->areSynonyms();
}
