#include "statisticsitem.h"

StatisticsItem::StatisticsItem(QObject *parent)
    : QObject(parent)
    , m_IsInitialUpdateDone{false}
    , m_IsEnhancedIncrementingUsed{false}
    , m_ObtainedScore{0}
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrements{Game::c_ScoreIncrements}
    , m_EnhancedScoreIncrements{Game::c_EnhancedScoreIncrements}
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
    Q_ASSERT(m_IsEnhancedIncrementingUsed ? m_EnhancedScoreIncrements.contains(m_GameLevel) : m_ScoreIncrements.contains(m_GameLevel));

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
        const int c_ScoreIncrement{m_IsEnhancedIncrementingUsed ? m_EnhancedScoreIncrements[m_GameLevel] : m_ScoreIncrements[m_GameLevel]};

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

void StatisticsItem::setIncrementForLevel(int increment, Game::Levels level, bool enhanced)
{
    if (increment > 0 && level != Game::Levels::LEVEL_NONE)
    {
        if (enhanced)
        {
            m_EnhancedScoreIncrements[level] = increment;
        }
        else
        {
            m_ScoreIncrements[level] = increment;
        }
    }
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

Game::Levels StatisticsItem::getGameLevel() const
{
    return m_GameLevel;
}

int StatisticsItem::getCurrentIncrement() const
{
    return m_IsEnhancedIncrementingUsed ? (m_EnhancedScoreIncrements.contains(m_GameLevel) ? m_EnhancedScoreIncrements[m_GameLevel]: -1)
                                        : (m_ScoreIncrements.contains(m_GameLevel) ? m_ScoreIncrements[m_GameLevel] : -1);
}

int StatisticsItem::isEnhancedIncrementingUsed() const
{
    return m_IsEnhancedIncrementingUsed;
}
