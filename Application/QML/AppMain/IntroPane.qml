import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

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

        AppButton {
            id: playBtn

            buttonEnabled: presenter.playEnabled
            dedicatedShortcutEnabled: presenter.introPaneVisible && presenter.playEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.playButtonLabel
            buttonToolTip: GameStrings.playButtonToolTip
            shortcutSequence: GameStrings.playButtonShortcut

            onButtonClicked: presenter.switchToPane(GamePresenter.MAIN)
        }

        AppButton {
            id: dataEntryBtn

            buttonEnabled: presenter.dataEntry.dataEntryEnabled
            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.dataEntryButtonLabel
            buttonToolTip: GameStrings.dataEntryButtonToolTip

            onButtonClicked: presenter.switchToPane(GamePresenter.DATA_ENTRY)
        }

        AppButton {
            id: helpBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.helpButtonLabel
            buttonToolTip: presenter.helpButtonToolTip

            onButtonClicked: presenter.switchToPane(GamePresenter.HELP)
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.5

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: presenter.quit()
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
