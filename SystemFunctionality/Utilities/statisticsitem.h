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
    explicit StatisticsItem(QObject *parent = nullptr);

    void initStatistics();
    void updateStatistics(Game::StatisticsUpdateTypes updateType);
    void setGameLevel(Game::Levels level);
    bool canResetStatistics() const;

    QString getObtainedScore() const;
    QString getTotalAvailableScore() const;
    QString getGuessedWordPairs() const;
    QString getTotalWordPairs() const;

signals:
    Q_SIGNAL void statisticsUpdated(Game::StatisticsUpdateTypes updateType);

private:
    // total number is the maximum number that can be correctly guessed by user
    bool m_IsInitialized;

    int m_ObtainedScore;
    int m_TotalAvailableScore;
    int m_GuessedWordPairs;
    int m_TotalWordPairs;

    Game::Levels m_GameLevel;
};

#endif // STATISTICSITEM_H
