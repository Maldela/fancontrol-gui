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

add_library(fancontrol-gui-lib STATIC ${LIB_SRCS})

target_link_libraries(fancontrol-gui-lib ${LIBRARIES})

install(DIRECTORY qml DESTINATION share/fancontrol-gui)
install(DIRECTORY javascript DESTINATION share/fancontrol-gui)
#install(TARGETS fancontrol-gui-lib DESTINATION lib/fancontrol-gui EXPORT fancontrol-gui-lib-targets)
#install(EXPORT fancontrol-gui-lib-targets DESTINATION lib/fancontrol-gui)
