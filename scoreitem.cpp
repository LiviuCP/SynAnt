#include "scoreitem.h"

ScoreItem::ScoreItem(QObject *parent)
    : QObject(parent)
    , m_ObtainedScore{0}                                                                             // all statistics (scores, number of pairs) initially set to 0 (this is reflected in the scores and number of pairs texts too)
    , m_TotalAvailableScore{0}
    , m_GuessedWordPairs{0}
    , m_TotalWordPairs{0}
    , m_ScoreIncrement{2}                                                                            // default level is medium, the number by which the scores are increased coresponds to this level

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

void ScoreItem::updateStatistics(bool partialUpdate)
{
    if (!partialUpdate)                                                                            // if user enters the words correctly and presses Submit, the obtained points and the number of guessed word pairs gets updated
    {
        m_ObtainedScore += m_ScoreIncrement;
        m_GuessedWordPairs++;
    }
    m_TotalAvailableScore += m_ScoreIncrement;                                                         // total available number of points and total number of pairs presented to the user will be updated
    m_TotalWordPairs++;                                                                              // anytime the user enters the words correctly (and pressses Submit) or he presses Show results button
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

QVector<int> ScoreItem::getStatistics() const
{
    QVector<int> statistics;
    statistics.append(m_ObtainedScore);
    statistics.append(m_TotalAvailableScore);
    statistics.append(m_GuessedWordPairs);
    statistics.append(m_TotalWordPairs);
    return statistics;
}
