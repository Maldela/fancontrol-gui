find_package(Qt5 REQUIRED Test)

include(ECMAddTests)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(TESTLIB_LIBS ${LIB_PRIVATE_LIBRARIES}
                 ${LIB_PUBLIC_LIBRARIES})

foreach(_src ${LIB_SRCS})
    set(TESTLIB_SRCS ${TESTLIB_SRCS}
                     "../${_src}")
endforeach()

set(TESTLIB_SRCS ${TESTLIB_SRCS}
                 testfan.cpp
                 testtemp.cpp
                 testpwmfan.cpp)

add_library(fancontrol_test_lib SHARED ${TESTLIB_SRCS})
set_target_properties(fancontrol_test_lib PROPERTIES CXX_VISIBILITY_PRESET default)
set_target_properties(fancontrol_test_lib PROPERTIES VISIBILITY_INLINES_HIDDEN OFF)
target_link_libraries(fancontrol_test_lib PUBLIC ${TESTLIB_LIBS})

set(TEST_LIBS Qt5::Test
              fancontrol_test_lib)


#temptest

ecm_add_test(temptest.cpp LINK_LIBRARIES ${TEST_LIBS})


#fantest

ecm_add_test(fantest.cpp LINK_LIBRARIES ${TEST_LIBS})


#pwmfantest

ecm_add_test(pwmfantest.cpp LINK_LIBRARIES ${TEST_LIBS})


#loadertest

ecm_add_test(loadertest.cpp LINK_LIBRARIES ${TEST_LIBS})

