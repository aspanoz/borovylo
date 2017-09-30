# Copy reaper_plugin.h and reaper_plugin_functions.h from https://github.com/nvaccess/osara

message("-- [cmake] Copy reaper_plugin headers from osara git submodule")

set(REAPER_PLUGIN_DIRS ${WDL_INCLUDE}/reaper/reaper_plugin)
set(OSARA_INCLUDE ${PROJECT_MODULES_DIR}/osara/include/reaper)

# Make directory 
execute_process (
  COMMAND bash -c "if [ ! -d ${WDL_INCLUDE}/reaper/reaper_plugin ]; then
    cd ${WDL_INCLUDE} && mkdir -p reaper/reaper_plugin
  fi"
)

# osara reaper_plugin have include path ../../WDL/path/to/header.h
# Copy files into ${WDL_INCLUDE}/WDL for correct include path
execute_process (
  COMMAND bash -c "if [ ! -f ${REAPER_PLUGIN_DIRS}/reaper_plugin.h ]; then
    cp ${OSARA_INCLUDE}/reaper_plugin.h ${REAPER_PLUGIN_DIRS}
  fi"
)
execute_process (
  COMMAND bash -c "if [ ! -f ${REAPER_PLUGIN_DIRS}/reaper_plugin_functions.h ]; then
    cp ${OSARA_INCLUDE}/reaper_plugin_functions.h ${REAPER_PLUGIN_DIRS}
  fi"
)
