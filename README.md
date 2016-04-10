# fancontrol-gui
GUI for Fancontrol. 
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file. 
Furthermore it communicates with systemd via dbus to control the fancontrol service.

KAuth currently doesn't support install prefixes other than where KAuth itself was installed.
If you want to use another install prefix, you have to run the application as root or another user with the necessary privileges to avoid the KAuth helper.

If you want to compile without systemd support set the option -DNO_SYSTEMD=true.

To compile the additional KCM set the cmake option -DBUILD_KCM=on.
The KCM is only build, if the -DNO_SYSTEMD option is unset or set to false.


Example:

git clone https://github.com/Maldela/fancontrol-gui.git
cd fancontrol-gui
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_KCM=on
make -j2
sudo make install
