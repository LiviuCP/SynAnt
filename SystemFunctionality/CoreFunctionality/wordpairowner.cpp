#include "wordpairowner.h"

WordPairOwner::WordPairOwner(QObject *parent)
    : QObject{parent}
    , m_AreSynonyms{false}
    , m_PersistentPieceSelectionIndex{-1}
    , m_NewPairAutoIndexSetupEnabled{false}
{
}

void WordPairOwner::setNewWordsPair(const QVector<QString>& content, const QString& firstWord, const QString& secondWord, bool areSynonyms,
                                    int firstWordFirstPieceIndex, int firstWordLastPieceIndex, int secondWordFirstPieceIndex, int secondWordLastPieceIndex)
{
    _buildMixedWordsPiecesArray(content, firstWordFirstPieceIndex, firstWordLastPieceIndex, secondWordFirstPieceIndex, secondWordLastPieceIndex);

    m_FirstReferenceWord = firstWord;
    m_SecondReferenceWord = secondWord;
    m_AreSynonyms = areSynonyms;

    Q_EMIT newWordsPairSetup();

    if (m_NewPairAutoIndexSetupEnabled)
    {
        clearPersistentPieceSelectionIndex();
        setPersistentPieceSelectionIndex(true);
    }
}

void WordPairOwner::markPieceAsAddedToInput(int wordPieceIndex, bool excludeEndPiecesFromPersistentIndex)
{
    if (m_PersistentPieceSelectionIndex != -1)
    {
        Q_ASSERT(m_PersistentPieceSelectionIndex == wordPieceIndex);
    }

    _updatePersistentPieceSelectionIndex(excludeEndPiecesFromPersistentIndex);
    _updateSingleWordPieceStatus(wordPieceIndex, true);
}

void WordPairOwner::markPiecesAsRemovedFromInput(QVector<int> wordPieceIndexes)
{
    _updateMultipleWordPiecesStatus(wordPieceIndexes, false);
}

void WordPairOwner::enableNewPairAutoIndexSetup(bool enabled)
{
    if (m_NewPairAutoIndexSetupEnabled != enabled)
    {
        m_NewPairAutoIndexSetupEnabled = enabled;
    }
}

void WordPairOwner::setPersistentPieceSelectionIndex(bool isStartPieceRequired, bool excludeEndPiecesFromPersistentIndex)
{
    if (m_PersistentPieceSelectionIndex == -1)
    {
        int firstAvailableIndex{-1};

        for (int pieceIndex{0}; pieceIndex < m_MixedWordsPieces.size(); ++pieceIndex)
        {
            if (excludeEndPiecesFromPersistentIndex && m_MixedWordsPieces[pieceIndex].pieceType == Game::PieceTypes::END_PIECE)
            {
                continue;
            }

            if (!m_MixedWordsPieces[pieceIndex].isAddedToInput)
            {
                firstAvailableIndex = pieceIndex;
                break;
            }
        }

        if (firstAvailableIndex != -1)
        {
            if (!isStartPieceRequired)
            {
                m_PersistentPieceSelectionIndex = firstAvailableIndex;
            }
            else
            {

                for (int pieceIndex{firstAvailableIndex}; pieceIndex < m_MixedWordsPieces.size(); ++pieceIndex)
                {
                    if (!m_MixedWordsPieces[pieceIndex].isAddedToInput && m_MixedWordsPieces[pieceIndex].pieceType == Game::PieceTypes::BEGIN_PIECE)
                    {
                        m_PersistentPieceSelectionIndex = pieceIndex;
                        break;
                    }
                }
            }

            Q_EMIT persistentIndexChanged();
        }
    }
}

void WordPairOwner::clearPersistentPieceSelectionIndex()
{
    if (m_PersistentPieceSelectionIndex != -1)
    {
        m_PersistentPieceSelectionIndex = -1;
        Q_EMIT persistentIndexChanged();
    }
}

