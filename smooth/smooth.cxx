#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkDiscreteGaussianImageFilter.h"
 
#include "QuickView.h"
 
int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [variance]" << std::endl;
    return EXIT_FAILURE;
    }
 
  // Parse command line argumentsa
  std::string inputFilename = argv[1];
 
  double variance = 4.0;
  if (argc > 2)
    {
    variance = atof(argv[2]);
    }
 
  // Setup types
  typedef itk::Image< unsigned char, 2 > UnsignedCharImageType;
  typedef itk::Image< float, 2 >         FloatImageType;
 
  typedef itk::ImageFileReader< UnsignedCharImageType >  readerType;
 
  typedef itk::DiscreteGaussianImageFilter<
    UnsignedCharImageType, FloatImageType >  filterType;
 
  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );
 
  // Create and setup a Gaussian filter
  filterType::Pointer gaussianFilter = filterType::New();
  gaussianFilter->SetInput( reader->GetOutput() );
  gaussianFilter->SetVariance(variance);
 
  QuickView viewer;
  viewer.AddImage<UnsignedCharImageType>(reader->GetOutput());
  viewer.AddImage<FloatImageType>(gaussianFilter->GetOutput());
  viewer.Visualize();
 
  return EXIT_SUCCESS;
}