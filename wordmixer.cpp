#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "wordmixer.h"
#include "game.h"
#include "gamestrings.h"

WordMixer::WordMixer(const QString &fname, QObject *parent)
    : QObject(parent),
      m_FileName{fname},
      m_RowNumber{-1},
      m_TotalRows{0},
      m_WordPieceSize{2},
      m_WordsPair{},
      m_MixedWords{},
      m_AreSynonyms{true},
      m_WordsBeginEndPositions{}
{
    int indexesCount{static_cast<int>(WordsBeginEndIndexes::IndexesCount)};
    m_WordsBeginEndPositions.resize(indexesCount);
    for (int index{0}; index<indexesCount; index++)
    {
        m_WordsBeginEndPositions[index] = -1;
    }

    QFileInfo wordPairsFileStatus{m_FileName};
    if (!wordPairsFileStatus.exists())
    {
        throw QString{GameStrings::c_FileNotFoundMessage};
    }

    QFile wordPairsFile{m_FileName};
    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw QString{GameStrings::c_CannotOpenFileMessage};
    }

    QTextStream allRowsReader(&wordPairsFile);
    while (!allRowsReader.atEnd())
    {
        allRowsReader.readLine();
        m_TotalRows++;
    }
    if (m_TotalRows == 0)
    {
        throw QString{GameStrings::c_EmptyFileMessage};
    }
    wordPairsFile.close();

    std::random_device rDev1{};
    m_RowNumberEngine.seed(rDev1());

    std::random_device rDev2{};
    m_IndexEngine.seed(rDev2());
}

void WordMixer::mixWords()
{
    _retrieveWords();

    int firstWordSize{m_WordsPair.first.size()}, secondWordSize{m_WordsPair.second.size()};
    int firstWordNrOfPieces
    {
        firstWordSize / m_WordPieceSize + ((firstWordSize % m_WordPieceSize) == 0 ? 0 : 1)
    };
    int secondWordNrOfPieces
    {
        secondWordSize / m_WordPieceSize + ((secondWordSize % m_WordPieceSize) == 0 ? 0 : 1)
    };
    int totalNrOfPieces{firstWordNrOfPieces + secondWordNrOfPieces};
    m_MixedWords.resize(totalNrOfPieces);
    QVector<int> wordPieceIndexes{};

    wordPieceIndexes.resize(totalNrOfPieces);
    for (int pieceNumber{0}; pieceNumber<totalNrOfPieces; pieceNumber++)
    {
        wordPieceIndexes[pieceNumber] = pieceNumber;
    }
    /* in the first phase a simple operation will be implemented: the pieces of the 2 words will be put in order in the mixedWords vector
       in the next phase this will be replaced with random placements
    */
    int firstWordLastPiecePos{m_WordPieceSize*(firstWordNrOfPieces-1)};
    int secondWordLastPiecePos{m_WordPieceSize*(secondWordNrOfPieces-1)};

    m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::FIRST_WORD_FIRST_PIECE)] = _insertWordPiece(m_WordsPair.first, 0, wordPieceIndexes);

    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<firstWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_WordsPair.first, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::FIRST_WORD_LAST_PIECE)] = _insertWordPiece(m_WordsPair.first, firstWordLastPiecePos, wordPieceIndexes);
    m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::SECOND_WORD_FIRST_PIECE)] = _insertWordPiece(m_WordsPair.second, 0, wordPieceIndexes);

    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<secondWordLastPiecePos;
                                      wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_WordsPair.second, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::SECOND_WORD_LAST_PIECE)] = _insertWordPiece(m_WordsPair.second, secondWordLastPiecePos, wordPieceIndexes);

    qDebug() << "First word:" << m_WordsPair.first;
    qDebug() << "Second word:" << m_WordsPair.second;
    qDebug() << "Mixed words pieces are (in this order): ";
    for (auto piece : m_MixedWords)
    {
        qDebug() << piece;
    }
}

const QVector<QString>& WordMixer::getMixedWordsStringArray() const
{
    return m_MixedWords;
}

QString WordMixer::getFirstWord() const
{
    return m_WordsPair.first;
}

QString WordMixer::getSecondWord() const
{
    return m_WordsPair.second;
}

bool WordMixer::areSynonyms() const
{
    return m_AreSynonyms;
}

int WordMixer::getFirstWordBeginIndex() const
{
    return m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::FIRST_WORD_FIRST_PIECE)];
}

