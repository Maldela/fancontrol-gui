cmake_minimum_required(VERSION 3.0.0)

project(fancontroller)

include(FeatureSummary)

option(NO_SYSTEMD "Compile without Systemd support. Reduces functionality significantly!")

#Find Qt5
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Qml_INCLUDE_DIRS}
                    ${Qt5Quick_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_AUTOMOC ON)

# Silence a warning
cmake_policy(SET CMP0037 OLD)

#Find KF5
find_package(ECM REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(KF5 COMPONENTS Auth Config REQUIRED)
find_package(Qt5DBus)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

add_subdirectory(helper)

#Systemd
find_library(SYSTEMD_FOUND NAMES systemd)

if(NOT NO_SYSTEMD AND SYSTEMD_FOUND AND Qt5DBus_FOUND)

    message(STATUS "Compiling for Systemd")

    include_directories(${Qt5DBus_INCLUDE_DIRS})

else(NOT NO_SYSTEMD AND SYSTEMD_FOUND AND Qt5DBus_FOUND)

    message(STATUS "Compiling without Systemd")
    set(NO_SYSTEMD true)

endif(NOT NO_SYSTEMD AND SYSTEMD_FOUND AND Qt5DBus_FOUND)

add_subdirectory(share)
add_subdirectory(fancontrol-gui)
