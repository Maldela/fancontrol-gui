set(LIBRARIES Qt5::Core
              KF5::QuickAddons
              KF5::CoreAddons
              KF5::I18n)

find_package(KF5 COMPONENTS CoreAddons Package Declarative KCMUtils REQUIRED)

add_library(kcm_fancontrol MODULE src/fancontrolkcm.cpp)

target_link_libraries(kcm_fancontrol ${LIBRARIES})

install(TARGETS kcm_fancontrol DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/kcms")
install(PROGRAMS fancontrol-kcm.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

if(${KF5_VERSION} VERSION_GREATER_EQUAL 5.42.0)
    kpackage_install_bundled_package(package org.kde.fancontrol.kcm kcms)
else(${KF5_VERSION} VERSION_GREATER_EQUAL 5.42.0)
    kpackage_install_package(package org.kde.fancontrol.kcm kcms)
endif(${KF5_VERSION} VERSION_GREATER_EQUAL 5.42.0)

kcoreaddons_desktop_to_json(kcm_fancontrol "fancontrol-kcm.desktop" SERVICE_TYPES kcmodule.desktop)
