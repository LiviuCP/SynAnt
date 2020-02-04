import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

Item {
    id: helpPane

    property QtObject presenter

    property bool quitButtonShortcutActivated: false

    readonly property double helpPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneRect.width - bottomBtnsLayout.spacing) * 0.5
    readonly property double buttonRadiusRatio: 0.01

    MouseArea {
        id: helpPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: presenter.helpPaneToolTip
        visible: helpPaneMouseArea.containsMouse
    }

    Rectangle {
        id: helpPaneRect

        width: parent.width
        height: helpPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Flickable {
            id: helpPaneFlickable

            clip: true
            contentHeight: helpPaneText.height

            anchors {
                fill: parent
                leftMargin: parent.width * 0.005
                rightMargin: parent.width * 0.005
            }

            Text {
                id: helpPaneText
                text: presenter.helpPaneMessage
                color: Styles.textColor
                width: parent.width
                wrapMode: Text.WordWrap
            }
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors {
            top: helpPaneRect.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: helpPaneRect.left
            right: helpPaneRect.right
        }

        height: bottomBtnsLayoutHeight

        AppButton {
            id: okBtn

            dedicatedShortcutEnabled: presenter.helpPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.okButtonLabel
            buttonToolTip: GameStrings.okButtonToolTip
            shortcutSequence: GameStrings.okButtonShortcut

            onButtonClicked: presenter.goBack()
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: presenter.quit()
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        Styles.updateButtonOpacityAtShortcutActivation(quitBtn);
        quitButtonShortcutActivated = false;
    }
}
