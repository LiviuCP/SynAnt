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
      m_TotalNrOfRows{0},
      m_WordPieceSize{Game::c_WordPieceSizes[Game::Level::MEDIUM]},
      m_WordsPair{},
      m_MixedWords{},
      m_AreSynonyms{true},
      m_WordsBeginEndPieceIndexes{}
{
    int indexesCount{static_cast<int>(WordsBeginEndPieces::PiecesCount)};
    m_WordsBeginEndPieceIndexes.resize(indexesCount);
    for (int index{0}; index<indexesCount; index++)
    {
        m_WordsBeginEndPieceIndexes[index] = -1;
    }

    std::random_device rDev1{};
    m_RowNumberEngine.seed(rDev1());

    std::random_device rDev2{};
    m_WordPieceIndexEngine.seed(rDev2());

    // if an empty filename is given assume unit testing mode
    if (fname == GameStrings::c_NoFile)
    {
        m_ManualWordsEntry = true;
    }
    else
    {
        m_ManualWordsEntry = false;

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
            m_TotalNrOfRows++;
        }
        if (m_TotalNrOfRows == 0)
        {
            throw QString{GameStrings::c_EmptyFileMessage};
        }
        wordPairsFile.close();
    }
}

void WordMixer::mixWords()
{
    _retrieveWords();

    Q_ASSERT(m_WordsPair.second.size() != 0 && m_WordsPair.first.size() != 0);

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

    m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE)] = _insertWordPiece(m_WordsPair.first, 0, wordPieceIndexes);

    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<firstWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_WordsPair.first, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::FIRST_WORD_LAST_PIECE)] = _insertWordPiece(m_WordsPair.first, firstWordLastPiecePos, wordPieceIndexes);
    m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE)] = _insertWordPiece(m_WordsPair.second, 0, wordPieceIndexes);

    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<secondWordLastPiecePos;
                                      wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_WordsPair.second, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::SECOND_WORD_LAST_PIECE)] = _insertWordPiece(m_WordsPair.second, secondWordLastPiecePos, wordPieceIndexes);

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

void WordMixer::setFirstWord(const QString &firstWord)
{
    m_WordsPair.first = firstWord;
}

void WordMixer::setSecondWord(const QString &secondWord)
{
    m_WordsPair.second = secondWord;
}

bool WordMixer::areSynonyms() const
{
    return m_AreSynonyms;
}

int WordMixer::getFirstWordFirstPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE)];
}

int WordMixer::getFirstWordLastPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::FIRST_WORD_LAST_PIECE)];
}

int WordMixer::getSecondWordFirstPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE)];
}

int WordMixer::getSecondWordLastPieceIndex() const
{
    return m_WordsBeginEndPieceIndexes[static_cast<int>(WordsBeginEndPieces::SECOND_WORD_LAST_PIECE)];
}

void WordMixer::setWordPieceSize(Game::Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Level::NrOfLevels));
    m_WordPieceSize = Game::c_WordPieceSizes[level];
}

void WordMixer::_getRowNumber()
{
    std::uniform_int_distribution<int> rowNumberDist{0,m_TotalNrOfRows-1};
    m_RowNumber = rowNumberDist(m_RowNumberEngine);
}

void WordMixer::_retrieveWords()
{
    if (!m_ManualWordsEntry)
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
            throw QString{GameStrings::c_EmptyRowMessage}.arg(rowNumberToString);
        }

        int synonymsSeparatorIndex{rowContent.indexOf('=')};
        int antonymsSeparatorIndex{rowContent.indexOf('!')};
        int separatorIndex{-1};
        if ((synonymsSeparatorIndex != rowContent.lastIndexOf('=')) || (antonymsSeparatorIndex != rowContent.lastIndexOf('!')))
        {
            throw QString{GameStrings::c_MultipleSeparatorsMessage}.arg(rowNumberToString);
        }
        else if (synonymsSeparatorIndex == antonymsSeparatorIndex)
        {
            throw QString{GameStrings::c_NoSeparatorMessage}.arg(rowNumberToString);
        }
        else if ((synonymsSeparatorIndex != -1) && (antonymsSeparatorIndex != -1))
        {
            throw QString{GameStrings::c_MultipleSeparatorsMessage}.arg(rowNumberToString);
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
            {
                throw QString{GameStrings::c_IllegalCharactersMessage}.arg(GameStrings::c_FirstWordCamelCase).arg(rowNumberToString);
            }
        }
        if (m_WordsPair.first.size() < minWordSize)
        {
            throw QString{GameStrings::c_LessThanMinCharsMessage}.arg(GameStrings::c_FirstWordCamelCase).arg(rowNumberToString);
        }
        m_WordsPair.second = rowContent.mid(separatorIndex+1);
        for (auto currentCharacter : m_WordsPair.second)
        {
            if (!(currentCharacter.isLower()))
            {
                throw QString{GameStrings::c_IllegalCharactersMessage}.arg(GameStrings::c_SecondWordCamelCase).arg(rowNumberToString);
            }
        }
        if (m_WordsPair.second.size() < minWordSize)
        {
            throw QString{GameStrings::c_LessThanMinCharsMessage}.arg(GameStrings::c_SecondWordCamelCase).arg(rowNumberToString);
        }
    }
}

int WordMixer::_insertWordPiece(const QString &word, int firstCharPos,
                                        QVector<int> &wordPieceIndexes)
{
    std::uniform_int_distribution<int> indexDist{0,wordPieceIndexes.size()-1};
    int insertOnPositionIndex{indexDist(m_WordPieceIndexEngine)};

    int insertOnPosition{wordPieceIndexes[insertOnPositionIndex]};
    m_MixedWords[insertOnPosition] = word.mid(firstCharPos,m_WordPieceSize);

    wordPieceIndexes.remove(insertOnPositionIndex);
    return insertOnPosition;
}
