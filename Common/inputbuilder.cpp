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

bool InputBuilder::addPieceToFirstWordInput(int index)
{
    return _addPieceToWordInput(m_FirstWordInput, m_SecondWordInput, index);
}

bool InputBuilder::addPieceToSecondWordInput(int index)
{
    return _addPieceToWordInput(m_SecondWordInput, m_FirstWordInput, index);
}

void InputBuilder::removePiecesFromFirstWordInput(int rangeStart)
{
    _removePiecesFromWordInput(m_FirstWordInput, m_SecondWordInput, rangeStart);
}

void InputBuilder::removePiecesFromSecondWordInput(int rangeStart)
{
    _removePiecesFromWordInput(m_SecondWordInput, m_FirstWordInput, rangeStart);
}

const QVector<int> InputBuilder::getFirstWordInputIndexes() const
{
    return m_FirstWordInput.indexes;
}

const QVector<int> InputBuilder::getSecondWordInputIndexes() const
{
    return m_SecondWordInput.indexes;
}

QString InputBuilder::getFirstInputWord() const
{
    QString firstInputWord;

    for (auto index : m_FirstWordInput.indexes)
    {
        firstInputWord.append(m_pWordPairOwner->getMixedWordsPieces().at(index).content);
    }

    return firstInputWord;
}

QString InputBuilder::getSecondInputWord() const
{
    QString secondInputWord;

    for (auto index : m_SecondWordInput.indexes)
    {
        secondInputWord.append(m_pWordPairOwner->getMixedWordsPieces().at(index).content);
    }

    return secondInputWord;
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

bool InputBuilder::_addPieceToWordInput(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput &otherWordInput, int pieceIndex)
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
        if (m_pWordPairOwner->getMixedWordsPieces().at(pieceIndex).pieceType == Game::PieceTypes::BEGIN_PIECE)
        {
            isValid = true;
            currentWordInput.state = WordInputState::BUILD_IN_PROGRESS;
        }
        break;
    case WordInputState::BUILD_IN_PROGRESS:
        if (m_pWordPairOwner->getMixedWordsPieces().at(pieceIndex).pieceType == Game::PieceTypes::MIDDLE_PIECE)
        {
            isValid = true;
        }
        else if (m_pWordPairOwner->getMixedWordsPieces().at(pieceIndex).pieceType == Game::PieceTypes::END_PIECE)
        {
            if (otherWordInput.state != WordInputState::COMPLETED)
            {
                isValid = true;
                currentWordInput.state = WordInputState::COMPLETED;
            }
            else
            {
                int nrOfPiecesNotSelected{0};

                for (auto piece : m_pWordPairOwner->getMixedWordsPieces())
                {
                    if (!piece.isSelected)
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
