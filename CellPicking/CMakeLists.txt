
cmake_minimum_required(VERSION 2.8)

PROJECT(CellPicking)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(CellPicking CellPicking.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(CellPicking ${VTK_LIBRARIES})
else()
  target_link_libraries(CellPicking vtkHybrid vtkWidgets)
endif()
