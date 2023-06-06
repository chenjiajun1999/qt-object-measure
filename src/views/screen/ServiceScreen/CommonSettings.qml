import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import "qrc:/views/widget" as Widgets
Pane{

    Component.onCompleted: {
        wipeOffCheckBox.checked =  cvFrameController.getServiceObjectByName("WipeOffService").isActive
    }

    id: root
    property var serviceName: "WipeOffService"
    Material.elevation: 1
    Material.background: Material.White

    Layout.fillWidth: true

    ColumnLayout{
        anchors.fill:parent
        Widgets.ExpandingSectionButton{
            id: expandingSectionButton
            imageLogoSource: "qrc:/assets/image/settings_black.png"
            text: qsTr("General")
        }

        ColumnLayout{
            Layout.leftMargin: 20
            spacing: 5
            id: settings
            visible: expandingSectionButton.visibility
            opacity: visible?1:0
            Behavior on opacity {
                NumberAnimation{duration: 500}
            }

            RowLayout{
                Layout.fillWidth: true
                Label{
                    text: "Video rescale:"
                }
                ComboBox {
                    id: displaySizeCombobox
                    Layout.fillWidth: true
                    model: ["Orginal", "4/3", "3/2", "Manual"]
                    onCurrentTextChanged: {
                        if(currentText === "Orginal")
                        {
                            //frameworkManager.detectionResolution = Qt.point(1, 1)
                        }else if(currentText!== "Manual"){
                            resolutionSlider.value = resolutionSlider.value
                        }
                    }
                }
            }

            RowLayout{
                visible: displaySizeCombobox.currentText === "4/3" || displaySizeCombobox.currentText === "3/2"
                opacity: visible?1:0
                Behavior on opacity {
                    NumberAnimation{duration: 500}
                }

                Label{
                    text: "Resolution: "+ resolutionSlider.getXRes() + "x" + resolutionSlider.getYRes()
                }
                Slider{
                    function factorial(m,n){
                        return (n===0) || (n===1) ? m : factorial(m*2,n-1);
                    }
                    function getXRes(){return (displaySizeCombobox.currentText === "4/3"?40:30)*resolutionSlider.value.toFixed(0)}
                    function getYRes(){return (displaySizeCombobox.currentText === "4/3"?30:20)*resolutionSlider.value.toFixed(0)}
                    //default resolution is multiplication of 4:3 format aka 40:30

                    id: resolutionSlider
                    Layout.fillWidth: true
                    value: 8
                    from: 2
                    to:24

                    onValueChanged: {
                        //frameworkManager.detectionResolution = Qt.point(getXRes(), getYRes())
                    }
                }
            }

            RowLayout{
                visible: displaySizeCombobox.currentText === "Manual"
                opacity: visible?1:0
                Behavior on opacity {
                    NumberAnimation{duration: 500}
                }
                Label{
                    text: " Width: "
                }
                TextEdit{
                    id: widthEditText
                    text: "300"
                    color: "gray"
                    font.bold: true
                    font.pixelSize: 15
                }
                Label{
                    text: ", Height: "
                }
                TextEdit{
                    id: heightEditText
                    text: "300"
                    color: "gray"
                    font.bold: true
                    font.pixelSize: 15
                }
                ToolButton{
                    id: applyMean
                    text: qsTr("Apply resolution")
                    font.pixelSize: 15
                    antialiasing: true
                    contentItem:

                        RowLayout{
                        anchors.centerIn: parent
                        Text {
                            text: applyMean.text
                            font: applyMean.font
                            opacity: enabled ? 1.0 : 0.9
                            color: "gray"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        Image{
                            source: "qrc:/assets/image/round_check_black_48dp.png"
                            sourceSize.height: 20
                            sourceSize.width: 20
                            opacity: 0.4
                        }

                    }
                    onClicked: {
                        //frameworkManager.detectionResolution = Qt.point(parseInt(widthEditText.text), parseInt(widthEditText.text))
                    }
                }
            }

            Widgets.SeparatorLine{}

            //            Switch{
            //                id: detectionColoring
            //                text: "Separate color for each detection class"
            //            }

            RowLayout{
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: -10

                CheckBox{
                    id: wipeOffCheckBox
                    checked: false
                    text: "Wipe Off active"
                    onCheckStateChanged:
                    {
                        cvFrameController.getServiceObjectByName(serviceName).isActive = checked
                    }
                }
                Label{
                    text: "Detection rectangle color "
                }
                Rectangle{
                    id: colorRect
                    border.color: "black"
                    border.width: 1
                    width:50
                    height: 20
                    color: "white"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: colorDialog.visible = true
                    }
                }

                ColorDialog {
                    id: colorDialog
                    title: "Detection rectangle color"
                    onAccepted: {
                        application.detectionRectanglesColor = colorDialog.color
                        colorRect.color = colorDialog.color
                    }
                }
            }

            Widgets.SeparatorLine{}

            RowLayout{
                Layout.leftMargin: 0

                Widgets.FolderLoader{
                    id: imageSaveLoader
                    Layout.fillWidth: true
                    labelText: "Photo Folder:"
                    folderText: fileHandler.getCurrentPath()
                    function onLoaded(){
                        folderText = folder
                        mainMediaPanel.imageSaveUrl = imageSaveLoader.folderText
                    }
                    onFolderChoosen: onLoaded

                }
            }

        }
    }
}
