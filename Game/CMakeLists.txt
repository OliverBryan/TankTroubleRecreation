project(Game VERSION 1.0 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SFML_BUILD_AUDIO FALSE)

message("Fetching SFML...")
FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)

message("Fetching Box2D...")
FetchContent_Declare(
	Box2D
	GIT_REPOSITORY https://github.com/erincatto/box2d.git
	GIT_TAG main
)
FetchContent_MakeAvailable(Box2D)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS True)
set(BUILD_SHARED_LIBS True)

message("Fetching Log...")
FetchContent_Declare(
    Log
    GIT_REPOSITORY https://github.com/OliverBryan/Log.git
    GIT_TAG main
)
FetchContent_MakeAvailable(Log)

set(BUILD_EXAMPLES OFF)
set(BUILD_TESTS OFF)

message("Fetching libconfig...")
FetchContent_Declare(
	libconfig
	GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
	GIT_TAG master
)
FetchContent_MakeAvailable(libconfig)

add_executable(Game ./src/Environment.cpp ./src/Environment.hpp 
			    ./src/Interface.cpp ./src/Interface.hpp
			    ./src/Main.cpp ./src/Maze.cpp ./src/Maze.hpp
			    ./src/Resources.cpp ./src/Resources.hpp
  			    ./src/Tank.cpp ./src/Tank.hpp
			    ./src/Config.cpp ./src/Config.hpp)
target_link_libraries(Game sfml-graphics sfml-network box2d Log libconfig++)

# copy dlls post build
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET Game POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE:sfml-network>
          $<TARGET_FILE:box2d>
          $<TARGET_FILE:Log>
          $<TARGET_FILE:libconfig++>
          $<TARGET_FILE_DIR:Game>)
  endif()
endif()

# copy other resources
add_custom_command(TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/res
  ${CMAKE_CURRENT_BINARY_DIR}/res)

add_custom_command(TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/res
  $<TARGET_FILE_DIR:Game>/res)
