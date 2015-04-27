set(Fancontrol-GUI_SRCS src/main.cpp src/loader.cpp src/hwmon.cpp src/sensors.cpp)

set(LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5Qml_Libraries} ${Qt5Quick_LIBRARIES})

if(NO_KF5_AUTH)
    add_definitions(-DNO_KF5_AUTH)
else(NO_KF5_AUTH)
    set(LIBRARIES ${LIBRARIES} KF5::Auth)
endif(NO_KF5_AUTH)

if(NO_SYSTEMD)
    add_definitions(-DNO_SYSTEMD)
else(NO_SYSTEMD)
    set(Fancontrol-GUI_SRCS ${Fancontrol-GUI_SRCS} src/systemdcommunicator.cpp)
    set(LIBRARIES ${LIBRARIES} Qt5::DBus)
endif(NO_SYSTEMD)

add_executable(fancontrol-gui ${Fancontrol-GUI_SRCS} ${RESOURCES})
target_link_libraries(fancontrol-gui ${LIBRARIES})
set_property(TARGET fancontrol-gui PROPERTY CXX_STANDARD 11)

install(TARGETS fancontrol-gui RUNTIME DESTINATION bin)
install(FILES other/fancontrol-gui.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES other/fancontrol-gui.svg DESTINATION ${ICON_INSTALL_DIR}/hicolor/scalable/apps)
install(FILES qml/fancontrol-gui.qml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/fancontrol-gui/qml)
