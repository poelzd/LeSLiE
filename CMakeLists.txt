# -- cmake --
project (leslie)

#-------------------------------------------------------------------------------
# general settings
#-------------------------------------------------------------------------------
cmake_minimum_required (VERSION 2.6)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


#-------------------------------------------------------------------------------
# include libraries
#-------------------------------------------------------------------------------
# EIGEN
include (FindEigen3) 
if (EIGEN_FOUND)
  add_definitions ("-DENABLE_EIGEN")
endif()

# TCLAP command line parser
include (FindTCLAP) 
if (TCLAP_FOUND)
  add_definitions ("-DENABLE_TCLAP")
endif()


#-------------------------------------------------------------------------------
# set paths
#-------------------------------------------------------------------------------
set (LESLIE_BASE_PATH  
  ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING
  "LESLIE_BASE_PATH" )

set (EXECUTABLE_PATH  
  ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING
  "EXECUTABLE_PATH" )


#-------------------------------------------------------------------------------
# includes
#-------------------------------------------------------------------------------
set (COMMON_INCLUDES
  ${LESLIE_BASE_PATH}
  ${EIGEN_INCLUDES}
  ${TCLAP_INCLUDES}
  )
include_directories (${COMMON_INCLUDES})


#-------------------------------------------------------------------------------
# targets
#-------------------------------------------------------------------------------
add_executable(lsqMain "${LESLIE_BASE_PATH}/src/lsqMain.C")
target_link_libraries (lsqMain ${COMMON_LIBS} )


#-------------------------------------------------------------------------------
# set output paths for executables
#-------------------------------------------------------------------------------
set_target_properties (
  lsqMain
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_PATH}
	)