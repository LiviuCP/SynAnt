/*
   This class connects the front-end data entry UI part of the code (data entry presenter) to the back-end data classes:
   1) Architecture is similar to connection between Gamepresenter and Gamefacade. However the facade does not directly manage any backend classes but connects to a proxy
      (DataEntryProxy) which together with the game manager intermediates access to the data source and the other classes related to data entry (cache, validator, etc).
      This facade has been primarily created for achieving consistency with the GamePresenter - GameFacade - Backend model.
   2) Enables adding entries to cache, resetting cache and saving data to database.
   3) Last but not least the facade is responsible for updating the status of the game (data entry part).
*/

#ifndef DATAENTRYFACADE_H
#define DATAENTRYFACADE_H

#include <QObject>

class DataEntryProxy;

class DataEntryFacade : public QObject
{
    Q_OBJECT
public:
    enum class StatusCodes
    {
        NO_DATA_ENTRY_REQUESTED,
        DATA_ENTRY_STARTED,
        DATA_ENTRY_STARTED_SAVE_IN_PROGRESS,
        DATA_ENTRY_RESUMED,
        DATA_ENTRY_STOPPED,
        DATA_ENTRY_STOPPED_SAVE_IN_PROGRESS,
        DATA_ENTRY_DISABLED,
        DATA_ENTRY_ADD_SUCCESS,
        Add_Failed_Status_Codes_Start, // should not be emitted to presenter, it's just for counting failed add status codes
        ADD_FAILED_LESS_MIN_CHARS_PER_WORD,
        ADD_FAILED_LESS_MIN_TOTAL_PAIR_CHARS,
        ADD_FAILED_MORE_MAX_TOTAL_PAIR_CHARS,
        ADD_FAILED_INVALID_CHARACTERS,
        ADD_FAILED_PAIR_ALREADY_EXISTS_IN_DATABASE,
        ADD_FAILED_IDENTICAL_WORDS,
        Add_Failed_Status_Codes_End, // should not be emitted to presenter, it's just for counting failed add status codes
        PAIR_ALREADY_ADDED_SAVE_OR_DISCARD,
        RESET_CACHE_REQUESTED,
        CACHE_RESET,
        FETCHING_DATA,
        DATA_FETCHING_FINISHED,
        DATA_FETCHING_FINISHED_SAVE_IN_PROGRESS,
        DATA_SAVE_IN_PROGRESS,
        DATA_SUCCESSFULLY_SAVED,
    };

    explicit DataEntryFacade(QObject *parent = nullptr);

    void startDataEntry();
    void resumeDataEntry();
    void stopDataEntry();

    void requestAddPairToCache(const QString& firstWord, const QString& secondWord, bool areSynonyms);
    void requestSaveDataToDb();
    void requestCacheReset();

    int getCurrentNrOfAddedPairs() const;
    int getLastSavedTotalNrOfPairs() const;
    int getLastNrOfPairsSavedToPrimaryLanguage() const;
    int getCurrentLanguageIndex() const;
    DataEntryFacade::StatusCodes getStatusCode() const;

    bool isDataEntryAllowed() const;
    bool isAddingToCacheAllowed() const;
    bool isCacheResetAllowed() const;
    bool isSavingToDbAllowed() const;
    bool isDataFetchingInProgress() const;
    bool isDataSavingInProgress() const;

    void setLanguage(int languageIndex);

signals:
    Q_SIGNAL void dataEntryAllowedChanged();
    Q_SIGNAL void addPairToCacheAllowedChanged();
    Q_SIGNAL void resetCacheAllowedChanged();
    Q_SIGNAL void saveNewPairsToDbAllowedChanged();
    Q_SIGNAL void languageChanged();
    Q_SIGNAL void statusChanged();
    Q_SIGNAL void fetchingInProgressChanged();

private slots:
    void _onFetchDataForDataEntryLanguageFinished(bool success);
    void _onDataEntryAllowed(bool success);
    void _onNewWordsPairAddedToCache();
    void _onAddInvalidWordsPairRequested();
    void _onWordsPairAlreadyContainedInCache();
    void _onCacheReset();
    void _onWriteDataToDbFinished();

private:
    void _allowAddToCache();
    void _blockAddToCache();
    void _allowCacheReset();
    void _blockCacheReset();
    void _allowSaveToDb();
    void _blockSaveToDb();
    DataEntryFacade::StatusCodes _retrieveInvalidWordsPairStatusCode();

    DataEntryProxy* m_pDataEntryProxy;

    DataEntryFacade::StatusCodes m_CurrentStatusCode;

    bool m_IsDataEntryAllowed;
    bool m_IsAddingToCacheAllowed;
    bool m_IsResettingCacheAllowed;
    bool m_IsSavingToDbAllowed;
    bool m_IsFetchingInProgress;
    bool m_IsSavingInProgress;
    int m_CurrentLanguageIndex;
    bool m_IsSavingDeferred;
};

#endif // DATAENTRYFACADE_H
