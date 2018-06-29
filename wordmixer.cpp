#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QtGlobal>
#include "wordmixer.h"

                                                                                                    // constructor
WordMixer::WordMixer(const QString &fname)
    : m_FileName{fname},                                                                              // absolute path of the file which contains the mixed words
      m_RowNumber{-1},                                                                                // will be assigned a new (random) value when generating the number of the row which will be read from file by calling getRowNumber()
      m_TotalRows{0},                                                                                 // will be updated by constructor when reading all rows from the file
      m_WordPieceSize{2},                                                                             // default size of the pieces in which the word is divided
      m_FirstWord{},                                                                                  // initialized as empty string, will be assigned a value when the words are retrieved from file by calling retrieveWords()
      m_SecondWord{},                                                                                 // same for the second word
      m_MixedWords{},                                                                                 // initially empty, will be filled in when the words are mixed by calling mixWords()
      m_AreSynonyms{true},                                                                            // by default the words are synonyms (this is just for having the variable initialized)
      statusMessage{"No user input so far."},                                                       // contains the message to be displayed in the errors/results label of the main game window
      wordsBeginEndPositions{},                                                                     // also initialized empty, will be filled in when the positions of the first/last piece of the each word are determined (in the words mixing process: mixWords() )
      m_ObtainedScore{0},                                                                             // all statistics (scores, number of pairs) initially set to 0 (this is reflected in the scores and number of pairs texts too)
      m_totalAvailableScore{0},
      m_GuessedWordPairs{0},
      m_TotalWordPairs{0},
      m_ScoreIncrement{2},                                                                            // default level is medium, the number by which the scores are increased coresponds to this level
      m_HighScoresMessage{"High-score: 0/0"},
      m_NrOfPairsMessage{"Word pairs: 0/0"}
{
    wordsBeginEndPositions.resize(IndexesCount);
    for (int index{0}; index<IndexesCount; index++)                                                 // default indexes for the 4 word pieces in the mixedWords vector will be -1;
    {
        wordsBeginEndPositions[index] = -1;                                                         // they will be changed when mixWords() is called
    }
    statusTexts.resize(StatusCodesCount);                                               // allocate the required number of strings for the success message (first element) and error messages templates
    statusTexts[SUCCESS] = "Congrats! You entered the correct words.";                        // template messages used for creating the final error & success messages
    statusTexts[MISSING_WORDS] =
                       "At least one of the words hasn't been entered.\n\nPlease try again!";
    statusTexts[INCORRECT_WORDS] =
                                  "You didn't enter the correct words.\n\nPlease try again!";

    QFileInfo wordPairsFileStatus{m_FileName};                                                        // check if the file containing the word pairs exists, if not throw an exception that will end the application
    if (!wordPairsFileStatus.exists())
    {
        throw QString{"File not found!"};
    }
                                                                                                    // open the file; throw fatal exception (program ends) if file cannot be opened
    QFile wordPairsFile{m_FileName};
    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw QString{"File cannot be opened!"};
    }
                                                                                                    // determine the number of rows from the file; throw fatal exception (program ends) if the file is empty
    QTextStream allRowsReader(&wordPairsFile);
    while (!allRowsReader.atEnd())
    {
        allRowsReader.readLine();                                                                   // discard the returned value of readLine(); line is read only for advancing in file and getting the number of rows
        m_TotalRows++;
    }
    if (m_TotalRows == 0)
    {
        throw QString{"Empty file!"};
    }
    wordPairsFile.close();
                                                                                                    // seed the engine that is involved in generating a random row number
    std::random_device rDev1{};
    m_RowNumberEngine.seed(rDev1());
                                                                                                    // seed the engine that is involved in determinining the index of the mixed words string where the current word piece is written
    std::random_device rDev2{};
    m_IndexEngine.seed(rDev2());
}
                                                                                                    /* this function mixes a pair of words into a single one
                                                                                                       first the words are divided in equal pieces (except the end of the word); then all pieces are mixed into a single word
                                                                                                    */
