#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkPNGReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
int main( int argc, char* argv[] )
{
  if( argc != 2 )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName>";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputFileName = argv[1];
  const unsigned int Dimension = 2;
  typedef unsigned char                      PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );
  
  typedef itk::ImageToVTKImageFilter< ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
   try
     {
     filter->Update();
     }
   catch( itk::ExceptionObject & error )
     {
     std::cerr << "Error: " << error << std::endl;
     return EXIT_FAILURE;
     }
  vtkImageData * myvtkImageData = filter->GetOutput();

  vtkSmartPointer<vtkImageViewer2> imageViewer =
    vtkSmartPointer<vtkImageViewer2>::New();
  imageViewer->SetInputData(myvtkImageData);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  imageViewer->SetupInteractor(renderWindowInteractor);
  imageViewer->Render();
  imageViewer->GetRenderer()->ResetCamera();
  imageViewer->Render();
 
  renderWindowInteractor->Start();
  return EXIT_SUCCESS;
}