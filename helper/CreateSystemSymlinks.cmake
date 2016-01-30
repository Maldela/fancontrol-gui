
set(helper_conf_file "etc/dbus-1/system.d/fancontrol.gui.helper.conf")
set(helper_service_file "share/dbus-1/system-services/fancontrol.gui.helper.service")
set(helper_conf_file_destination "/${helper_conf_file}")
set(helper_service_file_destination "/usr/${helper_service_file}")


execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${helper_conf_file} ${helper_conf_file_destination} RESULT_VARIABLE result OUTPUT_VARIABLE output)
message(STATUS "Creating symlink: ${helper_conf_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_conf_file}")

if(${result})
    message(WARNING "Creation of symlink ${helper_conf_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_conf_file} failed:")
    message(WARNING "${output}")
endif(${result})


execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${helper_service_file} ${helper_service_file_destination} RESULT_VARIABLE result OUTPUT_VARIABLE output)
message(STATUS "Creating symlink: ${helper_service_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_service_file}")

if(${result})
    message(WARNING "Creation of symlink ${helper_service_file_destination} -> ${CMAKE_INSTALL_PREFIX}/${helper_service_file} failed:")
    message(WARNING "${output}")
endif(${result})
