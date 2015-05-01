# fancontrol-gui
GUI for Fancontrol. 
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file. 
Furthermore it communicates with systemd via dbus to control the fancontrol service. 
If you want to compile without systemd support set the option -DNO_SYSTEMD=true.
