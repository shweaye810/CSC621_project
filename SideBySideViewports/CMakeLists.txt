
cmake_minimum_required(VERSION 2.8)

PROJECT(SideBySideViewports)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(SideBySideViewports SideBySideViewports.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(SideBySideViewports ${VTK_LIBRARIES})
else()
  target_link_libraries(SideBySideViewports vtkHybrid vtkWidgets)
endif()
