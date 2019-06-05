configure_file(org.kde.fancontrol.rules.in org.kde.fancontrol.rules @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/org.kde.fancontrol.rules" DESTINATION "/etc/polkit-1/rules.d")
