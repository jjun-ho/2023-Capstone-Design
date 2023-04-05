########################
# Look for the SDK stuff.  As of CUDA 3.0 NVSDKCUDA_ROOT has been replaced with
# NVSDKCOMPUTE_ROOT with the old CUDA C contents moved into the C subdirectory
if (NOT CUDA_SDK_ROOT_DIR)
  set (CUDA_SDK_ROOT_DIR "$ENV{HOME}/NVIDIA_GPU_Computing_SDK")
endif (NOT CUDA_SDK_ROOT_DIR)

if (NOT NVSDKCOMPUTE_ROOT)
  set (NVSDKCOMPUTE_ROOT ${CUDA_SDK_ROOT_DIR})
else (NOT NVSDKCOMPUTE_ROOT)
  set (CUDA_SDK_ROOT_DIR ${NVSDKCOMPUTE_ROOT})
endif (NOT NVSDKCOMPUTE_ROOT)

find_path(CUDA_SDK_ROOT_DIR common/inc/cutil.h
  "$ENV{NVSDKCOMPUTE_ROOT}/C"
  "$ENV{NVSDKCUDA_ROOT}"
  "{NVSDKCOMPUTE_ROOT}/C"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\NVIDIA Corporation\\Installed Products\\NVIDIA SDK 10\\Compute;InstallDir]"
  "/Developer/GPU\ Computing/C"
  )

# Keep the CUDA_SDK_ROOT_DIR first in order to be able to override the
# environment variables.
set(CUDA_SDK_SEARCH_PATH
  "${CUDA_SDK_ROOT_DIR}"
  "${CUDA_SDK_ROOT_DIR}/C"
  "${CUDA_TOOLKIT_ROOT_DIR}/local/NVSDK0.2"
  "${CUDA_TOOLKIT_ROOT_DIR}/NVSDK0.2"
  "${CUDA_TOOLKIT_ROOT_DIR}/NV_CUDA_SDK"
  "$ENV{HOME}/NVIDIA_CUDA_SDK"
  "$ENV{HOME}/NVIDIA_CUDA_SDK_MACOSX"
  "/Developer/CUDA"
  )

find_path(CUDA_CUT_INCLUDE_DIR
  cutil.h
  PATHS ${CUDA_SDK_SEARCH_PATH}
  PATH_SUFFIXES "common/inc"
  DOC "Location of cutil.h"
  NO_DEFAULT_PATH
  )
# Now search system paths
find_path(CUDA_CUT_INCLUDE_DIR cutil.h DOC "Location of cutil.h")

mark_as_advanced(CUDA_CUT_INCLUDE_DIR)

if (CUDA_CUT_INCLUDE_DIR)
  message(STATUS "Found CUDA SDK include path: ${CUDA_CUT_INCLUDE_DIR}")
endif (CUDA_CUT_INCLUDE_DIR)

# cutil library is called cutil64 for 64 bit builds on windows.  We don't want
# to get these confused, so we are setting the name based on the word size of
# the build.

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(LIBSUFFIXES "D")
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(win_dir "Win64")
    set(cuda_cutil_name "cutil64${LIBSUFFIXES}")
  else (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(cuda_cutil_name "cutil_x86_64${LIBSUFFIXES}")
  endif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
  if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(win_dir "Win32")
    set(cuda_cutil_name "cutil32${LIBSUFFIXES}")
  else (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(cuda_cutil_name "cutil_i386${LIBSUFFIXES}")
  endif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

find_library(CUDA_CUT_LIBRARY
  NAMES ${cuda_cutil_name}
  PATHS ${CUDA_SDK_SEARCH_PATH}
  # The new version of the sdk shows up in common/lib, but the old one is in lib
  PATH_SUFFIXES "common/lib" "lib" "common/lib/${win_dir}" "lib/${win_dir}"
  DOC "Location of cutil library"
  NO_DEFAULT_PATH
  )
# Now search system paths
find_library(CUDA_CUT_LIBRARY NAMES ${cuda_cutil_name} DOC
  "Location of cutil library")
mark_as_advanced(CUDA_CUT_LIBRARY)
set(CUDA_CUT_LIBRARIES ${CUDA_CUT_LIBRARY})

if (CUDA_CUT_LIBRARIES)
  message(STATUS "Found CUDA SDK libraries: ${CUDA_CUT_LIBRARIES}")
endif (CUDA_CUT_LIBRARIES)

IF (CUDA_CUT_INCLUDE_DIR AND CUDA_CUT_LIBRARIES)
   SET(CUDA_CUT_FOUND TRUE)
ENDIF (CUDA_CUT_INCLUDE_DIR AND CUDA_CUT_LIBRARIES)