# cmake_minimum_required(VERSION 3.16)
# project(internalLib VERSION 1.0.0)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# option(BUILD_SHARED_LIBS "Build shared library" OFF)

# # ==============================
# # LIBRARY
# # ==============================
# add_library(internalLib
#     src/Socket.cpp
# )

# target_include_directories(internalLib
#     PUBLIC
#         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Includes>
#         $<INSTALL_INTERFACE:include>
# )

# # If using Linux sockets
# target_link_libraries(internalLib PUBLIC pthread)

# # ==============================
# # TEST
# # ==============================
# # add_executable(TestInternalLib.exe test/test_socket.cpp)

# # target_link_libraries(TestInternalLib.exe internalLib)

# add_executable(server.exe
#     test/test_socket.cpp
# )

# target_compile_definitions(server.exe PRIVATE SERVER)
# target_link_libraries(server.exe internalLib)

# # ==============================
# # CLIENT EXECUTABLE
# # ==============================
# add_executable(client.exe
#     test/test_socket.cpp
# )

# target_compile_definitions(client.exe PRIVATE CLIENT)
# target_link_libraries(client.exe internalLib)

# # ==============================
# # INSTALL
# # ==============================
# include(GNUInstallDirs)

# install(TARGETS internalLib
#     EXPORT internalLibTargets
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # .a
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}   # .so
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# install(DIRECTORY Includes/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# # ==============================
# # EXPORT
# # ==============================
# install(EXPORT internalLibTargets
#     FILE internalLibTargets.cmake
#     NAMESPACE internalLib::
#     DESTINATION lib/cmake/internalLib
# )

# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/internalLibConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/internalLibConfig.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/internalLibConfig.cmake"
#     @ONLY
# )

# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/internalLibConfig.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/internalLibConfigVersion.cmake"
#     DESTINATION lib/cmake/internalLib
# )





cmake_minimum_required(VERSION 3.16)

project(internalLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_SHARED_LIBS "Build shared libs" OFF)

# ==========================
# Library
# ==========================
add_library(internalLib
    src/Socket.cpp
)

target_include_directories(internalLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Includes>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(internalLib PUBLIC Threads::Threads)

find_package(Threads REQUIRED)

# ==========================
# Test Apps
# ==========================
add_executable(server.exe test/test_socket.cpp)
target_compile_definitions(server.exe PRIVATE SERVER)
target_link_libraries(server.exe PRIVATE internalLib)

add_executable(client.exe test/test_socket.cpp)
target_compile_definitions(client.exe PRIVATE CLIENT)
target_link_libraries(client.exe PRIVATE internalLib)

# ==========================
# Install
# ==========================
include(GNUInstallDirs)

install(TARGETS internalLib
    EXPORT internalLibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY Includes/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ==========================
# Package Config
# ==========================
install(EXPORT internalLibTargets
    FILE internalLibTargets.cmake
    NAMESPACE internalLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/internalLib
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/internalLibConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/internalLibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/internalLibConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/internalLib
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/internalLibConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/internalLibConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/internalLib
)