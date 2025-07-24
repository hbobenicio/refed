# refed cmake project

# We rely on [target_sources FILE_SETS](https://cmake.org/cmake/help/latest/command/target_sources.html#file-sets)
# which were introduced in this version
cmake_minimum_required(VERSION 3.23)

project(refed
    VERSION      0.0.1  #TODO Check if this can be in sync with version control
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

# refed is a header-only Interface library
# @see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
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
target_sources(refed
    INTERFACE
        FILE_SET HEADERS
            # Any relative path here is treated as relative to the current source directory (i.e. CMAKE_CURRENT_SOURCE_DIR)
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
# ------------------------------------------------------------------------------
# TODO pkgconfig?
# TODO deb packaging?
# TODO tarball packaging?
# ------------------------------------------------------------------------------
set(refed_export_name                   "refed-targets")
set(refed_install_namespace             "refed::")
set(refed_install_cmake_destination     "${CMAKE_INSTALL_LIBDIR}/cmake/refed")
set(refed_install_cmake_config_file_in  "${CMAKE_CURRENT_LIST_DIR}/cmake/refed-config.cmake.in")
set(refed_install_cmake_config_file_out "${CMAKE_CURRENT_BINARY_DIR}/refed-config.cmake")
set(refed_install_cmake_version_file    "${CMAKE_CURRENT_BINARY_DIR}/refed-config-version.cmake")
set(refed_install_cmake_targets_file    "${CMAKE_CURRENT_BINARY_DIR}/${refed_export_name}.cmake")

# Install target output artifacts (public API headers, aka FILE_SET HEADERS + cmake files for find_package() and FetchContent)
# @see https://cmake.org/cmake/help/latest/command/install.html#targets
install(
    TARGETS refed
    EXPORT  "${refed_export_name}"
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    ARCHIVE          DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY          DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME          DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Exports the CMake Targets file to the build directory so that
# projects can use the our targets directly from the build dir without needing to install it
export(TARGETS refed
    NAMESPACE "${refed_install_namespace}"
    FILE "${refed_install_cmake_targets_file}"
)

# Install a CMake file exporting targets for dependent projects
# 
# This will produce a cmake file at `$CMAKE_INSTALL_PREFIX/lib/cmake/refed/${refed_export_name}.cmake`
# for projects to import the refed target with `refed::refed`
install(
    EXPORT      "${refed_export_name}"
    DESTINATION "${refed_install_cmake_destination}"
    NAMESPACE   "${refed_install_namespace}"
)

# CMakePackageConfigHelpers: Generate the CMake Config file
# @see https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:configure_package_config_file
configure_package_config_file(
    "${refed_install_cmake_config_file_in}"
    "${refed_install_cmake_config_file_out}"
    INSTALL_DESTINATION "${refed_install_cmake_destination}"
)

# CMakePackageConfigHelpers: Generate the CMake Config Version file
# @see https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:write_basic_package_version_file
write_basic_package_version_file(
    "${refed_install_cmake_version_file}"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

# Install cmake version and config files to the installation dir
install(
    FILES
        "${refed_install_cmake_config_file_out}"
        "${refed_install_cmake_version_file}"
    DESTINATION
        "${refed_install_cmake_destination}"
)

# ------------------------------------------------------------------------------
# Package
# ------------------------------------------------------------------------------
# - CPack: https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
# - TODO consider creating components and grouping them for separating
#        public header artifacts and cmake artifacts and pkg-config, etc...
# - TODO document these commands
#        `cmake -S . -B build -DCPACK_BINARY_DEB:BOOL=ON` # for configuring the build for the debian packaging
#        `cmake --build --target package`                 # for building the debian package
#        `cmake --build --target package_source`          # for building the debian source package
#        how to change package install prefix between `/usr` (default) and `/usr/local`
# ------------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME                "${PROJECT_NAME}")          #TODO do I need these redundant properties?
set(CPACK_PACKAGE_VENDOR              "br.com.hugobenicio")       #TODO do I need these redundant properties?
set(CPACK_PACKAGE_CONTACT             "hbobenicio <hbobenicio@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")   #TODO do I need these redundant properties?
set(CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}")       #TODO do I need these redundant properties?
set(CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}") #TODO do I need these redundant properties?
set(CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}") #TODO do I need these redundant properties?
set(CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}") #TODO do I need these redundant properties?

# set(CPACK_PACKAGE_INSTALL_DIRECTORY   "CPack Component Example")

#TODO how to manage optional cmake artifact files with cmake as an optional dependencies?
#     note that cmake may no be installed with the system package manager
# set(CPACK_DEBIAN_PACKAGE_DEPENDS      "")
#
# More examples:
#    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libc6 (< 2.4)")
#    list(APPEND CPACK_DEBIAN_PACKAGE_DEPENDS cmake)

include(CPack)
