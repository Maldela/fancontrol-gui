kauth_install_helper_files(Fancontrol-GUIHelper fancontrol.gui.helper root)
kauth_install_actions(fancontrol.gui.helper fancontrol-gui.actions)

set(LIBRARIES KF5::Auth)

if(NO_SYSTEMD)
    add_definitions(-DNO_SYSTEMD)
else(NO_SYSTEMD)
    set(LIBRARIES ${LIBRARIES} Qt5::DBus)
endif(NO_SYSTEMD)

add_executable(fancontrol-gui-helper src/helper.cpp)
target_link_libraries(fancontrol-gui-helper ${LIBRARIES})
install(TARGETS fancontrol-gui-helper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
