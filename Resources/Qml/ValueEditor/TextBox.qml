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
        MouseArea{
            id: hoverArea
            hoverEnabled: true
            propagateComposedEvents: true
            anchors.fill: parent
            onEntered:{
                exitAnimation.stop()
                enterAnimation.start()
                hoverArea.cursorShape = Qt.IBeamCursor
            }
            onExited:{
                enterAnimation.stop()
                exitAnimation.start()
                hoverArea.cursorShape = Qt.ArrowCursor
            }
            onPressed: (mouse)=> mouse.accepted = false
            onReleased:(mouse)=>  mouse.accepted = false
            onClicked:(mouse)=>  mouse.accepted = false
            onDoubleClicked:(mouse)=>  mouse.accepted = false
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

