/*
   This class connects the front-end UI part of the code (presenter) to the back-end code base:
   1) Backend classes are being accessed by presenter through facade by executing function calls.
   2) In return backend classes send signals to presenter through facade.
   3) The facade checks the user input created by InputBuilder against the reference words contained in WordPairOwner.
   4) The facade provides decoupling by hiding the backend functionality (WordMixer, ScoreItem, WordPairOwner, InputBuilder) entirely from presenter.
   5) Last but not least the facade is the only responsible class for updating the status of the game (which is displayed in the status box of the main pane).
*/

#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include <QObject>
#include <QTimer>

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
    void resumeGame();

    void addWordPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex);
    void removeWordPiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart);

    void handleSubmitRequest();
    void provideResultsToUser();
    void setLevel(Game::Level level);
    void resetStatistics();

    const QVector<Game::WordPiece> getMixedWordsPieces() const;

    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;

    bool isInputComplete() const;

    QString getFirstReferenceWord() const;
    QString getSecondReferenceWord() const;

    int getObtainedScore() const;
    int getTotalAvailableScore() const;
    int getGuessedWordPairs() const;
    int getTotalWordPairs() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void completionChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void statisticsChanged();
    Q_SIGNAL void statusChanged(Game::StatusCodes status);

private slots:
    void _onStatusUpdateTimeout();

private:
    void _updateStatus(Game::StatusCodes tempStatusCode, Game::StatusCodes permStatusCode = Game::StatusCodes::DEFAULT);

    QString m_ApplicationPath;
    WordPairOwner* m_pWordPairOwner;
    InputBuilder* m_pInputBuilder;
    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
    QTimer* m_pStatusUpdateTimer;
    Game::StatusCodes m_CurrentStatusCode;
    Game::StatusCodes m_NextStatusCode;
};

#endif // GAMEFACADE_H
