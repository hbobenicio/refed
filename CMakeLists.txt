# refed cmake project
#
# #### References
#
# - https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html
# - https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
# - https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
# - https://cmake.org/cmake/help/latest/command/install.html
# - https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#creating-packages
# - https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:configure_package_config_file
#
cmake_minimum_required(VERSION 3.23)

project(refed
    VERSION      0.0.1
    DESCRIPTION  "A simple C++ Defer implementation"
    HOMEPAGE_URL "https://github.com/hbobenicio/refed"
    LANGUAGES    CXX
)

# ------------------------------------------------------------------------------
# Options and Variables
# ------------------------------------------------------------------------------

#TODO consider renaming this to something like spdlog uses (SPDLOG_BUILD_TESTS)
option(REFED_TESTS_ENABLE "Refed: Enables building tests" OFF)

include(FetchContent)               # @see https://cmake.org/cmake/help/latest/module/FetchContent.html
include(GNUInstallDirs)             # @see https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
include(CMakePackageConfigHelpers)  # @see https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html

# ------------------------------------------------------------------------------
# Library Target: refed (aka refed::refed)
# ------------------------------------------------------------------------------

# This is a header only library
add_library(refed INTERFACE)
add_library(refed::refed ALIAS refed)

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

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

if(${REFED_TESTS_ENABLE})
    add_subdirectory("tests/unit")
endif()

# ------------------------------------------------------------------------------
# Install
#
# TODO pkgconfig?
# TODO deb packaging?
# TODO tarball packaging?
# ------------------------------------------------------------------------------
set(refed_install_namespace          "refed::")
set(refed_install_cmake_destination  "${CMAKE_INSTALL_LIBDIR}/cmake/refed")
set(refed_install_cmake_config_file  "${CMAKE_CURRENT_BINARY_DIR}/refedConfig.cmake")
set(refed_install_cmake_version_file "${CMAKE_CURRENT_BINARY_DIR}/refedConfigVersion.cmake")

# Install target output artifacts (public API headers + cmake files for find_package() and FetchContent)
# 
# @see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
# @see https://cmake.org/cmake/help/latest/command/install.html#targets
install(
    TARGETS refed
    EXPORT  refedTargets
    # DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
    # DESTINATION "${refed_install_cmake_destination}"
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    INCLUDES         DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    ARCHIVE          DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY          DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME          DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

export(TARGETS refed
    NAMESPACE "${refed_install_namespace}"
    FILE "${CMAKE_CURRENT_BINARY_DIR}/refedTargets.cmake"
)

# Install a CMake file exporting targets for dependent projects
# 
# This will produce a cmake file at `$CMAKE_INSTALL_PREFIX/lib/cmake/refed/refedTargets.cmake`
# for projects to import the refed target with `refed::refed`
install(EXPORT refedTargets
    DESTINATION "${refed_install_cmake_destination}"
    NAMESPACE   "${refed_install_namespace}"
)

# export(EXPORT refedTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/refedTargets.cmake")

# CMakePackageConfigHelpers: Create the Config file
# @see https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:configure_package_config_file
configure_package_config_file(
    "refedConfig.cmake.in"                # Input
    "${refed_install_cmake_config_file}"  # Output
    INSTALL_DESTINATION "${refed_install_cmake_destination}"
)

# CMakePackageConfigHelpers: Create the Version file
# @see https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:write_basic_package_version_file
write_basic_package_version_file(
    "${refed_install_cmake_version_file}"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${refed_install_cmake_config_file}"
        "${refed_install_cmake_version_file}"
    DESTINATION
        "${refed_install_cmake_destination}"
)
