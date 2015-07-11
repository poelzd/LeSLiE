# - Find Eigen
# Find the native Eigen includes and library
#
# EIGEN_INCLUDES - where to find signature_of_eigen3_matrix_library
# EIGEN_LIBRARIES - List of libraries when using EIGEN.
# EIGEN_FOUND - True if EIGEN found.
 
if (EIGEN_INCLUDES)
  # Already in cache, be silent
  set (EIGEN_FIND_QUIETLY TRUE)
endif (EIGEN_INCLUDES)
 
find_path(EIGEN_INCLUDES NAMES signature_of_eigen3_matrix_library
  PATHS
  /usr/include
  /usr/local/include
  ${LESLIE_BASE_PATH}/contrib/eigen
  PATH_SUFFIXES eigen3 eigen
  )
 
# handle the QUIETLY and REQUIRED arguments and set EIGEN_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (EIGEN DEFAULT_MSG EIGEN_INCLUDES)
mark_as_advanced (EIGEN_INCLUDES)