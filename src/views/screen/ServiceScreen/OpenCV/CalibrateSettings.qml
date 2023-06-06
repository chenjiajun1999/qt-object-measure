import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import QtQuick.Dialogs 1.3

import "qrc:/views/widget" as Widgets

Pane{

    Component.onCompleted: {
        undistortCheckBox.checked =  cvFrameController.getServiceObjectByName("UnDistortService").isActive
    }

    id: root
    property var serviceName: "UnDistortService"
    property var serviceObj: cvFrameController.getServiceObjectByName(root.serviceName)
    property var imageView: fileView
    Material.elevation: 1
    Material.background: Material.White
    Layout.fillWidth: true

    Widgets.ColorHelper{id: colorHelper}

    ColumnLayout{
        anchors.fill:parent

        Widgets.ExpandingSectionButton{
            id: expandingSectionButton
            imageLogoSource: serviceObj.iconSource
            text: "Calibration Settings"
        }


        ColumnLayout{
            Layout.leftMargin: 20
            id: settings
            visible: expandingSectionButton.visibility
            opacity: visible?1:0
            Behavior on opacity {
                NumberAnimation{duration: 500}
            }

            RowLayout{
                Layout.topMargin: 5
                Layout.leftMargin: -15
                ToolButton{

                    id: moreButton
                    icon.source: "qrc:/assets/image/plus.png"
                    Material.background: Material.BlueGrey
                    onClicked: imageDialog.open()
                }
                Label{
                    Layout.alignment: Qt.AlignRight
                    text: "Put in the picture to be calibrated (Can be dragged in)"
                }

                FileDialog{
                    id:imageDialog
                    folder: "StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]"
                    nameFilters: ["*.jpg","*.jpeg","*.png"]
                    selectMultiple: true
                    onAccepted: {
                        for(var i = 0; i < fileUrls.length; i++) {
                            fileView.addFile(fileUrls[i]);
                        }
                    }
                }

            }
            RowLayout{
                Layout.topMargin: 0

                Rectangle {
                    id: imageInputRectangle
                    Layout.fillWidth: true
                    height: 80
                    radius: 1

                    DropArea {
                        anchors.fill: parent;
                        onDropped: {
                            if(drop.hasUrls) {
                                for(var i = 0; i < drop.urls.length; i++) {
                                        console.log(drop.urls[i]);
                                        fileView.addFile(drop.urls[i]);
                                }
                            }
                        }
                    }
                    Rectangle
                    {
                        width: parent.width
                        height: parent.height
                        id:rect
                        anchors.centerIn: parent
                        color:"#f7f7f7"
                    }

                    DropShadow
                    {
                        anchors.fill: rect
                        radius: 8.0
                        samples: 16
                        color: "#a9a9a9"
                        source: rect
                    }

                    Widgets.ImageLoaderView {
                        id: fileView
                        service: serviceObj
                        anchors.fill: parent
                    }
                }
            }


            Button{
                id: calibrationButtion
                text: "Calibration"
                font.pixelSize: 10
                Layout.fillWidth: true
                onClicked: {
                    //fileView.cleanUp();
                    serviceObj.calibrate()
                }
                Material.background: "white"
            }

            Widgets.SeparatorLine{}

            RowLayout{
                Layout.leftMargin: -10
                CheckBox{
                    id: undistortCheckBox
                    checked: false
                    text: "Undistort active"
                    onCheckStateChanged:
                    {
                        serviceObj.isActive = checked
                    }
                }
            }
        }
    }
}

