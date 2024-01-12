cmake_minimum_required(VERSION 3.9)
project(ctwitch VERSION 0.1 DESCRIPTION "Single-threaded C99 library for Twitch API.")

# Find libCURL
find_library(CURL_LIB curl)
find_path(CURL_INCLUDE curl)

# Public headers
set(EDV_PUBLIC_INCLUDE_DIRECTORIES
  include/
)

# Private headers
set(EDV_PRIVATE_INCLUDE_DIRECTORIES
  src/
)

# Global headers
include(GNUInstallDirs)

# Library source code
add_library(ctwitch SHARED
  src/json/json.c
  src/utils/string_utils.c
  src/utils/array_utils.c
  src/utils/network_helix_utils.c
  src/utils/network_utils.c
  src/utils/parser.c
  src/utils/data_utils.c
  src/auth.c
  src/helix/data.c
  src/helix/users.c
  src/helix/streams.c
  src/helix/games.c
  src/helix/teams.c
  src/helix/channels.c
  src/helix/videos.c
  src/helix/search.c
  src/init.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC ${EDV_PUBLIC_INCLUDE_DIRECTORIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${EDV_PRIVATE_INCLUDE_DIRECTORIES})
target_link_libraries(ctwitch m ${CURL_LIB})

# Project settings
set_target_properties(ctwitch PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

# Install library
install(TARGETS ctwitch
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install library headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Generate pkg-config
set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set(PRIVATE_LIBS "-lctwitch")
configure_file(libctwitch.pc.in libctwitch.pc @ONLY)

# Install pkg-config
install(FILES ${CMAKE_BINARY_DIR}/libctwitch.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# Example target
add_executable(twitch-remote
  example/twitch-remote.c
  src/json/json.c
  src/utils/string_utils.c
  src/utils/array_utils.c
  src/utils/network_helix_utils.c
  src/utils/network_utils.c
  src/utils/parser.c
  src/utils/data_utils.c
  src/auth.c
  src/helix/data.c
  src/helix/users.c
  src/helix/streams.c
  src/helix/games.c
  src/helix/teams.c
  src/helix/channels.c
  src/helix/videos.c
  src/helix/search.c
  src/init.c
)

target_include_directories(twitch-remote PUBLIC ${EDV_PUBLIC_INCLUDE_DIRECTORIES})
target_include_directories(twitch-remote PRIVATE ${EDV_PRIVATE_INCLUDE_DIRECTORIES})
target_link_libraries(twitch-remote m ${CURL_LIB})
target_compile_options(twitch-remote PUBLIC -g)
