import QtQuick 2.7
import QtQuick.Controls 2.2

ComboBox {
    id: dropdown

    property int dataModelIndex: -1

    property var dataModel: null
    property string dropdownToolTip: ""

    enabled: dataModel !== null

    ToolTip.visible: hovered && dropdownToolTip !== ""
    ToolTip.text: dropdownToolTip
    ToolTip.delay: 1000
    ToolTip.timeout: 4000

    editable: false
    model: dataModel

    onCurrentIndexChanged: {
        dataModelIndex = currentIndex;
    }
}
