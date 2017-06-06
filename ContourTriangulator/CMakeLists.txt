
cmake_minimum_required(VERSION 2.8)

PROJECT(ContourTriangulator)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(ContourTriangulator ContourTriangulator.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(ContourTriangulator ${VTK_LIBRARIES})
else()
  target_link_libraries(ContourTriangulator vtkHybrid vtkWidgets)
endif()
