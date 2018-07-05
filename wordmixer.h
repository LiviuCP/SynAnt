                                                                                                    /*

                                                                                                       This class implements the application core, which interfaces with the user through the MainGameWindow class.

                                                                                                       The class fulfills following tasks:
                                                                                                       1) Reads a randomly chosen line from a file. The line contains 2 words which are either synonyms or antonyms.
                                                                                                       2) Puts the 2 words in separate strings.
                                                                                                       3) Divides each word in more pieces.
                                                                                                       4) Mixes the pieces from all words alltogether into a single word.
                                                                                                          The individual pieces are still visible as the mixed words are stored in a string vector, which is then displayed by the main window.
                                                                                                       5) Takes over the user input (the 2 words) and checks if they were correctly entered on not.
                                                                                                       6) If the words have been correctly entered, it creates a success message, which is then displayed by the main window.
                                                                                                          Also, steps 1) - 4) are repeated.
                                                                                                       7) If the words haven't been correctly entered, it creates a error message, which is then displayed by the main window.
                                                                                                          The user should enter the 2 words again.
                                                                                                       8) Hands over the 2 words to the main window if the user presses the Show results button.

                                                                                                    */


#ifndef WORDMIXER_H
#define WORDMIXER_H

#include <QVector>
#include <QString>
#include <QTextStream>
#include <QObject>
#include <random>
#include "fatalerrors.h"
#include "game.h"

class WordMixer : public QObject
{
    Q_OBJECT
public:
    explicit WordMixer(QObject *parent = nullptr, const QString &m_FileName = "");                  // constructor, takes the name of the file from which words are read as argument

    void mixWords();                                                                                // splits the words into pieces and mixes them into a single word
    bool checkWords(const QString &m_FirstWord, const QString &m_SecondWord);                       // checks the 2 words entered by user, returns true if they are correct
    const QVector<QString>& getMixedWordsStringArray() const;                                       // returns a reference to the mixed words array so it can be used by the main game window for creating the labels with the mixed pair of words
    void retrieveResults();                                                                         // called when the user clicks the Results button; creates the message with the solution (the 2 words that should have been guessed by user)

    const QString& getStatusMessage() const;                                                        // used by main window to update the results/errors section (use cases documented above, see variable statusText)

    int getFirstWordBeginIndex() const;                                                             // used by main window to locate the position of the first piece of the first word in the mixedWords vector
    int getFirstWordEndIndex() const;                                                               // used by main window to locate the position of the last piece of the first word in the mixedWords vector
    int getSecondWordBeginIndex() const;                                                            // used by main window to locate the position of the first piece of the second word in the mixedWords vector
    int getSecondWordEndIndex() const;                                                              // used by main window to locate the position of the last piece of the second word in the mixedWords vector

public slots:
    void setWordPieceSize(Game::Level level);                                                       // used when setting level

private:
    void _getRowNumber();                                                                           // generates a random number which is the row number from which the word pair is read
    void _retrieveWords();                                                                          // reads the row from the file, separates the words, writes them to the firstWord and secondWord strings and updates the areSynonyms variable
    void _createErrorMessage(const int errorCode);                                                  // creates the final error message
    void _createSuccessMessage();                                                                   // creates the final success message
    int  _insertWordPiece(const QString &word, int firstCharPos,                                    // inserts a piece (substring) from one of the words in one of the elements of the string vector containing the mixed words
                                    QVector<int> &wordPieceIndexes);

    QString m_FileName;                                                                             // stores the path of the file from which the randomly chosen row is read
    int m_RowNumber;                                                                                // contains the number of the randomly chosen line from the file
    int m_TotalRows;                                                                                // stores the total number of the rows contained by the file
    int m_WordPieceSize;                                                                            // number of characters contained by a word piece (prior to mixing the words are divided in equal pieces; last piece of each word might contain less characters)

    std::default_random_engine m_RowNumberEngine;                                                   // used for generating a row number
    std::default_random_engine m_IndexEngine;                                                       // used for determinining the index of the mixed words string where the current word piece is written

    QString m_FirstWord;                                                                            // first word contained on the line (out of 2 words)
    QString m_SecondWord;                                                                           // second word on the line
    QVector<QString> m_MixedWords;                                                                  // contains the 2 words after they've been mixed
    bool m_AreSynonyms;                                                                             // set true if the 2 read words are synonyms, false if antonyms

    enum StatusCodes {                                                                              // aliases for the success and (non-fatal) error codes (caution: no integer value assignment to be performed to any of the enum values!)
        SUCCESS,                                                                                    // 1) user entered both words correctly
        MISSING_WORDS,                                                                              // 2) at least one word hasn't been entered
        INCORRECT_WORDS,                                                                            // 3) the words were not correctly entered (at least one of them)
        StatusCodesCount                                                                            // 4) total number of error and success message templates
    };
    QVector<QString> m_StatusTexts;                                                                   // stores the "raw" error and success messages (they will be converted by specific functions to final messages, which will be displayed by main window);
    QString m_StatusMessage;                                                                          // stores the message which will be displayed in the results/errors section of the main window

    QVector<int> m_WordsBeginEndPositions;                                                            // stores the indexes of the first and last piece of the 2 words in the mixedWords string vector
    enum WordsBeginEndIndexes {                                                                     // aliases for the indexes and size of the wordsBeginEndPositions vector (caution: no integer value assignment to be performed to any of the enum values!)
        FIRST_BEGIN,                                                                                // 1) first element of the wordsBeginEndPositions vector, contains the position index of the first piece of the first word in the MixedWords vector
        FIRST_END,                                                                                  // 2) second element, position of the last piece of the first word in the MixedWords vector
        SECOND_BEGIN,                                                                               // 3) third element, position of the first piece of the second word in the MixedWords vector
        SECOND_END,                                                                                 // 4) fourth element, position of the last piece of the first word in the MixedWords vector
        IndexesCount                                                                                // 5) total number of indexes (4)
    };
};

#endif // WORDMIXER_H
