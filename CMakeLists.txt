cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)

project(QGSQ VERSION 0.0.1)

find_package(Qt5 5.6.0 COMPONENTS
    Core
    Network
)

set(QGSQ_API_LEVEL "0")

set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Forbid in-tree building
if(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
      message(STATUS "Please do an out-of-tree build:")
      message(STATUS "rm -f CMakeCache.txt && mkdir build && cd build; cmake .. && make")
      message(FATAL_ERROR "In-tree-build detected!")
endif(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

option(BUILD_TEST_APP "Build the command line test application" OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
    "/usr" CACHE PATH "cutelyst default install prefix" FORCE)
endif()

set(CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE}" CACHE PATH "Output directory for libraries")

# cmake config files
configure_file(${CMAKE_MODULE_PATH}/QGSQ-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QGSQConfig.cmake
    @ONLY
)
configure_file(${CMAKE_MODULE_PATH}/QGSQ-config-version.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QGSQConfigVersion.cmake
    @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QGSQConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/QGSQConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QGSQ/
)
#install(EXPORT QGSQTargets
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QGSQ/
#    FILE QGSQTargets.cmake
#    COMPONENT Devel
#)

add_subdirectory(QGSQ)
if (BUILD_TEST_APP)
add_subdirectory(testapp)
endif (BUILD_TEST_APP)
