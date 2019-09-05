#include "scoreitem.h"

ScoreItem::ScoreItem(QObject *parent)
    : QObject(parent)
    , m_IsInitialized{false}
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM]}
{
}

void ScoreItem::initStatistics()
{
    if (!m_IsInitialized)
    {
        m_IsInitialized = true;
        Q_EMIT statisticsUpdated(Game::StatisticsUpdateTypes::INIT);
    }
}

void ScoreItem::updateStatistics(Game::StatisticsUpdateTypes updateType)
{
    Q_ASSERT(m_IsInitialized);

    if (updateType == Game::StatisticsUpdateTypes::RESET)
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
        if (updateType == Game::StatisticsUpdateTypes::FULL_UPDATE)
        {
            m_ObtainedScore += m_ScoreIncrement;
            m_GuessedWordPairs++;
        }

        m_TotalAvailableScore += m_ScoreIncrement;
        m_TotalWordPairs++;

        Q_EMIT statisticsUpdated(updateType);
    }
}

void ScoreItem::setScoreIncrement(Game::Levels level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Levels::NrOfLevels));
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
