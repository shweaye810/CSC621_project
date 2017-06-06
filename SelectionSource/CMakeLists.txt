
cmake_minimum_required(VERSION 2.8)

PROJECT(SelectionSource)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(SelectionSource SelectionSource.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(SelectionSource ${VTK_LIBRARIES})
else()
  target_link_libraries(SelectionSource vtkHybrid vtkWidgets)
endif()
