import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import Qt.labs.platform 1.1

import "qrc:/views/widget" as Widgets



Pane{

    Component.onCompleted: {
        measureCoinCheckBox.checked =  cvFrameController.getServiceObjectByName("MeasureCoinService").isActive
        measureA4CheckBox.checked  = cvFrameController.getServiceObjectByName("MeasureA4Service").isActive
        grayCheckBox.checked =  cvFrameController.getServiceObjectByName("GrayService").isActive
    }

    id: root
    property var serviceName: "MeasureCoinService"
    property var serviceObj: cvFrameController.getServiceObjectByName(root.serviceName)
    Material.elevation: 1
    Material.background: Material.White
    Layout.fillWidth: true

    ColumnLayout{
        anchors.fill:parent
        Widgets.ExpandingSectionButton{
            id: expandingSectionButton
            imageLogoSource: serviceObj.iconSource
            text: "Detection Settings"
        }

        ColumnLayout{
            spacing: 0
            Layout.leftMargin: 20
            visible: expandingSectionButton.visibility
            opacity: visible?1:0
            Behavior on opacity {
                NumberAnimation{duration: 500}
            }

            RowLayout{
                Layout.leftMargin: -10
                CheckBox{
                    id: measureA4CheckBox
                    checked: false
                    text: "Measure by A4 paper active"
                    onCheckStateChanged:
                    {
                        cvFrameController.getServiceObjectByName("MeasureA4Service").isActive = checked
                    }
                }
                CheckBox{
                    id :grayCheckBox
                    Layout.leftMargin: -10
                    checked: false
                    text: "Use gray scale conversion"
                    onCheckStateChanged:
                    {
                        cvFrameController.getServiceObjectByName("GrayService").isActive = checked
                    }
                }
            }
            RowLayout{
                Layout.leftMargin: -10
                CheckBox{
                    id: measureCoinCheckBox
                    checked: false
                    text: "Measure by coin active"
                    onCheckStateChanged:
                    {
                        serviceObj.isActive = checked
                    }
                }

            }
            RowLayout{
                Label{
                    text: "Confidence lvl: "+ confidenceSlider.value.toFixed(2)
                }
                Slider{
                    id: confidenceSlider
                    Layout.fillWidth: true
                    value: 0
                    from: 0
                    to: 5
                    onValueChanged: {

                    }
                }
            }
        }
    }
}




