cmake_minimum_required(VERSION 3.10)
#set(CMAKE_C_COMPILER "gcc")
#set(CMAKE_CXX_COMPILER "g++")

set(MFEM_SRC_DIR "mfem")
set(MFEM_BUILD_DIR "mfem-build")
#########################FROM MFEM SRC######################################
set(USER_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/../${MFEM_SRC_DIR}/config/user.cmake" CACHE PATH
  "Path to optional user configuration file.")

# Load user settings before the defaults - this way the defaults will not
# overwrite the user set options. If the user has not set all options, we still
# have the defaults.
message(STATUS "(optional) USER_CONFIG = ${USER_CONFIG}")
include("${USER_CONFIG}" OPTIONAL)
include("${CMAKE_CURRENT_SOURCE_DIR}/config/defaults.cmake")
############################################################################
project(mfem-apps)

#set(CMAKE_EXE_LINKER_FLAGS "-s") #Strip Binary
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE=Debug)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../mfem/config/cmake/modules")

#if(OPENMP_FOUND)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()




find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories("${MPI_INCLUDE_PATH}")
find_package(HYPRE REQUIRED)
find_package(METIS REQUIRED)
find_package(ParMETIS REQUIRED)
find_package(SuiteSparse REQUIRED)
find_package(SuperLUDist)
find_package(PETSc REQUIRED)

#######################Mfem-app required packages#############################
find_package(jsoncpp REQUIRED)

set(INSTALLED_DIRS "/usr/include")
set(PETSC_INCUDE_DIR "${INSTALLED_DIRS}/petsc/")
include_directories(${HYPRE_INCLUDE_DIR})
include_directories(${SuiteSparse_DIR})
include_directories(${SuperLUDist_DIR})
include_directories(${PETSC_INCUDE_DIR})
#set(HEADER_FILES "${CMAKE_SOURCE_DIR}/../mfem/include/*.hpp")
# "${CMAKE_SOURCE_DIR}/../hypre/include/*.hpp" "${CMAKE_SOURCE_DIR}/../metis/include/*.h")

#file(GLOB_RECURSE HEADERS ${HEADER_FILES})


set(DYNAMIC_LIBS_SERIAL HYPRE HYPRE_core HYPRE_FEI metis suitesparseconfig klu umfpack gomp jsoncpp)
set(DYNAMIC_LIBS_PARALLEL ${DYNAMIC_LIBS_SERIAL} parmetis petsc superlu_dist)

set(MFEM_USE_MPI ON)
set(MFEM_USE_HYPRE ON)
set(MFEM_USE_PETSC ON)
set(MFEM_USE_SUITESPARSE ON)
#include_directories("${CMAKE_SOURCE_DIR}/../${MFEM_SRC_DIR}/")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/mfem/config")
include_directories("mfem/")
add_subdirectory("mfem/")
#########Newly added libraries#############################
include_directories("general//")
add_subdirectory("general/")
include_directories("SolidMechanics/")
add_subdirectory("SolidMechanics/")
include_directories("mfemAddOn/")
add_subdirectory("mfemAddOn/")


add_library(${PROJECT_NAME} mfem-apps.hpp)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set (mfemApps ${PROJECT_NAME} SolidMechanics mfemAddOn general)

#set(DYNAMIC_WITHOUT_CMAKE HYPRE)
#set(STATIC_LIBS "${CMAKE_SOURCE_DIR}/../${MFEM_BUILD_DIR}/libmfem.a")
# "${CMAKE_SOURCE_DIR}/../hypre/lib/libHYPRE.a" "${CMAKE_SOURCE_DIR}/../metis/libmetis/libmetis.a")

#if(NOT ${MFEM_SRC_DIR} STREQUAL ${MFEM_BUILD_DIR})
#    include_directories("${CMAKE_SOURCE_DIR}/../${MFEM_BUILD_DIR}/config/")
#endif()



#set(Array src/array/*.c)
#file(GLOB_RECURSE SRC "src/*/*.c")
set(EXE1 exe1)
add_executable(${EXE1} ex1.cpp) #${HEADERS})
target_link_libraries(${EXE1} mfem ${MPI_LIBRARIES} ${DYNAMIC_LIBS_PARALLEL})

if(MPI_COMPILE_FLAGS)
  set_target_properties(${EXE1} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${EXE1} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()


set(EXE1p exe1p)
add_executable(${EXE1p} ex1p.cpp) #${HEADERS})
target_link_libraries(${EXE1p} ${MPI_LIBRARIES} mfem ${DYNAMIC_LIBS_PARALLEL})

if(MPI_COMPILE_FLAGS)
  set_target_properties(${EXE1p} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${EXE1p} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()



set(EXE2 exe2)
add_executable(${EXE2} ex2.cpp) #${HEADERS})
target_link_libraries(${EXE2} ${MPI_LIBRARIES} mfem ${DYNAMIC_LIBS_PARALLEL})

if(MPI_COMPILE_FLAGS)
  set_target_properties(${EXE2} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${EXE2} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

set(L_ELSTC LinearElastic)
add_executable(${L_ELSTC} LinearElastic.cpp) #${HEADERS})
target_link_libraries(${L_ELSTC} ${MPI_LIBRARIES} mfem ${mfemApps} ${DYNAMIC_LIBS_PARALLEL})

if(MPI_COMPILE_FLAGS)
  set_target_properties(${L_ELSTC} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${L_ELSTC} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()


#target_include_directories(${PROJECT_NAME} PUBLIC Array Esolver Matrix Matvec Precision Precon Solver System Vector)

#if(OPENMP_FOUND)
#    target_link_libraries(ex1 OpenMP::OpenMP_CXX)
#else()
#    target_link_libraries(ex1)
#endif()

#set_target_properties(ex1 PROPERTIES LINKER_LANGUAGE CXX)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/SampleGmsh DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ )
