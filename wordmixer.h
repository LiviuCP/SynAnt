/*

   This class implements the application core, which interfaces with the user through the MainGameWindow class.

   The class fulfills following tasks:
   1) Reads a randomly chosen line from a file. The line contains 2 words which are either synonyms or antonyms.
   2) Puts the 2 words in separate strings.
   3) Divides each word in more pieces.
   4) Mixes the pieces from all words alltogether into a single word.
      The individual pieces are still visible as the mixed words are stored in a string vector, which is then displayed by the main window.
   5) Hands over the 2 words to the main window for checking and/or displaying.

*/


#ifndef WORDMIXER_H
#define WORDMIXER_H

#include <QObject>
#include <QVector>
#include <QPair>

#include <random>

#include "game.h"

class WordMixer : public QObject
{
    Q_OBJECT
public:
    explicit WordMixer(const QString &m_FileName = "", QObject *parent = nullptr);

    // splits the words into pieces and mixes them into an array
    void mixWords();
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

    QPair<QString,QString> m_WordsPair;
    QVector<QString> m_MixedWords;
    bool m_AreSynonyms;

    // stores the indexes of the first and last piece of the 2 words in the mixedWords string vector
    QVector<int> m_WordsBeginEndPositions;
    // used for getting these indexes from m_WordsBeginEndPositions
    enum class WordsBeginEndIndexes {
        FIRST_WORD_FIRST_PIECE,
        FIRST_WORD_LAST_PIECE,
        SECOND_WORD_FIRST_PIECE,
        SECOND_WORD_LAST_PIECE,
        IndexesCount
    };
};

#endif // WORDMIXER_H
