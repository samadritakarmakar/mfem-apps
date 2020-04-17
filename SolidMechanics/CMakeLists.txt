project(SolidMechanics)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../mfem")

add_library(${PROJECT_NAME} LinearElasticIntegrator.hpp LinearElasticIntegrator.cpp GetElasticityTensor.hpp
    GetElasticityTensor.cpp CalcStressSolids.hpp CalcStressSolids.cpp SolidMechanics.hpp Tensor2Voigt.hpp Tensor2Voigt.cpp)
    
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
