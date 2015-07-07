set(Fancontrol-GUI_SRCS src/main.cpp)

set(LIBRARIES fancontrol-gui-share
              KF5::Declarative
              KF5::I18n)

add_executable(fancontrol-gui ${Fancontrol-GUI_SRCS} ${RESOURCES})
target_link_libraries(fancontrol-gui ${LIBRARIES})
set_property(TARGET fancontrol-gui PROPERTY CXX_STANDARD 11)

install(TARGETS fancontrol-gui RUNTIME DESTINATION ${BIN_INSTALL_DIR})
install(FILES other/fancontrol-gui.desktop DESTINATION "${SHARE_INSTALL_PREFIX}/applications")
install(FILES other/fancontrol-gui.svg DESTINATION "${ICON_INSTALL_DIR}/hicolor/scalable/apps")
install(FILES qml/fancontrol-gui.qml DESTINATION "${SHARE_INSTALL_PREFIX}/fancontrol-gui/qml")

FILE(GLOB MO_FILES po/*.mo)

set(catalogname fancontrol-gui)

FOREACH(MO_FILE ${MO_FILES})
    
    GET_FILENAME_COMPONENT(_moFileName ${MO_FILE} NAME)
    STRING(REGEX REPLACE "^${catalogname}_?" "" _langCode ${_moFileName} )
    STRING(REGEX REPLACE "\\.mo$" "" _langCode ${_langCode} )

    IF( _langCode )
    
        INSTALL(FILES ${MO_FILE} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${catalogname}.mo)
        LIST(APPEND GMO_FILES ${_gmoFile})
    
    ENDIF( _langCode )

ENDFOREACH(MO_FILE ${MO_FILES})