/*
  This class implements the statistics that are associated with the correct guessing of words:
  1) Total score is the maximum score that could have been obtained by user if correctly guessing all words
  2) Obtained score is the one the user gets for the number of pairs correctly guessed without pressing the Show results button
  3) Guessed word pairs are the ones correctly guessed with pressing the Show results button
  4) Total word pairs include the ones retrieved by pressing Show results too
  5) Score increment depends on level (the higher the level the higher the increment)
*/

#ifndef STATISTICSITEM_H
#define STATISTICSITEM_H

#include <QObject>
#include <QVector>

#include "gameutils.h"

class StatisticsItem : public QObject
{
    Q_OBJECT

public:
    enum class StatisticsUpdateOperations
    {
        NO_UPDATE,
        INITIAL_UPDATE,
        FULL_UPDATE,
        PARTIAL_UPDATE,
        RESET,
    };

    explicit StatisticsItem(QObject *parent = nullptr);

    void doInitialUpdate();
    void updateStatistics(StatisticsItem::StatisticsUpdateOperations updateType);
    void setGameLevel(Game::Levels level);
    void setEnhancedIncrement(bool enhanced);
    void setIncrementForLevel(int increment, Game::Levels level, bool enhanced = false);
    bool canResetStatistics() const;

    StatisticsUpdateOperations getLastUpdateOperation() const;
    QString getObtainedScore() const;
    QString getTotalAvailableScore() const;
    QString getGuessedWordPairs() const;
    QString getTotalWordPairs() const;
    Game::Levels getGameLevel() const;
    int getCurrentIncrement() const;
    int isEnhancedIncrementingUsed() const;

signals:
    Q_SIGNAL void statisticsUpdated();

private:
    // total number is the maximum number that can be correctly guessed by user
    StatisticsUpdateOperations m_LastUpdateOperation;
    bool m_IsInitialUpdateDone;
    bool m_IsEnhancedIncrementingUsed; // use a special incrementing scheme for certain conditions (e.g. a time limit enabled)

    int m_ObtainedScore;
    int m_TotalAvailableScore;
    int m_GuessedWordPairs;
    int m_TotalWordPairs;

    QMap<Game::Levels, int> m_ScoreIncrements;
    QMap<Game::Levels, int> m_EnhancedScoreIncrements;

    Game::Levels m_GameLevel;
};

#endif // STATISTICSITEM_H
