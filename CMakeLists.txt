cmake_minimum_required(VERSION 3.0.0)

project(fancontroller)

include(FeatureSummary)

option(NO_SYSTEMD "Compile without Systemd support. Reduces functionality significantly!")
option(BUILD_CORE "Compile the shared library")
option(BUILD_GUI "Compile the GUI")

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

find_package(KF5 COMPONENTS Auth Config I18n Declarative REQUIRED)
find_package(Qt5DBus)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

#Systemd
find_library(SYSTEMD_FOUND NAMES systemd)

if(NOT NO_SYSTEMD AND SYSTEMD_FOUND AND Qt5DBus_FOUND)

    message(STATUS "Compiling for Systemd")

    include_directories(${Qt5DBus_INCLUDE_DIRS})

else(NOT NO_SYSTEMD AND SYSTEMD_FOUND AND Qt5DBus_FOUND)

    message(STATUS "Compiling without Systemd")
    set(NO_SYSTEMD true)

endif(NOT NO_SYSTEMD AND SYSTEMD_FOUND AND Qt5DBus_FOUND)

if(BUILD_CORE)

    message(STATUS "Compile core")
    add_subdirectory(helper)
    add_subdirectory(share)
    
endif(BUILD_CORE)

if(BUILD_GUI)

    message(STATUS "Compile GUI")
    add_subdirectory(fancontrol-gui)
    
endif(BUILD_GUI)

#translations
FILE(GLOB MO_FILES po/*.mo)

set(catalogname fancontrol-gui)

FOREACH(MO_FILE ${MO_FILES})
    
    GET_FILENAME_COMPONENT(_moFileName ${MO_FILE} NAME)
    STRING(REGEX REPLACE "^${catalogname}_?" "" _langCode ${_moFileName} )
    STRING(REGEX REPLACE "\\.mo$" "" _langCode ${_langCode} )

    IF( _langCode )
    
        INSTALL(FILES ${MO_FILE} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${catalogname}.mo)
        LIST(APPEND GMO_FILES ${_gmoFile})
    
    ENDIF( _langCode )

ENDFOREACH(MO_FILE ${MO_FILES})