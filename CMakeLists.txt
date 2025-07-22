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

# We include this just to use CMAKE_INSTALL_INCLUDEDIR (is that an overkill?!)
# @see https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
include(GNUInstallDirs)

###########################################
# Target: refed (the main library target) #
###########################################

# This is a header only library, so only use INTERFACE with no sources.
# We'll add the public API headers just bellow with the `target_sources(... FILE_SET HEADERS ...)` command.
add_library(refed INTERFACE)

# Since CMake 3.23, header files may be associated with a library
# by adding them to a header set using the target_sources() command.
#
# This provides better flexibility and let us avoid using
# `target_include_directories(FOO INTERFACE ...)` and `install(DIRECTORY include/ ...)`.
#
# When we specify the FILE_SET, the BASE_DIRS we define automatically become
# include directories in the usage requirements for the target and those directories are automatically added
# to the target's include directories.
#
# @see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
target_sources(refed PUBLIC
    FILE_SET HEADERS
        # Any relative path is treated as relative to the current source directory (i.e. CMAKE_CURRENT_SOURCE_DIR)
        BASE_DIRS "include/"
        FILES
            "include/refed/defer.hpp"
)

# This librarimplementation is based on lambda expressions which is a c++11 feature
target_compile_features(refed INTERFACE cxx_std_11)

##############
# TODO Tests #
############## 

###########
# Install #
###########

# Module to get access to some helper functions for creating config files that
# will ease the use of find_package command and integrate better with cmake FetchContent_MakeAvailable()
include(CMakePackageConfigHelpers)

# @see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
# @see https://cmake.org/cmake/help/latest/command/install.html#targets
install(TARGETS refed
    EXPORT refedTargets
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    INCLUDES         DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"

    # These below are not needed, at least for now...
    # ARCHIVE        DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    # LIBRARY        DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    # RUNTIME        DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# This will produce a cmake file at `$CMAKE_INSTALL_PREFIX/lib/cmake/refed/refedTargets.cmake`
# for projects to import the refed target with `refed::refed`
install(EXPORT refedTargets
    NAMESPACE   "refed::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/refed"
)

# https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#command:configure_package_config_file
configure_package_config_file(
    "Config.cmake.in"                                 # Input
    "${CMAKE_CURRENT_BINARY_DIR}/refedConfig.cmake"   # Output
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/refed"
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/refedConfig.cmake"
        #   "${CMAKE_CURRENT_BINARY_DIR}/refedConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/refed
)
