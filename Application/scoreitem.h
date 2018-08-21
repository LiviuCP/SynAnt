/*

  This class implements the statistics that are associated with the correct guessing of words:
  - total score: maximum score that could have been obtained by user if correctly guessing all words
  - obtained score: for the number of pairs correctly guessed without pressing the Show results button
  - same for the number of pairs
  - score increment depends on chosen level

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

signals:
    Q_SIGNAL void statisticsUpdated();

public slots:
    void setScoreIncrement(Game::Level level);

private:
    // total number is the maximum number that can be correctly guessed by user
    int m_ObtainedScore;
    int m_TotalAvailableScore;
    int m_GuessedWordPairs;
    int m_TotalWordPairs;
    int m_ScoreIncrement;

};
#endif // SCOREITEM_H
