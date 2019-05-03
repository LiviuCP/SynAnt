#include "inputbuilder.h"
#include "wordpairowner.h"

InputBuilder::InputBuilder(QObject *parent)
    : QObject(parent)
    , m_pWordPairOwner{nullptr}
    , m_FirstWordInput{}
    , m_SecondWordInput{}
{
}

void InputBuilder::connectToWordPairOwner(WordPairOwner* pWordPairOwner)
{
    Q_ASSERT(pWordPairOwner);
    m_pWordPairOwner = pWordPairOwner;
    bool connected{connect(m_pWordPairOwner, &WordPairOwner::mixedWordsAvailable, this, &InputBuilder::resetInput)};
    Q_ASSERT(connected);
}

bool InputBuilder::addPieceToInputWord(Game::InputWordNumber inputWordNumber, int index)
{
    return (inputWordNumber == Game::InputWordNumber::ONE ? _addPieceToInputWord(m_FirstWordInput, m_SecondWordInput, index) : _addPieceToInputWord(m_SecondWordInput, m_FirstWordInput, index));
}

void InputBuilder::removePiecesFromInputWord(Game::InputWordNumber inputWordNumber, int rangeStart)
{
    if (inputWordNumber == Game::InputWordNumber::ONE)
    {
        _removePiecesFromWordInput(m_FirstWordInput, m_SecondWordInput, rangeStart);
    }
    else
    {
        _removePiecesFromWordInput(m_SecondWordInput, m_FirstWordInput, rangeStart);
    }
}

bool InputBuilder::clearInput()
{
    bool firstWordInputCleared{false};
    bool secondWordInputCleared{false};
    bool success{false};

    WordInputState initialFirstWordInputState{m_FirstWordInput.state};
    WordInputState initialSecondWordInputState{m_SecondWordInput.state};

    QVector<int> removedPieceIndexes;

    if (m_FirstWordInput.state != WordInputState::EMPTY)
    {
        for (auto index : m_FirstWordInput.indexes)
        {
            removedPieceIndexes.append(index);
        }

        m_FirstWordInput.indexes.clear();
        m_FirstWordInput.state = WordInputState::EMPTY;
        firstWordInputCleared = true;
    }

    if (m_SecondWordInput.state != WordInputState::EMPTY)
    {
        for (auto index : m_SecondWordInput.indexes)
        {
            removedPieceIndexes.append(index);
        }

        m_SecondWordInput.indexes.clear();
        m_SecondWordInput.state = WordInputState::EMPTY;
        secondWordInputCleared = true;
    }

    if (firstWordInputCleared || secondWordInputCleared)
    {
        m_pWordPairOwner->updateMultipleWordPiecesSelection(removedPieceIndexes, false);
        success = true;

        if (initialFirstWordInputState == WordInputState::COMPLETED && initialSecondWordInputState == WordInputState::COMPLETED)
        {
            Q_EMIT completionChanged();
        }

        Q_EMIT inputChanged();
    }

    return success;
}

const QVector<int> InputBuilder::getFirstWordInputIndexes() const
{
    return m_FirstWordInput.indexes;
}

const QVector<int> InputBuilder::getSecondWordInputIndexes() const
{
    return m_SecondWordInput.indexes;
}

bool InputBuilder::isInputComplete() const
{
    return (m_FirstWordInput.state == WordInputState::COMPLETED && m_SecondWordInput.state == WordInputState::COMPLETED);
}

void InputBuilder::resetInput()
{
    bool resetCompleteInput{m_FirstWordInput.state == WordInputState::COMPLETED && m_SecondWordInput.state == WordInputState::COMPLETED};

    m_FirstWordInput.indexes.clear();
    m_SecondWordInput.indexes.clear();
    m_FirstWordInput.state = WordInputState::EMPTY;
    m_SecondWordInput.state = WordInputState::EMPTY;

    if (resetCompleteInput)
    {
        Q_EMIT completionChanged();
    }

    Q_EMIT inputChanged();
}

bool InputBuilder::_addPieceToInputWord(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput &otherWordInput, int pieceIndex)
{
    bool success{false};

    if (_checkAndUpdateState(currentWordInput, otherWordInput, pieceIndex))
    {

        currentWordInput.indexes.append(pieceIndex);
        m_pWordPairOwner->updateSingleWordPieceSelection(pieceIndex, true);
        success = true;

        Q_EMIT inputChanged();
    }

    return success;
}

bool InputBuilder::_checkAndUpdateState(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput& otherWordInput, int pieceIndex)
{
    bool isValid{false};

    switch (currentWordInput.state)
    {
    case WordInputState::EMPTY:
        if (m_pWordPairOwner->getMixedWordsPiecesTypes().at(pieceIndex) == Game::PieceTypes::BEGIN_PIECE)
        {
            isValid = true;
            currentWordInput.state = WordInputState::BUILD_IN_PROGRESS;
        }
        break;
    case WordInputState::BUILD_IN_PROGRESS:
        if (m_pWordPairOwner->getMixedWordsPiecesTypes().at(pieceIndex) == Game::PieceTypes::MIDDLE_PIECE)
        {
            isValid = true;
        }
        else if (m_pWordPairOwner->getMixedWordsPiecesTypes().at(pieceIndex) == Game::PieceTypes::END_PIECE)
        {
            if (otherWordInput.state != WordInputState::COMPLETED)
            {
                isValid = true;
                currentWordInput.state = WordInputState::COMPLETED;
            }
            else
            {
                int nrOfPiecesNotSelected{0};

                for (auto isPieceSelected : m_pWordPairOwner->getAreMixedWordsPiecesSelected())
                {
                    if (!isPieceSelected)
                    {
                        ++nrOfPiecesNotSelected;
                    }
                }

                if (nrOfPiecesNotSelected == 1)
                {
                    isValid = true;
                    currentWordInput.state = WordInputState::COMPLETED;

                    Q_EMIT completionChanged();
                }
            }
        }
        break;
    case WordInputState::COMPLETED:
        break;
    }

    return isValid;
}

void InputBuilder::_removePiecesFromWordInput(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput& otherWordInput, int rangeStart)
{
    Q_ASSERT(rangeStart>=0 && rangeStart < currentWordInput.indexes.size());

    QVector<int> removedPieceIndexes;

    for (int index{rangeStart}; index<currentWordInput.indexes.size(); ++index)
    {
        removedPieceIndexes.append(currentWordInput.indexes.at(index));
    }

    currentWordInput.indexes.remove(rangeStart, currentWordInput.indexes.size()-rangeStart);
    Q_EMIT inputChanged();

    m_pWordPairOwner->updateMultipleWordPiecesSelection(removedPieceIndexes, false);

    if (currentWordInput.state == WordInputState::BUILD_IN_PROGRESS)
    {
        currentWordInput.state = rangeStart > 0 ? WordInputState::BUILD_IN_PROGRESS : WordInputState::EMPTY;
    }
    else if (currentWordInput.state == WordInputState::COMPLETED)
    {
        currentWordInput.state = rangeStart > 0 ? WordInputState::BUILD_IN_PROGRESS : WordInputState::EMPTY;

        if (otherWordInput.state == WordInputState::COMPLETED)
        {
            Q_EMIT completionChanged();
        }
    }
}

InputBuilder::WordInput::WordInput()
    : indexes{}
    , state{WordInputState::EMPTY}
{
}
