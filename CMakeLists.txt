############################# DEFINE ##############################
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(reaper_borovylo)

# Define C standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set compile compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add cmake include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Define paths
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_MODULES_DIR ${ROOT}/modules)
set(WDL_INCLUDE ${PROJECT_MODULES_DIR}/WDL)
set(JSON_INCLUDE_DIRS ${PROJECT_MODULES_DIR}/json/src)
set(FPLUS_INCLUDE_DIRS ${PROJECT_MODULES_DIR}/fplus/include)
set(LOCAL_DIR /usr/local)

# @WARNING delete if unused
set(MACH7_INCLUDE_DIRS ${PROJECT_MODULES_DIR}/Mach7/code)

# Include cmake/reaper_plugin.cmake, copy reaper_plugin headers
include(reaper_plugin)

set(ALL_INCLUDE_SOURCE
  ${LOCAL_DIR}/include
  ${JSON_INCLUDE_DIRS}
  ${FPLUS_INCLUDE_DIRS}
  # ${MACH7_INCLUDE_DIRS}
  ${WDL_INCLUDE}
  ${WDL_INCLUDE}/reaper
  ${ROOT}/src
)

# Set extension version
set(MY_PRODUCT_NUMBER 0)
set(MY_PRODUCT_VERSION 1)
set(MY_BUILD_NUMBER 0)

############################ CONFIGURE ############################
# Generate version.rc file
configure_file(
  ${CMAKE_MODULE_PATH}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY
)

############################# BUILD ###############################
SET(SOURCES
  src/main.cpp
  src/csurf_cabl.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

############################ INCLUDE ##############################
target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE_SOURCE})

find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(${PROJECT_NAME} 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

# Add /usr/local path for libs
link_directories(${LOCAL_DIR}/lib)
