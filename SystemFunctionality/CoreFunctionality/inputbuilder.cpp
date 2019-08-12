#include "inputbuilder.h"

InputBuilder::InputBuilder(QObject *parent)
    : QObject(parent)
    , m_FirstWordInput{}
    , m_SecondWordInput{}
    , m_IsCloseInputAllowed{false}
{
}

bool InputBuilder::addPieceToInputWord(Game::InputWordNumber inputWordNumber, int index, Game::PieceTypes pieceType)
{
    return (inputWordNumber == Game::InputWordNumber::ONE ? _addPieceToInputWord(m_FirstWordInput, m_SecondWordInput, index, pieceType) : _addPieceToInputWord(m_SecondWordInput, m_FirstWordInput, index, pieceType));
}

bool InputBuilder::removePiecesFromInputWord(Game::InputWordNumber inputWordNumber, int rangeStart)
{
    bool success{false};

    if (inputWordNumber == Game::InputWordNumber::ONE)
    {
        success = _removePiecesFromWordInput(m_FirstWordInput, m_SecondWordInput, rangeStart);
    }
    else
    {
        success = _removePiecesFromWordInput(m_SecondWordInput, m_FirstWordInput, rangeStart);
    }

    if (success)
    {
        clearPersistentPiecesRemovalIndexes();

        // approval will be required again when selecting an end piece while the other word is already closed
        m_IsCloseInputAllowed = false;
    }

    return success;
}

bool InputBuilder::clearInput()
{
    bool success{_removePiecesFromWordInput(m_FirstWordInput, m_SecondWordInput, 0)};

    // ensure the remove function is executed for both words
    success = _removePiecesFromWordInput(m_SecondWordInput, m_FirstWordInput, 0) || success;

    if (success)
    {
        clearPersistentPiecesRemovalIndexes();

        // approval will be required again when selecting an end piece while the other word is already closed
        m_IsCloseInputAllowed = false;
    }

    return success;
}

void InputBuilder::setPersistentPiecesRemovalIndex(Game::InputWordNumber inputWordNumber)
{
    WordInput& mainInput{inputWordNumber == Game::InputWordNumber::ONE ? m_FirstWordInput : m_SecondWordInput};
    WordInput& otherInput{inputWordNumber == Game::InputWordNumber::ONE ? m_SecondWordInput : m_FirstWordInput};

    if (mainInput.persistentPiecesRemovalIndex == -1 && mainInput.indexes.size() != 0)
    {
        mainInput.persistentPiecesRemovalIndex = mainInput.indexes.size() - 1;
        otherInput.persistentPiecesRemovalIndex = -1;

        Q_EMIT persistentIndexesChanged();
    }
}

void InputBuilder::clearPersistentPiecesRemovalIndexes()
{
    if (m_FirstWordInput.persistentPiecesRemovalIndex != -1 || m_SecondWordInput.persistentPiecesRemovalIndex != -1)
    {
        m_FirstWordInput.persistentPiecesRemovalIndex = -1;
        m_SecondWordInput.persistentPiecesRemovalIndex = -1;

        Q_EMIT persistentIndexesChanged();
    }
}

void InputBuilder::increasePersistentPiecesRemovalIndex()
{
    Q_ASSERT(m_FirstWordInput.persistentPiecesRemovalIndex == -1 || m_SecondWordInput.persistentPiecesRemovalIndex == -1);

    if (m_FirstWordInput.persistentPiecesRemovalIndex != -1)
    {
        if (m_FirstWordInput.persistentPiecesRemovalIndex < m_FirstWordInput.indexes.size()-1)
        {
            ++m_FirstWordInput.persistentPiecesRemovalIndex;
            Q_EMIT persistentIndexesChanged();
        }
    }
    else if (m_SecondWordInput.persistentPiecesRemovalIndex != -1)
    {
        if (m_SecondWordInput.persistentPiecesRemovalIndex < m_SecondWordInput.indexes.size() - 1)
        {
            ++m_SecondWordInput.persistentPiecesRemovalIndex;
            Q_EMIT persistentIndexesChanged();
        }
    }
}

void InputBuilder::decreasePersistentPiecesRemovalIndex()
{
    Q_ASSERT(m_FirstWordInput.persistentPiecesRemovalIndex == -1 || m_SecondWordInput.persistentPiecesRemovalIndex == -1);

    if (m_FirstWordInput.persistentPiecesRemovalIndex != -1)
    {
        if (m_FirstWordInput.persistentPiecesRemovalIndex > 0)
        {
            --m_FirstWordInput.persistentPiecesRemovalIndex;
            Q_EMIT persistentIndexesChanged();
        }
    }
    else if (m_SecondWordInput.persistentPiecesRemovalIndex != -1)
    {
        if (m_SecondWordInput.persistentPiecesRemovalIndex > 0)
        {
            --m_SecondWordInput.persistentPiecesRemovalIndex;
            Q_EMIT persistentIndexesChanged();
        }
    }
}

