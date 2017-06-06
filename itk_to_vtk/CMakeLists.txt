cmake_minimum_required(VERSION 2.8)
 
project(itk_to_vtk)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

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
 
add_executable(itk_to_vtk itk_to_vtk.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(itk_to_vtk
    ${VTK_LIBRARIES} ${ITK_LIBRARIES})
else()
  target_link_libraries(PickPixel ${ITK_LIBRARIES} vtkHybrid vtkWidgets)
endif()
