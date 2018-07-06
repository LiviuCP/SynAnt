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
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include "hintswindow.h"
#include "wordmixer.h"
#include "scoreitem.h"

class HintsWindow;
class MainGameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainGameWindow(QWidget *parent = 0);
    void updateHintsWinPtr(HintsWindow *hw);
    void assignWordMixer(WordMixer *wm);
    bool windowAlreadyAccessed() const;
    void getFirstTwoWords();

public slots:
    void onStatisticsUpdated();

signals:
    void levelChanged(Game::Level level);

private slots:
    void _onButtonResetClicked();
    void _onButtonEasyToggled(bool checked);
    void _onButtonEasyShortcut();
    void _onButtonMediumToggled(bool checked);
    void _onButtonMediumShortcut();
    void _onButtonHardToggled(bool checked);
    void _onButtonHardShortcut();
    void _onButtonSubmitClicked();
    void _onButtonHintsClicked();
    void _onButtonResultsClicked();

private:
    HintsWindow* m_HintsWindow;
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
    WordMixer* m_WordMixer;
    ScoreItem* m_ScoreItem;
    // set true after first access to main game window
    bool m_AlreadyAccessed;
    // update all statistics or only the totals
    enum StatisticsUpdate {
        FULL_UPDATE,
        PARTIAL_UPDATE
    };

    void _removeMixedWordsLabels();
    void _createMixedWordsLabels();
    void _addMixedWordsLabels();
    void _switchToLevel(Game::Level level);
};

#endif // MAINGAMEWINDOW_H
