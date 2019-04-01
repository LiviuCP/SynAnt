#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include <QObject>

#include "game.h"

class WordPairOwner;
class WordMixer;
class ScoreItem;

class GameFacade : public QObject
{
    Q_OBJECT
public:
    explicit GameFacade(QString applicationPath, QObject *parent = nullptr);

    void startGame();
    bool handleSubmitRequest(const QString &firstWord, const QString &secondWord);
    void provideResultsToUser();
    void updateStatistics(Game::StatisticsUpdate updateType);
    void resetStatistics();
    void setLevel(Game::Level level);

    QVector<QString> getMixedWordsPiecesArray() const;

    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;
    int getObtainedScore() const;
    int getTotalAvailableScore() const;
    int getGuessedWordPairs() const;
    int getTotalWordPairs() const;

    QString getFirstWord() const;
    QString getSecondWord() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void statisticsChanged();
    Q_SIGNAL void statusChanged(Game::StatusCodes status);

private:
    QString m_ApplicationPath;
    WordPairOwner* m_pWordPairOwner;
    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
};

#endif // GAMEFACADE_H
