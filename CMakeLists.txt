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
add_definitions(-DSTANDARD_SERVICE_NAME="${STANDARD_SERVICE_NAME}")
add_definitions(-DSTANDARD_CONFIG_FILE="${STANDARD_CONFIG_FILE}")


#KCM can't be build without systemd support
if(BUILD_KCM AND NO_SYSTEMD)

    message(WARNING "KCM can't be build without systemd support")
    set(BUILD_KCM FALSE)

endif(BUILD_KCM AND NO_SYSTEMD)


#Silence warnings
cmake_policy(SET CMP0037 OLD)
cmake_policy(SET CMP0063 NEW)


#Find ECM
find_package(ECM REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

#includes
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(FindPkgConfig)

#Find Qt5
find_package(Qt5Core REQUIRED)
find_package(Qt5 5.8 COMPONENTS Quick QuickControls2)
set_package_properties(Qt5Quick PROPERTIES TYPE RUNTIME PURPOSE "Needed by the QML parts")
set_package_properties(Qt5QuickControls2 PROPERTIES TYPE RUNTIME PURPOSE "Needed by the QML parts")

#Find KF5
find_package(KF5 COMPONENTS I18n REQUIRED)
find_package(KF5 5.37 COMPONENTS Kirigami2)
set_package_properties(KF5Kirigami2 PROPERTIES TYPE RUNTIME PURPOSE "Needed by the QML parts")

if(${KF5_VERSION} VERSION_GREATER_EQUAL 5.33.0)
    include(ECMGenerateQmlTypes)
endif(${KF5_VERSION} VERSION_GREATER_EQUAL 5.33.0)

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
