#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include <QObject>

#include "game.h"

class WordMixer;
class ScoreItem;

class GameFacade : public QObject
{
    Q_OBJECT
public:
    explicit GameFacade(QString applicationPath, QObject *parent = nullptr);
    void startGame();
    void updateStatistics(Game::StatisticsUpdate updateType);
    void resetStatistics();
    void setLevel(Game::Level level);
    void provideResultsToUser();
    bool checkWords(const QString &firstWord, const QString &secondWord);

    QVector<QString> getMixedWordsStringArray() const;
    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;
    QString getFirstWord() const;
    QString getSecondWord() const;
    bool areSynonyms() const;

    int getObtainedScore() const;
    int getTotalAvailableScore() const;
    int getGuessedWordPairs() const;
    int getTotalWordPairs() const;

signals:
    Q_SIGNAL void statisticsChanged();
    Q_SIGNAL void statusChanged(Game::StatusCodes status);
    Q_SIGNAL void mixedWordsChanged();

private:
    QString m_ApplicationPath;
    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
};

#endif // GAMEFACADE_H
