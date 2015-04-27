cmake_minimum_required(VERSION 3.0.0)

project(fancontroller)

include(FeatureSummary)

option(NO_KF5_AUTH "Compile without KF5-Auth. Program will be unable to execute commands as root!")
option(NO_SYSTEMD "Compile without Systemd support. Reduces functionality significantly!")

#Find Qt5
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Qml_INCLUDE_DIRS} ${Qt5Quick_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_AUTOMOC ON)

# Silence a warning
cmake_policy(SET CMP0037 OLD)

#Find KF5
find_package(ECM)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(KF5Auth)
find_package(Qt5DBus)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

#KF5_Auth
if(ECM_FOUND AND KF5Auth_FOUND AND NOT NO_KF5_AUTH)
    message(STATUS "Compiling with KF5_AUTH")

    include(KDEInstallDirs)
    include(KDECMakeSettings)
    include(KDECompilerSettings)

    add_subdirectory(helper)
else(ECM_FOUND AND KF5Auth_FOUND AND NOT NO_KF5_AUTH)
    message(STATUS "Compiling without KF5_AUTH")
    set(NO_KF5_AUTH true)
endif(ECM_FOUND AND KF5Auth_FOUND AND NOT NO_KF5_AUTH)

#Systemd
find_library(SYSTEMD_FOUND NAMES systemd)

if(NOT NO_SYSTEMD AND SYSTEMD_FOUND)
    message(STATUS "Compiling for Systemd")

    include_directories(${Qt5DBus_INCLUDE_DIRS})
else(NOT NO_SYSTEMD AND SYSTEMD_FOUND)
    message(STATUS "Compiling without Systemd")
    set(NO_SYSTEMD true)
endif(NOT NO_SYSTEMD AND SYSTEMD_FOUND)

add_subdirectory(share)
add_subdirectory(fancontrol-gui)
