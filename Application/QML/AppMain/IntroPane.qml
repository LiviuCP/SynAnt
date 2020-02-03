import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: introPane

    property QtObject presenter

    property bool dataEntryButtonShortcutActivated: false
    property bool helpButtonShortcutActivated: false
    property bool quitButtonShortcutActivated: false

    readonly property double introPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (introPaneRect.width - 3 * bottomBtnsLayout.spacing) / 4
    readonly property double buttonRadiusRatio: 0.025

    MouseArea {
        id: introPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.introPaneToolTip
        visible: introPaneMouseArea.containsMouse
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
    }

    Rectangle {
        id: introPaneRect

        width: parent.width
        height: introPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Text {
            id: introPaneText

            text: presenter.introPaneMessage
            color: Styles.textColor

            anchors {
                fill: parent
                leftMargin: parent.width * 0.005
            }


        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            top: introPaneRect.bottom
            bottom: parent.bottom
            topMargin: parent.height * 0.01
            left: introPaneRect.left
            right: introPaneRect.right
        }

        Button {
            id: playBtn

            enabled: presenter.playEnabled

            property double buttonOpacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.playButtonLabel
                color: Styles.textColor
                opacity: playBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: playBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            // we will not use the Styles functions here as we need the prompt page to be activated when the timer triggers
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    playBtn.opacity = Styles.releasedButtonOpacity;
                    presenter.switchToPane(GamePresenter.MAIN);
                }
            }

            ToolTip {
                text: GameStrings.playButtonToolTip
                visible: playBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.playButtonShortcut
                enabled: presenter.playEnabled && presenter.introPaneVisible

                onActivated: {
                    playBtn.opacity = Styles.pressedButtonOpacity;
                    playBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: presenter.switchToPane(GamePresenter.MAIN)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: dataEntryBtn

            enabled: presenter.dataEntry.dataEntryEnabled

            property double buttonOpacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.dataEntryButtonLabel
                color: Styles.textColor
                opacity: dataEntryBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: dataEntryBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.dataEntryButtonToolTip
                visible: dataEntryBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.switchToPane(GamePresenter.DATA_ENTRY)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: helpBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.helpButtonLabel
                color: Styles.textColor
                opacity: helpBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: helpBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: presenter.helpButtonToolTip
                visible: helpBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.switchToPane(GamePresenter.HELP)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.5

            contentItem: Text {
                text: GameStrings.quitButtonLabel
                color: Styles.textColor
                opacity: quitBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: quitBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.quitButtonToolTip
                visible: quitBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    Connections {
        target: presenter.dataEntry

        onDataSaveInProgress: presenter.handleDataSaveInProgress()
    }

    onDataEntryButtonShortcutActivatedChanged: {
        if (dataEntryButtonShortcutActivated)
        {
            Styles.updateButtonOpacityAtShortcutActivation(dataEntryBtn);
            dataEntryButtonShortcutActivated = false;
        }
    }

    onHelpButtonShortcutActivatedChanged: {
        if (helpButtonShortcutActivated)
        {
            Styles.updateButtonOpacityAtShortcutActivation(helpBtn);
            helpButtonShortcutActivated = false;
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        if (quitButtonShortcutActivated)
        {
            Styles.updateButtonOpacityAtShortcutActivation(quitBtn);
            quitButtonShortcutActivated = false;
        }
    }
}
