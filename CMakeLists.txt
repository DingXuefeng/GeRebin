cmake_minimum_required(VERSION 3.4)
project(GeRebin)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
message(STATUS ${LOC_PATH})
if(EXISTS ${LOC_PATH})
    message(FATAL_ERROR "You cannot build in the source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory.")
endif()

add_executable(GeRebin GeRebin.cpp)
target_link_libraries(GeRebin Rebin)
target_compile_features(GeRebin PRIVATE cxx_range_for)
add_library(Rebin SHARED Spectrum.cpp 
  FileLoader.cpp 
  GeMPILoader.cpp
  GeSpcAnalysisManager.cpp
  StdAfx.cpp
  TRandom.cpp)
target_compile_features(Rebin PRIVATE cxx_range_for)

set(SCRIPTS
  Process.sh
  v_pur_final_160201_gempi4.spe
  bgfs_nov_2012_gempi4.spe
  joblists.txt
  )

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
