#include "scoreitem.h"

ScoreItem::ScoreItem(QObject *parent)
    : QObject(parent)
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{2}

{
    // nothing
}

void ScoreItem::setScoreIncrement(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));

    switch(level)
    {
    case Game::Level::EASY:
        m_ScoreIncrement = 1;
        break;
    case Game::Level::MEDIUM:
        m_ScoreIncrement = 2;
        break;
    case Game::Level::HARD:
        m_ScoreIncrement = 4;
    }
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
