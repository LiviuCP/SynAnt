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

void InputBuilder::resetInput()
{
    m_FirstWordInput.indexes.clear();
    m_SecondWordInput.indexes.clear();
    m_FirstWordInput.state = WordInputState::EMPTY;
    m_SecondWordInput.state = WordInputState::EMPTY;

    Q_EMIT inputChanged();
}

bool InputBuilder::_addPieceToWordInput(InputBuilder::WordInput &currentWordInput, const InputBuilder::WordInput &otherWordInput, int index)
{
    Q_UNUSED(otherWordInput);

    currentWordInput.indexes.append(index);
    m_pWordPairOwner->updateWordPieceSelection(index, true);

    qInfo("Index added. The word input now contains following indexes:");
    for (auto index : currentWordInput.indexes)
    {
        qInfo("%d", index);
    }

    qInfo("First word is now: %s", getFirstInputWord().toStdString().c_str());
    qInfo("Second word is now: %s", getSecondInputWord().toStdString().c_str());

    Q_EMIT inputChanged();

    return true;
}

InputBuilder::WordInput::WordInput()
    : indexes{}
    , state{WordInputState::EMPTY}
{
}
