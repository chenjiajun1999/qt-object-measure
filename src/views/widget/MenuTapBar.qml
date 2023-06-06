import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtMultimedia 5.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.12

TabBar {
    ColorHelper{id: colorHelper}
    MethodsHelpers{id: methodsHelpers}

    id: tabBar

    TabButton
    {
        icon.source: "qrc:/assets/image/camera.png"
        text: qsTr("media")
        Material.accent:  colorHelper.darkPastelOrange
        onClicked: {
            methodsHelpers.navigateToItem("media")
        }
    }

    TabButton
    {
        icon.source: "qrc:/assets/image/opencv_icon.png"
        text: qsTr("OpenCV")
        Material.accent: colorHelper.darkPasteBlue

        onClicked:
        {
            methodsHelpers.navigateToItem("application")
        }
    }

}
