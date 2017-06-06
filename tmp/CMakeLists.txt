cmake_minimum_required(VERSION 2.6)
 
PROJECT(tmp)

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
 
add_executable(tmp tmp.cxx)
target_link_libraries(tmp
  ${Glue}  ${VTK_LIBRARIES} ${ITK_LIBRARIES})