int WordMixer::getFirstWordEndIndex() const
{
    return m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::FIRST_WORD_LAST_PIECE)];
}

int WordMixer::getSecondWordBeginIndex() const
{
    return m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::SECOND_WORD_FIRST_PIECE)];
}

int WordMixer::getSecondWordEndIndex() const
{
    return m_WordsBeginEndPositions[static_cast<int>(WordsBeginEndIndexes::SECOND_WORD_LAST_PIECE)];
}

void WordMixer::setWordPieceSize(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));

    switch(level)
    {
    case Game::Level::EASY:
        m_WordPieceSize = 3;
        break;
    case Game::Level::MEDIUM:
        m_WordPieceSize = 2;
        break;
    case Game::Level::HARD:
        m_WordPieceSize = 1;
    }
}

void WordMixer::_getRowNumber()
{
    std::uniform_int_distribution<int> rowNumberDist{0,m_TotalRows-1};
    m_RowNumber = rowNumberDist(m_RowNumberEngine);
}

void WordMixer::_retrieveWords()
{
    _getRowNumber();
    const int minWordSize{5};
    QString rowNumberToString{};
    rowNumberToString.setNum(m_RowNumber);

    QFile wordPairsFile(m_FileName);
    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    throw QString{GameStrings::c_CannotOpenFileMessage};
    }

    QTextStream lineReader{&wordPairsFile};
    for (int i{0}; i<m_RowNumber; i++)
    {
    QString discardedRow{lineReader.readLine()};
    }

    QString rowContent{lineReader.readLine()};
    wordPairsFile.close();

    if (rowContent.size() == 0)
    {
        throw QString{"\nAn empty row has been read from file!\n\nRow number: " + rowNumberToString + "\n" };
    }

    int synonymsSeparatorIndex{rowContent.indexOf('=')};
    int antonymsSeparatorIndex{rowContent.indexOf('!')};
    int separatorIndex{-1};
    if ((synonymsSeparatorIndex != rowContent.lastIndexOf('=')) || (antonymsSeparatorIndex != rowContent.lastIndexOf('!')))

        {
            throw QString{"Multiple separators!\nRow number: " + rowNumberToString + "\n"};
        }
    else if (synonymsSeparatorIndex == antonymsSeparatorIndex)
    {
        throw QString{"No separator found!\nRow number: " + rowNumberToString + "\n"};
    }
    else if ((synonymsSeparatorIndex != -1) && (antonymsSeparatorIndex != -1))
    {
        throw QString{"Multiple separators!\nRow number: " + rowNumberToString + "\n"};
    }
    else if (synonymsSeparatorIndex != -1)
    {
        m_AreSynonyms = true;
        separatorIndex = synonymsSeparatorIndex;
    }
    else
    {
        m_AreSynonyms = false;
        separatorIndex = antonymsSeparatorIndex;
    }

    m_WordsPair.first = rowContent.left(separatorIndex);
    for (auto currentCharacter : m_WordsPair.first)
    {
        if (!(currentCharacter.isLower()))
        throw QString{"First word contains illegal characters!\nRow number: " + rowNumberToString + "\n"};
    }
    if (m_WordsPair.first.size() < minWordSize)
    {
        throw QString{"First word has less than the minimum required number of characters!\nRow number: " + rowNumberToString + "\n"};
    }
    m_WordsPair.second = rowContent.mid(separatorIndex+1);
    for (auto currentCharacter : m_WordsPair.second)
    {
        if (!(currentCharacter.isLower()))
        throw QString{"Second word contains illegal characters!\nRow number: " + rowNumberToString + "\n"};
    }
    if (m_WordsPair.second.size() < minWordSize)
    {
        throw QString{"Second word has less than the minimum required number of characters!\nRow number: " + rowNumberToString + "\n"};
    }
}

int WordMixer::_insertWordPiece(const QString &word, int firstCharPos,
                                        QVector<int> &wordPieceIndexes)
{
    std::uniform_int_distribution<int> indexDist{0,wordPieceIndexes.size()-1};
    int insertOnPositionIndex{indexDist(m_IndexEngine)};

    int insertOnPosition{wordPieceIndexes[insertOnPositionIndex]};
    m_MixedWords[insertOnPosition] = word.mid(firstCharPos,m_WordPieceSize);

    wordPieceIndexes.remove(insertOnPositionIndex);
    return insertOnPosition;
}
