set(LIBRARIES KF5::QuickAddons
              KF5::I18n
              fancontrol_gui_lib)
              
find_package(KF5Declarative REQUIRED)

add_library(kcm_fancontrol MODULE src/fancontrolkcm.cpp)
                                  
target_link_libraries(kcm_fancontrol ${LIBRARIES})

install(TARGETS kcm_fancontrol DESTINATION "${PLUGIN_INSTALL_DIR}/kcms")
install(FILES kcm_fancontrol.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kcoreaddons_desktop_to_json(kcm_fancontrol "kcm_fancontrol.desktop")