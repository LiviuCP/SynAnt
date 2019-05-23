/* Used by GameManager to avoid exposing full functionality to the WordPairOwner */

#ifndef WORDMIXERPROXY_H
#define WORDMIXERPROXY_H

#include <QObject>

class WordMixer;

class WordMixerProxy : public QObject
{
    Q_OBJECT
public:
    explicit WordMixerProxy(WordMixer* pWordMixer, QObject *parent = nullptr);

    const QVector<QString>& getMixedWordsPiecesContent() const;
    QString getFirstWord() const;
    QString getSecondWord() const;

    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;

    bool areSynonyms() const;

private:
    WordMixer* m_pWordMixer;
};

#endif // WORDMIXERPROXY_H
