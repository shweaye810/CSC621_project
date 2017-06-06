cmake_minimum_required(VERSION 2.8)
 
project(read_dcm_itk)
set(CMAKE_CXX_STANDARD 14)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
else()
  find_package(ItkVtkGlue REQUIRED)
  include(${ItkVtkGlue_USE_FILE})
  set(Glue ItkVtkGlue)
endif()
 
add_executable(read_dcm_itk read_dcm_itk.cxx)
target_link_libraries(read_dcm_itk
  ${Glue}  ${VTK_LIBRARIES} ${ITK_LIBRARIES})