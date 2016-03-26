set(LIBRARIES Qt5::Core
              KF5::QuickAddons
              KF5::CoreAddons
              KF5::I18n)

find_package(KF5 COMPONENTS CoreAddons Package Declarative REQUIRED)

add_library(kcm_fancontrol MODULE src/fancontrolkcm.cpp)

target_link_libraries(kcm_fancontrol ${LIBRARIES})

install(TARGETS kcm_fancontrol DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/kcms")
install(FILES kcm_fancontrol.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

kpackage_install_package(package kcm_fancontrol kcms)

kcoreaddons_desktop_to_json(kcm_fancontrol "kcm_fancontrol.desktop")
