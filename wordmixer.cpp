#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QtGlobal>
#include "wordmixer.h"
#include "gamestrings.h"

WordMixer::WordMixer(QObject *parent, const QString &fname)
    : QObject(parent),
      m_FileName{fname},
      m_RowNumber{-1},
      m_TotalRows{0},
      m_WordPieceSize{2},
      m_FirstWord{},
      m_SecondWord{},
      m_MixedWords{},
      m_AreSynonyms{true},
      m_StatusMessage{GameStrings::c_NoUserInputMessage},
      m_WordsBeginEndPositions{}
{
    m_WordsBeginEndPositions.resize(IndexesCount);
    for (int index{0}; index<IndexesCount; index++)
    {
        m_WordsBeginEndPositions[index] = -1;
    }

    // template messages used for creating the final error & success messages
    m_StatusTexts.resize(StatusCodesCount);
    m_StatusTexts[SUCCESS]         = GameStrings::c_SuccessMessage;
    m_StatusTexts[MISSING_WORDS]   = GameStrings::c_MissingWordsMessage;
    m_StatusTexts[INCORRECT_WORDS] = GameStrings::c_IncorrectWordsMessage;

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

    int firstWordSize{m_FirstWord.size()}, secondWordSize{m_SecondWord.size()};
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

    m_WordsBeginEndPositions[FIRST_BEGIN] = _insertWordPiece(m_FirstWord, 0, wordPieceIndexes);

    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<firstWordLastPiecePos; wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_FirstWord, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPositions[FIRST_END] = _insertWordPiece(m_FirstWord, firstWordLastPiecePos, wordPieceIndexes);
    m_WordsBeginEndPositions[SECOND_BEGIN] = _insertWordPiece(m_SecondWord, 0, wordPieceIndexes);

    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<secondWordLastPiecePos;
                                      wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_SecondWord, wordPieceStartPos, wordPieceIndexes);
    }
    m_WordsBeginEndPositions[SECOND_END] = _insertWordPiece(m_SecondWord, secondWordLastPiecePos, wordPieceIndexes);
}

bool WordMixer::checkWords(const QString &firstWord, const QString &secondWord)
{
    enum ReturnValue
    {
        INCORRECT_USER_INPUT,
        CORRECT_USER_INPUT
    };

    if (firstWord.isEmpty() || secondWord.isEmpty())
    {
        _createErrorMessage(MISSING_WORDS);
        return INCORRECT_USER_INPUT;
    }

    if (((firstWord == this -> m_FirstWord) && (secondWord == this -> m_SecondWord)) || ((firstWord == this -> m_SecondWord) && (secondWord == this -> m_FirstWord)))
    {
        _createSuccessMessage();
        return CORRECT_USER_INPUT;
    }
    _createErrorMessage(INCORRECT_WORDS);
    return INCORRECT_USER_INPUT;
}

const QVector<QString>& WordMixer::getMixedWordsStringArray() const
{
    return m_MixedWords;
}

void WordMixer::retrieveResults()
{
    m_StatusMessage = "The correct words are: \n\n";
    m_StatusMessage += "\t" + m_FirstWord + "\n";
    m_StatusMessage += "\t" + m_SecondWord + "\n";
    m_StatusMessage += "\nThe words are: \n\n\t";
    if (m_AreSynonyms)
    {
        m_StatusMessage += "synonyms";
    }
    else
    {
        m_StatusMessage += "antonyms";
    }
    m_StatusMessage += "\n\nNext pair of words is available below.";
}

const QString& WordMixer::getStatusMessage() const
{
    return m_StatusMessage;
}

int WordMixer::getFirstWordBeginIndex() const
{
    return m_WordsBeginEndPositions[FIRST_BEGIN];
}

int WordMixer::getFirstWordEndIndex() const
{
    return m_WordsBeginEndPositions[FIRST_END];
}

int WordMixer::getSecondWordBeginIndex() const
{
    return m_WordsBeginEndPositions[SECOND_BEGIN];
}

int WordMixer::getSecondWordEndIndex() const
{
    return m_WordsBeginEndPositions[SECOND_END];
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

    m_FirstWord = rowContent.left(separatorIndex);
    for (auto currentCharacter : m_FirstWord)
    {
        if (!(currentCharacter.isLower()))
        throw QString{"First word contains illegal characters!\nRow number: " + rowNumberToString + "\n"};
    }
    if (m_FirstWord.size() < minWordSize)
    {
        throw QString{"First word has less than the minimum required number of characters!\nRow number: " + rowNumberToString + "\n"};
    }
    m_SecondWord = rowContent.mid(separatorIndex+1);
    for (auto currentCharacter : m_SecondWord)
    {
        if (!(currentCharacter.isLower()))
        throw QString{"Second word contains illegal characters!\nRow number: " + rowNumberToString + "\n"};
    }
    if (m_SecondWord.size() < minWordSize)
    {
        throw QString{"Second word has less than the minimum required number of characters!\nRow number: " + rowNumberToString + "\n"};
    }
}

void WordMixer::_createErrorMessage(const int errorCode)
{
    switch(errorCode)
    {
    case MISSING_WORDS:
        m_StatusMessage = m_StatusTexts[MISSING_WORDS];
        break;
    case INCORRECT_WORDS:
        m_StatusMessage = m_StatusTexts[INCORRECT_WORDS];
        break;
    // reserved for future use
    default:
        ;
    }
}

void WordMixer::_createSuccessMessage()
{
    m_StatusMessage = m_StatusTexts[SUCCESS];
    m_StatusMessage += "\n\nThe two words are:\n\n";
    m_StatusMessage += "\t" + m_FirstWord + "\n";
    m_StatusMessage += "\t" + m_SecondWord + "\n";
    m_StatusMessage += "\nThe words are: \n\n\t";
    if (m_AreSynonyms)
    {
        m_StatusMessage += "synonyms";
    }
    else
    {
        m_StatusMessage += "antonyms";
    }
    m_StatusMessage += "\n\nNext pair of words is available below.";
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
