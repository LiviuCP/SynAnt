#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include <QObject>

#include "game.h"

class WordPairOwner;
class InputBuilder;
class WordMixer;
class ScoreItem;

class GameFacade : public QObject
{
    Q_OBJECT
public:
    explicit GameFacade(QString applicationPath, QObject *parent = nullptr);

    void startGame();
    void handleSubmitRequest();
    void provideResultsToUser();
    void resetStatistics();
    void setLevel(Game::Level level);

    void selectWordPieceForFirstInputWord(int wordPieceIndex);
    void selectWordPieceForSecondInputWord(int wordPieceIndex);
    void removeWordPiecesFromFirstInputWord(int inputRangeStart);
    void removeWordPiecesFromSecondInputWord(int inputRangeStart);

    const QVector<Game::WordPiece> getMixedWordsPieces() const;

    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;

    int getObtainedScore() const;
    int getTotalAvailableScore() const;
    int getGuessedWordPairs() const;
    int getTotalWordPairs() const;

    QString getFirstWord() const;
    QString getSecondWord() const;

    bool isInputComplete() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void statisticsChanged();
    Q_SIGNAL void completionChanged();
    Q_SIGNAL void statusChanged(Game::StatusCodes status);

private:
    QString m_ApplicationPath;
    WordPairOwner* m_pWordPairOwner;
    InputBuilder* m_pInputBuilder;
    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
};

#endif // GAMEFACADE_H
