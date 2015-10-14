cmake_minimum_required(VERSION 3.0.2)

project(fancontroller)


#options
option(NO_SYSTEMD "Compile without Systemd support. Reduces functionality significantly!" OFF)
option(BUILD_GUI "Build the standalone application" ON)
option(BUILD_KCM "Build the KCM" OFF)
option(INSTALL_SHARED "Install the shared parts" ON)
option(INSTALL_HELPER "Install the KHelper" ON)


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


#Find Qt5
find_package(Qt5Core REQUIRED)


#Find KF5
find_package(KF5 COMPONENTS I18n Package REQUIRED)


#includes 
include(GenerateExportHeader)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)


#Systemd
if(NOT NO_SYSTEMD)

    message(STATUS "Compiling for Systemd")
    find_package(Qt5DBus REQUIRED)
    include_directories(${Qt5DBus_INCLUDE_DIRS})

else(NOT NO_SYSTEMD)

    message(STATUS "Compiling without Systemd")
    set(NO_SYSTEMD true)
    add_definitions(-DNO_SYSTEMD)

endif(NOT NO_SYSTEMD)


#Shared library
add_subdirectory(lib)


#KHelper for actions that require superuser rights
if(INSTALL_HELPER)

    add_subdirectory(helper)
    
endif(INSTALL_HELPER)


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


#install the shared parts
if(INSTALL_SHARED)

    #KPackage containing the QML and javascript files
    kpackage_install_package(package kcm_fancontrol kcms)


    #icon
    install(FILES icon.svg RENAME "fancontrol_gui.svg" DESTINATION "${ICON_INSTALL_DIR}/hicolor/scalable/apps")


    #translations
    ki18n_install(po)

endif(INSTALL_SHARED)


#add tests
add_subdirectory(tests)


#summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)