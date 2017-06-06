#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkImageViewer2.h>
#include <vtkPNGReader.h>
#include <vtkPNGReader.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::RGBPixel<unsigned char>    RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  RGBImageType;


namespace team_wind
{
double picked[3] = {0, 0, 0};
}
// Define interaction style
class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
{
  public:
    static MouseInteractorStylePP *New();
    vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
        //std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
        this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                                            this->Interactor->GetEventPosition()[1],
                                            0, // always zero.
                                            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        this->Interactor->GetPicker()->GetPickPosition(team_wind::picked);
        std::cout << "Picked value: " << team_wind::picked[0] << " " << team_wind::picked[1] << " " << team_wind::picked[2] << std::endl;

        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
};

vtkStandardNewMacro(MouseInteractorStylePP);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " InputImageFile [radius]" << std::endl;
        return EXIT_FAILURE;
    }
    std::string inputFileName = argv[1];

    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFileName.c_str());
    reader->Update();

    /* get orginal */
    bool flip_axes[2] = {0, 1};
    typedef itk::FlipImageFilter<ImageType> flip_img_t;
    flip_img_t::Pointer flip_filter = flip_img_t::New();
    flip_filter->SetInput(reader->GetOutput());
    flip_filter->SetFlipAxes(flip_axes);

    /*
    typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
    ConfidenceConnectedFilterType::Pointer confidenceConnectedFilter = ConfidenceConnectedFilterType::New();
    confidenceConnectedFilter->SetInitialNeighborhoodRadius(3);
    confidenceConnectedFilter->SetMultiplier(3);
    confidenceConnectedFilter->SetNumberOfIterations(25);
    confidenceConnectedFilter->SetReplaceValue(255);

    // Set seed
    ImageType::IndexType seed;
    seed[0] = atoi(argv[2]);
    seed[1] = atoi(argv[3]);
    confidenceConnectedFilter->SetSeed(seed);
    confidenceConnectedFilter->SetInput(flip_filter->GetOutput());
*/
    /*
    QuickView viewer;

    viewer.AddImage<ImageType>(
        flip_filter->GetOutput(), false,
        itksys::SystemTools::GetFilenameName(inputFileName));

    std::stringstream desc;
    desc << "ConfidenceConnected Seed: " << seed[0] << ", " << seed[1];
    viewer.AddImage(
        confidenceConnectedFilter->GetOutput(),
        false,
        desc.str());

    viewer.Visualize();

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0]
                  << " Filename(png)" << std::endl;
        return EXIT_FAILURE;
    }

    //Parse input argument
    std::string inputFilename = argv[1];

    //Read the image
    vtkSmartPointer<vtkPNGReader> img =
        vtkSmartPointer<vtkPNGReader>::New();
    if (!img->CanReadFile(inputFilename.c_str()))
    {
        std::cout << argv[0] << ": Error reading file "
                  << inputFilename << endl
                  << "Exiting..." << endl;
        return EXIT_FAILURE;
    }
    img->SetFileName(inputFilename.c_str());
*/
    typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(flip_filter->GetOutput());
    try
    {
        filter->Update();
    }
    catch (itk::ExceptionObject &error)
    {
        std::cerr << "Error: " << error << std::endl;
        return EXIT_FAILURE;
    }
    vtkImageData *vtk_img = filter->GetOutput();
    vtk_img->SetOrigin(0, 0, 0);

    vtkSmartPointer<vtkPointPicker> pointPicker =
        vtkSmartPointer<vtkPointPicker>::New();

    vtkSmartPointer<vtkImageViewer2> imageViewer =
        vtkSmartPointer<vtkImageViewer2>::New();
    imageViewer->SetInputData(vtk_img);
    imageViewer->SetSize(600, 600);

    // Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer = imageViewer->GetRenderer();
    renderer->GradientBackgroundOn();
    renderer->SetBackground(0, 0, 0);
    renderer->SetBackground2(1, 1, 1);
    renderer->ResetCamera();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetPicker(pointPicker);

    imageViewer->SetupInteractor(renderWindowInteractor);
    //imageViewer->Render();

    vtkSmartPointer<MouseInteractorStylePP> style =
        vtkSmartPointer<MouseInteractorStylePP>::New();
    renderWindowInteractor->SetInteractorStyle(style);

    // Add the actor to the scene
    renderer->SetBackground(1, 1, 1); // Background color white
    renderer->ResetCamera();
    // Render and interact
    renderWindowInteractor->Start();

    typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
    ConfidenceConnectedFilterType::Pointer confidenceConnectedFilter = ConfidenceConnectedFilterType::New();
    confidenceConnectedFilter->SetInitialNeighborhoodRadius(3);
    confidenceConnectedFilter->SetMultiplier(3);
    confidenceConnectedFilter->SetNumberOfIterations(25);
    //confidenceConnectedFilter->SetReplaceValue(50);

    // Set seed
    ImageType::IndexType seed;
    seed[0] = team_wind::picked[0];
    seed[1] = team_wind::picked[1];
    confidenceConnectedFilter->SetSeed(seed);
    confidenceConnectedFilter->SetInput(flip_filter->GetOutput());

    typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;
    RGBFilterType::Pointer rgb_filter = RGBFilterType::New();
    rgb_filter->SetInput(confidenceConnectedFilter->GetOutput());
    rgb_filter->SetColormap(RGBFilterType::Red);
    QuickView viewer;
    viewer.AddImage(flip_filter->GetOutput(), false, "Original Image");
    viewer.AddImage(rgb_filter->GetOutput(), false, "Extracted region");
    viewer.Visualize();

    return EXIT_SUCCESS;
}
