import QtQuick;
import QtQuick.Controls;
import ColorPalette

Item{
    id: control
    property var value
    implicitHeight: lineEditor.implicitHeight + 2
    signal asValueChanged(text:var)
    function setValue(newText:var){
        if(newText !== value){
            value = newText
            asValueChanged(value)
        }
    }
    Rectangle {
        anchors.fill: parent
        border.color: ColorPalette.theme.textBoxBackground
        color: ColorPalette.theme.textBoxBackground
        border.width: 1
        clip: true
        MouseArea{
            id: dragArea
            hoverEnabled: true
            anchors.fill: parent
            onEntered:{
                exitAnimation.stop()
                enterAnimation.start()
            }
            onExited:{
                enterAnimation.stop()
                exitAnimation.start()
            }
        }
        TextArea{
            id: lineEditor
            enabled: true
            clip: true
            anchors.fill: parent
            anchors.leftMargin: 2
            anchors.rightMargin: 2
            text: control.value
            color: ColorPalette.theme.textPrimary
            wrapMode: TextInput.WordWrap
            verticalAlignment: Text.AlignVCenter
            onEditingFinished:{
                setValue(lineEditor.text)
            }
        }
        ColorAnimation on border.color{
            id: enterAnimation
            to: ColorPalette.theme.boxHover
            duration: 100
            running: false
        }
        ColorAnimation on border.color{
            id: exitAnimation
            to: ColorPalette.theme.textBoxBackground
            duration: 100
            running: false
        }
    }
}