void WordPairOwner::increasePersistentPieceSelectionIndex()
{
    if (m_PersistentPieceSelectionIndex != -1)
    {
        bool success{false};

        for (int index{m_PersistentPieceSelectionIndex+1}; index < m_MixedWordsPieces.size(); ++index)
        {
            if (!m_MixedWordsPieces[index].isAddedToInput)
            {
                m_PersistentPieceSelectionIndex = index;
                success = true;
                break;
            }
        }

        if (!success)
        {
            for (int index{0}; index < m_PersistentPieceSelectionIndex; ++index)
            {
                if (!m_MixedWordsPieces[index].isAddedToInput)
                {
                    m_PersistentPieceSelectionIndex = index;
                    success = true;
                    break;
                }
            }

            if (success)
            {
                Q_EMIT persistentIndexChanged();
            }
        }
        else
        {
            Q_EMIT persistentIndexChanged();
        }
    }
}

void WordPairOwner::decreasePersistentPieceSelectionIndex()
{
    if (m_PersistentPieceSelectionIndex != -1)
    {
        bool success{false};

        for (int index{m_PersistentPieceSelectionIndex-1}; index >= 0; --index)
        {
            if (!m_MixedWordsPieces[index].isAddedToInput)
            {
                m_PersistentPieceSelectionIndex = index;
                success = true;
                break;
            }
        }

        if (!success)
        {
            for (int index{m_MixedWordsPieces.size()-1}; index > m_PersistentPieceSelectionIndex; --index)
            {
                if (!m_MixedWordsPieces[index].isAddedToInput)
                {
                    m_PersistentPieceSelectionIndex = index;
                    success = true;
                    break;
                }
            }

            if (success)
            {
                Q_EMIT persistentIndexChanged();
            }
        }
        else
        {
            Q_EMIT persistentIndexChanged();
        }
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

int WordPairOwner::getPersistentPieceSelectionIndex() const
{
    return m_PersistentPieceSelectionIndex;
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

void WordPairOwner::_buildMixedWordsPiecesArray(const QVector<QString>& content, int firstBeginIndex, int firstEndIndex, int secondBeginIndex, int secondEndIndex)
{
    m_MixedWordsPieces.clear();
    m_MixedWordsPieces.resize(content.size());

    int index{0};

    for (auto& piece : m_MixedWordsPieces)
    {
        piece.content = content.at(index);
        piece.isAddedToInput = false;

        if (index == firstBeginIndex || index == secondBeginIndex)
        {
            piece.pieceType = Game::PieceTypes::BEGIN_PIECE;
        }
        else if (index == firstEndIndex || index == secondEndIndex)
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
        Q_EMIT piecesAddedToInputStateChanged();
    }
}

void WordPairOwner::_updateMultipleWordPiecesStatus(QVector<int> wordPieceIndex, bool selected)
{
    bool isSelectionChanged{false};

    for (auto index : wordPieceIndex)
    {
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
        Q_EMIT piecesAddedToInputStateChanged();
    }
}

void WordPairOwner::_updatePersistentPieceSelectionIndex(bool excludeEndPiecesFromPersistentIndex)
{
    if (m_PersistentPieceSelectionIndex != -1)
    {
        bool success{false};

        for (int index{m_PersistentPieceSelectionIndex+1}; index < m_MixedWordsPieces.size(); ++index)
        {
            if (excludeEndPiecesFromPersistentIndex && m_MixedWordsPieces[index].pieceType == Game::PieceTypes::END_PIECE)
            {
                continue;
            }

            if (!m_MixedWordsPieces[index].isAddedToInput)
            {
                m_PersistentPieceSelectionIndex = index;
                success = true;
                break;
            }
        }

        if (!success)
        {
            for (int index{m_PersistentPieceSelectionIndex-1}; index >= 0; --index)
            {
                if (excludeEndPiecesFromPersistentIndex && m_MixedWordsPieces[index].pieceType == Game::PieceTypes::END_PIECE)
                {
                    continue;
                }

                if (!m_MixedWordsPieces[index].isAddedToInput)
                {
                    m_PersistentPieceSelectionIndex = index;
                    success = true;
                    break;
                }
            }

            if (!success)
            {
                m_PersistentPieceSelectionIndex = -1;
            }
        }

        // the index will change no matter if another unselected item is available or not
        Q_EMIT persistentIndexChanged();
    }
}

WordPairOwner::WordPiece::WordPiece()
{
}
