#include "wordmixer.h"
#include "../Utilities/game.h"
#include "../Utilities/gamestrings.h"
#include "../Utilities/exceptions.h"

WordMixer::WordMixer(QObject *parent)
    : QObject(parent)
    , m_WordPieceSize{Game::c_WordPieceSizes[Game::Level::MEDIUM]}
    , m_WordsPair{}
    , m_MixedWordsPiecesContent{}
    , m_WordsBeginEndPieceIndexes{
                                    {WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE , -1},
                                    {WordsBeginEndPieces::FIRST_WORD_LAST_PIECE  , -1},
                                    {WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE, -1},
                                    {WordsBeginEndPieces::SECOND_WORD_LAST_PIECE , -1}
                                 }
    , m_AreSynonyms{true}
{
    std::random_device rDev2{};
    m_WordPieceIndexEngine.seed(rDev2());
}

void WordMixer::setWordPieceSize(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));
    m_WordPieceSize = Game::c_WordPieceSizes[level];
}

const QVector<QString>& WordMixer::getMixedWordsPiecesContent() const
{
    return m_MixedWordsPiecesContent;
}

QString WordMixer::getFirstWord() const
{
    return m_WordsPair.first;
}

QString WordMixer::getSecondWord() const
{
    return m_WordsPair.second;
}

int WordMixer::getFirstWordFirstPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE];
}

int WordMixer::getFirstWordLastPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_LAST_PIECE];
}

int WordMixer::getSecondWordFirstPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE];
}

int WordMixer::getSecondWordLastPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_LAST_PIECE];
}

bool WordMixer::areSynonyms() const
{
    return m_AreSynonyms;
}

void WordMixer::mixWords(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    Q_ASSERT(newWordsPair.first.size() != 0 && newWordsPair.second.size() != 0);

    auto insertWordPiece = [this](const QString &word, int firstCharPos, QVector<int> &wordPieceIndexes)
    {
        std::uniform_int_distribution<int> indexDist{0,wordPieceIndexes.size()-1};
        int insertOnPositionIndex{indexDist(m_WordPieceIndexEngine)};

        int insertOnPosition{wordPieceIndexes[insertOnPositionIndex]};
        m_MixedWordsPiecesContent[insertOnPosition] = word.mid(firstCharPos,m_WordPieceSize);

        wordPieceIndexes.remove(insertOnPositionIndex);
        return insertOnPosition;
    };

    m_WordsPair = newWordsPair;
    m_AreSynonyms = areSynonyms;

    int firstWordSize{m_WordsPair.first.size()};
    int secondWordSize{m_WordsPair.second.size()};
    int firstWordNrOfPieces{firstWordSize / m_WordPieceSize + ((firstWordSize % m_WordPieceSize) == 0 ? 0 : 1)};
    int secondWordNrOfPieces{secondWordSize / m_WordPieceSize + ((secondWordSize % m_WordPieceSize) == 0 ? 0 : 1)};
    int totalNrOfPieces{firstWordNrOfPieces + secondWordNrOfPieces};

    m_MixedWordsPiecesContent.resize(totalNrOfPieces);

    QVector<int> wordPieceIndexes{};
    for (int pieceIndex{0}; pieceIndex<totalNrOfPieces; ++pieceIndex)
    {
        wordPieceIndexes.append(pieceIndex);
    }

    int firstWordLastPiecePos{m_WordPieceSize*(firstWordNrOfPieces-1)};
    int secondWordLastPiecePos{m_WordPieceSize*(secondWordNrOfPieces-1)};

    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE] = insertWordPiece(m_WordsPair.first, 0, wordPieceIndexes);
    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<firstWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        insertWordPiece(m_WordsPair.first, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_LAST_PIECE] = insertWordPiece(m_WordsPair.first, firstWordLastPiecePos, wordPieceIndexes);

    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE] = insertWordPiece(m_WordsPair.second, 0, wordPieceIndexes);
    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<secondWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        insertWordPiece(m_WordsPair.second, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_LAST_PIECE] = insertWordPiece(m_WordsPair.second, secondWordLastPiecePos, wordPieceIndexes);

    Q_EMIT mixedWordsChanged();
}
