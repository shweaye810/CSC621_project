
cmake_minimum_required(VERSION 2.8)

PROJECT(HighlightSelectedPoints)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(HighlightSelectedPoints HighlightSelectedPoints.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(HighlightSelectedPoints ${VTK_LIBRARIES})
else()
  target_link_libraries(HighlightSelectedPoints vtkHybrid vtkWidgets)
endif()
