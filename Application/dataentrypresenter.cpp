#include "dataentrypresenter.h"
#include "dataentrystrings.h"
#include "timing.h"
#include "../SystemFunctionality/Management/dataentryfacade.h"
#include "../SystemFunctionality/ManagementProxies/dataproxy.h"
#include "../SystemFunctionality/Utilities/gameutils.h"

static const QMap<DataEntryFacade::StatusCodes, QString> c_InvalidPairEntryStatusMessages
{
    {DataEntryFacade::StatusCodes::ADD_FAILED_LESS_MIN_CHARS_PER_WORD, DataEntryStrings::Messages::c_WordHasLessThanMinCharacters},
    {DataEntryFacade::StatusCodes::ADD_FAILED_LESS_MIN_TOTAL_PAIR_CHARS, DataEntryStrings::Messages::c_PairHasLessThanMinCharacters},
    {DataEntryFacade::StatusCodes::ADD_FAILED_MORE_MAX_TOTAL_PAIR_CHARS, DataEntryStrings::Messages::c_PairHasMoreThanMaxCharacters},
    {DataEntryFacade::StatusCodes::ADD_FAILED_INVALID_CHARACTERS, DataEntryStrings::Messages::c_InvalidCharactersEntered},
    {DataEntryFacade::StatusCodes::ADD_FAILED_IDENTICAL_WORDS, DataEntryStrings::Messages::c_PairHasIdenticalWords},
    {DataEntryFacade::StatusCodes::ADD_FAILED_PAIR_ALREADY_EXISTS, DataEntryStrings::Messages::c_PairAlreadyExists}
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

void DataEntryPresenter::handleLanguageChangeRequest(int newLanguageIndex)
{
    Q_ASSERT(newLanguageIndex >= 0); // update it once the languages are better setup in backend (beyond facade)
    m_pDataEntryFacade->setLanguage(newLanguageIndex);
}

void DataEntryPresenter::handleAddWordsPairRequest(const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    m_pDataEntryFacade->requestAddPairToCache(firstWord, secondWord, areSynonyms);
}

void DataEntryPresenter::handleDiscardAddedWordPairsRequest()
{
    m_pDataEntryFacade->requestCacheReset();
}

void DataEntryPresenter::handleSaveAddedWordPairsRequest()
{
    m_pDataEntryFacade->requestSaveDataToDb();
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

bool DataEntryPresenter::isDataFetchingInProgress() const
{
    return m_pDataEntryFacade->isDataFetchingInProgress();
}

bool DataEntryPresenter::isDataSavingInProgress() const
{
    return m_pDataEntryFacade->isDataSavingInProgress();
}

int DataEntryPresenter::getLanguageIndex() const
{
    return m_pDataEntryFacade->getCurrentLanguageIndex();
}

QString DataEntryPresenter::getDataEntryPaneStatusMessage() const
{
    return m_DataEntryPaneStatusMessage;
}

void DataEntryPresenter::_onStatusChanged()
{
    DataEntryFacade::StatusCodes statusCode{m_pDataEntryFacade->getStatusCode()};

    if (m_pStatusUpdateTimer->isActive())
    {
        m_pStatusUpdateTimer->stop();
    }

    switch (statusCode)
    {
    case DataEntryFacade::StatusCodes::DATA_ENTRY_STARTED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryStartMessage, Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntryFacade::StatusCodes::DATA_ENTRY_STARTED_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntrySaveInProgressStartMessage, Timing::c_NoDelay);
        break;
    case DataEntryFacade::StatusCodes::DATA_ENTRY_RESUMED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryResumeMessage.arg(m_pDataEntryFacade->getCurrentNrOfAddedPairs()), Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntryFacade::StatusCodes::DATA_ENTRY_STOPPED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryStopMessage, Timing::c_NoDelay);
        Q_EMIT dataEntryStopped();
        break;
    case DataEntryFacade::StatusCodes::DATA_ENTRY_STOPPED_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntrySaveInProgressStopMessage, Timing::c_NoDelay);
        Q_EMIT dataEntryStopped();
        break;
    case DataEntryFacade::StatusCodes::DATA_ENTRY_DISABLED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryDisabledMessage, Timing::c_NoDelay);
        Q_EMIT dataEntryStopped();
        break;
    case DataEntryFacade::StatusCodes::DATA_ENTRY_ADD_SUCCESS:
        Q_EMIT dataEntryAddSucceeded();
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntrySuccessMessage.arg(m_pDataEntryFacade->getCurrentNrOfAddedPairs()), Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntryFacade::StatusCodes::ADD_FAILED_LESS_MIN_CHARS_PER_WORD:
    case DataEntryFacade::StatusCodes::ADD_FAILED_LESS_MIN_TOTAL_PAIR_CHARS:
    case DataEntryFacade::StatusCodes::ADD_FAILED_MORE_MAX_TOTAL_PAIR_CHARS:
    case DataEntryFacade::StatusCodes::ADD_FAILED_INVALID_CHARACTERS:
    case DataEntryFacade::StatusCodes::ADD_FAILED_IDENTICAL_WORDS:
    case DataEntryFacade::StatusCodes::ADD_FAILED_PAIR_ALREADY_EXISTS:
        Q_ASSERT(c_InvalidPairEntryStatusMessages.contains(statusCode));
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryInvalidPairMessage.arg(c_InvalidPairEntryStatusMessages[statusCode]),
                             Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        Q_EMIT dataEntryAddInvalid();
        break;
    case DataEntryFacade::StatusCodes::PAIR_ALREADY_ADDED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryPairAlreadyAddedMessage, Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntryFacade::StatusCodes::RESET_CACHE_REQUESTED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataCacheResetInProgressMessage, Timing::c_NoDelay);
        break;
    case DataEntryFacade::StatusCodes::CACHE_RESET:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataCacheResetMessage, Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntryFacade::StatusCodes::DATA_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataSaveInProgressMessage, Timing::c_NoDelay);
        Q_EMIT dataSaveInProgress();
        Q_EMIT dataSavingInProgressChanged();
        break;
    case DataEntryFacade::StatusCodes::DATA_SUCCESSFULLY_SAVED:
        _updateStatusMessage(DataEntryStrings::Messages::c_DataSuccessfullySavedMessage.arg(m_pDataEntryFacade->getLastSavedTotalNrOfPairs()).arg(m_pDataEntryFacade->getLastNrOfPairsSavedToPrimaryLanguage()), Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        Q_EMIT dataSavingInProgressChanged();
        break;
    case DataEntryFacade::StatusCodes::FETCHING_DATA:
        _updateStatusMessage(DataEntryStrings::Messages::c_FetchingDataMessage, Timing::c_NoDelay);
        break;
    case DataEntryFacade::StatusCodes::DATA_FETCHING_FINISHED:
        _updateStatusMessage(DataEntryStrings::Messages::c_LanguageChangedMessage, Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataEntryRequestMessage, Timing::c_ShortStatusUpdateDelay);
        break;
    case DataEntryFacade::StatusCodes::DATA_FETCHING_FINISHED_SAVE_IN_PROGRESS:
        _updateStatusMessage(DataEntryStrings::Messages::c_LanguageChangedMessage, Timing::c_NoDelay);
        _updateStatusMessage(DataEntryStrings::Messages::c_DataSaveInProgressMessage, Timing::c_ShortStatusUpdateDelay / 2);
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
