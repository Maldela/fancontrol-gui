set(Fancontrol-GUI_SRCS src/main.cpp)

add_executable(fancontrol-gui ${Fancontrol-GUI_SRCS} ${RESOURCES})
target_link_libraries(fancontrol-gui LINK_PUBLIC fancontrol-gui-lib)
set_property(TARGET fancontrol-gui PROPERTY CXX_STANDARD 11)

install(TARGETS fancontrol-gui RUNTIME DESTINATION bin)
install(FILES other/fancontrol-gui.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES other/fancontrol-gui.svg DESTINATION ${ICON_INSTALL_DIR}/hicolor/scalable/apps)
install(FILES qml/fancontrol-gui.qml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/fancontrol-gui/qml)
