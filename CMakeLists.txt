cmake_minimum_required(VERSION 3.0.2)

project(fancontroller)


#options
option(NO_SYSTEMD "Compile without Systemd support. Reduces functionality significantly!" OFF)
option(BUILD_GUI "Build the standalone application" ON)
option(BUILD_KCM "Build the KCM" OFF)
option(BUILD_HELPER "Build the KHelper" ON)
option(INSTALL_SHARED "Install the shared parts" ON)

#variables
set(STANDARD_SERVICE_NAME "fancontrol" CACHE STRING "The name of the systemd service for the fancontrol script")
set(STANDARD_CONFIG_FILE "/etc/fancontrol" CACHE STRING "The location of the standard config file for the fancontrol script")
set(STANDARD_HELPER_ID "fancontrol.gui.helper" CACHE STRING "The standard id for the KAuth helper")
add_definitions(-DSTANDARD_SERVICE_NAME="${STANDARD_SERVICE_NAME}")
add_definitions(-DSTANDARD_CONFIG_FILE="${STANDARD_CONFIG_FILE}")
add_definitions(-DSTANDARD_HELPER_ID="${STANDARD_HELPER_ID}")

#KCM can't be build without systemd support
if(BUILD_KCM AND NO_SYSTEMD)

    message(WARNING "KCM can't be build without systemd support")
    set(BUILD_KCM FALSE)

endif(BUILD_KCM AND NO_SYSTEMD)


#Silence warnings
cmake_policy(SET CMP0063 NEW)


#Find ECM
find_package(ECM 5.38 REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

#includes
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(FindPkgConfig)
include(ECMQMLModules)
include(ECMGenerateQmlTypes)

#Find Qt5
find_package(Qt5Core REQUIRED)

#Find KF5
find_package(KF5 COMPONENTS I18n REQUIRED)

#Find QML modules
ecm_find_qmlmodule(QtQuick 2.6)
ecm_find_qmlmodule(QtQuick.Controls 2.1)
ecm_find_qmlmodule(QtQuick.Layouts 1.2)
ecm_find_qmlmodule(QtQuick.Dialogs 1.2)
ecm_find_qmlmodule(org.kde.kirigami 2.3)

include_directories (${CMAKE_SOURCE_DIR})


#Systemd
if(NOT NO_SYSTEMD)

    message(STATUS "Compiling for Systemd")

else(NOT NO_SYSTEMD)

    message(STATUS "Compiling without Systemd")
    set(NO_SYSTEMD true)
    add_definitions(-DNO_SYSTEMD)

endif(NOT NO_SYSTEMD)


#KHelper for actions that require superuser rights
if(BUILD_HELPER)

    add_subdirectory(helper)

endif(BUILD_HELPER)


#Build the standalone application
if(BUILD_GUI)

    message(STATUS "Build the standalone application")
    add_subdirectory(fancontrol-gui)

endif(BUILD_GUI)


#Build the KCM
if(BUILD_KCM)

    message(STATUS "Build the KCM")
    add_subdirectory(kcm)

endif(BUILD_KCM)


#build and install the shared parts
if(INSTALL_SHARED)

    #qml plugin
    add_subdirectory(import)


    #icon
    include(ECMInstallIcons)
    ecm_install_icons(ICONS sc-apps-org.kde.fancontrol.gui.svg DESTINATION "${KDE_INSTALL_ICONDIR}")


    #translations
    ki18n_install(po)

endif(INSTALL_SHARED)


#summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
