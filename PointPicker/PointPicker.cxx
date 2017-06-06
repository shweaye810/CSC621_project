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

// Define interaction style
class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
{
  public:
    static MouseInteractorStylePP* New();
    vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);
 
    virtual void OnLeftButtonDown() 
    {
      std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
      this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0], 
                         this->Interactor->GetEventPosition()[1], 
                         0,  // always zero.
                         this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
      double picked[3];
      this->Interactor->GetPicker()->GetPickPosition(picked);
      std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
 
};

vtkStandardNewMacro(MouseInteractorStylePP);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0]
              << " Filename(jpeg)" << std::endl;
    return EXIT_FAILURE;
  }

  //Parse input argument
  std::string inputFilename = argv[1];

  //Read the image
  vtkSmartPointer<vtkPNGReader> jpegReader =
      vtkSmartPointer<vtkPNGReader>::New();
  if (!jpegReader->CanReadFile(inputFilename.c_str()))
  {
    std::cout << argv[0] << ": Error reading file "
              << inputFilename << endl
              << "Exiting..." << endl;
    return EXIT_FAILURE;
  }
  jpegReader->SetFileName(inputFilename.c_str());
  
  vtkSmartPointer<vtkPointPicker> pointPicker = 
    vtkSmartPointer<vtkPointPicker>::New();
  
  vtkSmartPointer<vtkImageViewer2> imageViewer =
    vtkSmartPointer<vtkImageViewer2>::New();
  imageViewer->SetInputConnection( jpegReader->GetOutputPort() );
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
  renderWindowInteractor->SetInteractorStyle( style );
  
  // Add the actor to the scene
  renderer->SetBackground(1,1,1); // Background color white
  renderer->ResetCamera();
  // Render and interact
  renderWindowInteractor->Start();
  return EXIT_SUCCESS;
}
