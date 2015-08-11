set(Fancontrol_GUI_SRCS src/main.cpp
                        src/gui.cpp)

set(LIBRARIES fancontrol_gui_lib
	      Qt5::Widgets
	      KF5::Declarative
              KF5::I18n)
              
find_package(Qt5Widgets REQUIRED)
find_package(KF5Declarative REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

add_executable(fancontrol_gui ${Fancontrol_GUI_SRCS})
target_link_libraries(fancontrol_gui ${LIBRARIES})

install(TARGETS fancontrol_gui ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES metadata.desktop RENAME "fancontrol_gui.desktop" DESTINATION ${XDG_APPS_INSTALL_DIR})