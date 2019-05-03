/*
   This class gets the output from the WordMixer class:
      - mixed words array
      - original words used for mixing
      - syn/ant flag

   It becomes the owner of the mixed words (pieces and reference words) and fulfills following tasks:
   1) Provides the owned data to the "consumer" classes (facade, presenter) on demand
   2) Keeps track of the selection status of the mixed words pieces (when a piece is added to input it is marked as selected and viceversa)
*/

#ifndef WORDPAIROWNER_H
#define WORDPAIROWNER_H

#include <QObject>
#include <QVector>

#include "game.h"

class WordMixer;

class WordPairOwner : public QObject
{
    Q_OBJECT
public:
    explicit WordPairOwner(QObject *parent = nullptr);

    void connectToWordMixer(WordMixer* pWordMixer);

    void updateSingleWordPieceSelection(int wordPieceIndex, bool selected);
    void updateMultipleWordPiecesSelection(QVector<int> wordPieceIndexes, bool selected);

    QVector<QString> getMixedWordsPiecesContent() const;
    QVector<Game::PieceTypes> getMixedWordsPiecesTypes() const;
    QVector<bool> getAreMixedWordsPiecesSelected() const;

    QString getFirstReferenceWord() const;
    QString getSecondReferenceWord() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsAvailable();
    Q_SIGNAL void selectionChanged();

private slots:
    void _onMixedWordsAvailable();

private:
    void _buildMixedWordsPieces();

    struct WordPiece
    {
        QString content;
        Game::PieceTypes pieceType;
        bool isSelected;
    };

    WordMixer* m_pWordMixer;
    QString m_FirstReferenceWord;
    QString m_SecondReferenceWord;
    QVector<WordPiece> m_MixedWordsPieces;
    bool m_AreSynonyms;

};

#endif // WORDPAIROWNER_H
