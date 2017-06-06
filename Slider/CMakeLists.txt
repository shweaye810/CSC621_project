
cmake_minimum_required(VERSION 2.8)

PROJECT(Slider)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(Slider MACOSX_BUNDLE Slider.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(Slider ${VTK_LIBRARIES})
else()
  target_link_libraries(Slider vtkHybrid vtkWidgets)
endif()
