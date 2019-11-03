# fancontrol-gui
GUI for fancontrol which is part of lm_sensors <link>https://github.com/lm-sensors/lm-sensors</link>.
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file.
Furthermore it communicates with systemd via dbus to control the fancontrol service.

KAuth currently doesn't support install prefixes other than where KAuth itself was installed.
If you want to use another install prefix, you have to run the application as root or another user with the necessary privileges to avoid the KAuth helper.
If you want to avoid authorizing yourself when using the helper you can set the option -DINSTALL_POLKIT=true. This will install a polkit rules file allowing members of the group 'fancontrol' to edit the config file and manipulate the systemd service. You can change the group name with the -DPOLKIT_GROUP_NAME option. Service name and config file can be set with the options -DSTANDARD_SERVICE_NAME and -DSTANDARD_CONFIG_FILE.

If you want to compile without systemd support set the option -DNO_SYSTEMD=true.

If your distro looks for QML plugins in /usr/lib/qt/qml instead of /usr/lib/qml you need to set the option -DKDE_INSTALL_USE_QT_SYS_PATHS=true.

To compile the additional KCM set the cmake option -DBUILD_KCM=on.
The KCM is only build, if the -DNO_SYSTEMD option is unset or set to false.

To compile the additional KDE Plasma plasmoid set the cmake option -DBUILD_PLASMOID=on.

# Build requirements
* Qt5: Base/Core, Widgets, Gui, QML
* KF5: I18n, Auth, Config, Package, Declarative, CoreAddons, DBusAddons, Extra-Cmake-Modules, Notifications
* Other: C++ compiler, Gettext, CMake

# Additional runtime requirements
* Qt5: Quick 2.6, QuickControls2 2.1, QuickLayouts 1.2, QuickDialogs 1.2
* KF5: Kirigami2 2.3

# Additional requirements for KCM
* KF5: KCMUtils

# Additional requirements for plasmoid
* KF5: Plasma

# Commands to install requirements
## Debian/Ubuntu command to install the build requirements:
`sudo apt-get install libkf5config-dev libkf5auth-dev libkf5package-dev libkf5declarative-dev libkf5coreaddons-dev libkf5dbusaddons-dev libkf5kcmutils-dev libkf5i18n-dev libkf5plasma-dev libqt5core5a libqt5widgets5 libqt5gui5 libqt5qml5 extra-cmake-modules qtbase5-dev libkf5notifications-dev qml-module-org-kde-kirigami2 qml-module-qtquick-dialogs qml-module-qtquick-controls2 qml-module-qtquick-layouts qml-module-qt-labs-settings qml-module-qt-labs-folderlistmodel cmake build-essential gettext`
**Note:** This was tested on KDE Neon User Edition 5.14, which is based on Ubuntu 18.04 LTS (Debian 9 Stretch/Sid).
## Fedora:
```
sudo dnf install qt5-devel kf5-ki18n-devel kf5-kauth-devel kf5-kconfig-devel kf5-kpackage-devel kf5-kcoreaddons-devel kf5-kdbusaddons-devel extra-cmake-modules kf5-knotifications-devel qt5-qtquickcontrols2-devel kf5-kconfigwidgets-devel kf5-kcmutils-devel kf5-plasma-devel cmake gettext qt5-qtbase-devel gcc-c++ kf5-kdeclarative-devel qt5-qtquickcontrols qt5-qtquickcontrols2
```
**Note:** This was tested on Fedora 31.

## Example:

```
git clone https://github.com/Maldela/fancontrol-gui.git
cd fancontrol-gui
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_KCM=on -DBUILD_PLASMOID=on
make -j
sudo make install
```
# fancontrol-gui
GUI for fancontrol which is part of lm_sensors <link>https://github.com/lm-sensors/lm-sensors</link>.
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file.
Furthermore it communicates with systemd via dbus to control the fancontrol service.

KAuth currently doesn't support install prefixes other than where KAuth itself was installed.
If you want to use another install prefix, you have to run the application as root or another user with the necessary privileges to avoid the KAuth helper.
If you want to avoid authorizing yourself when using the helper you can set the option -DINSTALL_POLKIT=true. This will install a polkit rules file allowing members of the group 'fancontrol' to edit the config file and manipulate the systemd service. You can change the group name with the -DPOLKIT_GROUP_NAME option. Service name and config file can be set with the options -DSTANDARD_SERVICE_NAME and -DSTANDARD_CONFIG_FILE.

If you want to compile without systemd support set the option -DNO_SYSTEMD=true.

If your distro looks for QML plugins in /usr/lib/qt/qml instead of /usr/lib/qml you need to set the option -DKDE_INSTALL_USE_QT_SYS_PATHS=true.

To compile the additional KCM set the cmake option -DBUILD_KCM=on.
The KCM is only build, if the -DNO_SYSTEMD option is unset or set to false.

To compile the additional KDE Plasma plasmoid set the cmake option -DBUILD_PLASMOID=on.

# Build requirements
* Qt5: Base/Core, Widgets, Gui, QML
* KF5: I18n, Auth, Config, Package, Declarative, CoreAddons, DBusAddons, Extra-Cmake-Modules, Notifications
* Other: C++ compiler, Gettext, CMake

# Additional runtime requirements
* Qt5: Quick 2.6, QuickControls2 2.1, QuickLayouts 1.2, QuickDialogs 1.2
* KF5: Kirigami2 2.3

# Additional requirements for KCM
* KF5: KCMUtils

# Additional requirements for plasmoid
* KF5: Plasma

# Commands to install requirements
## Debian/Ubuntu:
```
sudo apt-get install libkf5config-dev libkf5auth-dev libkf5package-dev libkf5declarative-dev libkf5coreaddons-dev libkf5dbusaddons-dev libkf5kcmutils-dev libkf5i18n-dev libkf5plasma-dev libqt5core5a libqt5widgets5 libqt5gui5 libqt5qml5 extra-cmake-modules qtbase5-dev libkf5notifications-dev qml-module-org-kde-kirigami2 qml-module-qtquick-dialogs qml-module-qtquick-controls2 qml-module-qtquick-layouts qml-module-qt-labs-settings qml-module-qt-labs-folderlistmodel cmake build-essential gettext
```

**Note:** This was tested on KDE Neon User Edition 5.14, which is based on Ubuntu 18.04 LTS (Debian 9 Stretch/Sid).
## Fedora:
```
sudo dnf install qt5-devel kf5-ki18n-devel kf5-kauth-devel kf5-kconfig-devel kf5-kpackage-devel kf5-kcoreaddons-devel kf5-kdbusaddons-devel extra-cmake-modules kf5-knotifications-devel qt5-qtquickcontrols2-devel kf5-kconfigwidgets-devel kf5-kcmutils-devel kf5-plasma-devel cmake gettext qt5-qtbase-devel gcc-c++ kf5-kdeclarative-devel qt5-qtquickcontrols qt5-qtquickcontrols2
```
**Note:** This was tested on Fedora 31.

## Example:

```
git clone https://github.com/Maldela/fancontrol-gui.git
cd fancontrol-gui
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_KCM=on -DBUILD_PLASMOID=on
make -j
sudo make install
```
