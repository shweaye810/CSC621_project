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
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation3D.h>
#include <vtkSliderWidget.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkBilateralImageFilter.h"
#include "itkImageDuplicator.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef itk::Image<RGBPixelType, 2> RGBImageType;
typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;

namespace team_wind
{
double picked[3] = {0, 0, 0};
ConfidenceConnectedFilterType::Pointer confidenceConnectedFilter = ConfidenceConnectedFilterType::New();
RGBFilterType::Pointer rgb_filter = RGBFilterType::New();
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

        // Set seed
        ImageType::IndexType seed;
        seed[0] = team_wind::picked[0];
        seed[1] = team_wind::picked[1];
        team_wind::confidenceConnectedFilter->SetSeed(seed);
        team_wind::confidenceConnectedFilter->Update();

        QuickView viewer;
        team_wind::rgb_filter->Update();
        viewer.AddImage(team_wind::rgb_filter->GetOutput(), false);

        viewer.Visualize();
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

    double rangeSigma = 2.0;
    double domainSigma = 2.0;
    typedef itk::BilateralImageFilter<
        ImageType, ImageType>
        Bilateral_filter_t;
    Bilateral_filter_t::Pointer bilateral_filter = Bilateral_filter_t::New();
    bilateral_filter->SetInput(reader->GetOutput());
    bilateral_filter->SetDomainSigma(domainSigma);
    bilateral_filter->SetRangeSigma(rangeSigma);

    /* get orginal */
    bool flip_axes[2] = {0, 1};
    typedef itk::FlipImageFilter<ImageType> flip_img_t;
    flip_img_t::Pointer flip_filter = flip_img_t::New();
    flip_filter->SetInput(bilateral_filter->GetOutput());
    flip_filter->SetFlipAxes(flip_axes);

    itk::SmartPointer<ImageType> flip_img = flip_filter->GetOutput();
    team_wind::confidenceConnectedFilter->SetInitialNeighborhoodRadius(2);
    team_wind::confidenceConnectedFilter->SetMultiplier(2.5);
    team_wind::confidenceConnectedFilter->SetNumberOfIterations(5);
    team_wind::confidenceConnectedFilter->SetInput(flip_img);

    team_wind::rgb_filter->SetInput(team_wind::confidenceConnectedFilter->GetOutput());
    team_wind::rgb_filter->SetColormap(RGBFilterType::Red);
    /**********/
    typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(flip_img);
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
    filter->SetInput(reader->GetOutput());

    vtk_img->SetOrigin(0, 0, 0);
    vtkSmartPointer<vtkPointPicker> pointPicker =
        vtkSmartPointer<vtkPointPicker>::New();

    vtkSmartPointer<vtkImageViewer2> imageViewer =
        vtkSmartPointer<vtkImageViewer2>::New();
    imageViewer->SetInputData(vtk_img);
    imageViewer->SetSize(600, 600);

    // Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderWindow> renderWindow = imageViewer->GetRenderWindow();

    vtkSmartPointer<vtkRenderer> left_renderer = imageViewer->GetRenderer();
    left_renderer->GradientBackgroundOn();
    left_renderer->SetBackground(0, 0, 0);
    left_renderer->SetBackground2(1, 1, 1);
    left_renderer->ResetCamera();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetPicker(pointPicker);

    imageViewer->SetupInteractor(renderWindowInteractor);
    //imageViewer->Render();

    vtkSmartPointer<MouseInteractorStylePP> style =
        vtkSmartPointer<MouseInteractorStylePP>::New();
    renderWindowInteractor->SetInteractorStyle(style);

    // Render and interact
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
