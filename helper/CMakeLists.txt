set(LIBRARIES Qt5::Core
              KF5::Auth
              KF5::I18n)

if(NOT NO_SYSTEMD)

    set(LIBRARIES ${LIBRARIES}
                  Qt5::DBus)

    find_package(Qt5DBus REQUIRED)
    include_directories(${Qt5DBus_INCLUDE_DIRS})

endif(NOT NO_SYSTEMD)

find_package(KF5Auth REQUIRED)

add_executable(fancontrol-gui-helper src/helper.cpp)
target_link_libraries(fancontrol-gui-helper ${LIBRARIES})
install(TARGETS fancontrol-gui-helper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})

kauth_install_helper_files(fancontrol-gui-helper fancontrol.gui.helper root)
kauth_install_actions(fancontrol.gui.helper fancontrol_gui.actions)
