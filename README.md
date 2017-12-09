# fancontrol-gui
GUI for Fancontrol. 
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file. 
Furthermore it communicates with systemd via dbus to control the fancontrol service.

KAuth currently doesn't support install prefixes other than where KAuth itself was installed.
If you want to use another install prefix, you have to run the application as root or another user with the necessary privileges to avoid the KAuth helper.

If you want to compile without systemd support set the option -DNO_SYSTEMD=true.

To compile the additional KCM set the cmake option -DBUILD_KCM=on.
The KCM is only build, if the -DNO_SYSTEMD option is unset or set to false.

# Requirements for building
* Qt5: Base/Core, Widgets, GUI, QML
* KF5: I18n, Auth, Config, Package, Declarative, CoreAddons, KCMUtils

## Debian/Ubuntu command to install the build requirements:
`sudo apt-get install libkf5config-dev libkf5auth-dev libkf5package-dev libkf5declarative-dev libkf5coreaddons-dev libkf5kcmutils-dev libkf5i18n-dev libqt5core5a libqt5widgets5 libqt5gui5 libqt5qml5 extra-cmake-modules qtbase5-dev`

**Note:** This was tested on Linux Mint 18.2 KDE, which is based on Ubuntu 16.04 LTS (Debian 9 Stretch/Sid).

## Example:

```
git clone https://github.com/Maldela/fancontrol-gui.git
cd fancontrol-gui
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_KCM=on
make -j2
sudo make install
```
