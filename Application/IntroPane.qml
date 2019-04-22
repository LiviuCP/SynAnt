import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: introPane

    property QtObject presenter

    readonly property double introPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: (introPaneRect.width - 2 * bottomBtnsLayout.spacing) / 3

    MouseArea {
        id: introPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: introPaneToolTip
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: GameStrings.introPaneToolTip
        visible: introPaneMouseArea.containsMouse
    }

    Rectangle {
        id: introPaneRect
        anchors.top: parent.top
        width: parent.width
        height: introPaneRectHeight
        color: Styles.backgroundColor

        border {
            color: Styles.borderColor
        }

        Text {
            id: introPaneText
            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.005
            text: GameStrings.introPaneMessage
            color: Styles.textColor
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors.top: introPaneRect.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height * 0.01
        anchors.left: introPaneRect.left
        anchors.right: introPaneRect.right

        height: bottomBtnsLayoutHeight

        Button {
            id: playBtn

            contentItem: Text {
                text: GameStrings.playButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.playButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: presenter.switchToPane(GamePresenter.MAIN)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: helpBtn

            contentItem: Text {
                text: GameStrings.helpButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.helpButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: presenter.switchToPane(GamePresenter.HELP)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            contentItem: Text {
                text: GameStrings.quitButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: Qt.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
