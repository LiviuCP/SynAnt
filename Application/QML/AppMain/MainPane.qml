import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

Item {
    id: mainPane

    property bool dataEntryButtonShortcutActivated: false
    property bool helpButtonShortcutActivated: false
    property bool quitButtonShortcutActivated: false
    property bool upArrowPressed: false
    property bool downArrowPressed: false

    property QtObject presenter

    readonly property double statisticsLayoutHeight: height * 0.1
    readonly property double mainLayoutHeight: height * 0.55
    readonly property double wordPiecesHeight: height * 0.06
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (mainLayout.width - 4 * bottomBtnsLayout.spacing) * 0.2

    MouseArea {
        id: mainPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.mainPaneToolTip
        visible: mainPaneMouseArea.containsMouse
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
    }

    RowLayout {
        id: statisticsLayout

        height: statisticsLayoutHeight
        width: parent.width

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Rectangle {
            id: highscoresRect

            Layout.minimumWidth: (statisticsLayout.width - 2 * statisticsLayout.spacing) / 3
            Layout.minimumHeight: parent.height / 2

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: highscoresMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.highscoresToolTip
                visible: highscoresMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                color: Styles.textColor
                text: presenter.mainPaneScoreMessage

                anchors {
                    left: parent.left
                    leftMargin: parent.width * 0.01
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle {
            id: wordPairsRect

            Layout.minimumWidth: 2 * (statisticsLayout.width - 0.5 * statisticsLayout.spacing) / 3
            Layout.minimumHeight: parent.height / 2

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: wordPairsMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.wordPairsToolTip
                visible: wordPairsMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                text: presenter.mainPaneWordPairsMessage
                color: Styles.textColor

                anchors {
                    left: parent.left
                    leftMargin: parent.width * 0.01
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        Item {
            id: placeholder
            Layout.fillWidth: true
        }
    }

    RowLayout {
        id: mainLayout

        width: parent.width
        height: mainLayoutHeight

        anchors {
            top: statisticsLayout.bottom
            topMargin: parent.height * 0.01
        }

        ColumnLayout {
            Layout.minimumWidth: highscoresRect.width
            Layout.minimumHeight: parent.height

            AppButton {
                id: resetBtn

                buttonEnabled: presenter.mainPaneStatisticsResetEnabled
                dedicatedShortcutEnabled: presenter.mainPaneVisible && presenter.mainPaneStatisticsResetEnabled

                Layout.minimumWidth: highscoresRect.width

                buttonLabel: GameStrings.mainPaneStatisticsResetButtonLabel
                buttonToolTip: GameStrings.mainPaneResetButtonToolTip
                shortcutSequence: GameStrings.mainPaneResetButtonShortcut

                onButtonClicked: presenter.handleMainPaneStatisticsResetRequest()
            }

            AppButton {
                id: dataEntryBtn

                buttonEnabled: presenter.dataEntry.dataEntryEnabled
                dedicatedShortcutEnabled: false

                Layout.minimumWidth: highscoresRect.width

                buttonLabel: GameStrings.dataEntryButtonLabel
                buttonToolTip: GameStrings.dataEntryButtonToolTip

                onButtonClicked: presenter.switchToPane(GamePresenter.DATA_ENTRY)
            }

            ComboBox {
                id: languageSelectionDropdown
                Layout.minimumWidth: highscoresRect.width
                editable: true
                model: ["Language"]
                onAccepted: console.log("Under construction")
            }

            Rectangle {
                id: levelBtnsRect

                Layout.minimumWidth: highscoresRect.width
                Layout.fillHeight: true

                color: Styles.backgroundColor
                border.color: Styles.borderColor

                MouseArea {
                    id: levelBtnsMouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                }

                ToolTip {
                    text: GameStrings.levelButtonsToolTip
                    visible: levelBtnsMouseArea.containsMouse
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                }

                ColumnLayout {
                    anchors.fill: parent

                    AppRadioButton {
                        id: easyLvlBtn

                        buttonChecked: false
                        dedicatedShortcutEnabled: presenter.mainPaneVisible

                        buttonLabel: GameStrings.levelEasyButtonLabel
                        shortcutSequence: GameStrings.levelEasyButtonShortcut

                        onButtonToggled: presenter.switchToLevel(presenter.levelEasy)
                    }

                    AppRadioButton {
                        id: mediumLvlBtn

                        buttonChecked: true
                        dedicatedShortcutEnabled: presenter.mainPaneVisible

                        buttonLabel: GameStrings.levelMediumButtonLabel
                        shortcutSequence: GameStrings.levelMediumButtonShortcut

                        onButtonToggled: presenter.switchToLevel(presenter.levelMedium)
                    }

                    AppRadioButton {
                        id: hardLvlBtn

                        buttonChecked: false
                        dedicatedShortcutEnabled: presenter.mainPaneVisible

                        buttonLabel: GameStrings.levelHardButtonLabel
                        shortcutSequence: GameStrings.levelHardButtonShortcut

                        onButtonToggled: presenter.switchToLevel(presenter.levelHard)
                    }
                }
            }
        }

        StatusBox {
            id: mainPaneStatusBox

            Layout.minimumWidth: wordPairsRect.width
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight

            statusMessage: presenter.mainPaneStatusMessage
            boxToolTip: GameStrings.mainPaneStatusBoxToolTip
        }
    }

    Row {
        id: firstWordsInputContainer

        height: wordPiecesHeight

        anchors {
            top: mainLayout.bottom
            topMargin: parent.width * 0.02
            left: mainLayout.left
            right: mainLayout.right
        }

        WordPiecesInput {
            id: firstWordInput
            gamePresenter: presenter
            pieceWidth: parent.width / mixedWordPieces.count
            pieceHeight: parent.height
        }

        Shortcut {
            sequence: GameStrings.clearMainPaneFirstInputWordShortcut
            enabled: presenter.mainPaneVisible

            onActivated: presenter.clearMainPaneFirstInputWord()
        }
    }

    Row {
        id: mixedWordPiecesContainer

        height: wordPiecesHeight

        anchors {
            top: firstWordsInputContainer.bottom
            topMargin: parent.height * 0.01
            left: mainLayout.left
            right: mainLayout.right
        }

        MixedWordPieces {
            id: mixedWordPieces

            gamePresenter: presenter
            pieceWidth: parent.width / mixedWordPieces.count
            pieceHeight: parent.height
        }
    }

    Row {
        id: secondWordsInputContainer

        height: wordPiecesHeight

        anchors {
            top: mixedWordPiecesContainer.bottom
            topMargin: parent.width * 0.01
            left: mainLayout.left
            right: mainLayout.right
        }

        WordPiecesInput {
            id: secondWordInput
            gamePresenter: presenter
            pieceWidth: parent.width / mixedWordPieces.count
            pieceHeight: parent.height
            isFirstWord: false
        }

        Shortcut {
            sequence: GameStrings.clearMainPaneSecondInputWordShortcut
            enabled: presenter.mainPaneVisible

            onActivated: presenter.clearMainPaneSecondInputWord()
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            bottom: parent.bottom
            left: mixedWordPiecesContainer.left
            right: mixedWordPiecesContainer.right
        }

        AppButton {
            id: submitBtn

            buttonEnabled: presenter.submitMainPaneInputEnabled
            dedicatedShortcutEnabled: presenter.mainPaneVisible && presenter.submitMainPaneInputEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.submitButtonLabel
            buttonToolTip: GameStrings.submitButtonToolTip
            shortcutSequence: GameStrings.submitButtonShortcut

            onButtonClicked: presenter.handleSubmitMainPaneInputRequest()
        }

        AppButton {
            id: clearInputBtn

            buttonEnabled: presenter.clearMainPaneInputEnabled
            dedicatedShortcutEnabled: presenter.mainPaneVisible && presenter.clearMainPaneInputEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.clearMainPaneInputButtonLabel
            buttonToolTip: GameStrings.clearMainPaneInputButtonToolTip
            shortcutSequence: GameStrings.clearMainPaneInputButtonShortcut

            onButtonClicked: presenter.clearMainPaneInput()
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
            id: showPairBtn

            dedicatedShortcutEnabled: presenter.mainPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.showPairButtonLabel
            buttonToolTip: GameStrings.showPairButtonToolTip
            shortcutSequence: GameStrings.showPairButtonShortcut

            onButtonClicked: presenter.handleDisplayCorrectWordsPairRequest()
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.2

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: presenter.quit()
        }
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

    onUpArrowPressedChanged: {
        if (upArrowPressed) {
            animatePiecesControlledByCursor();
            upArrowPressed = false;
        }
    }

    onDownArrowPressedChanged: {
        if (downArrowPressed) {
            animatePiecesControlledByCursor();
            downArrowPressed = false;
        }
    }

    function animatePiecesControlledByCursor() {
        if (presenter.pieceSelectionCursorPosition !== -1) {
            mixedWordPieces.animatePieceSelectedByCursor();
        }
        else if (presenter.piecesRemovalFirstWordCursorPosition !== -1) {
            firstWordInput.animatePiecesRemovedByCursor();
        }
        else if (presenter.piecesRemovalSecondWordCursorPosition !== -1) {
            secondWordInput.animatePiecesRemovedByCursor();
        }
    }
}
