#include <QApplication>
#include <QDesktopWidget>

#include "appmanager.h"
#include "../Common/wordmixer.h"

AppManager::AppManager(QObject *parent)
    : QObject(parent)
    , m_IsInitialized{false}
    , m_pWordMixer{nullptr}
{
}

void AppManager::init(const QString& filename)
{
    if (!m_IsInitialized)
    {
        m_pWordMixer = new WordMixer{filename, this};
        m_pIntroductionWindow = std::move(std::unique_ptr<IntroductionWindow>{new IntroductionWindow{}});
        m_pHintsWindow = std::move(std::unique_ptr<HintsWindow>{new HintsWindow{}});
        m_pMainGameWindow = std::move(std::unique_ptr<MainGameWindow>{new MainGameWindow{m_pWordMixer}});

        bool connected{connect(m_pIntroductionWindow.get(),&IntroductionWindow::switchedIntroToHints,this,&AppManager::_onSwitchedIntroToHints)};
        Q_ASSERT(connected);
        connected = connect(m_pIntroductionWindow.get(),&IntroductionWindow::switchedIntroToMain,this,&AppManager::_onSwitchedIntroToMain);
        Q_ASSERT(connected);
        connected = connect(m_pHintsWindow.get(),&HintsWindow::switchedHintsToMain,this,&AppManager::_onSwitchedHintsToMain);
        Q_ASSERT(connected);
        connected = connect(m_pMainGameWindow.get(),&MainGameWindow::switchedMaintoHints,this,&AppManager::_onSwitchedMainToHints);
        Q_ASSERT(connected);

        m_IsInitialized = true;

        // center the introduction window to the (primary) screen
        QRect desktopRect {QApplication::desktop()->availableGeometry()};
        m_pIntroductionWindow -> move((desktopRect.width() - m_pIntroductionWindow->width())/2, (desktopRect.height() - m_pIntroductionWindow->height())/2);

        m_pIntroductionWindow -> show();
    }
}

void AppManager::hideActiveWindow()
{
    if (m_pIntroductionWindow && m_pIntroductionWindow->isVisible())
    {
        m_pIntroductionWindow->hide();
    }
    else if (m_pHintsWindow && m_pHintsWindow->isVisible())
    {
        m_pHintsWindow->hide();
    }
    else if (m_pMainGameWindow && m_pMainGameWindow->isVisible())
    {
        m_pMainGameWindow->hide();
    }
}

void AppManager::_onSwitchedIntroToHints()
{
    m_pIntroductionWindow -> hide();

    QPointF pos{m_pIntroductionWindow->pos()};
    m_pHintsWindow->move(pos.x(), pos.y());

    m_pHintsWindow -> show();
}

void AppManager::_onSwitchedIntroToMain()
{
    m_pIntroductionWindow -> hide();

    m_pMainGameWindow -> init();

    QPointF pos{m_pIntroductionWindow->pos()};
    m_pMainGameWindow->move(pos.x(), pos.y());

    m_pMainGameWindow -> show();
}

void AppManager::_onSwitchedHintsToMain()
{
    m_pHintsWindow -> hide();

    if (!(m_pMainGameWindow -> isInitialized()))
    {
        m_pMainGameWindow -> init();
    }

    QPointF pos{m_pHintsWindow->pos()};
    m_pMainGameWindow->move(pos.x(), pos.y());

    m_pMainGameWindow -> show();
}

void AppManager::_onSwitchedMainToHints()
{
    m_pMainGameWindow -> hide();

    QPointF pos{m_pMainGameWindow->pos()};
    m_pHintsWindow->move(pos.x(), pos.y());

    m_pHintsWindow -> show();
}

