cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(reaper_controller)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")


############################# DEFINE ##############################
# Define C standart
set(CMAKE_CXX_STANDARD 14)

# Define paths
set(PROJECT_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/modules)
set(WDL_INCLUDE ${PROJECT_MODULES_DIR}/WDL)
set(LOCAL_DIR /usr/local)

# Set extension version
set(MY_PRODUCT_NUMBER 0)
set(MY_PRODUCT_VERSION 0)
set(MY_BUILD_NUMBER 1)


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
# Add WDL headers to project
include_directories(${WDL_INCLUDE}) 

# Include cmake/reaper_plugin.cmake, copy reaper_plugin headers
include(reaper_plugin)
include_directories(${WDL_INCLUDE}/reaper) 

# Add /usr/local path for headers and libs
include_directories(${LOCAL_DIR}/include)
link_directories(${LOCAL_DIR}/lib)


