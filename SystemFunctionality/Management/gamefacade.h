/*
   This class connects the front-end UI part of the code (presenter) to the back-end code base:
   1) Backend classes are being accessed by presenter through facade by executing function calls.
   2) In return backend classes send signals to presenter through facade.
   3) The facade checks the user input created by InputBuilder against the reference words contained in WordPairOwner.
   4) The facade intermediates the communication between data access classes and consumer (WordMixer) by using the DataSourceProxy.
   5) The facade provides decoupling by hiding the backend functionality (WordMixer, StatisticsItem, WordPairOwner, InputBuilder and data access classes) entirely from presenter.
   6) Last but not least the facade is responsible for updating the status of the game (except data entry).
*/

#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include <QObject>
#include <QTimer>

#include "../Utilities/gameutils.h"

class GameFunctionalityProxy;
class DataSourceAccessHelper;
class WordMixer;
class WordPairOwner;
class InputBuilder;
class StatisticsItem;
class Chronometer;

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

    void setGameLevel(Game::Levels level);
    void setLanguage(int languageIndex, bool revertLanguageWhenDataUnavailable);

    void enableTimeLimit();
    void disableTimeLimit();

    void enablePersistentMode();
    void disablePersistentMode();
    void goToNextPersistentModeContainer();
    void increasePersistentIndex();
    void decreasePersistentIndex();
    void executeFirstPersistentModeAction();
    void executeSecondPersistentModeAction();

    void addPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex);
    void removePiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart);
    void clearInputWord(Game::InputWordNumber inputWordNumber);
    void clearInput();
    void handleSubmitRequest();
    void provideCorrectWordsPairToUser();

    void resetGameStatistics();
    void handleDataSavingOperationInProgress();

    bool isDataFetchingInProgress() const;
    bool isDataAvailable() const;
    bool isPersistentModeEnabled() const;
    bool isInputComplete() const;
    bool areWordsFromCurrentPairSynonyms() const;
    bool isTimeLimitEnabled() const;
    bool canResetGameStatistics() const;

    int getCurrentLanguageIndex() const;

    QVector<QString> getMixedWordsPiecesContent() const;
    QVector<Game::PieceTypes> getMixedWordsPiecesTypes() const;
    QVector<bool> getAreMixedWordsPiecesSelected() const;
    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;
    QString getFirstReferenceWord() const;
    QString getSecondReferenceWord() const;

    int getPersistentPieceSelectionIndex() const;
    int getFirstPersistentPiecesRemovalIndex() const;
    int getSecondPersistentPiecesRemovalIndex() const;

    QPair<QString, QString> getRemainingTime() const;

    QString getObtainedScore() const;
    QString getTotalAvailableScore() const;
    QString getGuessedWordPairs() const;
    QString getTotalWordPairs() const;

signals:
    Q_SIGNAL void fetchingInProgressChanged();
    Q_SIGNAL void dataAvailableChanged();
    Q_SIGNAL void languageChanged();
    Q_SIGNAL void newMixedWordsAvailable();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void completionChanged();
    Q_SIGNAL void piecesAddedToInputChanged();
    Q_SIGNAL void persistentPieceSelectionIndexChanged();
    Q_SIGNAL void persistentPiecesRemovalIndexesChanged();
    Q_SIGNAL void persistentIndexModeEnabledChanged();
    Q_SIGNAL void timeLimitEnabledChanged();
    Q_SIGNAL void remainingTimeRefreshed();
    Q_SIGNAL void statisticsChanged();
    Q_SIGNAL void statusChanged(Game::StatusCodes status);

private slots:
    void _onFetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched);
    void _onFetchDataForSecondaryLanguageFinished(bool success);
    void _onEntryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areWordsFromCurrentPairSynonyms);
    void _onNewWordsPairMixed();
    void _onPiecesAddedToInputStateChanged();
    void _onPieceAddedToInput(int index);
    void _onPiecesRemovedFromInput(QVector<int> indexes);
    void _onChronometerTimeoutTriggered();
    void _onChronometerEnabledChanged();
    void _onStatisticsUpdated();
    void _onPrimaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries);
    void _onDataSavingErrorOccured();

private:
    void _connectToDataSource();
    void _pushCurrentGameLevel();
    void _addPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex);
    void _removePiecesFromInputWordInPersistentMode();

    GameFunctionalityProxy* m_pGameFunctionalityProxy;
    DataSourceAccessHelper* m_pDataSourceAccessHelper;
    WordMixer* m_pWordMixer;
    WordPairOwner* m_pWordPairOwner;
    InputBuilder* m_pInputBuilder;
    StatisticsItem* m_pStatisticsItem;
    Chronometer* m_pChronometer;

    Game::Levels m_GameLevel;
    int m_CurrentLanguageIndex;
    int m_PreviousLanguageIndex; // used for restoring the previous language in main pane in case no word can be fetched from currently setup one
    Game::StatusCodes m_CurrentStatusCode;

    bool m_IsConnectedToDataSource;
    bool m_IsDataAvailable;
    bool m_IsGameStarted;
    bool m_IsGamePaused;
    bool m_IsPersistentIndexModeEnabled;
    bool m_IsFetchingInProgress;
    bool m_ShouldRevertLanguageWhenDataUnavailable;
};

#endif // GAMEFACADE_H
