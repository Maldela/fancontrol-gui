# fancontrol-gui
GUI for Fancontrol. 
It uses the KAuth module of the KDE Frameworks 5 to write the generated config file. 
If you don't want to compile with KF5 set the compile option -DNO_KF5_KAUTH. 
Furthermore it communicates with systemd via dbus to control the fancontrol service. 
If you want to compile without systemd support set the -DNO_SYSTEMD option.
