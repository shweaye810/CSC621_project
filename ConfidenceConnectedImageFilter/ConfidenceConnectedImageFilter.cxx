#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkFlipImageFilter.h"

#include "itkScalarToRGBColormapImageFilter.h"
#include "itkRGBPixel.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image< double, 2 >  ImageType;
typedef itk::RGBPixel<unsigned char>    RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  RGBImageType;

int main( int argc, char *argv[])
{
  if(argc < 4)
    {
    std::cerr << "Required: filename.png seedX seedY" << std::endl;

    return EXIT_FAILURE;
    }
  std::string inputFileName = argv[1];
  
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();

  bool flip_axes[2] {0, 1};
  /* get orginal */
  typedef itk::FlipImageFilter<ImageType> flip_img_t;
  flip_img_t::Pointer flip_filter = flip_img_t::New();
  flip_filter->SetInput(reader->GetOutput());
  flip_filter->SetFlipAxes(flip_axes);



  typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
  ConfidenceConnectedFilterType::Pointer confidenceConnectedFilter = ConfidenceConnectedFilterType::New();
  confidenceConnectedFilter->SetInitialNeighborhoodRadius(3);
  confidenceConnectedFilter->SetMultiplier(3);
  confidenceConnectedFilter->SetNumberOfIterations(25);
  confidenceConnectedFilter->SetReplaceValue(50);

  // Set seed
  ImageType::IndexType seed;
  seed[0] = atoi(argv[2]);
  seed[1] = atoi(argv[3]);
  confidenceConnectedFilter->SetSeed(seed);
  confidenceConnectedFilter->SetInput(flip_filter->GetOutput());

  typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(confidenceConnectedFilter->GetOutput());
  rgbfilter->SetColormap( RGBFilterType::Hot );
  QuickView viewer;

  viewer.AddImage<ImageType>(
    flip_filter->GetOutput(),false,
    itksys::SystemTools::GetFilenameName(inputFileName));  

  std::stringstream desc;
  desc << "ConfidenceConnected Seed: " << seed[0] << ", " << seed[1];
  viewer.AddImage(
    confidenceConnectedFilter->GetOutput(),
    false,
    desc.str());  
  
  viewer.Visualize();

  return EXIT_SUCCESS;
}
