import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SmileFaceRHI

Window {

    width: 640
    height: 480
    visible: true
    title: qsTr("SmileFace Framebuffer")

    MenuBar {
        id: mainMenu
        width: parent.width

        Menu {
            title: "File"
            Action {
                text: "New"
                shortcut: "Ctrl+N"
                onTriggered: {
                    console.log("New file created");
                }
            }
            Action {
                text: "Open"
                shortcut: "Ctrl+O"
                onTriggered: {
                    console.log("File opened");
                }
            }
            MenuSeparator {}
            Action {
                text: "Exit"
                shortcut: "Ctrl+Q"
                onTriggered: {
                    Qt.quit();
                }
            }
        }
        Menu {
            title: "Edit"
            Action {
                text: "Undo"
                shortcut: "Ctrl+Z"
                onTriggered: {
                    console.log("Undo action triggered");
                }
            }
            Action {
                text: "Redo"
                shortcut: "Ctrl+Y"
                onTriggered: {
                    console.log("Redo action triggered");
                }
            }
        }
        Menu {
            title: "Edit"
            Action {
                text: "Undo"
                shortcut: "Ctrl+Z"
                onTriggered: {
                    console.log("Undo action triggered");
                }
            }
            Action {
                text: "Redo"
                shortcut: "Ctrl+Y"
                onTriggered: {
                    console.log("Redo action triggered");
                }
            }
        }
        Menu {
            title: "Help"
            Action {
                text: "About"
                onTriggered: {
                    console.log("About dialog opened");
                }
            }
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.top: mainMenu.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        spacing: 0

        Rectangle {
            id: toolbar
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 48

            // border
            Rectangle {
                id: toolbarborderBottom
                y: parent.height - height
                width: parent.width
                height:1
                color: "black"
            }

        }

        Rectangle {
            id: workspace
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width
            Layout.fillHeight: true


            SplitView {
                id: splitView1
                anchors.fill: parent
                orientation: Qt.Horizontal

                Rectangle {
                    SplitView.preferredWidth: 200
                    SplitView.maximumWidth: 400
                }

                SplitView {
                    id: splitView2
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    orientation: Qt.Vertical

                    SmileFace {
                        SplitView.fillWidth: true
                        SplitView.fillHeight: true
                    }

                    Rectangle {
                        SplitView.preferredHeight: 200
                    }
                }

                Rectangle {
                    id: centerItem
                    SplitView.preferredWidth: 200
                    SplitView.maximumWidth: 400
                    SplitView.fillWidth: true
                }
            }
        }
    }

}