void WordMixer::mixWords()
{
    _retrieveWords();                                                                                // read a new pair of words from the file
                                                                                                    // then retrieve the number of pieces each word consists of
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
    QVector<int> wordPieceIndexes{};                                                                // contains the indexes of the positions of mixedWords that still need to be filled
                                                                                                    // each time a position is filled the element with the its index is removed from wordPieceIndexes (vector is resized)
    wordPieceIndexes.resize(totalNrOfPieces);
    for (int pieceNumber{0}; pieceNumber<totalNrOfPieces; pieceNumber++)
    {
        wordPieceIndexes[pieceNumber] = pieceNumber;                                                // initial value of each element is equal to its index (it's the index of the coresponding element in mixedWords)
    }
                                                                                                    /* in the first phase a simple operation will be implemented:
                                                                                                       the pieces of the 2 words will be put in order in the mixedWords vector
                                                                                                       in the next phase this will be replaced with random placements
                                                                                                       in any case an element from wordPieceIndexes will be removed for every placement operation
                                                                                                    */
    int firstWordLastPiecePos{m_WordPieceSize*(firstWordNrOfPieces-1)};                               // substring start position of the last piece of the first word
    int secondWordLastPiecePos{m_WordPieceSize*(secondWordNrOfPieces-1)};                             // substring start position of the last piece of the second word
                                                                                                    // start inserting the pieces of the first word
    wordsBeginEndPositions[FIRST_BEGIN] =                                                           // insert the first piece of the first word and store the position index for further use
        _insertWordPiece(m_FirstWord, 0, wordPieceIndexes);
                                                                                                    // then continue inserting the other pieces of the first word until the last one is reached
    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<firstWordLastPiecePos;
                                      wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_FirstWord, wordPieceStartPos, wordPieceIndexes);
    }
    wordsBeginEndPositions[FIRST_END] =                                                             // insert the last piece of the first word and store the position index for further use
                    _insertWordPiece(m_FirstWord, firstWordLastPiecePos, wordPieceIndexes);
    wordsBeginEndPositions[SECOND_BEGIN] =                                                          // insert the first piece of the second word and store the position index for further use
                    _insertWordPiece(m_SecondWord, 0, wordPieceIndexes);
                                                                                                    // then continue inserting the pieces of the second word until the last one is reached
    for (int wordPieceStartPos{m_WordPieceSize}; wordPieceStartPos<secondWordLastPiecePos;
                                      wordPieceStartPos+=m_WordPieceSize)
    {
        _insertWordPiece(m_SecondWord, wordPieceStartPos, wordPieceIndexes);
    }
    wordsBeginEndPositions[SECOND_END] =                                                            // insert the last piece of the second word and store the position index for further use
                  _insertWordPiece(m_SecondWord, secondWordLastPiecePos, wordPieceIndexes);

}
                                                                                                    /* This function checks the words entered by user
                                                                                                       It will deliver either:
                                                                                                       1) an error message (if words are not (correctly) entered)
                                                                                                       2) or a success message and a new pair of words (if both words were correctly entered)
                                                                                                    */
bool WordMixer::checkWords(const QString &firstWord, const QString &secondWord)
{
    enum ReturnValue                                                                               // enum for the bool return values of the function
    {
        INCORRECT_USER_INPUT,                                                                      // true (both words entered correctly by user
        CORRECT_USER_INPUT                                                                         // false (user didn't enter the correct strings or at least one string hasn't been entered at all)
    };
                                                                                                   // first check if any of the words hasn't been entered by user
    if (firstWord.isEmpty() || secondWord.isEmpty())
    {
        _createErrorMessage(MISSING_WORDS);
        return INCORRECT_USER_INPUT;
    }
                                                                                                   // second, check if both words were correctly entered
    if (((firstWord == this -> m_FirstWord) && (secondWord == this -> m_SecondWord)) ||                // case 1: words entered correctly and in order
        ((firstWord == this -> m_SecondWord) && (secondWord == this -> m_FirstWord)))                  // case 2: words entered correctly and in reverse order
    {
        _createSuccessMessage();
        return CORRECT_USER_INPUT;
    }
    _createErrorMessage(INCORRECT_WORDS);                                                           // any other case: error message, incorrect input
    return INCORRECT_USER_INPUT;
}
                                                                                                   // this function returns a reference to the mixed words array so it can be used by the main game window for creating the labels with the mixed pair of words
