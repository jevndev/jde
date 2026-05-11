#   /$$$$$$   /$$$$$$  /$$   /$$ /$$$$$$$$ /$$$$$$  /$$$$$$
#  /$$__  $$ /$$__  $$| $$$ | $$| $$_____/|_  $$_/ /$$__  $$
# | $$  \__/| $$  \ $$| $$$$| $$| $$        | $$  | $$  \__/
# | $$      | $$  | $$| $$ $$ $$| $$$$$     | $$  | $$ /$$$$
# | $$      | $$  | $$| $$  $$$$| $$__/     | $$  | $$|_  $$
# | $$    $$| $$  | $$| $$\  $$$| $$        | $$  | $$  \ $$
# |  $$$$$$/|  $$$$$$/| $$ \  $$| $$       /$$$$$$|  $$$$$$/
#  \______/  \______/ |__/  \__/|__/      |______/ \______/
cmake_minimum_required(VERSION 3.24)
project(jde VERSION 1.0.0)

add_library(jde INTERFACE)
add_library(jde::jde ALIAS jde)

target_include_directories(jde INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(jde INTERFACE cxx_std_23)


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS jde EXPORT jdeTargets)
install(DIRECTORY src/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT jdeTargets
    FILE jdeTargets.cmake
    NAMESPACE jde::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jde
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/jdeConfig.cmake.in [[
@PACKAGE_INIT@
include("${CMAKE_CURRENT_LIST_DIR}/jdeTargets.cmake")
check_required_components(jde)
]])

configure_package_config_file(
    ${CMAKE_CURRENT_BINARY_DIR}/jdeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/jdeConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jde
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/jdeVersionConfig.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/jdeConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/jdeVersionConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jde
)

#  /$$$$$$$$ /$$$$$$$$  /$$$$$$  /$$$$$$$$ /$$$$$$ /$$   /$$  /$$$$$$
# |__  $$__/| $$_____/ /$$__  $$|__  $$__/|_  $$_/| $$$ | $$ /$$__  $$
#    | $$   | $$      | $$  \__/   | $$     | $$  | $$$$| $$| $$  \__/
#    | $$   | $$$$$   |  $$$$$$    | $$     | $$  | $$ $$ $$| $$ /$$$$
#    | $$   | $$__/    \____  $$   | $$     | $$  | $$  $$$$| $$|_  $$
#    | $$   | $$       /$$  \ $$   | $$     | $$  | $$\  $$$| $$  \ $$
#    | $$   | $$$$$$$$|  $$$$$$/   | $$    /$$$$$$| $$ \  $$|  $$$$$$/
#    |__/   |________/ \______/    |__/   |______/|__/  \__/ \______/
option(JDE_BUILD_TESTS "Build jde tests" ${PROJECT_IS_TOP_LEVEL})

if(JDE_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        FIND_PACKAGE_ARGS NAMES GTest
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

    add_executable(run_tests ${TEST_SOURCES})

    target_link_libraries(run_tests PRIVATE
        jde::jde
        GTest::gtest_main
    )


    target_include_directories(run_tests PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

    include(GoogleTest)
    gtest_discover_tests(run_tests)
endif()
