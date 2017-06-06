cmake_minimum_required(VERSION 2.8)
 
PROJECT(read_dcm)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(read_dcm read_dcm.cxx)
 
if(VTK_LIBRARIES)
  target_link_libraries(read_dcm ${VTK_LIBRARIES})
else()
  target_link_libraries(read_dcm vtkHybrid vtkWidgets)
endif()