find_package(Qt5 REQUIRED Test)

include(ECMAddTests)

ecm_add_tests(loadertest.cpp LINK_LIBRARIES Qt5::Test fancontrol_gui_lib)