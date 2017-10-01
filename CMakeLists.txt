############################# DEFINE ##############################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(reaper_controller)

# Define C standart
set(CMAKE_CXX_STANDARD 17)

# set compile compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add cmake include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Define paths
set(ROOT ${CMAKE_CURRENT_LIST_DIR})
set(PROJECT_MODULES_DIR ${ROOT}/modules)
set(WDL_INCLUDE ${PROJECT_MODULES_DIR}/WDL)
set(FPLUS_INCLUDE_DIRS ${PROJECT_MODULES_DIR}/fplus/include )
set(MACH7_INCLUDE_DIRS ${PROJECT_MODULES_DIR}/Mach7/code )
set(LOCAL_DIR /usr/local)

set(ALL_INCLUDE_SOURCE
  ${LOCAL_DIR}/include
  ${FPLUS_INCLUDE_DIRS}
  ${MACH7_INCLUDE_DIRS}
  ${WDL_INCLUDE}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set extension version
set(MY_PRODUCT_NUMBER 0)
set(MY_PRODUCT_VERSION 0)
set(MY_BUILD_NUMBER 4)

############################ CONFIGURE ############################
# Generate version.rc file
configure_file(
  ${CMAKE_MODULE_PATH}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY
)

############################# BUILD ###############################
SET(SOURCES src/main.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

############################ INCLUDE ##############################
# Include cmake/reaper_plugin.cmake, copy reaper_plugin headers
include(reaper_plugin)

target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE_SOURCE})

# Add /usr/local path for libs
link_directories(${LOCAL_DIR}/lib)
