import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: dataEntryPane

    property QtObject presenter

    property bool helpButtonShortcutActivated: false
    property bool quitButtonShortcutActivated: false

    readonly property double dataEntryPaneRectHeight: height * 0.6
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double synAntRadioButtonLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (dataEntryStatusBox.width - 5 * bottomBtnsLayout.spacing) / 6
    readonly property double buttonRadiusRatio: 0.05



    function clearTextFields() {
        firstWordTextField.clear();
        secondWordTextField.clear();
        firstWordTextField.forceActiveFocus();
        clearBtn.enabled = false;
    }

    function handleBackButtonAction() {
        if (presenter.dataEntry.saveAddedWordPairsEnabled) {
            presenter.promptForSavingAddedWordPairs();
        } else {
            presenter.goBack();
        }
    }

    MouseArea {
        id: entryPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.dataEntryPaneToolTip
        visible: entryPaneMouseArea.containsMouse
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
    }

    Rectangle {
        id: dataEntryStatusBox

        width: parent.width
        height: dataEntryPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Text {
            text: presenter.dataEntry.dataEntryPaneStatusMessage;
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }

    RowLayout {
        id: synAntRadioButtonsLayout

        height: wordsEntryLayoutHeight

        anchors {
            top: dataEntryStatusBox.bottom
            topMargin: parent.width * 0.01
            left: dataEntryStatusBox.left
            right: dataEntryStatusBox.right
        }

        RadioButton {
            id: synonymsSelectionButton
            checked: true
            text: GameStrings.synonymsSelectionButtonLabel
            onToggled: checked = true
        }

        RadioButton {
            id: antonymsSelectionButton
            checked: false
            text: GameStrings.antonymsSelectionButtonLabel
            onToggled: checked = true
        }
    }

    RowLayout {
        id: wordsEntryLayout

        height: wordsEntryLayoutHeight

        anchors {
            top: synAntRadioButtonsLayout.bottom
            topMargin: parent.width * 0.05
            left: synAntRadioButtonsLayout.left
            right: synAntRadioButtonsLayout.right
        }

        TextField {
            id: firstWordTextField

            Layout.minimumWidth: parent.width * 0.414
            Layout.minimumHeight: parent.height

            ToolTip {
                text: GameStrings.dataEntryFirstWordToolTip
                visible: firstWordTextField.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onVisibleChanged: {
                if (visible) {
                    forceActiveFocus();
                }
            }

            Keys.onReleased: clearBtn.enabled = (firstWordTextField.text.length != 0 || secondWordTextField.text.length != 0) ? true : false
        }

        TextField {
            id: secondWordTextField

            Layout.minimumWidth: parent.width * 0.414
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight

            ToolTip {
                text: GameStrings.dataEntrySecondWordToolTip
                visible: secondWordTextField.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Keys.onReleased: clearBtn.enabled = (firstWordTextField.text.length != 0 || secondWordTextField.text.length != 0) ? true : false
        }

        Button {
            id: clearBtn

            enabled: false

            property double buttonOpacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.dataEntryClearButtonLabel
                color: Styles.textColor
                opacity: clearBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: clearBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.dataEntryClearButtonToolTip
                visible: clearBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.dataEntryClearButtonShortcut
                enabled: clearBtn.enabled && presenter.dataEntryPaneVisible

                onActivated: dataEntryPane.clearTextFields()
            }

            onClicked: dataEntryPane.clearTextFields()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            top: wordsEntryLayout.bottom
            topMargin: parent.height * 0.05
            bottom: parent.bottom
            left: wordsEntryLayout.left
            right: wordsEntryLayout.right
        }

        Button {
            id: addPairBtn

            enabled: presenter.dataEntry.addWordsPairEnabled

            property double buttonOpacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.addPairButtonLabel
                color: Styles.textColor
                opacity: addPairBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: addPairBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.addPairButtonToolTip
                visible: addPairBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.addPairButtonShortcut
                enabled: addPairBtn.enabled && presenter.dataEntryPaneVisible

                onActivated: {
                    Styles.updateButtonOpacityAtShortcutActivation(addPairBtn);
                    presenter.dataEntry.handleAddWordsPairRequest(firstWordTextField.text, secondWordTextField.text, synonymsSelectionButton.checked);
                }
            }

            onClicked: presenter.dataEntry.handleAddWordsPairRequest(firstWordTextField.text, secondWordTextField.text, synonymsSelectionButton.checked)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: backBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.backButtonLabel
                color: Styles.textColor
                opacity: backBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: backBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.backButtonToolTip
                visible: backBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            // we will not use the Styles functions here as we need the prompt page to be activated when the timer triggers
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    backBtn.opacity = Styles.releasedButtonOpacity;
                    dataEntryPane.handleBackButtonAction();
                }
            }

            Shortcut {
                sequence: GameStrings.backButtonShortcut
                enabled: presenter.dataEntryPaneVisible

                onActivated: {
                    backBtn.opacity = Styles.pressedButtonOpacity;
                    backBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: dataEntryPane.handleBackButtonAction()
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
                text: GameStrings.helpButtonToolTip
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
            id: discardBtn

            enabled: presenter.dataEntry.discardAddedWordPairsEnabled

            property double buttonOpacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.discardButtonLabel
                color: Styles.textColor
                opacity: discardBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: discardBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.discardButtonToolTip
                visible: discardBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            // we will not use the Styles functions here as we need the prompt page to be activated when the timer triggers
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    discardBtn.opacity = Styles.releasedButtonOpacity;
                    presenter.promptForDiscardingAddedWordPairs();
                    firstWordTextField.forceActiveFocus();
                }
            }

            Shortcut {
                sequence: GameStrings.discardButtonShortcut
                enabled: discardBtn.enabled && presenter.dataEntryPaneVisible

                onActivated: {
                    discardBtn.opacity = Styles.pressedButtonOpacity;
                    discardBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: {
                presenter.promptForDiscardingAddedWordPairs();
                firstWordTextField.forceActiveFocus();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: saveBtn

            enabled: presenter.dataEntry.saveAddedWordPairsEnabled

            property double buttonOpacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.saveButtonLabel
                color: Styles.textColor
                opacity: saveBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: saveBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.saveButtonToolTip
                visible: saveBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.saveButtonShortcut
                enabled: saveBtn.enabled && presenter.dataEntryPaneVisible

                onActivated: {
                    presenter.dataEntry.handleSaveAddedWordPairsRequest();
                    firstWordTextField.forceActiveFocus();
                }
            }

            onClicked: {
                presenter.dataEntry.handleSaveAddedWordPairsRequest();
                firstWordTextField.forceActiveFocus();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

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

            onClicked: {
                if (presenter.dataEntry.saveAddedWordPairsEnabled) {
                    presenter.quitGameDeferred = true;
                    presenter.promptForSavingAddedWordPairs();
                } else {
                    presenter.quit();
                }
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    Connections {
        target: presenter.dataEntry

        onDataEntryAddSucceeded: dataEntryPane.clearTextFields()
        onDataEntryAddInvalid: firstWordTextField.forceActiveFocus()
        onDataEntryStopped: dataEntryPane.clearTextFields()
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
