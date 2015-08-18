set(LIB_SRCS src/hwmon.cpp
             src/loader.cpp
             src/sensors.cpp
             src/guibase.cpp
             src/config.cpp)
                        
set(LIB_PRIVATE_LIBRARIES Qt5::Qml
                          KF5::Auth
                          KF5::ConfigCore)

set(LIB_PUBLIC_LIBRARIES Qt5::Core)
              
if(NOT NO_SYSTEMD)

    set(LIB_SRCS ${LIB_SRCS}
                 src/systemdcommunicator.cpp)

    set(LIB_PRIVATE_LIBRARIES ${LIB_PRIVATE_LIBRARIES}
                              Qt5::DBus)

endif(NOT NO_SYSTEMD)

find_package(Qt5Qml REQUIRED)
find_package(KF5 COMPONENTS Auth Config REQUIRED)

include_directories(${Qt5Qml_INCLUDE_DIRS})

add_library(fancontrol_gui_lib SHARED ${LIB_SRCS})
target_link_libraries(fancontrol_gui_lib PRIVATE ${LIB_PRIVATE_LIBRARIES} PUBLIC ${LIB_PUBLIC_LIBRARIES})

target_compile_features(fancontrol_gui_lib PRIVATE cxx_lambdas)

generate_export_header(fancontrol_gui_lib)

if(INSTALL_SHARED)

    install(TARGETS fancontrol_gui_lib ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
    
endif(INSTALL_SHARED)