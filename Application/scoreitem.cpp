#include "scoreitem.h"

static constexpr int c_EasyLevelScoreIncrement{1};
static constexpr int c_MediumLevelScoreIncrement{2};
static constexpr int c_HardLevelScoreIncrement{4};

ScoreItem::ScoreItem(QObject *parent)
    : QObject(parent)
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{c_MediumLevelScoreIncrement}

{
    // nothing
}

void ScoreItem::setScoreIncrement(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));

    switch(level)
    {
    case Game::Level::EASY:
        m_ScoreIncrement = c_EasyLevelScoreIncrement;
        break;
    case Game::Level::MEDIUM:
        m_ScoreIncrement = c_MediumLevelScoreIncrement;
        break;
    case Game::Level::HARD:
        m_ScoreIncrement = c_HardLevelScoreIncrement;
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
