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

#include "../Utilities/game.h"

class WordMixerProxy;

class WordPairOwner : public QObject
{
    Q_OBJECT
public slots:
    void onPieceAddedToInput(int wordPieceIndex);
    void onPiecesRemovedFromInput(QVector<int> wordPieceIndexes);
    void onMixedWordsAvailable();

public:
    explicit WordPairOwner(QObject *parent = nullptr);

    void setWordMixerProxy(WordMixerProxy* pWordMixerProxy);

    QVector<QString> getMixedWordsPiecesContent() const;
    QVector<Game::PieceTypes> getMixedWordsPiecesTypes() const;
    QVector<bool> getAreMixedWordsPiecesAddedToInput() const;
    Game::PieceTypes getWordPieceType(int index) const;
    bool getIsWordPieceSelected(int index) const;

    QString getFirstReferenceWord() const;
    QString getSecondReferenceWord() const;

    bool isLastAvailableWordPiece() const;
    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsAvailable();
    Q_SIGNAL void selectionChanged();

private:
    void _buildMixedWordsPiecesArray();
    void _updateSingleWordPieceStatus(int wordPieceIndex, bool addedToInput);
    void _updateMultipleWordPiecesStatus(QVector<int> wordPieceIndexes, bool addedToInput);

    struct WordPiece
    {
        WordPiece();

        QString content;
        Game::PieceTypes pieceType;
        bool isAddedToInput;
    };

    WordMixerProxy* m_pWordMixerProxy;
    QString m_FirstReferenceWord;
    QString m_SecondReferenceWord;
    QVector<WordPiece> m_MixedWordsPieces;
    bool m_AreSynonyms;
};

#endif // WORDPAIROWNER_H
