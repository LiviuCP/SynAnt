#include "dataentrypresenter.h"
#include "../SystemFunctionality/Management/dataentryfacade.h"
#include "../SystemFunctionality/Utilities/gamestrings.h"
#include "../SystemFunctionality/Proxies/dataproxy.h"

static const QMap<Game::ValidationCodes, QString> c_InvalidPairEntryReasonMessages
{
    {Game::ValidationCodes::LESS_MIN_CHARS_PER_WORD, GameStrings::c_WordHasLessThanMinimumCharacters},
    {Game::ValidationCodes::LESS_MIN_TOTAL_PAIR_CHARS, GameStrings::c_PairHasLessThanMinimumCharacters},
    {Game::ValidationCodes::MORE_MAX_TOTAL_PAIR_CHARS, GameStrings::c_PairHasMoreThanMaximumCharacters},
    {Game::ValidationCodes::INVALID_CHARACTERS, GameStrings::c_InvalidCharacters},
    {Game::ValidationCodes::IDENTICAL_WORDS, GameStrings::c_PairHasIdenticalWords},
    {Game::ValidationCodes::PAIR_ALREADY_EXISTS, GameStrings::c_PairAlreadyExists}
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

void DataEntryPresenter::startDataEntry()
{
    m_pDataEntryFacade->startDataEntry();
}

void DataEntryPresenter::resumeDataEntry()
{
    m_pDataEntryFacade->resumeDataEntry();
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

QString DataEntryPresenter::getDataEntryPaneStatusMessage() const
{
    return m_DataEntryPaneStatusMessage;
}

void DataEntryPresenter::_onStatusChanged(Game::DataEntryStatusCodes statusCode)
{
    if (m_pStatusUpdateTimer->isActive())
    {
        m_pStatusUpdateTimer->stop();
    }

    switch (statusCode)
    {
    case Game::DataEntryStatusCodes::DATA_ENTRY_STARTED:
        _updateStatusMessage(GameStrings::c_DataEntryStartMessage, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::DataEntryStatusCodes::DATA_ENTRY_RESUMED:
        _updateStatusMessage(GameStrings::c_DataEntryResumeMessage.arg(m_pDataEntryFacade->getCurrentNrOfAddedWordPairs()), Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::DataEntryStatusCodes::DATA_ENTRY_ADD_SUCCESS:
        Q_EMIT dataEntryAddSucceeded();
        _updateStatusMessage(GameStrings::c_DataEntrySuccessMessage.arg(m_pDataEntryFacade->getCurrentNrOfAddedWordPairs()), Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::DataEntryStatusCodes::DATA_ENTRY_ADD_INVALID:
        _updateStatusMessage(GameStrings::c_DataEntryInvalidPairMessage.arg(c_InvalidPairEntryReasonMessages[m_pDataEntryFacade->getDataEntryValidationCode()]), Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
        Q_EMIT dataEntryAddInvalid();
        break;
    case Game::DataEntryStatusCodes::PAIR_ALREADY_ADDED:
        _updateStatusMessage(GameStrings::c_PairAlreadyAddedMessage, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::DataEntryStatusCodes::RESET_CACHE_REQUESTED:
        _updateStatusMessage(GameStrings::c_DataEntryCacheResetRequestedMessage, Game::c_NoDelay);
        break;
    case Game::DataEntryStatusCodes::CACHE_RESET:
        _updateStatusMessage(GameStrings::c_DataEntryCacheResetMessage, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::DataEntryStatusCodes::DATA_SAVE_IN_PROGRESS:
        _updateStatusMessage(GameStrings::c_DataSaveInProgressMessage, Game::c_NoDelay);
        Q_EMIT dataSaveInProgress();
        break;
    case Game::DataEntryStatusCodes::DATA_SUCCESSFULLY_SAVED:
        _updateStatusMessage(GameStrings::c_DataSuccessfullySavedMessage.arg(m_pDataEntryFacade->getLastSavedNrOfWordPairs()), Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Game::c_ShortStatusUpdateDelay);
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
