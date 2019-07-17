/*
   This class connects the front-end UI part of the code (presenter) to the back-end code base:
   1) Backend classes are being accessed by presenter through facade by executing function calls.
   2) In return backend classes send signals to presenter through facade.
   3) The facade checks the user input created by InputBuilder against the reference words contained in WordPairOwner.
   4) The facade intermediates the data communication between source and consumer (WordMixer).
   5) The facade provides decoupling by hiding the backend functionality (WordMixer, ScoreItem, WordPairOwner, InputBuilder, DataSource and DataSourceAccess) entirely from presenter.
   6) Last but not least the facade is the only responsible class for updating the status of the game (which is displayed in the status box of the main pane).
*/

#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include <QObject>
#include <QTimer>

#include "../Utilities/game.h"

class GameFunctionalityProxy;
class DataSourceProxy;
class DataSourceAccessHelper;
class WordMixer;
class WordPairOwner;
class InputBuilder;
class ScoreItem;

class GameFacade : public QObject
{
    Q_OBJECT
public:
    explicit GameFacade(QObject *parent = nullptr);

    void init();

    void startGame();
    void resumeGame();
    void pauseGame();
    void quitGame();

    void startWordEntry();
    void resumeWordEntry();

    void addWordPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex);
    void removeWordPiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart);
    void clearInput();

    void handleSubmitRequest();
    void requestAddPairToCache(const QString& firstWord, const QString& secondWord, bool areSynonyms);
    void requestSaveDataToDb();
    void requestCacheReset();
    void provideCorrectWordsPairToUser();
    void setLevel(Game::Level level);
    void resetGameStatistics();

    QVector<QString> getMixedWordsPiecesContent() const;
    QVector<Game::PieceTypes> getMixedWordsPiecesTypes() const;
    QVector<bool> getAreMixedWordsPiecesSelected() const;

    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;

    bool isInputComplete() const;

    QString getFirstReferenceWord() const;
    QString getSecondReferenceWord() const;

    int getObtainedScore() const;
    int getTotalAvailableScore() const;
    int getGuessedWordPairs() const;
    int getTotalWordPairs() const;
    int getCachedWordPairs() const;

    bool isDataAvailable() const;
    bool isDataEntryAllowed() const;
    bool isAddingToCacheAllowed() const;
    bool isCacheResetAllowed() const;
    bool isSavingToDbAllowed() const;
    bool areSynonyms() const;

signals:
    Q_SIGNAL void dataAvailableChanged();
    Q_SIGNAL void dataEntryAllowedChanged();
    Q_SIGNAL void addPairToCacheAllowedChanged();
    Q_SIGNAL void resetCacheAllowedChanged();
    Q_SIGNAL void saveNewPairsToDbAllowedChanged();
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void completionChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void statisticsChanged();
    Q_SIGNAL void statusChanged(Game::StatusCodes status);

private slots:
    void _onCloseInputPermissionRequested();
    void _onReadDataFromDbFinished(bool success);
    void _onInvalidWordsPairAddedByUser();
    void _onNewWordsPairAddedToCache();
    void _onCacheReset();
    void _onWriteDataToDbFinished(int nrOfEntries);
    void _onWriteDataToDbErrorOccured();
    void _connectDataSourceToWordMixer();

private:
    void _allowAddToCache();
    void _blockAddToCache();
    void _allowCacheReset();
    void _blockCacheReset();
    void _allowSaveToDb();
    void _blockSaveToDb();

    GameFunctionalityProxy* m_pGameFunctionalityProxy;
    DataSourceProxy* m_pDataSourceProxy;
    DataSourceAccessHelper* m_pDataSourceAccessHelper;
    WordMixer* m_pWordMixer;
    WordPairOwner* m_pWordPairOwner;
    InputBuilder* m_pInputBuilder;
    ScoreItem* m_pScoreItem;
    Game::StatusCodes m_CurrentStatusCode;
    bool m_IsDataAvailable;
    bool m_IsDataEntryAllowed;
    bool m_IsAddingToCacheAllowed;
    bool m_IsResettingCacheAllowed;
    int m_CachedWordPairs;
    bool m_IsSavingToDbAllowed;
    bool m_IsGameStarted;
    bool m_IsGamePaused;
};

#endif // GAMEFACADE_H
