set(LIB_SRCS src/loader.cpp
             src/hwmon.cpp
             src/sensors.cpp)

set(LIB_HEADER src/hwmon.h
               src/loader.h
               src/sensors.h
               src/export.h)

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

    set(LIB_HEADER ${LIB_HEADER}
                   src/systemdcommunicator.h)

    set(LIBRARIES ${LIBRARIES}
                  Qt5::DBus)

endif(NO_SYSTEMD)

add_definitions(-DFANCONTROL_GUI_LIBRARY)

add_library(fancontrol-gui-share ${LIB_SRCS})

target_link_libraries(fancontrol-gui-share ${LIBRARIES})

install(DIRECTORY qml DESTINATION share/fancontrol-gui)
install(DIRECTORY javascript DESTINATION share/fancontrol-gui)
install(FILES ${LIB_HEADER} DESTINATION lib/fancontrol-gui/include)
install(TARGETS fancontrol-gui-share DESTINATION lib EXPORT libfancontrol-gui-share-targets)
install(EXPORT libfancontrol-gui-share-targets DESTINATION lib/fancontrol-gui)
