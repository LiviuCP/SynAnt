#ifndef WORDPAIROWNER_H
#define WORDPAIROWNER_H

#include <QObject>
#include <QVector>

class WordMixer;

class WordPairOwner : public QObject
{
    Q_OBJECT
public:
    explicit WordPairOwner(QObject *parent = nullptr);

    void connectToWordMixer(WordMixer* pWordMixer);
    void fetchNewMixedWords();

    QVector<QString> getMixedWordsPiecesArray() const;

    QString getFirstWord() const;
    QString getSecondWord() const;

    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsAvailable();
public slots:

private:
    WordMixer* m_pWordMixer;
    QString m_FirstWord;
    QString m_SecondWord;
    QVector<QString> m_MixedWordsPiecesArray;
    int m_FirstWordFirstPieceIndex;
    int m_FirstWordLastPieceIndex;
    int m_SecondWordFirstPieceIndex;
    int m_SecondWordLastPieceIndex;
    bool m_AreSynonyms;

private slots:
    void _onMixedWordsAvailable();
};

#endif // WORDPAIROWNER_H
