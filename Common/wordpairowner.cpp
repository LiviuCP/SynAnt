#include "wordpairowner.h"
#include "wordmixer.h"

WordPairOwner::WordPairOwner(QObject *parent)
    : QObject{parent}
    , m_pWordMixer{nullptr}
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

void WordPairOwner::updateSingleWordPieceSelection(int wordPieceIndex, bool selected)
{
    Q_ASSERT(wordPieceIndex >= 0 && wordPieceIndex < m_MixedWordsPieces.size());

    if (m_MixedWordsPieces[wordPieceIndex].isSelected != selected)
    {
        m_MixedWordsPieces[wordPieceIndex].isSelected = selected;
        Q_EMIT selectionChanged();
    }
}

void WordPairOwner::updateMultipleWordPiecesSelection(QVector<int> wordPieceIndex, bool selected)
{
    bool isSelectionChanged{false};

    for (auto index : wordPieceIndex)
    {
        Q_ASSERT(index >= 0 && index < m_MixedWordsPieces.size());

        if (m_MixedWordsPieces[index].isSelected != selected)
        {
            m_MixedWordsPieces[index].isSelected = selected;

            if (!isSelectionChanged)
            {
                isSelectionChanged = true;
            }
        }
    }

    if (isSelectionChanged)
    {
        Q_EMIT selectionChanged();
    }
}

const QVector<Game::WordPiece> WordPairOwner::getMixedWordsPieces() const
{
    return m_MixedWordsPieces;
}

QString WordPairOwner::getFirstReferenceWord() const
{
    return m_FirstReferenceWord;
}

QString WordPairOwner::getSecondReferenceWord() const
{
    return m_SecondReferenceWord;
}

bool WordPairOwner::areSynonyms() const
{
    return m_pWordMixer->areSynonyms();
}

void WordPairOwner::_onMixedWordsAvailable()
{
    _buildMixedWordsPieces();

    m_FirstReferenceWord = m_pWordMixer->getFirstWord();
    m_SecondReferenceWord = m_pWordMixer->getSecondWord();
    m_AreSynonyms = m_pWordMixer->areSynonyms();

    Q_EMIT mixedWordsAvailable();
}

void WordPairOwner::_buildMixedWordsPieces()
{
    m_MixedWordsPieces.clear();
    m_MixedWordsPieces.resize(m_pWordMixer->getMixedWordsPiecesContent().size());

    int index{0};

    for (auto& piece : m_MixedWordsPieces)
    {
        piece.content = m_pWordMixer->getMixedWordsPiecesContent().at(index);
        piece.isSelected = false;

        if (index == m_pWordMixer->getFirstWordFirstPieceIndex() || index == m_pWordMixer->getSecondWordFirstPieceIndex())
        {
            piece.pieceType = Game::PieceTypes::BEGIN_PIECE;
        }
        else if (index == m_pWordMixer->getFirstWordLastPieceIndex() || index == m_pWordMixer->getSecondWordLastPieceIndex())
        {
            piece.pieceType = Game::PieceTypes::END_PIECE;
        }
        else
        {
            piece.pieceType = Game::PieceTypes::MIDDLE_PIECE;
        }

        ++index;
    }
}
