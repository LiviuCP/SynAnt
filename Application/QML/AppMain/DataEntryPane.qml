import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

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

    // keep mouse area and tooltip as the status box doesn't cover most part of the window (like in other panes)
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

    StatusBox {
        id: dataEntryStatusBox

        width: parent.width
        height: dataEntryPaneRectHeight

        anchors.top: parent.top

        statusMessage: presenter.dataEntry.dataEntryPaneStatusMessage
        boxToolTip: GameStrings.dataEntryPaneToolTip
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

        AppRadioButton {
            id: synonymsSelectionButton

            Layout.minimumWidth: 1.2655 * quitBtn.width

            buttonChecked: true
            isActiveFocusOnTabEnabled: true
            dedicatedShortcutEnabled: false

            buttonLabel: GameStrings.synonymsSelectionButtonLabel

            onButtonToggled: firstWordTextField.forceActiveFocus()
        }

        AppRadioButton {
            id: antonymsSelectionButton

            Layout.minimumWidth: 1.2655 * quitBtn.width

            buttonChecked: false
            isActiveFocusOnTabEnabled: true
            dedicatedShortcutEnabled: false

            buttonLabel: GameStrings.antonymsSelectionButtonLabel

            onButtonToggled: firstWordTextField.forceActiveFocus()
        }

        Dropdown {
            id: languageSelectionDropdown

            Layout.fillWidth: true

            dataModel: ["English", "German"]
            dropdownToolTip: GameStrings.dataEntryLanguageSelectionToolTip

            dropdownEnabled: presenter.languageSelectionEnabled

            onItemChanged: console.log("Item changed, current index is now: " + currentIndex)
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

            activeFocusOnTab: true

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

            activeFocusOnTab: true

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

        AppButton {
            id: clearBtn

            isActiveFocusOnTabEnabled: false
            buttonEnabled: false
            dedicatedShortcutEnabled: enabled && presenter.dataEntryPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.dataEntryClearButtonLabel
            buttonToolTip: GameStrings.dataEntryClearButtonToolTip
            shortcutSequence: GameStrings.dataEntryClearButtonShortcut

            onButtonClicked: dataEntryPane.clearTextFields()
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

        AppButton {
            id: addPairBtn

            isActiveFocusOnTabEnabled: false
            buttonEnabled: presenter.dataEntry.addWordsPairEnabled
            dedicatedShortcutEnabled: presenter.dataEntryPaneVisible && presenter.dataEntry.addWordsPairEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.addPairButtonLabel
            buttonToolTip: GameStrings.addPairButtonToolTip
            shortcutSequence: GameStrings.addPairButtonShortcut

            onButtonClicked: presenter.dataEntry.handleAddWordsPairRequest(firstWordTextField.text, secondWordTextField.text, synonymsSelectionButton.checked)
        }

        AppButton {
            id: backBtn

            isActiveFocusOnTabEnabled: false
            dedicatedShortcutEnabled: presenter.dataEntryPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.backButtonLabel
            buttonToolTip: GameStrings.backButtonToolTip
            shortcutSequence: GameStrings.backButtonShortcut

            onButtonClicked: dataEntryPane.handleBackButtonAction()
        }

        AppButton {
            id: helpBtn

            isActiveFocusOnTabEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.helpButtonLabel
            buttonToolTip: presenter.helpButtonToolTip

            onButtonClicked: presenter.switchToPane(GamePresenter.HELP)
        }

        AppButton {
            id: discardBtn

            isActiveFocusOnTabEnabled: false
            buttonEnabled: presenter.dataEntry.discardAddedWordPairsEnabled
            dedicatedShortcutEnabled: presenter.dataEntryPaneVisible && presenter.dataEntry.discardAddedWordPairsEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.discardButtonLabel
            buttonToolTip: GameStrings.discardButtonToolTip
            shortcutSequence: GameStrings.discardButtonShortcut

            onButtonClicked: {
                presenter.promptForDiscardingAddedWordPairs();
                firstWordTextField.forceActiveFocus();
            }
        }

        AppButton {
            id: saveBtn

            isActiveFocusOnTabEnabled: false
            buttonEnabled: presenter.dataEntry.saveAddedWordPairsEnabled
            dedicatedShortcutEnabled: presenter.dataEntryPaneVisible && presenter.dataEntry.saveAddedWordPairsEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.saveButtonLabel
            buttonToolTip: GameStrings.saveButtonToolTip
            shortcutSequence: GameStrings.saveButtonShortcut

            onButtonClicked: {
                presenter.dataEntry.handleSaveAddedWordPairsRequest();
                firstWordTextField.forceActiveFocus();
            }
        }

        AppButton {
            id: quitBtn

            isActiveFocusOnTabEnabled: false
            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: {
                if (presenter.dataEntry.saveAddedWordPairsEnabled) {
                    presenter.quitGameDeferred = true;
                    presenter.promptForSavingAddedWordPairs();
                } else {
                    presenter.quit();
                }
            }
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
