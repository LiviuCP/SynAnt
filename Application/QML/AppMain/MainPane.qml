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

    readonly property double scoresLayoutHeight: height * 0.1
    readonly property double infoLevelsAndStatusLayoutHeight: height * 0.55
    readonly property double wordPiecesHeight: height * 0.06
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (infoLevelsAndStatusLayout.width - 4 * bottomBtnsLayout.spacing) * 0.2
    readonly property double buttonRadiusRatio: 0.035

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
        id: scoresLayout

        height: scoresLayoutHeight

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Rectangle {
            id: highscoresRect

            Layout.minimumWidth: (infoRect.width - scoresLayout.spacing) / 2
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

            Layout.minimumWidth: (infoRect.width - scoresLayout.spacing) / 2
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

        AppButton {
            id: resetBtn

            buttonEnabled: presenter.mainPaneStatisticsResetEnabled
            dedicatedShortcutEnabled: presenter.mainPaneVisible && presenter.mainPaneStatisticsResetEnabled

            Layout.minimumWidth: showPairBtn.width
            Layout.alignment: Qt.AlignRight
            Layout.leftMargin: 0.05 * mainPane.width

            buttonLabel: GameStrings.mainPaneStatisticsResetButtonLabel
            buttonToolTip: GameStrings.mainPaneResetButtonToolTip
            shortcutSequence: GameStrings.mainPaneResetButtonShortcut

            onButtonClicked: presenter.handleMainPaneStatisticsResetRequest()
        }

        AppButton {
            id: dataEntryBtn

            buttonEnabled: presenter.dataEntry.dataEntryEnabled
            dedicatedShortcutEnabled: false

            Layout.minimumWidth: quitBtn.width

            buttonLabel: GameStrings.dataEntryButtonLabel
            buttonToolTip: GameStrings.dataEntryButtonToolTip

            onButtonClicked: presenter.switchToPane(GamePresenter.DATA_ENTRY)
        }
    }

    RowLayout {
        id: infoLevelsAndStatusLayout

        width: parent.width
        height: infoLevelsAndStatusLayoutHeight

        anchors {
            top: scoresLayout.bottom
            topMargin: parent.height * 0.05
        }

        Rectangle {
            id: infoRect

            Layout.minimumWidth: parent.width * 0.55
            Layout.minimumHeight: parent.height

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: infoMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.mainPaneInstructionsBoxToolTip
                visible: infoMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                text: GameStrings.mainPaneInstructionsMessage
                color: Styles.textColor
                width: parent.width
                wrapMode: Text.WordWrap

                anchors {
                    fill: parent
                    leftMargin: parent.width * 0.01
                    rightMargin: parent.width * 0.01
                }
            }
        }

        Rectangle {
            id: levelBtnsRect

            Layout.minimumWidth: parent.width * 0.15
            Layout.minimumHeight: parent.height * 0.6

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

                RadioButton {
                    id: easyLvlBtn

                    checked: false
                    text: GameStrings.levelEasyButtonLabel

                    Shortcut {
                        sequence: GameStrings.levelEasyButtonShortcut
                        enabled: presenter.mainPaneVisible

                        onActivated: {
                            if (!easyLvlBtn.checked) {
                                easyLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelEasy);
                            }
                        }
                    }

                    onToggled: {
                        checked = true;
                        presenter.switchToLevel(presenter.levelEasy);
                    }
                }

                RadioButton {
                    id: mediumLvlBtn

                    checked: true
                    text: GameStrings.levelMediumButtonLabel

                    Shortcut {
                        sequence: GameStrings.levelMediumButtonShortcut
                        enabled: presenter.mainPaneVisible

                        onActivated: {
                            if (!mediumLvlBtn.checked) {
                                mediumLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelMedium);
                            }
                        }
                    }

                    onToggled: {
                        checked = true;
                        presenter.switchToLevel(presenter.levelMedium);
                    }
                }

                RadioButton {
                    id: hardLvlBtn

                    checked: false
                    text: GameStrings.levelHardButtonLabel

                    Shortcut {
                        sequence: GameStrings.levelHardButtonShortcut
                        enabled: presenter.mainPaneVisible

                        onActivated: {
                            if (!hardLvlBtn.checked) {
                                hardLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelHard);
                            }
                        }
                    }

                    onToggled: {
                        checked = true;
                        presenter.switchToLevel(presenter.levelHard);
                    }
                }
            }
        }

        Rectangle {
            id: statusRect

            Layout.minimumWidth: parent.width * 0.242
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: statusMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.mainPaneStatusBoxToolTip
                visible: statusMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                text: presenter.mainPaneStatusMessage
                color: Styles.textColor

                width: parent.width
                wrapMode: Text.WordWrap

                font.pointSize: 12
                minimumPointSize: 8
                fontSizeMode: Text.Fit

                anchors {
                    fill: parent
                    leftMargin: parent.width * 0.02
                    rightMargin: parent.width * 0.02
                }
            }
        }
    }

    Row {
        id: wordPieces

        height: wordPiecesHeight

        anchors {
            top: infoLevelsAndStatusLayout.bottom
            topMargin: parent.height * 0.025
            left: infoLevelsAndStatusLayout.left
            right: infoLevelsAndStatusLayout.right
        }

        Repeater {
            id: mixedWordsRepeater

            model: presenter.mixedWordsPiecesContent

            property Timer cursorSelectedPieceTimer : Timer {
                interval: 100
            }

            Rectangle {
                width: parent.width / mixedWordsRepeater.count
                height: parent.height

                visible: !presenter.mixedWordsPiecesSelections[index]

                color: Styles.mixedPiecesBackgroundColor

                border.color: index === presenter.pieceSelectionCursorPosition && !mixedWordsRepeater.cursorSelectedPieceTimer.running ? Styles.selectedBorderColor
                                                                                                                                              : Styles.mixedPiecesBorderColor
                border.width: index === presenter.pieceSelectionCursorPosition && !mixedWordsRepeater.cursorSelectedPieceTimer.running ? Styles.selectedBorderWidth
                                                                                                                                              : Styles.borderWidth

                opacity: (firstWordClickSelectedPieceTimer.running ||
                          secondWordClickSelectedPieceTimer.running ||
                          (mixedWordsRepeater.cursorSelectedPieceTimer.running &&
                           index === presenter.pieceSelectionCursorPosition)) ? Styles.pressedOpacity
                                                                              : Styles.defaultOpacity

                Timer {
                    id: clickTimer
                    interval: 250
                    onTriggered: {
                        firstWordClickSelectedPieceTimer.start();
                    }
                }

                Timer {
                    id: firstWordClickSelectedPieceTimer
                    interval: 100

                    onTriggered: presenter.selectPieceForFirstInputWord(index)
                }

                Timer {
                    id: secondWordClickSelectedPieceTimer
                    interval: 100

                    onTriggered: presenter.selectPieceForSecondInputWord(index)
                }

                Text {
                    text: modelData
                    font.pointSize: wordPieces.height * 0.4
                    anchors.centerIn: parent
                    color: presenter.mixedWordsPiecesTextColors[index]
                }

                MouseArea {
                    id: mixedWordsCurrentPieceMouseArea

                    hoverEnabled: true
                    anchors.fill: parent

                    // single click for assigning piece to first word, double click for assigning to second word
                    onClicked: {
                        if (clickTimer.running) {
                            secondWordClickSelectedPieceTimer.start();
                            clickTimer.stop();
                        }
                        else {
                            clickTimer.restart();
                        }
                    }
                }

                ToolTip {
                    text: presenter.mixedWordsPiecesSelections[index] ? GameStrings.wordPieceAlreadySelectedToolTip : GameStrings.selectWordPieceToolTip
                    visible: !presenter.persistentModeEnabled && mixedWordsCurrentPieceMouseArea.containsMouse
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                }
            }

            function animatePieceSelectedByCursor() {
                cursorSelectedPieceTimer.start();
            }
        }
    }

    Row {
        id: wordsEntryLayout

        height: wordsEntryLayoutHeight

        anchors {
            top: wordPieces.bottom
            topMargin: parent.width * 0.01
            left: wordPieces.left
            right: wordPieces.right
        }

        WordPiecesInput {
            id: firstWordInput
            gamePresenter: presenter
            pieceWidth: parent.width / mixedWordsRepeater.count
            pieceHeight: parent.height
        }

        Shortcut {
            sequence: GameStrings.clearMainPaneFirstInputWordShortcut
            enabled: presenter.mainPaneVisible

            onActivated: presenter.clearMainPaneFirstInputWord()
        }

        WordPiecesInput {
            id: secondWordInput
            gamePresenter: presenter
            pieceWidth: parent.width / mixedWordsRepeater.count
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
            left: wordsEntryLayout.left
            right: wordsEntryLayout.right
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
            mixedWordsRepeater.animatePieceSelectedByCursor();
        }
        else if (presenter.piecesRemovalFirstWordCursorPosition !== -1) {
            firstWordInput.animatePiecesRemovedByCursor();
        }
        else if (presenter.piecesRemovalSecondWordCursorPosition !== -1) {
            secondWordInput.animatePiecesRemovedByCursor();
        }
    }
}
