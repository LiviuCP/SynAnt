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

#include "game.h"

class QLabel;
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
    QVector<QLabel*> m_MixedWords;
    QHBoxLayout* m_MixedWordsLayout;
    QLabel* m_ResultsErrors;
    QLineEdit* m_FirstWord;
    QLineEdit* m_SecondWord;
    QLabel* m_HighScores;
    QLabel* m_NrOfWordPairs;
    QRadioButton* m_LevelEasyButton;
    QRadioButton* m_LevelMediumButton;
    QRadioButton* m_LevelHardButton;
    QPushButton* m_ResetButton;
    QShortcut* m_ResetButtonShortcut;
    WordMixer* m_WordMixer;
    ScoreItem* m_ScoreItem;
    // set true after first access to main game window
    bool m_IsInitialized;

    void _removeMixedWordsLabels();
    void _createMixedWordsLabels();
    void _addMixedWordsLabels();
    void _switchToLevel(Game::Level level);
    void _updateStatusMessage(Game::StatusCodes statusCode);
};

#endif // MAINGAMEWINDOW_H
