import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.1

Item {

    Component.onCompleted: {
        root.reload()
    }

    id: root

    property alias fileUrls: myModel
    property var service

    function reload(){

        myModel.clear()
        var list = service.getImagePath()

        for(var i = 0;i<list.length;i++)
            myModel.append({ fileName : list[i] })

    }

    function addFile(fileName) {

        if(!fileHandler.isImage(fileName)||service.isExistImagePath(fileName))
            return
        service.insertImagePath(fileName)
        myModel.append({ fileName : fileName })
    }

    function cleanUp() {
        service.clearImagePath()
        myModel.clear()
    }

    Component {
        id: delegate

        Rectangle {
            width: 60
            height: 60
            radius: 2
            clip: true
            color: hovered ? "#AAA" : "transparent";
            property bool hovered: false;

            Image {
                id: image
                width: 32
                height: 32
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                source: "image://imageProvider/" + fileHandler.imageToIcon(fileName)
            }

            Text {
                id: name
                color: "Gray"
                font.family: "Microsoft YaHei UI"
                width: parent.width - 4
                anchors.top:image.bottom
                anchors.topMargin: 8
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: fileHandler.fileName(fileName)
                clip: true
                horizontalAlignment: contentWidth < width ? Text.AlignHCenter : Text.AlignLeft;
                elide: Text.ElideRight
            }

//            MyToolTip {
//                visible: parent.hovered
//                text: name.text
//            }
            ToolTip {
                id: tip
                x: 10
                y: 40
                opacity: 0
                visible: parent.hovered
                text: name.text
                font.pointSize: 8
                font.family: "Microsoft YaHei UI"
                background: Rectangle {
                    radius: 1
                    color: "#FFFFFF"
                    border.color: "#888"
                    border.width: 1
                }

                NumberAnimation {
                    id: animation
                    target: root
                    running: false
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 700
                    easing.type: Easing.InOutQuad
                }

                onVisibleChanged: if (visible) animation.restart();
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.hovered = true;
                onExited: parent.hovered = false;
                onClicked: {
                    service.deleteImagePath(myModel.get(index).fileName)
                    myModel.remove(index)
                }
            }
        }
    }

    GridView {
        id: gridView
        clip: true
        focus: true
        cellWidth: 65
        cellHeight: 65
        anchors.fill: parent
        anchors.margins: 8
        displaced: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
        }
        model: ListModel {
            id: myModel
        }
        delegate: delegate
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
    }
}