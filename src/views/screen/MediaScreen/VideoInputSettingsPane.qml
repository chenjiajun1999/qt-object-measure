import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import "qrc:/views/widget" as Widgets
Item {
    id: root
    property var mediaSource: mainMediaPanel.mediaSource

    onMediaSourceChanged: {
        switch(mediaSource)
        {
        case "camera":
            videoPropertiesLoader.sourceComponent = cameraComponent
            break;
        case "rstp":
            videoPropertiesLoader.sourceComponent = rstpComponent
            break;
        }
    }


    Pane{
        anchors.fill: parent
        Material.elevation: 6
        Material.background: Material.White
    }
    Flickable{
        clip: true
        anchors.fill: parent
        contentHeight: layout.implicitHeight
        ColumnLayout{
            id: layout
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            RowLayout{
                Image {
                    sourceSize.width: 40
                    source: "qrc:/assets/image/media.png"
                }
                Text {
                    font.pixelSize: 15
                    text: qsTr( "Properties "+root.mediaSource)
                    opacity: 0.6
                }
            }

            Widgets.SeparatorLine{}

            Loader{
                id: videoPropertiesLoader
                sourceComponent:  cameraComponent
            }

            Item{
                Layout.fillHeight: true
            }

            Component{
                id: cameraComponent
                ColumnLayout{
                    Item{
                        Layout.leftMargin: 5
                        Label{
                            text:"Resolution: "+ mainMediaPanel.viewfinder.srcWidth + "x"+ mainMediaPanel.viewfinder.srcHeight
                        }
                    }
                }
            }

            Component{
                id: rstpComponent
                ColumnLayout{
                    Item{
                        Layout.leftMargin: 5
                        Label{
                            text:"RSTP properties"
                        }
                    }
                }
            }
        }
    }
}



