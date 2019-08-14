/*
   This class fulfills following tasks:
   1) Gets two words for mixing (from the DataSource class).
   2) Divides each word in equal pieces and mixes them alltogether into a single array.
   3) Hands over the mixed word pieces array and original words to an owner class (WordPairOwner) for further usage

   The size of the word piece is modifiable and depends on the selected level.
*/


#ifndef WORDMIXER_H
#define WORDMIXER_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QMap>

#include <random>

#include "../Utilities/game.h"

class WordMixer : public QObject
{
    Q_OBJECT

public:
    explicit WordMixer(QObject *parent = nullptr);

    // splits the words into equal pieces and mixes them into an array (last piece might have less characters than the others)
    void mixWords(QPair<QString, QString> newWordsPair, bool areSynonyms);

    // sets number of characters for each word piece
    void setWordPieceSize(Game::Level level);

    const QVector<QString>& getMixedWordsPiecesContent() const;
    QString getFirstWord() const;
    QString getSecondWord() const;

    // indexes of the beginning and the end piece of each word in the mixed words array
    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void newWordsPairMixed();

private:
    // used for retrieving the mixed words string vector indexes of the first and last piece of each of the 2 words
    enum class WordsBeginEndPieces
    {
        FIRST_WORD_FIRST_PIECE,
        FIRST_WORD_LAST_PIECE,
        SECOND_WORD_FIRST_PIECE,
        SECOND_WORD_LAST_PIECE,
        PiecesCount
    };

    int m_WordPieceSize;
    QPair<QString,QString> m_WordsPair;
    QVector<QString> m_MixedWordsPiecesContent;

    // stores the indexes of the first and last piece of the 2 words in the mixed words string vector
    QMap<WordsBeginEndPieces,int> m_WordsBeginEndPieceIndexes;

    bool m_AreSynonyms;

    // get the index of the mixed words string array where current piece is written
    std::default_random_engine m_WordPieceIndexEngine;
};

#endif // WORDMIXER_H
