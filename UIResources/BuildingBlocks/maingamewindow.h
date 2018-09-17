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
    void keyReleased();

protected:
    void keyReleaseEvent(QKeyEvent *keyEvent) override;

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
    void _onKeyReleased();

private:
    // both words split into equal pieces and mixed into a single array
    QVector<SelectableLabel*> m_MixedWordsLabels;
    QHBoxLayout* m_pMixedWordsLayout;
    QLabel* m_pResultsErrorsLabel;
    QLineEdit* m_pFirstWordLineEdit;
    QLineEdit* m_pSecondWordLineEdit;
    QLabel* m_pHighScoresLabel;
    QLabel* m_pNrOfWordPairsLabel;
    QPushButton* m_pSubmitButton;
    QShortcut* m_pSubmitButtonShortcut;
    QRadioButton* m_pLevelEasyButton;
    QRadioButton* m_pLevelMediumButton;
    QRadioButton* m_pLevelHardButton;
    QPushButton* m_pResetButton;
    QShortcut* m_pResetButtonShortcut;
    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
    // set true after first access to main game window
    bool m_IsInitialized;
    // needed for easier enabling/disabling of the buttons AND shortcuts
    bool m_IsSubmitEnabled;
    bool m_IsResetEnabled;

    Game::StatusCodes _checkWords(const QString &firstWord, const QString &secondWord);
    void _removeMixedWordsLabels();
    void _createMixedWordsLabels();
    void _addMixedWordsLabels();
    void _switchToLevel(Game::Level level);
    void _updateStatusMessage(Game::StatusCodes statusCode);
    void _setSubmitEnabled(bool enabled);
    void _setResetEnabled(bool enabled);
};

#endif // MAINGAMEWINDOW_H
