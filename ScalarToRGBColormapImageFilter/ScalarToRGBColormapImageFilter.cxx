#include "itkImage.h"
#include "itkImageFileReader.h"

#include "itkImageRegionConstIterator.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"
#include "QuickView.h"


typedef itk::RGBPixel<unsigned char>    RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  RGBImageType;

typedef itk::Image<float, 2>  FloatImageType;
typedef itk::Image<unsigned char, 2>  ImageType;

int main( int argc, char *argv[])
{
  /*
  FloatImageType::Pointer image = FloatImageType::New();

  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(20);

  itk::ImageRegion<2> region(start, size);

  image->SetRegions(region);
  image->Allocate();

  for(unsigned int i = 0; i < 20; i++)
    {
    for(unsigned int j = 0; j < 20; j++)
      {
      itk::Index<2> pixel;
      pixel[0] = i;
      pixel[1] = j;
      image->SetPixel(pixel, j);
      }
    }

  typedef itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(image);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  {
  typedef  itk::ImageFileWriter< UnsignedCharImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("original.png");
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
  }
*/
  if(argc < 2)
    {
    std::cerr << "Required: filename.png" << std::endl;

    return EXIT_FAILURE;
    }
  std::string inputFileName = argv[1];
  
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();

  typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(reader->GetOutput());
  rgbfilter->SetColormap( RGBFilterType::Hot );

  QuickView viewer;
  viewer.AddImage(
    rgbfilter->GetOutput(),
    false,
    "rgb");
  
  viewer.Visualize();
  return EXIT_SUCCESS;
}