const QVector<QString>& WordMixer::getMixedWordsStringArray() const
{
    return m_MixedWords;
}
                                                                                                   // This function is called by the slot of the Show results button for requesting the message with the correct solution
void WordMixer::retrieveResults()
{                                                                                                  // shared variable statusMessage is used as results message (when the user pushes the Show results button)
    statusMessage = "The correct words are: \n\n";
    statusMessage += "\t" + m_FirstWord + "\n";
    statusMessage += "\t" + m_SecondWord + "\n";
    statusMessage += "\nThe words are: \n\n\t";
    if (m_AreSynonyms)
    {
        statusMessage += "synonyms";
    }
    else
    {
        statusMessage += "antonyms";
    }
    statusMessage += "\n\nNext pair of words is available below.";
}
                                                                                                   // this function is used by main window to update the results/errors section (use cases documented above, see variable statusText)
const QString& WordMixer::getStatusMessage() const
{
    return statusMessage;
}

                                                                                                   // this function is used by main window to update the high-scores label
const QString& WordMixer::getHighScoresMessage() const
{
    return m_HighScoresMessage;
}
                                                                                                   // this function is used by main window to update the number of word pairs label
const QString& WordMixer::getNrOfPairsMessage() const
{
    return m_NrOfPairsMessage;
}
                                                                                                   // this function is used by main window to locate the position of the first piece of the first word in the mixedWords vector
int WordMixer::getFirstWordBeginIndex() const
{
    return wordsBeginEndPositions[FIRST_BEGIN];
}
                                                                                                   // this function is used by main window to locate the position of the last piece of the first word in the mixedWords vector
int WordMixer::getFirstWordEndIndex() const
{
    return wordsBeginEndPositions[FIRST_END];
}
                                                                                                   // this function is used by main window to locate the position of the first piece of the second word in the mixedWords vector
int WordMixer::getSecondWordBeginIndex() const
{
    return wordsBeginEndPositions[SECOND_BEGIN];
}
                                                                                                   // this function is used by main window to locate the position of the last piece of the second word in the mixedWords vector
int WordMixer::getSecondWordEndIndex() const
{
    return wordsBeginEndPositions[SECOND_END];
}
                                                                                                   // this function is used for changing the level parameters to specific values when user changes level
void WordMixer::setLevel(Level level)
{
    statusMessage = "\nLevel changed.\n\nNo user input so far.\n";                                 // message to be displayed in the errors/results box of the main window when level is changed

    setWordPieceSize(level);
    setScoreIncrement(level);
}
                                                                                                   // used for setting level params
void WordMixer::setWordPieceSize(Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Level::NrOfLevels));

    switch(level)
    {
    case Level::EASY:
        m_WordPieceSize = 3;
        break;
    case Level::MEDIUM:
        m_WordPieceSize = 2;
        break;
    case Level::HARD:
        m_WordPieceSize = 1;
    }
}
                                                                                                   // used for setting level params
void WordMixer::setScoreIncrement(Level level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Level::NrOfLevels));

    switch(level)
    {
    case Level::EASY:
        m_ScoreIncrement = 1;
        break;
    case Level::MEDIUM:
        m_ScoreIncrement = 2;
        break;
    case Level::HARD:
        m_ScoreIncrement = 4;
    }
}
                                                                                                   /* this function is used for updating the high-score and number of word pairs variables
                                                                                                      it also triggers updating of the high-score and number of word pairs texts which are displayed in the main game window
                                                                                                      update can be partial (only the total score and the total number of pairs) or full (obtained score and correctly guessed
                                                                                                      number of word pairs are updated too)
                                                                                                   */
void WordMixer::updateStatistics(const bool partialUpdate)
{
    if (!partialUpdate)                                                                            // if user enters the words correctly and presses Submit, the obtained points and the number of guessed word pairs gets updated
    {
        m_ObtainedScore += m_ScoreIncrement;
        m_GuessedWordPairs++;
    }
    m_totalAvailableScore += m_ScoreIncrement;                                                         // total available number of points and total number of pairs presented to the user will be updated
    m_TotalWordPairs++;                                                                              // anytime the user enters the words correctly (and pressses Submit) or he presses Show results button
    _createHighScoresMessage();                                                                     // also update the texts to be written in the high-scores and number of pairs labels of the main game window
    _createNrOfPairsMessage();
}
                                                                                                   // this function is used for resetting all statistics (scores, number of pairs) variables to 0 when the user presses the Reset button
