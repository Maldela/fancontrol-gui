# file(RENAME "${CMAKE_INSTALL_PREFIX} "/etc/dbus-1/system.d/fancontrol.gui.helper.conf")
# file(RENAME "${CMAKE_INSTALL_PREFIX}/lib/kauth/fancontrol-gui-helper" "/usr/lib/kauth/fancontrol-gui-helper")
# file(RENAME "${CMAKE_INSTALL_PREFIX}/share/dbus-1/system-services/fancontrol.gui.helper.service" "/usr/share/dbus-1/system-services/fancontrol.gui.helper.service")

set(helper_conf_file "etc/dbus-1/system.d/fancontrol.gui.helper.conf")
set(helper_bin_file "lib/kauth/fancontrol-gui-helper")
set(helper_service_file "share/dbus-1/system-services/fancontrol.gui.helper.service")
set(helper_conf_file_destination "/${helper_conf_file}")
set(helper_bin_file_destination "/usr/${helper_bin_file}")
set(helper_service_file_destination "/usr/${helper_service_file}")

execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${helper_conf_file} ${helper_conf_file_destination} RESULT_VARIABLE result OUTPUT_VARIABLE output)
file(APPEND "${CMAKE_SOURCE_DIR}/install_manifest.txt" "${helper_conf_file_destination}")
message(STATUS "Creating symlink: ${helper_conf_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_conf_file}")

if(${result})
    message(WARNING "Creation of symlink ${helper_conf_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_conf_file} ${result} ${output} failed:")
    message(WARNING "${output}")
endif(${result})

execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${helper_bin_file} ${helper_bin_file_destination} RESULT_VARIABLE result OUTPUT_VARIABLE output)
file(APPEND "${CMAKE_SOURCE_DIR}/install_manifest.txt" "${helper_bin_file_destination}")
message(STATUS "Creating symlink: ${helper_bin_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_bin_file}")

if(${result})
    message(WARNING "Creation of symlink ${helper_bin_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_bin_file} ${result} ${output} failed:")
    message(WARNING "${output}")
endif(${result})

execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${helper_service_file} ${helper_service_file_destination} RESULT_VARIABLE result OUTPUT_VARIABLE output)
file(APPEND "${CMAKE_SOURCE_DIR}/install_manifest.txt" "${helper_service_file_destination}")
message(STATUS "Creating symlink: ${helper_service_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_service_file}")

if(${result})
    message(WARNING "Creation of symlink ${helper_service_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_service_file} ${result} ${output} failed:")
    message(WARNING "${output}")
endif(${result})
