#include "dataentrypresenter.h"
#include "../SystemFunctionality/Management/dataentryfacade.h"
#include "../SystemFunctionality/Proxies/dataproxy.h"
#include "../SystemFunctionality/Utilities/dataentryutils.h"
#include "../SystemFunctionality/Utilities/gameutils.h"

static const QMap<DataEntry::ValidationCodes, QString> c_InvalidPairEntryReasonMessages
{
    {DataEntry::ValidationCodes::LESS_MIN_CHARS_PER_WORD, DataEntry::Messages::c_WordHasLessThanMinCharacters},
    {DataEntry::ValidationCodes::LESS_MIN_TOTAL_PAIR_CHARS, DataEntry::Messages::c_PairHasLessThanMinCharacters},
    {DataEntry::ValidationCodes::MORE_MAX_TOTAL_PAIR_CHARS, DataEntry::Messages::c_PairHasMoreThanMaxCharacters},
    {DataEntry::ValidationCodes::INVALID_CHARACTERS, DataEntry::Messages::c_InvalidCharactersEntered},
    {DataEntry::ValidationCodes::IDENTICAL_WORDS, DataEntry::Messages::c_PairHasIdenticalWords},
    {DataEntry::ValidationCodes::PAIR_ALREADY_EXISTS, DataEntry::Messages::c_PairAlreadyExists}
};