void WordMixer::resetStatistics()
{
    m_ObtainedScore = 0;
    m_totalAvailableScore = 0;
    m_GuessedWordPairs = 0;
    m_TotalWordPairs = 0;
    _createHighScoresMessage();                                                                     // after reset, texts are recalculated to reflect the new values in the scores and number of pairs labels
    _createNrOfPairsMessage();
    statusMessage = "\nScores reset.\n\nLevel unchanged.\n\nNo user input so far.\n";              // message to be written in the results/errors box in main game window
}
                                                                                                   /* this function generates a random number which has a value between 0 and the maximum number of rows of the file
                                                                                                      the row with this number will then be read from the file by the retrieveWords() function
                                                                                                      in the testing phase the number of the first line will be returned
                                                                                                      then the function will create and return a random number
                                                                                                   */
void WordMixer::_getRowNumber()
{
    std::uniform_int_distribution<int> rowNumberDist{0,m_TotalRows-1};                               // random number will be between 0 (first row) and the total number of rows minus 1 (last row)
    m_RowNumber = rowNumberDist(m_RowNumberEngine);
}
                                                                                                   /* This function reads a row from the file and splits it into 2 words based on a chosen separator
                                                                                                      Any (fatal) exceptions will be sent to the calling function (mixWords()), which will handle them (stack unwinding)
                                                                                                   */
void WordMixer::_retrieveWords()
{
    _getRowNumber();                                                                                // get the number of the file row from which the words will be read
    const int minWordSize{5};                                                                      // minimum allowed number of characters in a word
    QString rowNumberToString{};
    rowNumberToString.setNum(m_RowNumber);                                                           // convert the row number to a string for passing it when throwing exceptions
                                                                                                   // open the file; throw fatal exception (program ends) if file cannot be opened
    QFile wordPairsFile(m_FileName);
    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    throw QString{"File cannot be opened!"};
    }
                                                                                                   // read all lines before the one that is required; discard them
    QTextStream lineReader{&wordPairsFile};
    for (int i{0}; i<m_RowNumber; i++)
    {
    QString discardedRow{lineReader.readLine()};
    }
                                                                                                   // read the required line from the file
    QString rowContent{lineReader.readLine()};
    wordPairsFile.close();
                                                                                                   // check if the read row is empty, if yes throw fatal error (program will end)
    if (rowContent.size() == 0)
    {
    throw QString{"\nAn empty row has been read from file!\n"} +
                  "\nRow number: " + rowNumberToString + "\n";
    }
                                                                                                   // search for a separator (= for synonyms, ! for antonyms)
    int synonymsSeparatorIndex{rowContent.indexOf('=')};
    int antonymsSeparatorIndex{rowContent.indexOf('!')};
    int separatorIndex{-1};                                                                        // stores the value of the found separator, no matter which type; used for actual words separation
    if ((synonymsSeparatorIndex != rowContent.lastIndexOf('=')) ||                                 // throw fatal error exception if a separator has more occurences within the string of the read row
        (antonymsSeparatorIndex != rowContent.lastIndexOf('!')))
        {
            throw QString{"Multiple separators!"} +
                          "\nRow number: " + rowNumberToString + "\n";
        }
    else if (synonymsSeparatorIndex == antonymsSeparatorIndex)                                     // throw fatal error if none are found (both returned index values are -1)
    {
        throw QString{"No separator found!"} +
                      "\nRow number: " + rowNumberToString + "\n";
    }
    else if ((synonymsSeparatorIndex != -1) && (antonymsSeparatorIndex != -1))                     // throw fatal error if both separators are found
    {
        throw QString{"Multiple separators!"} +
                      "\nRow number: " + rowNumberToString + "\n";
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
                                                                                                   // separate words, write them into the appropriate strings so they are available to the other functions
    m_FirstWord = rowContent.left(separatorIndex);
    for (auto currentCharacter : m_FirstWord)                                                        // check if first word includes only small letters; no other characters (numbers, spaces, etc) allowed
    {
        if (!(currentCharacter.isLower()))
        throw QString{"First word contains illegal characters!"} +
                      "\nRow number: " + rowNumberToString + "\n";
    }
    if (m_FirstWord.size() < minWordSize)                                                            // check if first word has the minimum required number of characters
    {
        throw QString{"First word has less than the minimum required number of characters!"} +
                      "\nRow number: " + rowNumberToString + "\n";

    }
    m_SecondWord = rowContent.mid(separatorIndex+1);
    for (auto currentCharacter : m_SecondWord)                                                       // check if second word includes only small letters; no other characters (numbers, spaces, etc) allowed
    {
        if (!(currentCharacter.isLower()))
        throw QString{"Second word contains illegal characters!"} +
                      "\nRow number: " + rowNumberToString + "\n";
    }
    if (m_SecondWord.size() < minWordSize)                                                           // check if second word has the minimum required number of characters
    {
        throw QString{"Second word has less than the minimum required number of characters!"} +
                      "\nRow number: " + rowNumberToString + "\n";

    }
}
                                                                                                   /* This function creates the final error message to be displayed to the user
                                                                                                      if the words have not been correctly entered
                                                                                                   */
