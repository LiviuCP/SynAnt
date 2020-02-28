/*
  This class implements a time limit that is enforced to the user to guess the correct word pair:
  1) If the user doesn't guess the words within provided time frame no score points would be provided (as if the user requests displaying them without guessing)
  2) The time interval depends on chosen level (higher for a more difficult level)
  3) Provides the facade a current time in minutes/seconds once per second
*/

#ifndef CHRONOMETER_H
#define CHRONOMETER_H

#include <QObject>
#include <QTimer>

class Chronometer : public QObject
{
    Q_OBJECT

public:
    explicit Chronometer(QObject *parent = nullptr);

    void enable();
    void disable();
    void start();
    void stop();
    void restart();
    void pause();
    void resume();

    bool setTotalCountdownTime(int time);

    QPair<QString, QString> getRemainingTimeMinSec();
    bool isEnabled() const;

signals:
    Q_SIGNAL void timeoutTriggered();
    Q_SIGNAL void refreshTriggered();
    Q_SIGNAL void enabledChanged();

private slots:
    void _onRemainingTimeUpdateTimerTimeout();
    void _onTimeExpiredTimerTimeout();

private:
    void _resetRemainingTime();

    QTimer* m_pRemainingTimeUpdateTimer;
    QTimer* m_pTimeExpiredSignallingTimer;
    int m_InitialRemainingTimeInSeconds;
    int m_CurrentRemainingTimeInSeconds;
    bool m_Enabled;
    bool m_IsRefreshIntervalTimerPaused;
    bool m_IsTimeExpirationTimerPaused;
    int m_RemainingTimerDurationWhenPaused;
};

#endif // CHRONOMETER_H
