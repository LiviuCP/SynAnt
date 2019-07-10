import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: dataEntryPane

    property QtObject presenter

    readonly property double dataEntryPaneRectHeight: height * 0.6
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double synAntRadioButtonLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (dataEntryStatusBox.width - 5 * bottomBtnsLayout.spacing) / 6

    function clearTextFields() {
        firstWordTextField.clear();
        secondWordTextField.clear();
        firstWordTextField.forceActiveFocus();
        clearBtn.enabled = false;
    }

    MouseArea {
        id: entryPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: dataEntryPaneToolTip
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: GameStrings.dataEntryPaneToolTip
        visible: entryPaneMouseArea.containsMouse
    }

    Rectangle {
        id: dataEntryStatusBox
        anchors.top: parent.top
        width: parent.width
        height: dataEntryPaneRectHeight
        color: Styles.backgroundColor

        border {
            color: Styles.borderColor
        }

        Text {
            text: presenter.dataEntryPaneStatusMessage;
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }

    RowLayout {
        id: synAntRadioButtonsLayout

        anchors.top: dataEntryStatusBox.bottom
        anchors.left: dataEntryStatusBox.left
        anchors.right: dataEntryStatusBox.right
        anchors.topMargin: parent.width * 0.01

        height: wordsEntryLayoutHeight

        RadioButton {
            id: synonymsSelectionButton
            checked: true

            text: GameStrings.synonymsSelectionButtonLabel

            onToggled: {
                checked = true;
            }
        }

        RadioButton {
            id: antonymsSelectionButton
            checked: false

            text: GameStrings.antonymsSelectionButtonLabel

            onToggled: {
                checked = true;
            }
        }
    }

    RowLayout {
        id: wordsEntryLayout

        anchors.top: synAntRadioButtonsLayout.bottom
        anchors.left: synAntRadioButtonsLayout.left
        anchors.right: synAntRadioButtonsLayout.right
        anchors.topMargin: parent.width * 0.05

        height: wordsEntryLayoutHeight

        TextField {
            id: firstWordTextField

            Layout.minimumWidth: parent.width * 0.414
            Layout.minimumHeight: parent.height

            ToolTip.text: GameStrings.dataEntryFirstWordToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

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

            ToolTip.text: GameStrings.dataEntrySecondWordToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Keys.onReleased: clearBtn.enabled = (firstWordTextField.text.length != 0 || secondWordTextField.text.length != 0) ? true : false
        }

        Button {
            id: clearBtn
            enabled: false

            contentItem: Text {
                text: GameStrings.dataEntryClearButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.05
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.dataEntryClearButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.dataEntryClearButtonShortcut
                enabled: clearBtn.enabled && clearBtn.visible
                onActivated: {
                    dataEntryPane.clearTextFields();
                }
            }

            onClicked: dataEntryPane.clearTextFields()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity


        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors.top: wordsEntryLayout.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height * 0.05
        anchors.left: wordsEntryLayout.left
        anchors.right: wordsEntryLayout.right

        height: bottomBtnsLayoutHeight

        Button {
            id: addPairBtn
            enabled: presenter.addDataEntryEnabled

            contentItem: Text {
                text: GameStrings.addPairButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.05
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.addPairButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.addPairButtonShortcut
                enabled: addPairBtn.enabled
                onActivated: {
                    presenter.handleAddWordsPairRequest(firstWordTextField.text, secondWordTextField.text, synonymsSelectionButton.checked);
                }
            }

            onClicked: presenter.handleAddWordsPairRequest(firstWordTextField.text, secondWordTextField.text, synonymsSelectionButton.checked)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: backBtn

            contentItem: Text {
                text: GameStrings.backButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.05
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.backButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.backButtonShortcut
                onActivated: {
                    presenter.goBack();
                }
            }

            onClicked: presenter.goBack()
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
                radius: width * 0.05
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
            id: discardBtn
            enabled: presenter.clearDataEntryBufferEnabled

            contentItem: Text {
                text: GameStrings.discardButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.05
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.discardButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.discardButtonShortcut
                enabled: discardBtn.enabled
                onActivated: {
                    presenter.handleClearDataEntryBufferRequest();
                }
            }

            onClicked: presenter.handleClearDataEntryBufferRequest()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: saveBtn
            enabled: presenter.saveEntriesEnabled

            contentItem: Text {
                text: GameStrings.saveButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.05
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.saveButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.saveButtonShortcut
                enabled: saveBtn.enabled
                onActivated: {
                    presenter.handleSaveNewWordPairsRequest();
                }
            }

            onClicked: presenter.handleSaveNewWordPairsRequest()
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
                radius: width * 0.05
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: presenter.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    Connections {
        target: presenter
        onDataEntrySucceeded: {
            firstWordTextField.clear();
            secondWordTextField.clear();
            firstWordTextField.forceActiveFocus();
        }
    }
}