void WordMixer::_createErrorMessage(const int errorCode)
{
    switch(errorCode)
    {
    case MISSING_WORDS:
        statusMessage = statusTexts[MISSING_WORDS];
        break;
    case INCORRECT_WORDS:
        statusMessage = statusTexts[INCORRECT_WORDS];
        break;
    default:
        ;                                                                                          // reserved for future use
    }
}
                                                                                                   /* This function creates the success message to be displayed to the user
                                                                                                      if the words have been successfully entered
                                                                                                   */
void WordMixer::_createSuccessMessage()
{                                                                                                  // shared variable statusMessage is used as success message
    statusMessage = statusTexts[SUCCESS];
    statusMessage += "\n\nThe two words are:\n\n";
    statusMessage += "\t" + m_FirstWord + "\n";
    statusMessage += "\t" + m_SecondWord + "\n";
    statusMessage += "\nThe words are: \n\n\t";
    if (m_AreSynonyms)
    {
        statusMessage += "synonyms";
    }
    else
    {
        statusMessage += "antonyms";
    }
    statusMessage += "\n\nNext pair of words is available below.";
}
                                                                                                   // this function creates the text with the high-scores to be displayed in the main game window when the scores get updated
void WordMixer::_createHighScoresMessage()
{
    m_HighScoresMessage = "High-score: " + QString::number(m_ObtainedScore) + "/" +
                                         QString::number(m_totalAvailableScore);
}
                                                                                                   /* this function creates the text with the number of pairs to be displayed in the main game window
                                                                                                      when a new pair of words is presented to the user
                                                                                                   */
void WordMixer::_createNrOfPairsMessage()
{
    m_NrOfPairsMessage = "Word pairs: " + QString::number(m_GuessedWordPairs) + "/" +
                                        QString::number(m_TotalWordPairs);
}
                                                                                                   /* This function inserts one of the pieces (substrings) belonging to one of the words (const QString &word)
                                                                                                      into a element of the mixedWords vector
                                                                                                      The function returns the position index in the mixed words string vector where the piece was inserted
                                                                                                   */
int WordMixer::_insertWordPiece(const QString &word, int firstCharPos,
                                        QVector<int> &wordPieceIndexes)
{
                                                                                                   /* first, choose one of the elements of the indexes vector randomly, whose value is the index of the mixed words
                                                                                                      string vector where the current word piece will be placed
                                                                                                   */
    std::uniform_int_distribution<int> indexDist{0,wordPieceIndexes.size()-1};
    int insertOnPositionIndex{indexDist(m_IndexEngine)};
                                                                                                   // second, place the word piece on the chosen position in the mixed words string vector
    int insertOnPosition{wordPieceIndexes[insertOnPositionIndex]};
    m_MixedWords[insertOnPosition] = word.mid(firstCharPos,m_WordPieceSize);
                                                                                                   // third, remove the occupied position from the list of available positions and return the position index
    wordPieceIndexes.remove(insertOnPositionIndex);
    return insertOnPosition;
}


