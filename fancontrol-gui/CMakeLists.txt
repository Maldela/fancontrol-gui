set(Fancontrol_GUI_SRCS src/main.cpp
                        src/windowconfig.cpp
                        src/systemtrayicon.cpp)

set(LIBRARIES Qt5::Gui
              Qt5::Widgets
              KF5::CoreAddons
              KF5::DBusAddons
              KF5::Declarative
              KF5::I18n
              KF5::ConfigGui
              KF5::ConfigCore
              KF5::Notifications)

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)
find_package(KF5 COMPONENTS CoreAddons DBusAddons Package Declarative Config Notifications REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

add_executable(fancontrol_gui ${Fancontrol_GUI_SRCS})
target_link_libraries(fancontrol_gui ${LIBRARIES})

install(TARGETS fancontrol_gui ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.fancontrol.gui.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.fancontrol.gui.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

kpackage_install_package(package org.kde.fancontrol.gui genericqml)
