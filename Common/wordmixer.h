/*

   This class fulfills following tasks:
   1) Reads a randomly chosen line from a file. The line contains 2 words which are either synonyms or antonyms.
   2) Puts the 2 words into separate strings.
   3) Divides each word in more pieces.
   4) Mixes the pieces from all words alltogether into a single array.
      The array is then displayed by the main window.
   5) Hands over the original words to the facade for checking and sending to presenter for displaying.

*/


#ifndef WORDMIXER_H
#define WORDMIXER_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QMap>

#include <random>

#include "game.h"

class WordMixer : public QObject
{
    Q_OBJECT
public:
    explicit WordMixer(const QString &m_FileName, QObject *parent = nullptr);

    // splits the words into equal pieces and mixes them into an array (last piece might have less characters than the others)
    void mixWords();

    // sets number of characters for each word piece
    void setWordPieceSize(Game::Level level);

    const QVector<QString>& getMixedWordsStringArray() const;
    QString getFirstWord() const;
    QString getSecondWord() const;

    // indexes of the beginning and the end piece of each word in the mixed words array
    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;

    bool areSynonyms() const;

    // for unit-testing purposes only
    void setFirstWord(const QString &firstWord);
    void setSecondWord(const QString &secondWord);
    void fetchWordsFromRowContent(const QString &rowContent);

signals:
    Q_SIGNAL void mixedWordsChanged();

private:
    void _getRowNumber();
    void _getRowContent();
    void _retrieveWords();
    void _checkWordIsCorrect(const QString& word, const QString& wordIdentifier);
    int  _insertWordPiece(const QString &word, int firstCharPos, QVector<int> &wordPieceIndexes);

    // used for retrieving the mixed words string vector indexes of the first and last piece of each of the 2 words
    enum class WordsBeginEndPieces {
        FIRST_WORD_FIRST_PIECE,
        FIRST_WORD_LAST_PIECE,
        SECOND_WORD_FIRST_PIECE,
        SECOND_WORD_LAST_PIECE,
        PiecesCount
    };

    int m_WordPieceSize;
    QString m_FileName;
    int m_TotalNrOfRows;
    int m_RowNumber;
    QString m_RowContent;
    QPair<QString,QString> m_WordsPair;
    QVector<QString> m_MixedWords;

    // stores the indexes of the first and last piece of the 2 words in the mixed words string vector
    QMap<WordsBeginEndPieces,int> m_WordsBeginEndPieceIndexes;

    bool m_AreSynonyms;

    // get the row number from the file which contains the word pair to be mixed
    std::default_random_engine m_RowNumberEngine;
    // get the index of the mixed words string array where current piece is written
    std::default_random_engine m_WordPieceIndexEngine;

    // only to be set true if unit-testing the code
    bool m_ManualWordsEntry;
};

#endif // WORDMIXER_H
