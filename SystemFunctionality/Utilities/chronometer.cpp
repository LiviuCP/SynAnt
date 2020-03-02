#include "chronometer.h"

constexpr int c_RefreshIntervalMilliseconds{1000};
constexpr int c_DefaultTimeIntervalSeconds{30};
constexpr int c_MillisecondsBeforeTimeout{250};

Chronometer::Chronometer(QObject *parent)
    : QObject(parent)
    , m_pRemainingTimeUpdateTimer{new QTimer{this}}
    , m_pTimeExpiredSignallingTimer{new QTimer{this}}
    , m_InitialRemainingTimeInSeconds{c_DefaultTimeIntervalSeconds}
    , m_CurrentRemainingTimeInSeconds{c_DefaultTimeIntervalSeconds}
    , m_State{ChronoState::DISABLED}
    , m_RemainingTimerDurationWhenPaused{0}
{
    bool connected{connect(m_pRemainingTimeUpdateTimer, &QTimer::timeout, this, &Chronometer::_onRemainingTimeUpdateTimerTimeout)};
    Q_ASSERT(connected);
    connected = connect(m_pTimeExpiredSignallingTimer, &QTimer::timeout, this, &Chronometer::_onTimeExpiredTimerTimeout);
    Q_ASSERT(connected);

    // both timers to be started manually after timeout to avoid (remaining) interval issues when pausing any of them
    m_pRemainingTimeUpdateTimer->setSingleShot(true);
    m_pTimeExpiredSignallingTimer->setSingleShot(true);
}

void Chronometer::enable()
{
    if (m_State == ChronoState::DISABLED)
    {
        m_State = ChronoState::ENABLED_NOT_RUNNING;
        _resetRemainingTime();

        Q_EMIT enabledChanged();
    }
}

void Chronometer::disable()
{
    if (m_State != ChronoState::DISABLED)
    {
        stop();
        m_State = ChronoState::DISABLED;

        _resetRemainingTime();

        Q_EMIT enabledChanged();
    }
}

void Chronometer::start()
{
    if (m_State == ChronoState::ENABLED_NOT_RUNNING)
    {
        if (m_CurrentRemainingTimeInSeconds != m_InitialRemainingTimeInSeconds)
        {
            _resetRemainingTime();
        }

        m_pRemainingTimeUpdateTimer->start(c_RefreshIntervalMilliseconds);
        m_State = ChronoState::RUNNING;
    }
}

void Chronometer::stop()
{
    if (m_State != ChronoState::DISABLED && m_State != ChronoState::ENABLED_NOT_RUNNING)
    {
        if (m_pRemainingTimeUpdateTimer->isActive())
        {
            m_pRemainingTimeUpdateTimer->stop();
        }
        else if (m_pTimeExpiredSignallingTimer->isActive())
        {
            m_pTimeExpiredSignallingTimer->stop();
        }
        else if (m_State == ChronoState::PRIMARY_TIMER_PAUSED || m_State == ChronoState::SECONDARY_TIMER_PAUSED)
        {
            m_RemainingTimerDurationWhenPaused = 0;
        }

        m_State = ChronoState::ENABLED_NOT_RUNNING;
    }
}

void Chronometer::restart()
{
    if (m_State != ChronoState::DISABLED && m_State != ChronoState::ENABLED_NOT_RUNNING)
    {
        if (m_pRemainingTimeUpdateTimer->isActive())
        {
            m_pRemainingTimeUpdateTimer->stop();
        }
        else if (m_pTimeExpiredSignallingTimer->isActive())
        {
            m_pTimeExpiredSignallingTimer->stop();
        }

        _resetRemainingTime();

        m_pRemainingTimeUpdateTimer->start(c_RefreshIntervalMilliseconds);
        m_State = ChronoState::RUNNING;
    }
}

void Chronometer::pause()
{
    if (m_State == ChronoState::RUNNING)
    {
        Q_ASSERT(m_RemainingTimerDurationWhenPaused == 0); // avoid the situation when no timer is paused but a remaining duration at pause is recorded

        if (m_pRemainingTimeUpdateTimer->isActive())
        {
            m_RemainingTimerDurationWhenPaused = m_pRemainingTimeUpdateTimer->remainingTime();
            m_pRemainingTimeUpdateTimer->stop();
            m_State = ChronoState::PRIMARY_TIMER_PAUSED;
        }
        else if (m_pTimeExpiredSignallingTimer->isActive())
        {
            m_RemainingTimerDurationWhenPaused = m_pTimeExpiredSignallingTimer->remainingTime();
            m_pTimeExpiredSignallingTimer->stop();
            m_State = ChronoState::SECONDARY_TIMER_PAUSED;
        }
    }
}

void Chronometer::resume()
{
    if (m_State == ChronoState::PRIMARY_TIMER_PAUSED)
    {
        m_pRemainingTimeUpdateTimer->start(m_RemainingTimerDurationWhenPaused);
        m_State = ChronoState::RUNNING;
    }
    else if (m_State == ChronoState::SECONDARY_TIMER_PAUSED)
    {
        m_pTimeExpiredSignallingTimer->start(m_RemainingTimerDurationWhenPaused);
        m_State = ChronoState::RUNNING;
    }
    else
    {
        Q_ASSERT(m_RemainingTimerDurationWhenPaused == 0); // avoid the situation when no timer is paused but a remaining duration at pause is recorded
    }
}

bool Chronometer::setTotalCountdownTime(int time)
{
    bool success{false};

    if ((m_State == ChronoState::DISABLED || m_State == ChronoState::ENABLED_NOT_RUNNING) && time > 0)
    {
        m_InitialRemainingTimeInSeconds = time;
        m_CurrentRemainingTimeInSeconds = m_InitialRemainingTimeInSeconds;
        success = true;

        Q_EMIT refreshTriggered();
    }

    return success;
}

QPair<QString, QString> Chronometer::getRemainingTimeMinSec()
{
    const int c_SecondsInAMinute{60};

    QString minutesString{QString::number(m_CurrentRemainingTimeInSeconds / c_SecondsInAMinute)};
    QString secondsString{QString::number(m_CurrentRemainingTimeInSeconds % c_SecondsInAMinute)};

    if (minutesString.size() == 1)
    {
        minutesString.prepend("0"); // padding zero to ensure we always have two digits
    }

    if (secondsString.size() == 1)
    {
        secondsString.prepend("0"); // padding zero to ensure we always have two digits
    }

    return QPair<QString, QString>{minutesString, secondsString};
}

bool Chronometer::isEnabled() const
{
    return (m_State != ChronoState::DISABLED);
}

void Chronometer::_onRemainingTimeUpdateTimerTimeout()
{
    --m_CurrentRemainingTimeInSeconds;
    Q_EMIT refreshTriggered();

    if (m_CurrentRemainingTimeInSeconds == 0)
    {
        m_pTimeExpiredSignallingTimer->start(c_MillisecondsBeforeTimeout);
    }
    else
    {
        m_pRemainingTimeUpdateTimer->start(c_RefreshIntervalMilliseconds);
    }
}

void Chronometer::_onTimeExpiredTimerTimeout()
{
    m_State = ChronoState::COUNTDOWN_FINISHED;
    Q_EMIT timeoutTriggered();
}

void Chronometer::_resetRemainingTime()
{
    m_CurrentRemainingTimeInSeconds = m_InitialRemainingTimeInSeconds;
    Q_EMIT refreshTriggered();
}
