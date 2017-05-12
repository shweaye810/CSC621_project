cmake_minimum_required(VERSION 2.6)
 
PROJECT(pick_region)

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
 
add_executable(pick_region pick_region.cxx)
target_link_libraries(pick_region
  ${Glue}  ${VTK_LIBRARIES} ${ITK_LIBRARIES})