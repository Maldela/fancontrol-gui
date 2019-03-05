find_package(KF5Plasma REQUIRED)

ecm_find_qmlmodule(org.kde.plasma.core 2.0)
ecm_find_qmlmodule(org.kde.plasma.components 2.0)

plasma_install_package(package org.kde.fancontrol.plasmoid)
