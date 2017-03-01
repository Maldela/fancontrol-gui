set(Fancontrol_GUI_SRCS src/main.cpp
                        src/windowconfig.cpp)

set(LIBRARIES Qt5::Gui
              Qt5::Widgets
              KF5::CoreAddons
              KF5::Declarative
              KF5::I18n
              KF5::ConfigGui
              KF5::ConfigCore)

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)
find_package(KF5 COMPONENTS CoreAddons Package Declarative Config REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

add_executable(fancontrol_gui ${Fancontrol_GUI_SRCS})
target_link_libraries(fancontrol_gui ${LIBRARIES})

install(TARGETS fancontrol_gui ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES metadata.desktop RENAME "fancontrol_gui.desktop" DESTINATION ${KDE_INSTALL_APPDIR})

kpackage_install_package(package org.kde.fancontrol.gui genericqml)
