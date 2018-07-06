/*

   This class implements the application core, which interfaces with the user through the MainGameWindow class.

   The class fulfills following tasks:
   1) Reads a randomly chosen line from a file. The line contains 2 words which are either synonyms or antonyms.
   2) Puts the 2 words in separate strings.
   3) Divides each word in more pieces.
   4) Mixes the pieces from all words alltogether into a single word.
      The individual pieces are still visible as the mixed words are stored in a string vector, which is then displayed by the main window.
   5) Takes over the user input (the 2 words) and checks if they were correctly entered on not.
   6) Hands over the 2 words to the main window if the user presses the Show results button.

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
    explicit WordMixer(QObject *parent = nullptr, const QString &m_FileName = "");

    // splits the words into pieces and mixes them into an array
    void mixWords();
    Game::StatusCodes checkWords(const QString &m_FirstWord, const QString &m_SecondWord);
    const QVector<QString>& getMixedWordsStringArray() const;

    QString getFirstWord() const;
    QString getSecondWord() const;
    bool areSynonyms() const;

    // indexes of the beginning and the end piece of each word in the mixed words array
    int getFirstWordBeginIndex() const;
    int getFirstWordEndIndex() const;
    int getSecondWordBeginIndex() const;
    int getSecondWordEndIndex() const;

public slots:
    void setWordPieceSize(Game::Level level);

private:
    void _getRowNumber();
    void _retrieveWords();
    int  _insertWordPiece(const QString &word, int firstCharPos, QVector<int> &wordPieceIndexes);


    QString m_FileName;
    int m_RowNumber;
    int m_TotalRows;
    int m_WordPieceSize;

    // get the row number from the file which contains the word pair to be mixed
    std::default_random_engine m_RowNumberEngine;
    // get the index of the mixed words string array where current piece is written
    std::default_random_engine m_IndexEngine;

    QString m_FirstWord;
    QString m_SecondWord;
    QVector<QString> m_MixedWords;
    bool m_AreSynonyms;

    // stores the indexes of the first and last piece of the 2 words in the mixedWords string vector
    QVector<int> m_WordsBeginEndPositions;
    // used for getting these indexes from m_WordsBeginEndPositions
    enum WordsBeginEndIndexes {
        FIRST_BEGIN,
        FIRST_END,
        SECOND_BEGIN,
        SECOND_END,
        IndexesCount
    };
};

#endif // WORDMIXER_H
