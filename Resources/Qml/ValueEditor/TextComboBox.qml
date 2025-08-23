import QtQuick;
import QtQuick.Controls;
import Qt5Compat.GraphicalEffects
import ColorPalette

Item{
    id: editor
    property var value
    property var model
    implicitHeight: 25
    signal asValueChanged(value:var)
    function setValue(newValue:var){
        if(newValue !== value){
            value = newValue
            asValueChanged(value)
        }
    }
    ComboBox {
        id: control
        anchors.margins: 2
        anchors.fill: parent
        model: editor.model
        onCurrentTextChanged: {
            setValue(currentText)
        }
        delegate: ItemDelegate {
            id: itemDelegate
            width: control.width
            height: 25
            padding: 5
            background: Rectangle {
                color: itemDelegate.highlighted ? ColorPalette.theme.comboBoxItemBackgroundHover
                      : itemDelegate.hovered ? ColorPalette.theme.comboBoxItemBackgroundHover
                      : ColorPalette.theme.comboBoxItemBackground
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
            }
            contentItem: Text {
                text: modelData
                color: ColorPalette.theme.textPrimary
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter  // 文字垂直居中
                horizontalAlignment: Text.AlignLeft   // 文字左对齐
                padding: 0
            }
            highlighted: control.highlightedIndex === index
            required property int index
            required property var modelData
        }

        indicator: Image {
            id: indicator
            x: control.width - width/2 - control.rightPadding
            y: control.topPadding + (control.availableHeight - height) / 2
            width: 13
            height: 13
            mipmap: true 
            source: "qrc:/Resources/Icon/expand.png" 
            ColorOverlay {
                anchors.fill: parent
                source: parent
                color: ColorPalette.theme.rowIndicator  
                opacity: 1.0 
            }
        }
 
        contentItem: Text {
            leftPadding: 3
            rightPadding: control.indicator.width + control.spacing
 
            text: control.displayText
            font: control.font
            color: ColorPalette.theme.textPrimary
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
 
        background: Rectangle {
            color: ColorPalette.theme.comboBoxBackground
        }
 
        popup: Popup {
            y: control.height
            width: control.width
            implicitHeight: contentItem.implicitHeight + 5
            padding : 2
            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight 
                model: control.popup.visible ? control.delegateModel : null
                currentIndex: control.highlightedIndex
                ScrollIndicator.vertical: ScrollIndicator { }
            }
 
            background: Rectangle {
                color: ColorPalette.theme.comboBoxBackground
            }
        }
    }
}

