cmake_minimum_required(VERSION 2.8)
 
project(sharpen)
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
 
add_executable(sharpen sharpen.cxx)
target_link_libraries(sharpen
  ${Glue}  ${VTK_LIBRARIES} ${ITK_LIBRARIES})