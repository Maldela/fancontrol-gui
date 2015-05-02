set(Fancontrol-GUI_SRCS src/main.cpp)

set(LIBRARIES fancontrol-gui-share)

add_executable(fancontrol-gui ${Fancontrol-GUI_SRCS} ${RESOURCES})
target_link_libraries(fancontrol-gui ${LIBRARIES})
set_property(TARGET fancontrol-gui PROPERTY CXX_STANDARD 11)

install(TARGETS fancontrol-gui RUNTIME DESTINATION bin)
install(FILES other/fancontrol-gui.desktop DESTINATION share/applications)
install(FILES other/fancontrol-gui.svg DESTINATION ${ICON_INSTALL_DIR}/hicolor/scalable/apps)
install(FILES qml/fancontrol-gui.qml DESTINATION share/fancontrol-gui/qml)
