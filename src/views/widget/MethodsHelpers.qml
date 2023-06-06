import QtQuick 2.12

QtObject {
    function navigateToItem(item){
        switch(item){
            case "media":
                if(stackView.currentItem !== mediaScreen)
                    stackView.replace(mediaScreen)
                break;

            case "application":
                if(stackView.currentItem !== serviceScreen){
                   stackView.replace(serviceScreen)
                   //serviceScreen.calibrateComponent.imageView.reload()
                }
                break;
        }
    }
}
