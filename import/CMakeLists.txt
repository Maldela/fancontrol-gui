set(LIB_SRCS src/hwmon.cpp
             src/sensor.cpp
             src/temp.cpp
             src/fan.cpp
             src/pwmfan.cpp
             src/loader.cpp
             src/guibase.cpp
             src/config.cpp
             src/pwmfanmodel.cpp
             src/tempmodel.cpp
             src/fancontrolqmlextension.cpp)

set(QML_FILES qml/qmldir
              qml/ErrorDialog.qml
              qml/OptionInput.qml
              qml/FanItem.qml
              qml/PwmPoint.qml
              qml/StatusPoint.qml
              qml/colors.js
              qml/math.js
              qml/units.js)

set(LIB_PRIVATE_LIBRARIES Qt5::Qml
                          KF5::Auth
                          KF5::ConfigCore
                          KF5::I18n)

set(LIB_PUBLIC_LIBRARIES Qt5::Core)

if(NOT NO_SYSTEMD)

    set(LIB_SRCS ${LIB_SRCS}
                 src/systemdcommunicator.cpp)

    set(LIB_PRIVATE_LIBRARIES ${LIB_PRIVATE_LIBRARIES}
                              Qt5::DBus)

    find_package(Qt5DBus REQUIRED)
    include_directories(${Qt5DBus_INCLUDE_DIRS})

endif(NOT NO_SYSTEMD)

find_package(Qt5Qml REQUIRED)
find_package(KF5 COMPONENTS Auth Config REQUIRED)

include_directories(${Qt5Qml_INCLUDE_DIRS})

add_library(fancontrol_qml_plugin SHARED ${LIB_SRCS})
target_link_libraries(fancontrol_qml_plugin PRIVATE ${LIB_PRIVATE_LIBRARIES} PUBLIC ${LIB_PUBLIC_LIBRARIES})

install(TARGETS fancontrol_qml_plugin DESTINATION "${KDE_INSTALL_FULL_QMLDIR}/Fancontrol/Qml/")
install(FILES ${QML_FILES} DESTINATION "${KDE_INSTALL_FULL_QMLDIR}/Fancontrol/Qml/")
