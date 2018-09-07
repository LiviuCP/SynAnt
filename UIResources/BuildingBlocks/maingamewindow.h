/*

  This class implements the main game window

  This window can be accessed from 2 possible locations:
  - from the introduction window, by clicking the Play button
  - from the hints (help) window, by clicking the Ok button

  The role of this window is to play the actual game:
  - enter the correct words
  - click the Submit button

  From this window the user can execute following actions:
  1) Jump to the hints (help) window by clicking the Hints button
  2) Play the game as mentioned above
  3) Quit the game by clicking the Quit button

*/

#ifndef MAINGAMEWINDOW_H
#define MAINGAMEWINDOW_H

#include <QWidget>
#include <QVector>

#include "../../Common/game.h"

class QLabel;
class SelectableLabel;
class QRadioButton;
class QPushButton;
class QHBoxLayout;
class QLineEdit;
class QShortcut;
class WordMixer;
class ScoreItem;

class MainGameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainGameWindow(WordMixer *wm = nullptr, QWidget *parent = nullptr);
    bool isInitialized() const;
    void init();

public slots:
    void onStatisticsUpdated();

signals:
    void levelChanged(Game::Level level);
    void switchedMaintoHints();

private slots:
    void _onButtonResetClicked();
    void _onButtonEasyToggled(bool checked);
    void _onButtonEasyShortcutEntered();
    void _onButtonMediumToggled(bool checked);
    void _onButtonMediumShortcutEntered();
    void _onButtonHardToggled(bool checked);
    void _onButtonHardShortcutEntered();
    void _onButtonSubmitClicked();
    void _onButtonResultsClicked();

private:
    // both words split into equal pieces and mixed into a single array
    QVector<SelectableLabel*> m_MixedWords;
    QHBoxLayout* m_pMixedWordsLayout;
    QLabel* m_pResultsErrors;
    QLineEdit* m_pFirstWordLineEdit;
    QLineEdit* m_pSecondWordLineEdit;
    QLabel* m_pHighScores;
    QLabel* m_pNrOfWordPairs;
    QRadioButton* m_pLevelEasyButton;
    QRadioButton* m_pLevelMediumButton;
    QRadioButton* m_pLevelHardButton;
    QPushButton* m_pResetButton;
    QShortcut* m_pResetButtonShortcut;
    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
    // set true after first access to main game window
    bool m_IsInitialized;

    Game::StatusCodes _checkWords(const QString &firstWord, const QString &secondWord);
    void _removeMixedWordsLabels();
    void _createMixedWordsLabels();
    void _addMixedWordsLabels();
    void _switchToLevel(Game::Level level);
    void _updateStatusMessage(Game::StatusCodes statusCode);
};

#endif // MAINGAMEWINDOW_H
