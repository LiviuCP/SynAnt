import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0

Item {
    id: mainPane

    property QtObject presenter

    readonly property double scoresLayoutHeight: height * 0.1
    readonly property double infoLevelsAndStatusLayoutHeight: height * 0.55
    readonly property double wordPiecesHeight: height * 0.06
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: (infoLevelsAndStatusLayout.width - 3 * bottomBtnsLayout.spacing) * 0.25

    readonly property double pressedButtonOpacity: 0.5
    readonly property double releasedButtonOpacity: 1.0
    readonly property double disabledButtonOpacity: 0.3

    readonly property color paneColor: presenter.backgroundColor
    readonly property color buttonColor: presenter.pushButtonColor
    readonly property color borderColor: presenter.borderColor
    readonly property color textColor: presenter.textColor
    readonly property color wordFirstPieceColor: presenter.wordFirstPieceColor
    readonly property color wordLastPieceColor: presenter.wordLastPieceColor
    readonly property color wordPieceSelectedColor: presenter.wordPieceSelectedColor

    function switchToLvl(lvl) {
        firstWordTextField.clear();
        secondWordTextField.clear();
        presenter.switchToLevel(lvl);
        firstWordTextField.forceActiveFocus();
    }

    MouseArea {
        id: mainPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: mainPaneToolTip
        delay: presenter.toolTipDelay
        text: presenter.mainPaneToolTip
        visible: mainPaneMouseArea.containsMouse
    }

    RowLayout {
        id: scoresLayout

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: scoresLayoutHeight

        Rectangle {
            id: highscoresRect

            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2

            color: paneColor

            border {
                color: borderColor
            }

            MouseArea {
                id: highscoresMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: highscoresToolTip
                delay: presenter.toolTipDelay
                text: presenter.highscoresToolTip
                visible: highscoresMouseArea.containsMouse
            }

            Text {
                color: textColor
                text: presenter.mainPaneScoreMessage

                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            id: wordPairsRect

            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2
            Layout.leftMargin: parent.width * (-0.04)

            color: paneColor

            border {
                color: borderColor
            }

            MouseArea {
                id: wordPairsMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: wordPairsToolTip
                delay: presenter.toolTipDelay
                text: presenter.wordPairsToolTip
                visible: wordPairsMouseArea.containsMouse
            }

            Text {
                color: textColor
                text: presenter.mainPaneWordPairsMessage

                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Button {
            id: resetBtn

            contentItem: Text {
                text: presenter.resetButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? releasedButtonOpacity : disabledButtonOpacity
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
                opacity: enabled ? releasedButtonOpacity : disabledButtonOpacity
            }

            Layout.minimumWidth: quitBtn.width
            Layout.alignment: Qt.AlignRight

            ToolTip.text: presenter.resetButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            enabled: presenter.resetEnabled

            Shortcut {
                sequence: presenter.resetButtonShortcut
                onActivated: {
                    presenter.handleResetRequest();
                }
            }

            onClicked: {
                presenter.handleResetRequest();
                firstWordTextField.forceActiveFocus();
            }

            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }
    }

    RowLayout {
        id: infoLevelsAndStatusLayout

        anchors.top: scoresLayout.bottom
        anchors.topMargin: parent.height * 0.05

        width: parent.width
        height: infoLevelsAndStatusLayoutHeight

        Rectangle {
            id: infoRect

            Layout.minimumWidth: parent.width * 0.55
            Layout.minimumHeight: parent.height

            color: paneColor

            border {
                color: borderColor
            }

            MouseArea {
                id: infoMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: infoToolTip
                delay: presenter.toolTipDelay
                text: presenter.gameInstructionsToolTip
                visible: infoMouseArea.containsMouse
            }

            Text {
                color: textColor
                text: presenter.mainPaneInstructionsMessage

                anchors.fill: parent
                anchors.leftMargin: parent.width * 0.01
                anchors.rightMargin: parent.width * 0.01

                width: parent.width
                wrapMode: Text.WordWrap
            }
        }

        Rectangle {
            id: levelBtnsRect

            Layout.minimumWidth: parent.width * 0.15
            Layout.minimumHeight: parent.height * 0.6

            color: paneColor

            border {
                color: borderColor
            }

            MouseArea {
                id: levelBtnsMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: levelBtnsToolTip
                delay: presenter.toolTipDelay
                text: presenter.levelButtonsToolTip
                visible: levelBtnsMouseArea.containsMouse
            }

            ColumnLayout {
                anchors.fill: parent

                RadioButton {
                    id: easyLvlBtn
                    text: presenter.levelEasyButtonLabel
                    checked: false

                    Shortcut {
                        sequence: presenter.levelEasyButtonShortcut
                        onActivated: {
                            if (!easyLvlBtn.checked) {
                                easyLvlBtn.checked = true;
                                mainPane.switchToLvl(presenter.levelEasy);
                            }
                        }
                    }

                    onToggled: {
                        checked = true
                        mainPane.switchToLvl(presenter.levelEasy);
                    }
                }

                RadioButton {
                    id: mediumLvlBtn
                    text: presenter.levelMediumButtonLabel
                    checked: true

                    Shortcut {
                        sequence: presenter.levelMediumButtonShortcut
                        onActivated: {
                            if (!mediumLvlBtn.checked) {
                                mediumLvlBtn.checked = true;
                                mainPane.switchToLvl(presenter.levelMedium);
                            }
                        }
                    }

                    onToggled: {
                        checked = true
                        mainPane.switchToLvl(presenter.levelMedium);
                    }
                }

                RadioButton {
                    id: hardLvlBtn
                    text: presenter.levelHardButtonLabel
                    checked: false

                    Shortcut {
                        sequence: presenter.levelHardButtonShortcut
                        onActivated: {
                            if (!hardLvlBtn.checked) {
                                hardLvlBtn.checked = true;
                                mainPane.switchToLvl(presenter.levelHard);
                            }
                        }
                    }

                    onToggled: {
                        checked = true
                        mainPane.switchToLvl(presenter.levelHard);
                    }
                }
            }
        }

        Rectangle {
            id: statusRect

            Layout.minimumWidth: parent.width * 0.242
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight

            color: paneColor

            border {
                color: borderColor
            }

            MouseArea {
                id: statusMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: statusToolTip
                delay: presenter.toolTipDelay
                text: presenter.gameStatusToolTip
                visible: statusMouseArea.containsMouse
            }

            Text {
                color: textColor

                text: presenter.mainPaneStatusMessage

                font.pointSize: 12
                minimumPointSize: 8
                fontSizeMode: Text.Fit

                anchors.fill: parent
                anchors.leftMargin: parent.width * 0.02
                anchors.rightMargin: parent.width * 0.02

                width: parent.width
                wrapMode: Text.WordWrap
            }
        }
    }

    Row {
        id: wordPieces

        anchors.top: infoLevelsAndStatusLayout.bottom
        anchors.left: infoLevelsAndStatusLayout.left
        anchors.right: infoLevelsAndStatusLayout.right
        anchors.topMargin: parent.height * 0.025

        height: wordPiecesHeight

        Repeater {
            id: mixedWordsRepeater
            model: presenter.mixedWordsPieces

            Rectangle {
                id: wordPiecesRectangle
                width: parent.width / mixedWordsRepeater.count
                height: parent.height

                color: paneColor
                border.color: borderColor

                Text {
                    font.pointSize: wordPieces.height * 0.4
                    anchors.centerIn: parent
                    text: modelData
                    color: index === presenter.firstWordBeginIndex || index === presenter.secondWordBeginIndex ? wordFirstPieceColor :
                           index === presenter.firstWordEndIndex || index === presenter.secondWordEndIndex ? wordLastPieceColor :
                           textColor
                }

                MouseArea {
                    id: wordPiecesMouseArea
                    anchors.fill: parent
                    onClicked: wordPiecesRectangle.color = wordPiecesRectangle.color === paneColor ? wordPieceSelectedColor : paneColor
                }
            }
        }
    }

    RowLayout {
        id: wordsEntryLayout

        anchors.top: wordPieces.bottom
        anchors.left: wordPieces.left
        anchors.right: wordPieces.right
        anchors.topMargin: parent.width * 0.01

        height: wordsEntryLayoutHeight

        TextField {
            id: firstWordTextField

            Layout.minimumWidth: parent.width * 0.49
            Layout.minimumHeight: parent.height

            ToolTip.text: presenter.firstWordToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onVisibleChanged: {
                if (visible) {
                    forceActiveFocus();
                }
            }
        }

        TextField {
            id: secondWordTextField

            Layout.minimumWidth: parent.width * 0.49
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight

            ToolTip.text: presenter.secondWordToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors.bottom: parent.bottom
        anchors.left: wordsEntryLayout.left
        anchors.right: wordsEntryLayout.right

        height: bottomBtnsLayoutHeight



        Button {
            id: submitBtn

            contentItem: Text {
                text: presenter.submitButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.submitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            function submitWords() {
                var clearTextFields = presenter.handleSubmitRequest(firstWordTextField.text, secondWordTextField.text);
                if (clearTextFields) {
                    firstWordTextField.clear();
                    secondWordTextField.clear();
                }
                firstWordTextField.forceActiveFocus();
            }

            Shortcut {
                sequence: presenter.submitButtonShortcut
                onActivated: {
                    submitBtn.submitWords();
                }
            }

            onClicked: submitWords()
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }

        Button {
            id: helpBtn

            contentItem: Text {
                text: presenter.helpButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.helpButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: presenter.switchToPane(GamePresenter.HELP)
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }

        Button {
            id: resultsBtn

            contentItem: Text {
                text: presenter.resultsButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.resultsButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            function getResults() {
                firstWordTextField.clear();
                secondWordTextField.clear();
                firstWordTextField.forceActiveFocus();
                presenter.handleResultsRequest();
            }

            Shortcut {
                sequence: presenter.resultsButtonShortcut
                onActivated: {
                    resultsBtn.getResults();
                }
            }

            onClicked: getResults()
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }

        Button {
            id: quitBtn

            contentItem: Text {
                text: presenter.quitButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.25

            ToolTip.text: presenter.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: Qt.quit()
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }
    }
}
