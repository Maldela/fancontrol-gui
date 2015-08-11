cmake_minimum_required(VERSION 3.0.2)

project(fancontroller)


#options
option(NO_SYSTEMD "Compile without Systemd support. Reduces functionality significantly!" OFF)
option(BUILD_GUI "Build the standalone application" ON)
option(BUILD_KCM "Build the KCM" OFF)
option(INSTALL_SHARED "Install the shared parts" ON)


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
add_subdirectory(helper)


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


#summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)


#install the shared parts
if(INSTALL_SHARED)

    #KPackage containing the QML and javascript files
    kpackage_install_package(package kcm_fancontrol kcms)


    #icon
    install(FILES icon.svg RENAME "fancontrol_gui.svg" DESTINATION "${ICON_INSTALL_DIR}/hicolor/scalable/apps")


    #translations
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    
	ki18n_install(po)
	
    endif (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    
endif(INSTALL_SHARED)