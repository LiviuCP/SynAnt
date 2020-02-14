import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

Repeater {
    id: mixedWordsPieces

    property QtObject gamePresenter
    property double pieceWidth
    property double pieceHeight

    model: gamePresenter.mixedWordsPiecesContent

    property Timer cursorSelectedPieceTimer : Timer {
        interval: 100
    }

    Rectangle {
        width: pieceWidth
        height: pieceHeight

        visible: !gamePresenter.mixedWordsPiecesSelections[index] && !presenter.dataLoadingInProgress

        color: Styles.mixedPiecesBackgroundColor

        border.color: index === gamePresenter.pieceSelectionCursorPosition && !mixedWordsPieces.cursorSelectedPieceTimer.running ? Styles.selectedBorderColor
                                                                                                                                      : Styles.mixedPiecesBorderColor
        border.width: index === gamePresenter.pieceSelectionCursorPosition && !mixedWordsPieces.cursorSelectedPieceTimer.running ? Styles.selectedBorderWidth
                                                                                                                                      : Styles.borderWidth

        opacity: (firstWordClickSelectedPieceTimer.running ||
                  secondWordClickSelectedPieceTimer.running ||
                  (mixedWordsPieces.cursorSelectedPieceTimer.running &&
                   index === gamePresenter.pieceSelectionCursorPosition)) ? Styles.pressedOpacity
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

            onTriggered: gamePresenter.selectPieceForFirstInputWord(index)
        }

        Timer {
            id: secondWordClickSelectedPieceTimer
            interval: 100

            onTriggered: gamePresenter.selectPieceForSecondInputWord(index)
        }

        Text {
            text: modelData
            font.pointSize: pieceHeight * 0.4
            anchors.centerIn: parent
            color: gamePresenter.mixedWordsPiecesTextColors[index]
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
            text: gamePresenter.mixedWordsPiecesSelections[index] ? GameStrings.wordPieceAlreadySelectedToolTip : GameStrings.selectWordPieceToolTip
            visible: !gamePresenter.persistentModeEnabled && mixedWordsCurrentPieceMouseArea.containsMouse
            delay: gamePresenter.toolTipDelay
            timeout: gamePresenter.toolTipTimeout
        }
    }

    function animatePieceSelectedByCursor() {
        cursorSelectedPieceTimer.start();
    }
}
