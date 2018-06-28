import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: helpPane

    property QtObject presenter;

    readonly property double helpPaneRectHeight: height * 0.9;
    readonly property double bottomBtnsLayoutHeight: height * 0.1;
    readonly property double bottomBtnsMinWidth: (helpPaneRect.width - bottomBtnsLayout.spacing) * 0.5

    Rectangle {
        id: helpPaneRect
        anchors.top: parent.top
        width: parent.width
        height: helpPaneRectHeight
        color: "grey"
        border {
            color: "white"
        }

    }

    RowLayout {
        id: bottomBtnsLayout
        anchors.top: helpPaneRect.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height * 0.01
        anchors.left: helpPaneRect.left
        anchors.right: helpPaneRect.right
        height: bottomBtnsLayoutHeight

        Button {
            id: okBtn
            text: "Ok"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: {
                presenter.switchToMainPane();
            }
        }

        Button {
            id: quitBtn
            text: "Quit"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: Qt.quit()
        }
    }
}
