cmake_minimum_required(VERSION 3.9)
project(ctwitch VERSION 0.1 DESCRIPTION "Single-threaded C99 library for Twitch API.")
include(GNUInstallDirs)

# Public headers
set(EDV_PUBLIC_INCLUDE_DIRECTORIES
  include/
)

# Private headers
set(EDV_PRIVATE_INCLUDE_DIRECTORIES
  src/
)

# Library source code
add_library(ctwitch SHARED
  src/json/json.c
  src/utils/string_utils.c
  src/utils/array_utils.c
  src/utils/network_utils.c
  src/utils/parser.c
  src/data.c
  src/v5/users.c
  src/v5/search.c
  src/v5/streams.c
  src/init.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${EDV_PUBLIC_INCLUDE_DIRECTORIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${EDV_PRIVATE_INCLUDE_DIRECTORIES})

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

# Example target
add_executable(twitch-remote
  example/twitch-remote.c
  src/json/json.c
  src/utils/string_utils.c
  src/utils/array_utils.c
  src/utils/network_utils.c
  src/utils/parser.c
  src/data.c
  src/v5/users.c
  src/v5/search.c
  src/v5/streams.c
  src/init.c
)

target_include_directories(twitch-remote PUBLIC ${EDV_PUBLIC_INCLUDE_DIRECTORIES})
target_include_directories(twitch-remote PRIVATE ${EDV_PRIVATE_INCLUDE_DIRECTORIES})
target_link_libraries(twitch-remote m curl)
