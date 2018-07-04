#ifndef SCOREITEM_H
#define SCOREITEM_H
#include <QVector>
#include <QObject>
#include "game.h"

class ScoreItem : public QObject
{
    Q_OBJECT

public:
    explicit ScoreItem(QObject *parent = nullptr);

    void updateStatistics(bool partialUpdate);                                                      // updates the score/number of word pairs variables, triggers update of the score/number of word pairs texts to be displayed in the main window
    void resetStatistics();
    QVector<int> getStatistics() const;

signals:
    Q_SIGNAL void statisticsUpdated();

public slots:
    void setScoreIncrement(Game::Level level);                                                      // used when setting level

private:
    int m_ObtainedScore;                                                                            // score currently obtained by user (increases with each successful guess)
    int m_TotalAvailableScore;                                                                      // score that could have been obtained by user if all word pairs had been correctly guessed
    int m_GuessedWordPairs;                                                                         // number of pairs which have been correctly guessed by user
    int m_TotalWordPairs;                                                                           // total number of pairs available for guessing until the current moment
    int m_ScoreIncrement;                                                                           // number of points by which the scores increase (depends on difficulty level)

};
#endif // SCOREITEM_H
