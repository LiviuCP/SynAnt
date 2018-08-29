#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "wordmixer.h"
#include "game.h"
#include "gamestrings.h"

static constexpr int c_MinWordSize{5};
static constexpr char c_SynonymsSeparator{'='};
static constexpr char c_AntonymsSeparator{'!'};

WordMixer::WordMixer(const QString &fname, QObject *parent)
    : QObject(parent),
      m_FileName{fname},
      m_RowNumber{-1},
      m_RowContent{},
      m_TotalNrOfRows{0},
      m_WordPieceSize{Game::c_WordPieceSizes[Game::Level::MEDIUM]},
      m_WordsPair{},
      m_MixedWords{},
      m_AreSynonyms{true},
      m_WordsBeginEndPieceIndexes{
                                    {WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE , -1},
                                    {WordsBeginEndPieces::FIRST_WORD_LAST_PIECE  , -1},
                                    {WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE, -1},
                                    {WordsBeginEndPieces::SECOND_WORD_LAST_PIECE , -1}
                                 }
{
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

    int firstWordSize{m_WordsPair.first.size()};
    int secondWordSize{m_WordsPair.second.size()};
    int firstWordNrOfPieces{firstWordSize / m_WordPieceSize + ((firstWordSize % m_WordPieceSize) == 0 ? 0 : 1)};
    int secondWordNrOfPieces{secondWordSize / m_WordPieceSize + ((secondWordSize % m_WordPieceSize) == 0 ? 0 : 1)};
    int totalNrOfPieces{firstWordNrOfPieces + secondWordNrOfPieces};

    m_MixedWords.resize(totalNrOfPieces);

    QVector<int> wordPieceIndexes{};
    for (int pieceIndex{0}; pieceIndex<totalNrOfPieces; ++pieceIndex)
    {
        wordPieceIndexes.append(pieceIndex);
    }

    int firstWordLastPiecePos{m_WordPieceSize*(firstWordNrOfPieces-1)};
    int secondWordLastPiecePos{m_WordPieceSize*(secondWordNrOfPieces-1)};

    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_FIRST_PIECE] = _insertWordPiece(m_WordsPair.first, 0, wordPieceIndexes);
    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<firstWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_WordsPair.first, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::FIRST_WORD_LAST_PIECE] = _insertWordPiece(m_WordsPair.first, firstWordLastPiecePos, wordPieceIndexes);

    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_FIRST_PIECE] = _insertWordPiece(m_WordsPair.second, 0, wordPieceIndexes);
    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<secondWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_WordsPair.second, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPieceIndexes[WordsBeginEndPieces::SECOND_WORD_LAST_PIECE] = _insertWordPiece(m_WordsPair.second, secondWordLastPiecePos, wordPieceIndexes);
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

void WordMixer::_getRowContent()
{
    QFile wordPairsFile(m_FileName);
    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw QString{GameStrings::c_CannotOpenFileMessage};
    }

    QTextStream lineReader{&wordPairsFile};
    for (int currentRowNr{0}; currentRowNr<m_RowNumber; ++currentRowNr)
    {
        (void)lineReader.readLine();
    }
    m_RowContent = lineReader.readLine();

    wordPairsFile.close();
}

void WordMixer::_retrieveWords()
{
    if (!m_ManualWordsEntry)
    {
        _getRowNumber();
        _getRowContent();

        if (m_RowContent.size() == 0)
        {
            throw QString{GameStrings::c_EmptyRowMessage}.arg(m_RowNumber);
        }

        int synonymsSeparatorIndex{m_RowContent.indexOf(c_SynonymsSeparator)};
        int antonymsSeparatorIndex{m_RowContent.indexOf(c_AntonymsSeparator)};
        int separatorIndex{-1};

        if ((synonymsSeparatorIndex != m_RowContent.lastIndexOf(c_SynonymsSeparator)) || (antonymsSeparatorIndex != m_RowContent.lastIndexOf(c_AntonymsSeparator)))
        {
            throw QString{GameStrings::c_MultipleSeparatorsMessage}.arg(m_RowNumber);
        }
        else if (synonymsSeparatorIndex == antonymsSeparatorIndex)
        {
            throw QString{GameStrings::c_NoSeparatorMessage}.arg(m_RowNumber);
        }
        else if ((synonymsSeparatorIndex != -1) && (antonymsSeparatorIndex != -1))
        {
            throw QString{GameStrings::c_MultipleSeparatorsMessage}.arg(m_RowNumber);
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

        m_WordsPair.first = m_RowContent.left(separatorIndex);
        _checkWordIsCorrect(m_WordsPair.first, GameStrings::c_FirstWordCamelCase);

        m_WordsPair.second = m_RowContent.mid(separatorIndex+1);
        _checkWordIsCorrect(m_WordsPair.second, GameStrings::c_SecondWordCamelCase);
    }
}

void WordMixer::_checkWordIsCorrect(const QString &word, const QString& wordIdentifier)
{
    for (auto currentCharacter : word)
    {
        if (!(currentCharacter.isLower()))
        {
            throw QString{GameStrings::c_IllegalCharactersMessage}.arg(wordIdentifier).arg(m_RowNumber);
        }
    }
    if (word.size() < c_MinWordSize)
    {
        throw QString{GameStrings::c_LessThanMinCharsMessage}.arg(wordIdentifier).arg(m_RowNumber);
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
