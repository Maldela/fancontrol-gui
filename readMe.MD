# `fancontrol-gui`

A GUI for `fancontrol` which is part of `lm_sensors` <link>https://github.com/lm-sensors/lm-sensors</link>. It uses the KAuth module of KDE Frameworks 5 to write the generated config file. Furthermore, it communicates with SystemD via DBus to control the `fancontrol` service.

KAuth currently doesn't support install prefixes other than where KAuth itself was installed. If you want to use another install prefix, you have to run the application as root or another user with the necessary privileges to avoid the KAuth helper.

If you want to avoid authorizing yourself when using the helper you can set the option `-DINSTALL_POLKIT=true`. This will install a Polkit rules file allowing members of the group '`fancontrol`' to edit the config file and manipulate the SystemD service. You can change the group name with the `-DPOLKIT_GROUP_NAME` option. Service name and config file can be set with the options `-DSTANDARD_SERVICE_NAME` and `-DSTANDARD_CONFIG_FILE`.

If you want to compile without SystemD support set the option `-DNO_SYSTEMD=true`.

If your distro looks for QML plugins in `/usr/lib/qt/qml` instead of `/usr/lib/qml` you need to set the option `-DKDE_INSTALL_USE_QT_SYS_PATHS=true`.

To compile the additional KCM set the CMake option `-DBUILD_KCM=on`. The KCM is only built, if the `-DNO_SYSTEMD` option is unset or set to false.

To compile the additional KDE Plasma plasmoid set the CMake option `-DBUILD_PLASMOID=on`.

# Screenshots

> ![image](https://user-images.githubusercontent.com/8409391/89116324-02da7480-d4bd-11ea-867c-3172edf87f2e.png) ![image](https://user-images.githubusercontent.com/8409391/89116322-f8b87600-d4bc-11ea-89e4-515121cd7d71.png) ![image](https://user-images.githubusercontent.com/8409391/89116328-0a9a1900-d4bd-11ea-955f-f4e80c885d8b.png) ![image](https://user-images.githubusercontent.com/8409391/89116329-108ffa00-d4bd-11ea-990c-2c1f2ca3dc90.png) <!-- Allows them to render as a grid in landscape. -->

# Build Requirements

<table>
  <tr>
    <th>Qt5</th>
    <td>
       <ul>
         <li>Base/Core</li>
         <li>Widgets</li>
         <li>Gui</li>
         <li>QML</li>
       </ul>
    </td>
  </tr>
  <tr>
    <th>KF5</th>
    <td>
       <ul>
         <li>I18n</li>
         <li>Auth</li>
         <li>Config</li>
         <li>Package</li>
         <li>Declarative</li>
         <li>CoreAddons</li>
         <li>DBusAddons</li>
         <li>Extra-Cmake-Modules</li>
         <li>Notifications</li>
       </ul>
    </td>
  </tr>
  <tr>
    <th>Other</th>
    <td>
       <ul>
         <li>C++ compiler</li>
         <li>Gettext</li>
         <li>CMake</li>
       </ul>
    </td>
  </tr>
</table>

## Additional Runtime Requirements

<table>
  <tr>
    <th>Qt5</th>
    <td>
       <ul>
         <li>BQuick 2.6</li>
         <li>QuickControls2 2.1</li>
         <li>QuickLayouts 1.2</li>
         <li>QuickDialogs 1.2</li>
       </ul>
    </td>
  </tr>
  <tr>
    <th>KF5</th>
    <td>Kirigami2 2.3</td>
  </tr>
</table>

## Additional Requirements for KCM

<table>
  <tr>
    <th>KF5</th>
    <td>KCMUtils</td>
  </tr>
</table>

## Additional Requirements for Plasmoid

<table>
  <tr>
    <th>KF5</th>
    <td>Plasma</td>
  </tr>
</table>

## Commands to Install Requirements

### Debian/Ubuntu Command to Install the Compilation Requirements

```sh
sudo apt-get install ca-certificates git build-essential cmake gcc g++ libkf5config-dev libkf5auth-dev libkf5package-dev libkf5declarative-dev libkf5coreaddons-dev libkf5dbusaddons-dev libkf5kcmutils-dev libkf5i18n-dev libkf5plasma-dev libqt5core5a libqt5widgets5 libqt5gui5 libqt5qml5 extra-cmake-modules qtbase5-dev libkf5notifications-dev qml-module-org-kde-kirigami2 qml-module-qtquick-dialogs qml-module-qtquick-controls2 qml-module-qtquick-layouts qml-module-qt-labs-settings qml-module-qt-labs-folderlistmodel gettext
```

> [!NOTE]  
> This was tested on `Ubuntu 20.04.1 LTS`, `Ubuntu 24.04 LTS` and `Debian 10`.

### Fedora

```sh
sudo dnf install git gcc g++ cmake kf5-ki18n-devel kf5-kauth-devel kf5-kconfig-devel kf5-kpackage-devel kf5-kcoreaddons-devel kf5-kdbusaddons-devel extra-cmake-modules kf5-knotifications-devel qt5-qtquickcontrols2-devel kf5-kconfigwidgets-devel kf5-kcmutils-devel kf5-plasma-devel cmake gettext qt5-qtbase-devel gcc-c++ kf5-kdeclarative-devel qt5-qtquickcontrols qt5-qtquickcontrols2
```

> [!NOTE]  
> This was tested on `Fedora 33`.

### Nix

This repository offers a nix flake:

```sh
nix develop
nix build
```

# Installation

```sh
git clone https://github.com/Maldela/fancontrol-gui.git
cd fancontrol-gui
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_KCM=on -DBUILD_PLASMOID=on
make -j
sudo make install
```

## Nix

This repository offers a home-manager module or alternatively the package itself.

Add this repository to your inputs.
```nix
fancontrol-gui.url = "github:Maldela/fancontrol-gui";
```

Enable the home-manager module.
```nix
inputs.fancontrol-gui.homeManagerModules.default
```

And add fancontrol-gui to your enabled programs.
```nix
programs.fancontrol-gui.enable = true;
```

# Uninstallation

```sh
cd fancontrol-gui/build
sudo make uninstall
```
