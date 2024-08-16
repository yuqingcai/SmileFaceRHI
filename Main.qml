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

                        mirrorVertically: cbMirror.checked
                        sampleCount: cbMSAA.checked ? 4 : 1
                        fixedColorBufferWidth: cbSize.checked ? slSize.value.toFixed(0) : 0
                        fixedColorBufferHeight: cbSize.checked ? slSize.value.toFixed(0) : 0
                        alphaBlending: cbBlend.checked
                        colorBufferFormat: rdFormatRGBA8.checked ? ExampleRhiItem.TextureFormat.RGBA8
                                : rdFormatFP16.checked ? ExampleRhiItem.TextureFormat.RGBA16F
                                : rdFormatFP32.checked ? ExampleRhiItem.TextureFormat.RGBA32F
                                : ExampleRhiItem.TextureFormat.RGB10A2

                        // custom properties provided by the QQuickRhiItem subclass: angle, backgroundAlpha
                        NumberAnimation on angle {
                            from: 0
                            to: 360
                            duration: 5000
                            loops: Animation.Infinite
                            running: cbAnimate.checked
                        }

                        backgroundAlpha: slAlpha.value

                        // The transform is just to show something interesting..
                        transform: [
                            Rotation { id: rotation;
                                axis.x: 0;
                                axis.z: 0;
                                axis.y: 1;
                                angle: 0;
                                origin.x: renderer.width / 2;
                                origin.y: renderer.height / 2; },
                            Translate { id: txOut;
                                x: -renderer.width / 2;
                                y: -renderer.height / 2
                            },
                            Scale { id: scale; },
                            Translate {
                                id: txIn;
                                x: renderer.width / 2;
                                y: renderer.height / 2
                            }
                        ]
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

