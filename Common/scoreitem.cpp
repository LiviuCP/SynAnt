#include "scoreitem.h"

ScoreItem::ScoreItem(QObject *parent)
    : QObject(parent)
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{Game::c_ScoreIncrements[Game::Level::MEDIUM]}

{
    // not used
}

void ScoreItem::setScoreIncrement(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));
    m_ScoreIncrement = Game::c_ScoreIncrements[level];
}

void ScoreItem::updateStatistics(Game::StatisticsUpdate updateType)
{
    if (updateType == Game::StatisticsUpdate::FULL_UPDATE)
    {
        m_ObtainedScore += m_ScoreIncrement;
        m_GuessedWordPairs++;
    }
    m_TotalAvailableScore += m_ScoreIncrement;
    m_TotalWordPairs++;
    Q_EMIT statisticsUpdated();
}

void ScoreItem::resetStatistics()
{
    m_ObtainedScore = 0;
    m_TotalAvailableScore = 0;
    m_GuessedWordPairs = 0;
    m_TotalWordPairs = 0;
    Q_EMIT statisticsUpdated();
}

int ScoreItem::getObtainedScore() const
{
    return m_ObtainedScore;
}

int ScoreItem::getTotalAvailableScore() const
{
    return m_TotalAvailableScore;
}

int ScoreItem::getGuessedWordPairs() const
{
    return m_GuessedWordPairs;
}

int ScoreItem::getTotalWordPairs() const
{
    return m_TotalWordPairs;
}
