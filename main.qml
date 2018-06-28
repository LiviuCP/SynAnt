import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import GameManagers 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
//    maximumWidth: 640
//    maximumHeight: 480
    title: qsTr("SynAnt")
    color: "grey"

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
}
