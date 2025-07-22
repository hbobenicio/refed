##
# References
# - https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html
# - https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
# - https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
# - https://cmake.org/cmake/help/latest/command/install.html
# - https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#creating-packages
# - https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:configure_package_config_file
cmake_minimum_required(VERSION 3.23)

project(refed
    VERSION   0.0.1
    LANGUAGES CXX
)

include(FetchContent)

# We include this just to use CMAKE_INSTALL_INCLUDEDIR and CMAKE_INSTALL_LIBDIR (is that an overkill?!)
# @see https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
include(GNUInstallDirs)

# Module to get access to some helper functions for creating config files that
# will ease the use of find_package command by client projects and integrate better with cmake FetchContent_MakeAvailable()
include(CMakePackageConfigHelpers)

###########################################
# Target: refed (the main library target) #
###########################################

# This is a header only library
add_library(refed INTERFACE)

# This defines the API public headers.
#
# Since CMake 3.23, header files may be associated with a library
# by adding them to a header set using the target_sources() command.
# This frees us of needing to manage install(FILE ...) or install(DIRECTORY) for them.
#
# When we specify the FILE_SET, the BASE_DIRS we define automatically become
# include directories in the usage requirements for the target and those directories are automatically added
# to the target's include directories.
#
# @see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
target_sources(refed INTERFACE
    FILE_SET HEADERS
        # Any relative path is treated as relative to the current source directory (i.e. CMAKE_CURRENT_SOURCE_DIR)
        BASE_DIRS "include/"
        FILES
            "include/refed/defer.hpp"
)

# This library implementation is based on lambda expressions which is a C++11 feature
target_compile_features(refed INTERFACE cxx_std_11)

##############
# Tests      #
##############

# GoogleTest requires at least C++17
# TODO do this only for the tests target
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

######################################################################
# Tests: Target: refed-tests-unit (Unit Tests for the refed library) #
######################################################################

add_executable(refed-tests-unit
    "${CMAKE_SOURCE_DIR}/tests/dummy.cpp"
)

target_link_libraries(refed-tests-unit
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(refed-tests-unit)

###########
# Install #
###########

# Install the target and headers
# 
# @see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
# @see https://cmake.org/cmake/help/latest/command/install.html#targets
install(TARGETS refed
    EXPORT refedTargets
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    # INCLUDES         DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"

    # These below are not needed, at least for now...
    # ARCHIVE        DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    # LIBRARY        DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    # RUNTIME        DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install the export set
# 
# This will produce a cmake file at `$CMAKE_INSTALL_PREFIX/lib/cmake/refed/refedTargets.cmake`
# for projects to import the refed target with `refed::refed`
install(EXPORT refedTargets
    NAMESPACE   "refed::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/refed"
)

# Generate the config and version files
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/refedConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

# https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:configure_package_config_file
configure_package_config_file(
    "refedConfig.cmake.in"                            # Input
    "${CMAKE_CURRENT_BINARY_DIR}/refedConfig.cmake"   # Output
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/refed"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/refedConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/refedConfigVersion.cmake"
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/refed"
)
