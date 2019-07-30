#include "wordpairowner.h"
#include "Proxies/wordmixerproxy.h"

void WordPairOwner::onPieceAddedToInput(int wordPieceIndex)
{
    _updateSingleWordPieceStatus(wordPieceIndex, true);
}

void WordPairOwner::onPiecesRemovedFromInput(QVector<int> wordPieceIndexes)
{
    _updateMultipleWordPiecesStatus(wordPieceIndexes, false);
}

void WordPairOwner::onNewMixedWordsAvailable()
{
    Q_ASSERT(m_pWordMixerProxy);

    _buildMixedWordsPiecesArray();

    m_FirstReferenceWord = m_pWordMixerProxy->getFirstWord();
    m_SecondReferenceWord = m_pWordMixerProxy->getSecondWord();
    m_AreSynonyms = m_pWordMixerProxy->areSynonyms();

    Q_EMIT newWordPiecesAvailable();
}

WordPairOwner::WordPairOwner(QObject *parent)
    : QObject{parent}
    , m_pWordMixerProxy{nullptr}
    , m_AreSynonyms{false}
{
}

void WordPairOwner::setWordMixerProxy(WordMixerProxy* pWordMixerProxy)
{
    Q_ASSERT(pWordMixerProxy);

    if (!m_pWordMixerProxy)
    {
        m_pWordMixerProxy = pWordMixerProxy;
    }
}

QVector<QString> WordPairOwner::getMixedWordsPiecesContent() const
{
    QVector<QString> content;

    for (const auto& piece : m_MixedWordsPieces)
    {
        content.append(piece.content);
    }

    return content;
}

QVector<Game::PieceTypes> WordPairOwner::getMixedWordsPiecesTypes() const
{
    QVector<Game::PieceTypes> piecesTypes;

    for (const auto& piece : m_MixedWordsPieces)
    {
        piecesTypes.append(piece.pieceType);
    }

    return piecesTypes;
}

QVector<bool> WordPairOwner::getAreMixedWordsPiecesAddedToInput() const
{
    QVector<bool> areWordPiecesSelected;

    for (const auto& piece : m_MixedWordsPieces)
    {
        areWordPiecesSelected.append(piece.isAddedToInput);
    }

    return areWordPiecesSelected;
}

Game::PieceTypes WordPairOwner::getWordPieceType(int index) const
{
    Q_ASSERT(index>=0 && index<m_MixedWordsPieces.size());
    return m_MixedWordsPieces.at(index).pieceType;
}

bool WordPairOwner::getIsWordPieceAddedToInput(int index) const
{
    Q_ASSERT(index>=0 && index<m_MixedWordsPieces.size());
    return m_MixedWordsPieces.at(index).isAddedToInput;
}

QString WordPairOwner::getFirstReferenceWord() const
{
    return m_FirstReferenceWord;
}

QString WordPairOwner::getSecondReferenceWord() const
{
    return m_SecondReferenceWord;
}

bool WordPairOwner::isOnePieceLeftToAddToInput() const
{
    bool result{false};
    int nrOfPiecesNotSelected{0};

    for (auto piece : m_MixedWordsPieces)
    {
        if (!piece.isAddedToInput)
        {
            ++nrOfPiecesNotSelected;
        }
    }

    if (nrOfPiecesNotSelected == 1)
    {
        result = true;
    }

    return result;
}

bool WordPairOwner::areSynonyms() const
{
    return m_AreSynonyms;
}

void WordPairOwner::_buildMixedWordsPiecesArray()
{
    m_MixedWordsPieces.clear();
    m_MixedWordsPieces.resize(m_pWordMixerProxy->getMixedWordsPiecesContent().size());

    int index{0};

    for (auto& piece : m_MixedWordsPieces)
    {
        piece.content = m_pWordMixerProxy->getMixedWordsPiecesContent().at(index);
        piece.isAddedToInput = false;

        if (index == m_pWordMixerProxy->getFirstWordFirstPieceIndex() || index == m_pWordMixerProxy->getSecondWordFirstPieceIndex())
        {
            piece.pieceType = Game::PieceTypes::BEGIN_PIECE;
        }
        else if (index == m_pWordMixerProxy->getFirstWordLastPieceIndex() || index == m_pWordMixerProxy->getSecondWordLastPieceIndex())
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

void WordPairOwner::_updateSingleWordPieceStatus(int wordPieceIndex, bool selected)
{
    Q_ASSERT(wordPieceIndex >= 0 && wordPieceIndex < m_MixedWordsPieces.size());

    if (m_MixedWordsPieces[wordPieceIndex].isAddedToInput != selected)
    {
        m_MixedWordsPieces[wordPieceIndex].isAddedToInput = selected;
        Q_EMIT piecesAddedToInputChanged();
    }
}

void WordPairOwner::_updateMultipleWordPiecesStatus(QVector<int> wordPieceIndex, bool selected)
{
    bool isSelectionChanged{false};

    for (auto index : wordPieceIndex)
    {
        // investigate this!!!
        Q_ASSERT(index >= 0 && index < m_MixedWordsPieces.size());

        if (m_MixedWordsPieces[index].isAddedToInput != selected)
        {
            m_MixedWordsPieces[index].isAddedToInput = selected;

            if (!isSelectionChanged)
            {
                isSelectionChanged = true;
            }
        }
    }

    if (isSelectionChanged)
    {
        Q_EMIT piecesAddedToInputChanged();
    }
}

WordPairOwner::WordPiece::WordPiece()
{
}
