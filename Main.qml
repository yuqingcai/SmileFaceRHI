import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SmileFaceRHI
import Qt.labs.platform

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("SmileFaceRHI")

    MenuBar {
        id: mainMenu
        // width: parent.width

        Menu {
            id: fileMenu
            title: qsTr("File")

            MenuItem {
                text: qsTr("Ololo")
                onTriggered: console.debug("ololo menu item clicked")
            }

            // this one won't show up in File menu because of its `role`,
            // and it will actually appear in the main menu - under the application name,
            // where you'd usually expect About menu to be
            MenuItem {
                //text: qsTr("About")    // no need to set the text,
                role: MenuItem.AboutRole // because it gets generated
                onTriggered: console.debug("show your About window")
            }
        }

        Menu {
            id: editMenu
            title: qsTr("Edit")

            MenuItem {
                text: qsTr("Stuff")
                onTriggered: console.debug("stuff menu item clicked")
            }
            MenuItem {
                text: qsTr("Stuff")
                onTriggered: console.debug("stuff menu item clicked")
            }
            MenuItem {
                text: qsTr("Stuff")
                onTriggered: console.debug("stuff menu item clicked")
            }
            MenuItem {
                text: qsTr("Stuff")
                onTriggered: console.debug("stuff menu item clicked")
            }
        }
    }

    Rectangle {
        id: toolbar
        anchors.top: mainMenu.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height : 48
        color: Control.background

        // border
        Rectangle {
            id: toolbarborderBottom
            y: parent.height - height
            width: parent.width
            height:1
            color: "darkgray"
        }

    }


    SplitView {
        id: splitView
        anchors.top: toolbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        handle: Rectangle {
            id: handleDelegate1
            implicitWidth: 1
            implicitHeight: 1
            color: SplitHandle.pressed ? "darkgray"
                : (SplitHandle.hovered ? Qt.lighter("lightgray", 1.1) : "darkgray")

            containmentMask: Item {
                x: (handleDelegate1.width - width) / 2
                width: 16
                height: splitView.height
            }
        }


        Rectangle {
            SplitView.preferredWidth: 200
            SplitView.maximumWidth: 400
        }


        SplitView {
            id: splitView2
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            orientation: Qt.Vertical


            handle: Rectangle {
                id: handleDelegate2
                implicitWidth: 1
                implicitHeight: 1
                color: SplitHandle.pressed ? "darkgray"
                    : (SplitHandle.hovered ? Qt.lighter("lightgray", 1.1) : "darkgray")

                containmentMask: Item {
                    y: (handleDelegate2.height - height) / 2
                    width: splitView.width
                    height: 16
                }
            }

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
            SplitView.preferredWidth: 200
            SplitView.maximumWidth: 400
        }
    }
}