DataEntryPresenter::DataEntryPresenter(QObject *parent)
    : QObject(parent)
    , m_pDataEntryFacade{nullptr}
    , m_pDataProxy {new DataProxy{this}}
    , m_pStatusUpdateTimer {new QTimer{this}}
{
    m_pDataEntryFacade = m_pDataProxy->getDataEntryFacade();
    Q_ASSERT(m_pDataEntryFacade);

    m_pStatusUpdateTimer->setSingleShot(true);

    bool connected{connect(m_pDataEntryFacade, &DataEntryFacade::dataEntryAllowedChanged, this, &DataEntryPresenter::dataEntryEnabledChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryFacade, &DataEntryFacade::addPairToCacheAllowedChanged, this, &DataEntryPresenter::addWordsPairEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryFacade, &DataEntryFacade::resetCacheAllowedChanged, this, &DataEntryPresenter::discardAddedWordPairsEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryFacade, &DataEntryFacade::saveNewPairsToDbAllowedChanged, this, &DataEntryPresenter::saveAddedWordPairsEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryFacade, &DataEntryFacade::languageChanged, this, &DataEntryPresenter::languageChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryFacade, &DataEntryFacade::fetchingInProgressChanged, this, &DataEntryPresenter::dataFetchingInProgressChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryFacade, &DataEntryFacade::statusChanged, this, &DataEntryPresenter::_onStatusChanged);
    Q_ASSERT(connected);
    connected = connect(m_pStatusUpdateTimer, &QTimer::timeout, this, &DataEntryPresenter::_updateMessage);
    Q_ASSERT(connected);
}

void DataEntryPresenter::handleAddWordsPairRequest(const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    m_pDataEntryFacade->requestAddPairToCache(firstWord, secondWord, areSynonyms);
}

void DataEntryPresenter::handleClearAddedWordPairsRequest()
{
    m_pDataEntryFacade->requestCacheReset();
}

void DataEntryPresenter::handleSaveAddedWordPairsRequest()
{
    m_pDataEntryFacade->requestSaveDataToDb();
}

void DataEntryPresenter::handleLanguageChangeRequest(int newLanguageIndex)
{
    Q_ASSERT(newLanguageIndex >= 0); // update it once the languages are better setup in backend (beyond facade)
    m_pDataEntryFacade->setLanguage(newLanguageIndex);
}

void DataEntryPresenter::startDataEntry()
{
    m_pDataEntryFacade->startDataEntry();
}

void DataEntryPresenter::resumeDataEntry()
{
    m_pDataEntryFacade->resumeDataEntry();
}

void DataEntryPresenter::stopDataEntry()
{
    m_pDataEntryFacade->stopDataEntry();
}

bool DataEntryPresenter::isDataEntryEnabled() const
{
    return m_pDataEntryFacade->isDataEntryAllowed();
}

bool DataEntryPresenter::isAddWordsPairEnabled() const
{
    return m_pDataEntryFacade->isAddingToCacheAllowed();
}

bool DataEntryPresenter::isDiscardAddedWordPairsEnabled() const
{
    return m_pDataEntryFacade->isCacheResetAllowed();
}

bool DataEntryPresenter::isSaveAddedWordPairsEnabled() const
{
    return m_pDataEntryFacade->isSavingToDbAllowed();
}

bool DataEntryPresenter::getDataFetchingInProgress() const
{
    return m_pDataEntryFacade->isDataFetchingInProgress();
}

bool DataEntryPresenter::getDataSavingInProgress() const
{
    return m_pDataEntryFacade->isDataSavingInProgress();
}

QString DataEntryPresenter::getDataEntryPaneStatusMessage() const
{
    return m_DataEntryPaneStatusMessage;
}

int DataEntryPresenter::getLanguageIndex() const
{
    return m_pDataEntryFacade->getCurrentLanguageIndex();
}

void DataEntryPresenter::_onStatusChanged(DataEntry::StatusCodes statusCode)
{
    if (m_pStatusUpdateTimer->isActive())
    {
        m_pStatusUpdateTimer->stop();
    }

    switch (statusCode)
    {
    case DataEntry::StatusCodes::DATA_ENTRY_STARTED:
        _updateStatusMessage(DataEntry::Messages::c_DataEntryStartMessage, Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_STARTED_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntry::Messages::c_DataEntrySaveInProgressStartMessage, Game::Timing::c_NoDelay);
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_RESUMED:
        _updateStatusMessage(DataEntry::Messages::c_DataEntryResumeMessage.arg(m_pDataEntryFacade->getCurrentNrOfAddedWordPairs()), Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_STOPPED:
        _updateStatusMessage(DataEntry::Messages::c_DataEntryStopMessage, Game::Timing::c_NoDelay);
        Q_EMIT dataEntryStopped();
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_STOPPED_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntry::Messages::c_DataEntrySaveInProgressStopMessage, Game::Timing::c_NoDelay);
        Q_EMIT dataEntryStopped();
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_DISABLED:
        _updateStatusMessage(DataEntry::Messages::c_DataEntryDisabledMessage, Game::Timing::c_NoDelay);
        Q_EMIT dataEntryStopped();
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_ADD_SUCCESS:
        Q_EMIT dataEntryAddSucceeded();
        _updateStatusMessage(DataEntry::Messages::c_DataEntrySuccessMessage.arg(m_pDataEntryFacade->getCurrentNrOfAddedWordPairs()), Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntry::StatusCodes::DATA_ENTRY_ADD_INVALID:
        _updateStatusMessage(DataEntry::Messages::c_DataEntryInvalidPairMessage.arg(c_InvalidPairEntryReasonMessages[m_pDataEntryFacade->getDataEntryValidationCode()]),
                             Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        Q_EMIT dataEntryAddInvalid();
        break;
    case DataEntry::StatusCodes::PAIR_ALREADY_ADDED:
        _updateStatusMessage(DataEntry::Messages::c_DataEntryPairAlreadyAddedMessage, Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntry::StatusCodes::RESET_CACHE_REQUESTED:
        _updateStatusMessage(DataEntry::Messages::c_DataCacheResetInProgressMessage, Game::Timing::c_NoDelay);
        break;
    case DataEntry::StatusCodes::CACHE_RESET:
        _updateStatusMessage(DataEntry::Messages::c_DataCacheResetMessage, Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntry::StatusCodes::DATA_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntry::Messages::c_DataSaveInProgressMessage, Game::Timing::c_NoDelay);
        Q_EMIT dataSaveInProgress();
        Q_EMIT dataSavingInProgressChanged();
        break;
    case DataEntry::StatusCodes::DATA_SUCCESSFULLY_SAVED:
        _updateStatusMessage(DataEntry::Messages::c_DataSuccessfullySavedMessage.arg(m_pDataEntryFacade->getLastSavedTotalNrOfEntries()).arg(m_pDataEntryFacade->getLastNrOfEntriesSavedToPrimaryLanguage()), Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        Q_EMIT dataSavingInProgressChanged();
        break;
    case DataEntry::StatusCodes::FETCHING_DATA:
        _updateStatusMessage(Game::Messages::c_FetchingDataMessage, Game::Timing::c_NoDelay);
        break;
    case DataEntry::StatusCodes::DATA_FETCHING_FINISHED:
        _updateStatusMessage(Game::Messages::c_LanguageChangedMessage, Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataEntryRequestMessage, Game::Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntry::StatusCodes::DATA_FETCHING_FINISHED_SAVE_IN_PROGRESS:
        _updateStatusMessage(Game::Messages::c_LanguageChangedMessage, Game::Timing::c_NoDelay);
        _updateStatusMessage(DataEntry::Messages::c_DataSaveInProgressMessage, Game::Timing::c_ShortStatusUpdateDelay / 2);
        Q_EMIT dataSaveInProgress();
        Q_EMIT dataSavingInProgressChanged();
        break;
    default:
        Q_ASSERT(false);
    }
}

void DataEntryPresenter::_updateStatusMessage(const QString &message, int delay)
{
    m_CurrentStatusMessage = message;

    if (delay > 0)
    {
        m_pStatusUpdateTimer->start(delay);
    }
    else
    {
        _updateMessage();
    }
}

void DataEntryPresenter::_updateMessage()
{
    m_DataEntryPaneStatusMessage = m_CurrentStatusMessage;
    Q_EMIT dataEntryPaneStatusMessageChanged();
}
