# fancontrol-gui
GUI for Fancontrol. 
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file. 
Furthermore it communicates with systemd via dbus to control the fancontrol service. 
If you want to compile without systemd support set the option -DNO_SYSTEMD=true.


At the moment it consists of two parts:

The Core part which I plan to reuse for a kcm in the future and can be build by providing -DBUILD_CORE=1
The GUI part which can be build by providing -DBUILD_GUI=1
