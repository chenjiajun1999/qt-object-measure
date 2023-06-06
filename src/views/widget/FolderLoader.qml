import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import Qt.labs.platform 1.1



RowLayout{
    property alias folderText: folderEditText.text
    property alias labelText: label.text
    property alias folder: folderDialog.folder
    property var onFolderChoosen
    id: root
    spacing: 5
    Label{
        id: label
    }

    TextInput{
        height:moreButton.height
        id: folderEditText
        clip: true
        Layout.fillWidth: true
        font.pixelSize: 15

        Rectangle{
            anchors{
                topMargin: -5
                bottomMargin: -5
            }

            opacity: 0.2
            anchors.fill: parent
            border.color: "gray"
            color: "lightgray"

        }
    }
    ToolButton{
        id: moreButton
        icon.source: "qrc:/assets/image/moreVert.png"
        Material.background: Material.BlueGrey
        onClicked: folderDialog.open()
    }

    FolderDialog{
        id:folderDialog
        onAccepted: root.onFolderChoosen(folder)
    }
}
