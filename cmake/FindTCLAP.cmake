# search for tclap Templatized C++ Command Line Parser
#
# TCLAP_INCLUDES - where to find CmdLine.h
# TCLAP_LIBRARIES - List of libraries when using TCLAP.
# TCLAP_FOUND - True if TCLAP found.
 
if (TCLAP_INCLUDES)
  # Already in cache, be silent
  set (TCLAP_FIND_QUIETLY TRUE)
endif (TCLAP_INCLUDES)
 
find_path(TCLAP_INCLUDES NAMES tclap/CmdLine.h
  PATHS
  /usr/include
  /usr/local/include
  ${LESLIE_BASE_PATH}/contrib/tclap/tclap/include
  )

# handle the QUIETLY and REQUIRED arguments and set TCLAP_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (TCLAP DEFAULT_MSG TCLAP_INCLUDES)
mark_as_advanced (TCLAP_INCLUDES)