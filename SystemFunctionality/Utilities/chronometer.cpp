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
    , m_Enabled{false}
    , m_IsRefreshIntervalTimerPaused{false}
    , m_IsTimeExpirationTimerPaused{false}
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
    if (!m_Enabled)
    {
        m_Enabled = true;
        Q_EMIT enabledChanged();
    }
}

void Chronometer::disable()
{
    if (m_Enabled)
    {
        stop();
        m_Enabled = false;

        Q_EMIT enabledChanged();
    }
}

void Chronometer::start()
{
    if (m_Enabled && !m_pRemainingTimeUpdateTimer->isActive() && !m_pTimeExpiredSignallingTimer->isActive())
    {
        _resetRemainingTime();
        m_pRemainingTimeUpdateTimer->start(m_InitialRemainingTimeInSeconds);
    }
}

void Chronometer::stop()
{
    if (m_pRemainingTimeUpdateTimer->isActive())
    {
        m_pRemainingTimeUpdateTimer->stop();
    }
    else if (m_IsRefreshIntervalTimerPaused)
    {
        m_IsRefreshIntervalTimerPaused = false;
        m_RemainingTimerDurationWhenPaused = 0;
    }
    else if (m_pTimeExpiredSignallingTimer->isActive())
    {
        m_pTimeExpiredSignallingTimer->stop();
    }
    else if (m_IsTimeExpirationTimerPaused)
    {
        m_IsTimeExpirationTimerPaused = false;
        m_RemainingTimerDurationWhenPaused = 0;
    }

    _resetRemainingTime();
}

void Chronometer::restart()
{
    if (m_Enabled)
    {
        if (m_pRemainingTimeUpdateTimer->isActive())
        {
            m_pRemainingTimeUpdateTimer->stop();
        }
        else if (m_pTimeExpiredSignallingTimer->isActive())
        {
            m_pTimeExpiredSignallingTimer->stop();
        }

        if (m_CurrentRemainingTimeInSeconds != m_InitialRemainingTimeInSeconds)
        {
            _resetRemainingTime();
        }

        m_pRemainingTimeUpdateTimer->start(c_RefreshIntervalMilliseconds);
    }
}

void Chronometer::pause()
{
    if (!m_IsRefreshIntervalTimerPaused && !m_IsTimeExpirationTimerPaused)
    {
        Q_ASSERT(m_RemainingTimerDurationWhenPaused == 0); // avoid the situation when no timer is paised but a remaining duration at pause is recorded

        if (m_pRemainingTimeUpdateTimer->isActive())
        {
            m_RemainingTimerDurationWhenPaused = m_pRemainingTimeUpdateTimer->remainingTime();
            m_pRemainingTimeUpdateTimer->stop();
            m_IsRefreshIntervalTimerPaused = true;
        }
        else if (m_pTimeExpiredSignallingTimer->isActive())
        {
            m_RemainingTimerDurationWhenPaused = m_pTimeExpiredSignallingTimer->remainingTime();
            m_pTimeExpiredSignallingTimer->stop();
            m_IsTimeExpirationTimerPaused = true;
        }
    }
}

void Chronometer::resume()
{
    if (m_RemainingTimerDurationWhenPaused != 0)
    {
        if (m_IsRefreshIntervalTimerPaused)
        {
            m_IsRefreshIntervalTimerPaused = false;
            m_pRemainingTimeUpdateTimer->start(m_RemainingTimerDurationWhenPaused);
        }
        else if (m_IsTimeExpirationTimerPaused)
        {
            m_IsTimeExpirationTimerPaused = false;
            m_pTimeExpiredSignallingTimer->start(m_RemainingTimerDurationWhenPaused);
        }
        else
        {
            Q_ASSERT(false); // avoid the situation when no timer is paised but a remaining duration at pause is recorded
        }

        m_RemainingTimerDurationWhenPaused = 0;
    }
}

bool Chronometer::setTotalCountdownTime(int time)
{
    bool success{false};

    if (!m_pRemainingTimeUpdateTimer->isActive() && !m_pTimeExpiredSignallingTimer->isActive() && !m_IsRefreshIntervalTimerPaused && !m_IsTimeExpirationTimerPaused && time > 0)
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
    return m_Enabled;
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
    Q_EMIT timeoutTriggered();
}

void Chronometer::_resetRemainingTime()
{
    m_CurrentRemainingTimeInSeconds = m_InitialRemainingTimeInSeconds;
    Q_EMIT refreshTriggered();
}
