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

#include "../Utilities/dataentryutils.h"

class DataFunctionalityProxy;
class DataEntryProxy;

class DataEntryFacade : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryFacade(QObject *parent = nullptr);

    void startDataEntry();
    void resumeDataEntry();
    void stopDataEntry();

    void requestAddPairToCache(const QString& firstWord, const QString& secondWord, bool areSynonyms);
    void requestSaveDataToDb();
    void requestCacheReset();

    int getCurrentNrOfAddedWordPairs() const;
    int getLastSavedTotalNrOfEntries() const;
    int getLastNrOfEntriesSavedToPrimaryLanguage() const;
    int getCurrentLanguageIndex() const;
    DataEntry::ValidationCodes getDataEntryValidationCode() const;

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
    Q_SIGNAL void statusChanged(DataEntry::StatusCodes status);
    Q_SIGNAL void fetchingInProgressChanged();

private slots:
    void _onDataEntryAllowed(bool success);
    void _onNewWordsPairAddedToCache();
    void _onAddInvalidWordsPairRequested();
    void _onWordsPairAlreadyContainedInCache();
    void _onCacheReset();
    void _onWriteDataToDbFinished();
    void _onFetchDataForDataEntryLanguageFinished(bool success);

private:
    void _allowAddToCache();
    void _blockAddToCache();
    void _allowCacheReset();
    void _blockCacheReset();
    void _allowSaveToDb();
    void _blockSaveToDb();

    DataFunctionalityProxy* m_pDataFunctionalityProxy;
    DataEntryProxy* m_pDataEntryProxy;

    DataEntry::StatusCodes m_CurrentStatusCode;

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
