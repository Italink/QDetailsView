import QtQuick;
import QtQuick.Controls;
import QtQuick.Dialogs

Item{
    id: control
    property var value
    implicitHeight: 25
    signal asValueChanged(text:var)

    function setValue(newValue:var){
        if(newValue !== value){
            value = newValue
            asValueChanged(value)
        }
    }
    Button{
        anchors.margins:  2
        anchors.fill: parent
        palette {
            button: value
        }   
        background: Rectangle {
            color: value
        }
        onClicked: {
            colorDialog.open()
        }
    }
    ColorDialog {
        id: colorDialog
        selectedColor: value
        onAccepted: {
            control.setValue(selectedColor)
        }
    }

}

