import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import "qrc:/views/widget" as Widgets

import "qrc:/views/screen/ServiceScreen/OpenCV" as OpenCv

Item {
    Component.onCompleted: {
        mainMediaPanel.state = "small"
    }

    Widgets.ColorHelper{id: colorHelper}
    Rectangle {
        id: background
        anchors.fill: parent
        color: colorHelper.darkPasteBlue
        RadialGradient {
           anchors.fill: parent
           gradient: Gradient {
               GradientStop { position: 0.0; color: "white" }
               GradientStop { position: 0.5; color: "transparent" }
           }
       }
    }

    Item
    {
        id: mediaPanel
        anchors{
            top: parent.top
            left: parent.left
        }
        height: mainMediaPanel.height
        width: mainMediaPanel.width

        OpacityMask {
                anchors.fill: mediaPanel
                source: mainMediaPanel
                maskSource: background
        }
    }

    Pane{
        id: configPane
        anchors{
            left: mediaPanel.right
            top: parent.top
            right: parent.right
            bottom: parent.bottom
            //rightMargin: 5
            bottomMargin: 5
            leftMargin: 5
        }
        Material.elevation: 6
        Material.background: Material.White

        Flickable{
            clip: true
            anchors.fill: parent
            contentHeight: settingsLayout.implicitHeight
            ColumnLayout{
                id: settingsLayout
                anchors.fill: parent
                spacing: 0
                anchors.margins: 1          

                CommonSettings{}

                Label{
                    Layout.topMargin: 30
                    opacity: 0.4
                    text: "CURRENT MEDIA PROCESSING:"
                }

                Component{
                    id: calibrateSettings
                    OpenCv.CalibrateSettings{}
                }

                Loader{
                    Layout.fillWidth: true
                    id: calibrateSettingsLoader
                    sourceComponent: calibrateSettings
                }

                //Models
                Label{
                    Layout.topMargin: 30
                    opacity: 0.4
                    text: "AVAILABLE MEDIA PROCESSING:"
                }
                OpenCv.MeasureSettings{
                    visible: true
                    opacity: visible?1:0
                    Behavior on opacity {
                        NumberAnimation{duration: 500}
                    }
                }


            }
        }
    }

    Pane{
        id: statsPane
        anchors{
            left: parent.left
            top: mediaPanel.bottom
            right: mediaPanel.right
            bottom: parent.bottom
            bottomMargin: 5
            topMargin: 5
        }
        Material.elevation: 6
        Material.background: Material.White

        Flickable{
            clip: true
            anchors.fill: parent
            contentHeight: settingsLayout.implicitHeight
            ColumnLayout{
                id: statsLayout
                anchors.fill: parent
                RowLayout{
                    Image {
                        sourceSize.width: 40
                        source: "qrc:/assets/image/data.png"
                    }
                    Text {
                        font.pixelSize: 15
                        text: qsTr("Objects measurement statistics")
                        opacity: 0.6
                    }
                }


                Widgets.SeparatorLine{Layout.fillWidth: true}

                ColumnLayout{
                    spacing: 5
                    Label{
                        id: detectedObjectsCount
                        text: "Measured objects: "
                    }
                    Label{
                        text: "Measurement time  : " + " ms"
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
