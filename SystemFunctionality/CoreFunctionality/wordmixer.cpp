#include "wordmixer.h"
#include "../Utilities/gameutils.h"
#include "../Utilities/exceptions.h"

const QMap<Game::Levels, int> c_WordPieceSizes
{
    {Game::Levels::LEVEL_EASY,   3},
    {Game::Levels::LEVEL_MEDIUM, 2},
    {Game::Levels::LEVEL_HARD,   1}
};

WordMixer::WordMixer(QObject *parent)
    : QObject(parent)
    , m_GameLevel{Game::Levels::LEVEL_NONE}
    , m_WordsPair{}
    , m_MixedWordsPiecesContent{}
    , m_WordPieceSizes{c_WordPieceSizes}
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

void WordMixer::mixWords(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    Q_ASSERT(m_GameLevel != Game::Levels::LEVEL_NONE && m_WordPieceSizes.contains(m_GameLevel));
    Q_ASSERT(newWordsPair.first.size() > m_WordPieceSizes[m_GameLevel] && newWordsPair.second.size() > m_WordPieceSizes[m_GameLevel] && m_WordPieceSizes[m_GameLevel] > 0);

    auto insertWordPiece = [this](const QString &word, int firstCharPos, QVector<int> &wordPieceIndexes)
    {
        std::uniform_int_distribution<int> indexDist{0,wordPieceIndexes.size()-1};
        int insertOnPositionIndex{indexDist(m_WordPieceIndexEngine)};

        int insertOnPosition{wordPieceIndexes[insertOnPositionIndex]};
        m_MixedWordsPiecesContent[insertOnPosition] = word.mid(firstCharPos, m_WordPieceSizes[m_GameLevel]);

        wordPieceIndexes.remove(insertOnPositionIndex);
        return insertOnPosition;
    };

    m_WordsPair = newWordsPair;
    m_AreSynonyms = areSynonyms;

    int firstWordSize{m_WordsPair.first.size()};
    int secondWordSize{m_WordsPair.second.size()};
    int firstWordNrOfPieces{firstWordSize / m_WordPieceSizes[m_GameLevel] + ((firstWordSize % m_WordPieceSizes[m_GameLevel]) == 0 ? 0 : 1)};
    int secondWordNrOfPieces{secondWordSize / m_WordPieceSizes[m_GameLevel] + ((secondWordSize % m_WordPieceSizes[m_GameLevel]) == 0 ? 0 : 1)};
    int totalNrOfPieces{firstWordNrOfPieces + secondWordNrOfPieces};

    m_MixedWordsPiecesContent.resize(totalNrOfPieces);

    QVector<int> wordPieceIndexes{};
    for (int pieceIndex{0}; pieceIndex<totalNrOfPieces; ++pieceIndex)
    {
        wordPieceIndexes.append(pieceIndex);
    }

    int firstWordLastPiecePos{m_WordPieceSizes[m_GameLevel]*(firstWordNrOfPieces-1)};
    int secondWordLastPiecePos{m_WordPieceSizes[m_GameLevel]*(secondWordNrOfPieces-1)};

    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE] = insertWordPiece(m_WordsPair.first, 0, wordPieceIndexes);
    for (int wordPieceStartPos{m_WordPieceSizes[m_GameLevel]}; wordPieceStartPos<firstWordLastPiecePos; wordPieceStartPos+=m_WordPieceSizes[m_GameLevel])
    {
        insertWordPiece(m_WordsPair.first, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_LAST_PIECE] = insertWordPiece(m_WordsPair.first, firstWordLastPiecePos, wordPieceIndexes);

    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE] = insertWordPiece(m_WordsPair.second, 0, wordPieceIndexes);
    for (int wordPieceStartPos{m_WordPieceSizes[m_GameLevel]}; wordPieceStartPos<secondWordLastPiecePos; wordPieceStartPos+=m_WordPieceSizes[m_GameLevel])
    {
        insertWordPiece(m_WordsPair.second, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_LAST_PIECE] = insertWordPiece(m_WordsPair.second, secondWordLastPiecePos, wordPieceIndexes);

    Q_EMIT newWordsPairMixed();
}

void WordMixer::setGameLevel(Game::Levels level)
{
    if (level != Game::Levels::LEVEL_NONE)
    {
        m_GameLevel = level;
    }
}

void WordMixer::setPieceSizeForLevel(int size, Game::Levels level)
{
    if (size > 0 && level != Game::Levels::LEVEL_NONE)
    {
        m_WordPieceSizes[level] = size;
    }
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

int WordMixer::getPieceSizeForLevel(Game::Levels level) const
{
    return m_WordPieceSizes.contains(level) ? m_WordPieceSizes[level] : -1;
}

Game::Levels WordMixer::getGameLevel() const
{
    return m_GameLevel;
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
