/*

  This class implements the statistics that are associated with the correct guessing of words:
  1) Total score is the maximum score that could have been obtained by user if correctly guessing all words
  2) Obtained score is the one the user gets for the number of pairs correctly guessed without pressing the Show results button
  3) Guessed word pairs are the ones correctly guessed with pressing the Show results button
  4) Total word pairs include the ones retrieved by pressing Show results too
  5) Score increment depends on level (the higher the level the higher the increment)

*/

#ifndef SCOREITEM_H
#define SCOREITEM_H

#include <QObject>
#include <QVector>

#include "game.h"

class ScoreItem : public QObject
{
    Q_OBJECT

public:
    explicit ScoreItem(QObject *parent = nullptr);

    void updateStatistics(Game::StatisticsUpdate updateType);
    void resetStatistics();

    int getObtainedScore() const;
    int getTotalAvailableScore() const;
    int getGuessedWordPairs() const;
    int getTotalWordPairs() const;

public slots:
    void setScoreIncrement(Game::Level level);

signals:
    Q_SIGNAL void statisticsUpdated();

private:
    // total number is the maximum number that can be correctly guessed by user
    int m_ObtainedScore;
    int m_TotalAvailableScore;
    int m_GuessedWordPairs;
    int m_TotalWordPairs;
    int m_ScoreIncrement;
};

#endif // SCOREITEM_H
