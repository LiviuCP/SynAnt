import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: mainPane

    property QtObject presenter;

    readonly property double scoresLayoutHeight: height * 0.1;
    readonly property double infoLevelsAndStatusLayoutHeight: height * 0.55
    readonly property double wordLabelsLayoutHeight: height * 0.05
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: (infoLevelsAndStatusLayout.width - 3 * bottomBtnsLayout.spacing) * 0.25

    readonly property color paneColor: "grey"
    readonly property color borderColor: "white"
    readonly property color textColor: "white"

    RowLayout {
        id: scoresLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: scoresLayoutHeight

        Rectangle {
            id: highScoresRect
            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2
            color: paneColor

            border {
                color: borderColor
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
            color: paneColor
            Layout.leftMargin: parent.width * (-0.04)

            border {
                color: borderColor
            }

            Text {
                color: textColor
                text: presenter.mainPaneNrOfPairsMessage
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Button {
            id: resetBtn
            text: presenter.resetButtonLabel
            Layout.minimumWidth: quitBtn.width
            Layout.alignment: Qt.AlignRight
            enabled: presenter.resetEnabled
            onClicked: {
                presenter.handleResetRequest();
                firstWordTextField.forceActiveFocus();
            }
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
            id: radioBtnsRect
            Layout.minimumWidth: parent.width * 0.15
            Layout.minimumHeight: parent.height * 0.6
            color: paneColor

            border {
                color: borderColor
            }

            ColumnLayout {
                anchors.fill: parent

                RadioButton {
                    id: easyLvlBtn
                    text: presenter.levelEasyButtonLabel
                    checked: false
                    onToggled: {
                        firstWordTextField.clear();
                        secondWordTextField.clear();
                        presenter.switchToEasyLevel();
                    }
                }

                RadioButton {
                    id: mediumLvlBtn
                    text: presenter.levelMediumButtonLabel
                    checked: true
                    onToggled: {
                        firstWordTextField.clear();
                        secondWordTextField.clear();
                        presenter.switchToMediumLevel();
                    }
                }

                RadioButton {
                    id: hardLvlBtn
                    text: presenter.levelHardButtonLabel
                    checked: false
                    onToggled: {
                        firstWordTextField.clear();
                        secondWordTextField.clear();
                        presenter.switchToHardLevel();
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

    RowLayout {
        id: wordLabelsLayout
        anchors.top: infoLevelsAndStatusLayout.bottom
        anchors.left: infoLevelsAndStatusLayout.left
        anchors.right: infoLevelsAndStatusLayout.right
        anchors.topMargin: parent.height * 0.025
        height: wordLabelsLayoutHeight

        Label {
            id: label1
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: textColor
            text: "Label "
        }

        Label {
            id: label2
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: textColor
            text: "area."
        }

        Label {
            id: label3
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: textColor
            text: "Under"
        }

        Label {
            id: label4
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: textColor
            text: "construction! "
        }
    }

    RowLayout {
        id: wordsEntryLayout
        anchors.top: wordLabelsLayout.bottom
        anchors.left: wordLabelsLayout.left
        anchors.right: wordLabelsLayout.right
        anchors.topMargin: parent.width * 0.01
        height: wordsEntryLayoutHeight

        TextField {
            id: firstWordTextField
            Layout.minimumWidth: parent.width * 0.49
            Layout.minimumHeight: parent.height

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
            text: presenter.submitButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: {
                var clearTextFields = presenter.handleSubmitRequest(firstWordTextField.text, secondWordTextField.text);
                if (clearTextFields) {
                    firstWordTextField.clear();
                    secondWordTextField.clear();
                }
                firstWordTextField.forceActiveFocus();
            }
        }

        Button {
            id: helpBtn
            text: presenter.helpButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth

            onClicked: {
                presenter.switchToHelpPane();
            }
        }

        Button {
            id: resultsBtn
            text: presenter.resultsButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: {
                firstWordTextField.clear();
                secondWordTextField.clear();
                firstWordTextField.forceActiveFocus();
                presenter.handleResultsRequest();
            }
        }

        Button {
            id: quitBtn
            text: presenter.quitButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.25
            onClicked: Qt.quit()
        }
    }
}
