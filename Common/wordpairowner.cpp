#include "wordpairowner.h"
#include "wordmixer.h"

WordPairOwner::WordPairOwner(QObject *parent)
    : QObject{parent}
    , m_pWordMixer{nullptr}
    , m_FirstWordFirstPieceIndex{-1}
    , m_FirstWordLastPieceIndex{-1}
    , m_SecondWordFirstPieceIndex{-1}
    , m_SecondWordLastPieceIndex{-1}
    , m_AreSynonyms{false}
{

}

void WordPairOwner::connectToWordMixer(WordMixer* pWordMixer)
{
    Q_ASSERT(pWordMixer);
    m_pWordMixer = pWordMixer;
    bool connected{connect(m_pWordMixer, &WordMixer::mixedWordsChanged, this, &WordPairOwner::_onMixedWordsAvailable)};
    Q_ASSERT(connected);
}

QVector<QString> WordPairOwner::getMixedWordsPiecesArray() const
{
    return m_MixedWordsPiecesArray;
}

QString WordPairOwner::getFirstWord() const
{
    return m_FirstWord;
}

QString WordPairOwner::getSecondWord() const
{
    return m_SecondWord;
}

int WordPairOwner::getFirstWordFirstPieceIndex() const
{
    return m_FirstWordFirstPieceIndex;
}

int WordPairOwner::getFirstWordLastPieceIndex() const
{
    return m_FirstWordLastPieceIndex;
}

int WordPairOwner::getSecondWordFirstPieceIndex() const
{
    return m_SecondWordFirstPieceIndex;
}

int WordPairOwner::getSecondWordLastPieceIndex() const
{
    return m_SecondWordLastPieceIndex;
}

bool WordPairOwner::areSynonyms() const
{
    return m_pWordMixer->areSynonyms();
}

void WordPairOwner::_onMixedWordsAvailable()
{
    m_MixedWordsPiecesArray = m_pWordMixer->getMixedWordsPiecesArray();
    m_FirstWord = m_pWordMixer->getFirstWord();
    m_SecondWord = m_pWordMixer->getSecondWord();
    m_FirstWordFirstPieceIndex = m_pWordMixer->getFirstWordFirstPieceIndex();
    m_FirstWordLastPieceIndex = m_pWordMixer->getFirstWordLastPieceIndex();
    m_SecondWordFirstPieceIndex = m_pWordMixer->getSecondWordFirstPieceIndex();
    m_SecondWordLastPieceIndex = m_pWordMixer->getSecondWordLastPieceIndex();
    m_AreSynonyms = m_pWordMixer->areSynonyms();

    Q_EMIT mixedWordsAvailable();
}
