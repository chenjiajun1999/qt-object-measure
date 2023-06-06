import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import "qrc:/views/widget" as Widgets

import online.hachikuji.component 1.0

Item {
    id: rootPanel

    property var viewfinder: mediaPlayer
    property var mediaUrl: ""
    property var mediaSource: "camera"
    property var imageSaveUrl: ""
    property bool framevorkView: false
    property var urlMediaStatePlay

    state: "small"
    width: parent.width
    height: parent.height

    onMediaSourceChanged: {
    }

    function getCurrentVideoOutput(){
        return mediaSource
    }


    function play(){

        switch(mediaSource)
        {

        case "image":
            mediaPlayer.openPicture(mediaUrl)
            //cameraButton.visible = false
            mediaControls.visible = false
            break;
        case "link":
            mediaPlayer.openWebPicture(mediaUrl)
            //cameraButton.visible = false
            mediaControls.visible = false
            break;
        case "video":
            mediaPlayer.openVideo(mediaUrl)
            //cameraButton.visible = false
            mediaControls.visible = true
            break;
        case "camera":
            mediaPlayer.openCamera()
            //cameraButton.visible = true
            mediaControls.visible = false
            break;

        case "rstp":
            mediaPlayer.openWebCam(mediaUrl)
            cameraButton.visible = true
            mediaControls.visible = false
            break;
        }
    }

    function stop(){
        mediaPlayer.stop()
    }

    CvMediaPlayer{

        id: mediaPlayer
        anchors.fill: parent
    }

    SequentialAnimation {
        id: seqanimation
        running: false
        loops: 1
        NumberAnimation {
            target: mediaPlayer
            property: "opacity"
            duration: 150
            to: 0.2
            easing.type: Easing.InOutQuad
        }

        NumberAnimation {
            target: mediaPlayer
            property: "opacity"
            duration: 150
            to: 1.0
            easing.type: Easing.InOutQuad
        }
    }

      Image{
            id: cameraButton
            anchors{
                verticalCenter: rootPanel.verticalCenter
                right: rootPanel.right
                margins: 15
            }

            sourceSize.width: 40
            sourceSize.height: 40
            visible: mediaSource == "rstp" || mediaSource == "camera"

            source: "qrc:/assets/image/camera.png"
            MouseArea{
                anchors.fill: parent
                onClicked:
                {
                    if(imageSaveUrl=="")
                        imageSaveUrl = fileHandler.getCurrentPath()
                    var name = mediaPlayer.savePicture(imageSaveUrl)
                    cvFrameController.getServiceObjectByName("UnDistortService").insertImagePath(imageSaveUrl+"/"+ name);
                    seqanimation.start();
                }
            }
      }


    anchors{
        top: parent.top
    }

    Image{
        id: fulscreenButton
        anchors{
            top: rootPanel.top
            right: rootPanel.right
            margins: 10
        }

        sourceSize.width: 50
        sourceSize.height: 50
        visible: (rootPanel.state == "fullscreen" || rootPanel.state == "small" )

        source: "qrc:/assets/image/round_fullscreen_white_48dp.png"
        MouseArea{
            anchors.fill: parent
            onClicked:
            {

                switch (rootPanel.state){

                case "fullscreen":
                    fulscreenButton.state = "small"
                    rootPanel.state = "small"
                    break;
                case "small":
                    rootPanel.state = "fullscreen"
                    fulscreenButton.state = "fullscreen"
                    break;

                }
            }
        }

        states: [
            State {
                name: "small"
                PropertyChanges {
                    target: fulscreenButton
                    source: "qrc:/assets/image/round_fullscreen_white_48dp.png"
                }
            },
            State {
                name: "fullscreen"
                PropertyChanges {
                    target: fulscreenButton
                    source: "qrc:/assets/image/round_close_fullscreen_white_48dp.png"
                }
            }
        ]
    }

    Label{
        text: "FPS: " + mediaPlayer.fps.toFixed(2)
        color: "white"
        anchors{
            top: fulscreenButton.bottom
            right: rootPanel.right
            margins: 10
        }
    }

    Rectangle{
        radius: 4
        color: "white"
        opacity: 0.5
        height: 40
        id: mediaControls
        visible: false
        anchors{
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
        }

        Image {
            width:height
            states: [
                State {
                    name: "play"
                    PropertyChanges {
                        target: playButton
                        source: "qrc:/assets/image/pause.png"
                    }
                },
                State {
                    name: "stop"
                    PropertyChanges {
                        target: playButton
                        source: "qrc:/assets/image/stop.png"
                    }
                },
                State {
                    name: "pause"
                    PropertyChanges {
                        target: playButton
                        source: "qrc:/assets/image/play.png"
                    }
                }
            ]
            id: playButton
            state: "play"
            source: "qrc:/assets/image/play.png"
            anchors{
                top: parent.top
                left: parent.left
                bottom: parent.bottom
                margins: 5
            }

            MouseArea{
                anchors.fill:parent
                onClicked: {

                    switch(playButton.state){
                    case "play":
                        mediaPlayer.pause()
                        playButton.state = "pause"
                        break;
                    case "stop":
                        mediaPlayer.play()
                        playButton.state = "stop"
                        break;
                    case "pause":
                        mediaPlayer.play()
                        playButton.state = "play"
                        break;
                    }
                }
            }
        }

        Label{
            id: durationLabel
            text: {
                return mediaController.convertVideoTimeToTimeString(mediaPlayer.position)
                        +" / "+
                        mediaController.convertVideoTimeToTimeString(mediaPlayer.duration)
            }
            anchors{
                left: playButton.right
                verticalCenter: parent.verticalCenter
                margins: 5
            }
            onTextChanged: {
                durationSLider.changeValue(mediaPlayer.position)
            }
        }

        Slider{
            anchors{
                top: parent.top
                left: durationLabel.right
                right: parent.right
                bottom: parent.bottom
                margins: 5
            }

            function changeValue(value){
                if(value!= durationSLider.value)
                    durationSLider.value = value;
            }

            onValueChanged: mediaPlayer.seek(value)

            id: durationSLider
            from: 0
            to: mediaPlayer.duration
        }
    }

    Behavior on height {
        NumberAnimation{duration: 300}
    }
    states: [
        State {
            name: "fullscreen"
            PropertyChanges {
                target: rootPanel
                height: application.height
                width: application.width

            }
        },
        State {
            name: "small"

            PropertyChanges {
                target: rootPanel
                height: application.height/2
                width: application.width/2
            }
        },
        State {
            name: "tile"

            PropertyChanges {
                target: rootPanel
                height: application.height/8
                width: application.width/8
            }
        },
        State {
            name: "hidden"

            PropertyChanges {
                target: rootPanel
                height: 0
                width: 0
            }
        }
    ]
}
