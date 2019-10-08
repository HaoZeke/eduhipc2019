
# CMake version
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For rtags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name
project(yodaChill)

FIND_PACKAGE( Boost 1.55.0 REQUIRED COMPONENTS system filesystem )
  # find_package (Eigen3 3.3 REQUIRED NO_MODULE)

#Bring the headers, such as Student.h into the project
include_directories(include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR}  )
include_directories("/opt/pgi/linux86-64/2019/include/")
#Can manually add the sources using the set command as follows:
set(SOURCES 
	src/main.cpp
  src/inputOutput.cpp
  src/rdf.cpp 
	)
option(use_PGI "use PGI" OFF)
option(use_OpenACC "use OpenACC" OFF)
option(use_OpenMP "use OpenMP" OFF)
option(use_OpenMPI "use OpenMPI" OFF)
option(use_GPU "use GPU" OFF)

if (${use_PGI})
  set(CMAKE_COMPILER_VENDOR "PGI")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse -Mipa=fast,inline -O4 -Minfo=all")
  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fastsse -Mipa=fast,inline -O4 -Minfo=all")
  # set(CMAKE_Fortran_FLAGS "-fastsse -Mipa=fast,inline -O4 -Minfo=all")
  set(CMAKE_C_COMPILER /opt/pgi/linux86-64/2019/bin/pgcc)
  set(CMAKE_CXX_COMPILER /opt/pgi/linux86-64/2019/bin/pgc++)
  set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "/opt/pgi/linux86-64/2019/include/")
  set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE)
endif (${use_PGI})

if (${use_GPU})
    add_definitions(-DUSE_GPU)
endif(${use_GPU})

# OpenACC
if (${use_OpenACC})
    find_package(OpenACC REQUIRED)
    if (${OpenACC_CXX_FOUND})
        message(STATUS "OpenACC CXX compiler found")
    endif ()
    if (${OpenACC_C_FOUND})
      message(STATUS "OpenACC C Compiler found")
    endif ()
endif (${use_OpenACC})

# OpenMP
if (${use_OpenMP})
    add_definitions(-DUSE_OPENMP)
    find_package(OpenMP REQUIRED)
    if (${OpenMP_CXX_FOUND})
        message(STATUS "OpenMP CXX found")
    endif ()
    if (${OpenMP_C_FOUND})
        message(STATUS "OpenMP C found")
    endif ()
endif (${use_OpenMP})

# OpenMPI
if (${use_OpenMPI})
    find_package(MPI REQUIRED)
endif (${use_OpenMPI})

##
## Compile option selector
##

# Largely borrowed from https://github.com/ghb24/NECI_STABLE/tree/master/cmake
macro (AddOptimizeOption)
  if (TARGET_ARCH STREQUAL "INTEL_F_TCS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Kfast,parallel,optmsg=2 -V")

  elseif (USE_F_TCS STREQUAL "YES")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V")
    # -Xg   : gcc compatible flag
    # -fPIC : PIC flag
    # -Nfjcex : to link PMlib

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")
    set(CMAKE_Fortran_FLAGS "-O3 -Wall")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHOST -O3 -qopt-report=3 -DMPICH_IGNORE_CXX_SEEK")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -xHOST -O3 -qopt-report=3")
    set(CMAKE_Fortran_FLAGS "-xHOST -O3 -qopt-report=3")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse -Mipa=fast,inline -O4 -Minfo=all")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fastsse -Mipa=fast,inline -O4 -Minfo=all")
    set(CMAKE_Fortran_FLAGS "-fastsse -Mipa=fast,inline -O4 -Minfo=all")

  else()
    message("using default option")
  endif()
endmacro()
# However, the file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.cpp")
 
add_executable(runYoda ${SOURCES})


TARGET_LINK_LIBRARIES( runYoda LINK_PUBLIC ${Boost_LIBRARIES})
