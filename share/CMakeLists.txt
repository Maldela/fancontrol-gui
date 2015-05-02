set(LIB_SRCS src/loader.cpp
             src/hwmon.cpp
             src/sensors.cpp)

set(LIBRARIES ${Qt5Widgets_LIBRARIES}
              ${Qt5Qml_Libraries}
              ${Qt5Quick_LIBRARIES}
              KF5::Auth
              KF5::ConfigCore)

if(NO_SYSTEMD)

    add_definitions(-DNO_SYSTEMD)

else(NO_SYSTEMD)

    set(LIB_SRCS ${LIB_SRCS}
                 src/systemdcommunicator.cpp)

    set(LIBRARIES ${LIBRARIES}
                  Qt5::DBus)

endif(NO_SYSTEMD)

add_definitions(-DFANCONTROL_GUI_LIBRARY)

add_library(fancontrol-gui-lib ${LIB_SRCS})

target_link_libraries(fancontrol-gui-lib ${LIBRARIES})
target_include_directories (fancontrol-gui-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(DIRECTORY qml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/fancontrol-gui)
install(DIRECTORY javascript DESTINATION ${CMAKE_INSTALL_PREFIX}/share/fancontrol-gui)
