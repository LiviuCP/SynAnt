#include "scoreitem.h"

ScoreItem::ScoreItem(QObject *parent)
    : QObject(parent)
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{Game::c_ScoreIncrements[Game::Level::MEDIUM]}
{
}

void ScoreItem::updateStatistics(Game::StatisticsUpdate updateType)
{
    if (updateType == Game::StatisticsUpdate::RESET)
    {
        if (m_TotalAvailableScore != 0 && m_TotalWordPairs != 0)
        {
            m_ObtainedScore = 0;
            m_TotalAvailableScore = 0;
            m_GuessedWordPairs = 0;
            m_TotalWordPairs = 0;

            Q_EMIT statisticsUpdated(updateType);
        }
        else
        {
            qWarning("attempt to reset null statistics");
        }
    }
    else
    {
        if (updateType == Game::StatisticsUpdate::FULL_UPDATE)
        {
            m_ObtainedScore += m_ScoreIncrement;
            m_GuessedWordPairs++;
        }

        m_TotalAvailableScore += m_ScoreIncrement;
        m_TotalWordPairs++;

        Q_EMIT statisticsUpdated(updateType);
    }
}

void ScoreItem::setScoreIncrement(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));
    m_ScoreIncrement = Game::c_ScoreIncrements[level];
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
