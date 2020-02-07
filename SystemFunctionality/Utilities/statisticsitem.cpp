#include "statisticsitem.h"

StatisticsItem::StatisticsItem(QObject *parent)
    : QObject(parent)
    , m_IsInitialized{false}
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM]}
{
}

void StatisticsItem::initStatistics()
{
    if (!m_IsInitialized)
    {
        m_IsInitialized = true;
        Q_EMIT statisticsUpdated(Game::StatisticsUpdateTypes::INIT);
    }
}

void StatisticsItem::updateStatistics(Game::StatisticsUpdateTypes updateType)
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

void StatisticsItem::setScoreIncrement(Game::Levels level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Levels::NrOfLevels));
    m_ScoreIncrement = Game::c_ScoreIncrements[level];
}

bool StatisticsItem::canResetStatistics() const
{
    return (m_ObtainedScore != 0 || m_TotalAvailableScore != 0 || m_GuessedWordPairs != 0 || m_TotalWordPairs != 0);
}

QString StatisticsItem::getObtainedScore() const
{
    return QString::number(m_ObtainedScore);
}

QString StatisticsItem::getTotalAvailableScore() const
{
    return QString::number(m_TotalAvailableScore);
}

QString StatisticsItem::getGuessedWordPairs() const
{
    return QString::number(m_GuessedWordPairs);
}

QString StatisticsItem::getTotalWordPairs() const
{
    return QString::number(m_TotalWordPairs);
}
