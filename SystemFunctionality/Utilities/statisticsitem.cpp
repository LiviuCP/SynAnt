#include "statisticsitem.h"

StatisticsItem::StatisticsItem(QObject *parent)
    : QObject(parent)
    , m_IsInitialUpdateDone{false}
    , m_IsEnhancedIncrementingUsed{false}
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_GameLevel{Game::Levels::LEVEL_NONE}
{
}

void StatisticsItem::doInitialUpdate()
{
    Q_ASSERT(m_GameLevel != Game::Levels::LEVEL_NONE);

    if (!m_IsInitialUpdateDone)
    {
        m_IsInitialUpdateDone = true;
        Q_EMIT statisticsUpdated(Game::StatisticsUpdateTypes::INITIAL_UPDATE);
    }
}

void StatisticsItem::updateStatistics(Game::StatisticsUpdateTypes updateType)
{
    Q_ASSERT(m_IsInitialUpdateDone && m_GameLevel != Game::Levels::LEVEL_NONE);

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
        const int c_ScoreIncrement{m_IsEnhancedIncrementingUsed ? Game::c_EnhancedScoreIncrements[m_GameLevel] : Game::c_ScoreIncrements[m_GameLevel]};

        if (updateType == Game::StatisticsUpdateTypes::FULL_UPDATE)
        {
            m_ObtainedScore += c_ScoreIncrement;
            m_GuessedWordPairs++;
        }

        m_TotalAvailableScore += c_ScoreIncrement;
        m_TotalWordPairs++;

        Q_EMIT statisticsUpdated(updateType);
    }
}

void StatisticsItem::setGameLevel(Game::Levels level)
{
    if (level != Game::Levels::LEVEL_NONE)
    {
        m_GameLevel = level;
    }
}

void StatisticsItem::setEnhancedIncrement(bool enhanced)
{
    m_IsEnhancedIncrementingUsed = enhanced;
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
