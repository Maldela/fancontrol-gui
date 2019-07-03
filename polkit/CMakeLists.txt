configure_file(rules.in ${STANDARD_HELPER_ID}.rules @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${STANDARD_HELPER_ID}.rules DESTINATION /etc/polkit-1/rules.d)
