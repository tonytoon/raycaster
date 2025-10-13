cmake_minimum_required(VERSION 3.28.2)

project(raycaster C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(
  ${SDL2_INCLUDE_DIRS} ${LibXml2_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS}
  ${tmx_INCLUDE_DIRS} ${LIBMIKMOD_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(raycaster main.c raycaster.c vectors.c sdlstuff.c input.c
                         player.c)
add_custom_command(
  TARGET raycaster
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:raycaster>
          ${CMAKE_SOURCE_DIR}/raycaster)
target_link_libraries(raycaster ${SDL2_LIBRARIES} SDL2_image)
# add_compile_options(-std=gnu11)