void InputBuilder::setCloseInputAllowed()
{
    m_IsCloseInputAllowed = true;
}

const QVector<int> InputBuilder::getFirstWordInputIndexes() const
{
    return m_FirstWordInput.indexes;
}

const QVector<int> InputBuilder::getSecondWordInputIndexes() const
{
    return m_SecondWordInput.indexes;
}

int InputBuilder::getFirstWordPersistentPiecesRemovalIndex() const
{
    return m_FirstWordInput.persistentPiecesRemovalIndex;
}

int InputBuilder::getSecondWordPersistentPiecesRemovalIndex() const
{
    return m_SecondWordInput.persistentPiecesRemovalIndex;
}

bool InputBuilder::isEmptyInput() const
{
    return (m_FirstWordInput.state == WordInputState::EMPTY && m_SecondWordInput.state == WordInputState::EMPTY);
}

bool InputBuilder::isHalfInput() const
{
    return ((m_FirstWordInput.state == WordInputState::EMPTY && m_SecondWordInput.state == WordInputState::COMPLETED) ||
            (m_FirstWordInput.state == WordInputState::COMPLETED && m_SecondWordInput.state == WordInputState::EMPTY));
}

bool InputBuilder::isInputComplete() const
{
    return (m_FirstWordInput.state == WordInputState::COMPLETED && m_SecondWordInput.state == WordInputState::COMPLETED);
}

void InputBuilder::onNewWordPiecesAvailable()
{
    bool resetCompleteInput{m_FirstWordInput.state == WordInputState::COMPLETED && m_SecondWordInput.state == WordInputState::COMPLETED};

    m_FirstWordInput.indexes.clear();
    m_SecondWordInput.indexes.clear();
    m_FirstWordInput.state = WordInputState::EMPTY;
    m_SecondWordInput.state = WordInputState::EMPTY;

    if (resetCompleteInput)
    {
        Q_EMIT inputCompletionChanged();
    }

    Q_EMIT inputChanged();
}

bool InputBuilder::_addPieceToInputWord(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput &otherWordInput, int pieceIndex, Game::PieceTypes pieceType)
{
    bool success{false};

    if (_checkAndUpdateState(currentWordInput, otherWordInput, pieceType))
    {

        currentWordInput.indexes.append(pieceIndex);
        Q_EMIT pieceAddedToInput(pieceIndex);
        success = true;

        Q_EMIT inputChanged();
    }

    return success;
}

bool InputBuilder::_checkAndUpdateState(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput& otherWordInput, Game::PieceTypes pieceType)
{
    bool isValid{false};

    switch (currentWordInput.state)
    {
    case WordInputState::EMPTY:
        if (pieceType == Game::PieceTypes::BEGIN_PIECE)
        {
            isValid = true;
            currentWordInput.state = WordInputState::BUILD_IN_PROGRESS;
        }
        break;
    case WordInputState::BUILD_IN_PROGRESS:
        if (pieceType == Game::PieceTypes::MIDDLE_PIECE)
        {
            isValid = true;
        }
        else if (pieceType == Game::PieceTypes::END_PIECE)
        {
            if (otherWordInput.state != WordInputState::COMPLETED)
            {
                isValid = true;
                currentWordInput.state = WordInputState::COMPLETED;
            }
            else
            {
                if(m_IsCloseInputAllowed)
                {
                    isValid = true;
                    currentWordInput.state = WordInputState::COMPLETED;

                    Q_EMIT inputCompletionChanged();
                }
            }
        }
        break;
    case WordInputState::COMPLETED:
        break;
    }

    return isValid;
}

bool InputBuilder::_removePiecesFromWordInput(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput& otherWordInput, int rangeStart)
{
    bool success{false};

    if (rangeStart>=0 && rangeStart < currentWordInput.indexes.size())
    {
        QVector<int> removedPieceIndexes;

        for (int index{rangeStart}; index<currentWordInput.indexes.size(); ++index)
        {
            removedPieceIndexes.append(currentWordInput.indexes.at(index));
        }

        currentWordInput.indexes.remove(rangeStart, currentWordInput.indexes.size()-rangeStart);

        Q_EMIT inputChanged();
        Q_EMIT piecesRemovedFromInput(removedPieceIndexes);

        if (currentWordInput.state == WordInputState::BUILD_IN_PROGRESS)
        {
            currentWordInput.state = rangeStart > 0 ? WordInputState::BUILD_IN_PROGRESS : WordInputState::EMPTY;
        }
        else if (currentWordInput.state == WordInputState::COMPLETED)
        {
            currentWordInput.state = rangeStart > 0 ? WordInputState::BUILD_IN_PROGRESS : WordInputState::EMPTY;

            if (otherWordInput.state == WordInputState::COMPLETED)
            {
                Q_EMIT inputCompletionChanged();
            }
        }

        success = true;
    }

    return success;
}

InputBuilder::WordInput::WordInput()
    : indexes{}
    , state{WordInputState::EMPTY}
    , persistentPiecesRemovalIndex{-1}
{
}
