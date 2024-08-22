import QtQuick
import QtQuick.Controls

Window {
    id: projectManagerWindow
    width: Screen.width * 0.4
    height: Screen.height * 0.45

    visible: true
    title: qsTr("Project Manager")

    MainMenu {
        id: mainMenu
    }

}
