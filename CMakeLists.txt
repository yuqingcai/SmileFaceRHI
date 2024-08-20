cmake_minimum_required(VERSION 3.16)

project(SmileFaceRHI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick ShaderTools)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appSmileFaceRHI
    smileface.cpp
    smileface.h
    main.cpp
)

qt_add_qml_module(appSmileFaceRHI
    URI SmileFaceRHI
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES smileface.h smileface.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSmileFaceRHI PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSmileFaceRHI
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSmileFaceRHI
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::GuiPrivate
    Qt6::Qml
    Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appSmileFaceRHI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_add_shaders(appSmileFaceRHI "appSmileFaceRHI_shaders"
    # PRECOMPILE
    # OPTIMIZED
    PREFIX
        /SmileFaceRHI
    FILES
        shaders/color.vert
        shaders/color.frag
)

include_directories(${CMAKE_SOURCE_DIR}/3rdParty/include)
