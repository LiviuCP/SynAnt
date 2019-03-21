import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import GameManagers 1.0

ApplicationWindow {
    visible: true

    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480

    screen: Qt.application.screens[0]

    x: (Qt.application.screens[0].width - minimumWidth)/2
    y: (Qt.application.screens[0].height - minimumHeight)/2

    title: gamePresenter.windowTitle
    color: gamePresenter.backgroundColor

    property double windowMargins: width * 0.01

    GamePresenter {
        id: gamePresenter
    }

    IntroPane {
        id: introPane
        presenter: gamePresenter
        visible: gamePresenter.introPaneVisible
        anchors.fill: parent
        anchors.margins: windowMargins
    }

    MainPane {
        id: mainPane
        presenter: gamePresenter
        visible: gamePresenter.mainPaneVisible
        anchors.fill: parent
        anchors.margins: windowMargins
    }

    HelpPane {
        id: helpPane
        presenter: gamePresenter
        visible: gamePresenter.helpPaneVisible
        anchors.fill: parent
        anchors.margins: windowMargins
    }

    ErrorPane {
        id: errorPane
        presenter: gamePresenter
        visible: gamePresenter.errorOccured
        anchors.fill: parent
        anchors.margins: windowMargins
    }

    // shortcuts shared among multiple panes

    Shortcut {
        sequence: gamePresenter.playButtonShortcut
        onActivated: {
            gamePresenter.switchToPane(GamePresenter.MAIN);
        }
    }

    Shortcut {
        sequence: gamePresenter.helpButtonShortcut
        onActivated: {
            gamePresenter.switchToPane(GamePresenter.HELP);
        }
    }

    Shortcut {
        sequence: gamePresenter.quitButtonShortcut
        onActivated: {
            Qt.quit();
        }
    }
}
