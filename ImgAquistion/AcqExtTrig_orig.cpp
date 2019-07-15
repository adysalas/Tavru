/************************************************************
 * Software for Image Acquisition with External Trigger     *
 *                                                          *
 * Author Tiago Santos tsantos@lsa.isep.ipp.pt              *
 * Date 2011/02/28                                          *
 ************************************************************/

// include files to use the PYLON API
#include <pylon/PylonIncludes.h>
#include <stdlib.h>

// Use PYLON related namespaces
using namespace GenICam;
using namespace Pylon;
 
#if defined( USE_1394 )
// settings to use  Basler 1394 cameras
#include <pylon/1394/Basler1394Camera.h>
typedef Pylon::CBasler1394Camera Camera_t;
using namespace Basler_IIDC1394CameraParams;
using namespace Basler_IIDC1394StreamParams;
#elif defined ( USE_GIGE )
// settings to use Basler GigE cameras
#include <pylon/gige/BaslerGigECamera.h>
typedef Pylon::CBaslerGigECamera Camera_t;
using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;
#else	/* 
 */
#error Camera type is not specified. For example, define USE_GIGE for using GigE cameras
#endif	/* 
 */

// namespace for using cout
using namespace std;

// Include System Files
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h> 

// Include OpenCV files
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class CGrabBuffer 
{

public: CGrabBuffer (const size_t ImageSize);

~CGrabBuffer ();

uint8_t * GetBufferPointer (void)
  {
    return m_pBuffer;
  }

StreamBufferHandle GetBufferHandle (void)
  {
    return m_hBuffer;
  }
  
void SetBufferHandle (StreamBufferHandle hBuffer)
  {
    m_hBuffer = hBuffer;
  };

 
protected: uint8_t * m_pBuffer;
  
StreamBufferHandle m_hBuffer;

};
 
// Constructor allocates the image buffer
CGrabBuffer::CGrabBuffer (const size_t ImageSize):
m_pBuffer (NULL) 
{
  
m_pBuffer = new uint8_t[ImageSize];
  
if (NULL == m_pBuffer)
    
    {
      
GenICam::GenericException e ("Not enough memory to allocate image buffer", __FILE__, __LINE__);
throw e;
    }
}
 
// Freeing the memory
  CGrabBuffer::~CGrabBuffer () 
{ 
if (NULL != m_pBuffer)   
delete[]m_pBuffer;
}
 
// Buffers for grabbing
static const uint32_t c_nBuffers =100; // 15 10

int main (int argc, char *argv[]) 
{
/* always check */ 
int NumberImg = argc - 1;
if (argc != 2)
  cerr << "ERROR - Usage: " << argv[0] << " CamId" << endl;
  
int CamId = atoi (argv[1]), i = 0, n = 0;
char LogFileName[50], ImgFileName[50];
struct timespec ImgTime;
fstream fImgTime;

// Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
// is initialized during lifetime of this object.
Pylon::PylonAutoInitTerm autoInitTerm;

try 
  { 
      // Get the transport layer factory
      CTlFactory & TlFactory = CTlFactory::GetInstance ();
    
      // Create the transport layer object needed to enumerate or
      // create a camera object of type Camera_t::DeviceClass()
      ITransportLayer * pTl = TlFactory.CreateTl (Camera_t::DeviceClass ());
    
      // Exit application if the specific transport layer is not available
      if (!pTl)  
      {
	cerr << "Failed to create transport layer!" << endl;
	return 1;
      }
 
      // Get all attached cameras and exit application if no camera is found
      DeviceInfoList_t devices;
    
      if (0 == pTl->EnumerateDevices (devices))
      {
	cerr << "No camera present!" << endl;
	return 1;
      }
      //cout << "Number of cameras found: " << devices.size() << endl;
      
      for (i = 0; i < pTl->EnumerateDevices (devices); i++)
      {
	  //if (CamId == atoi (Camera.DeviceUserID.GetValue ()))
	  if (i == CamId)
	  {    
	      // Create the camera object of the first available camera
	      // The camera object is used to set and get all available
	      // camera features.
	      Camera_t Camera (pTl->CreateDevice (devices[CamId]));
	
	      // Open the camera
	      Camera.Open();
	      
	      // Get the first stream grabber object of the selected camera
	      Camera_t::StreamGrabber_t StreamGrabber (Camera.GetStreamGrabber (0));
 
	      // Open the stream grabber
	      StreamGrabber.Open ();
 
	      // Get camera device information
	      cout << "Camera Device Information" << endl; 
	      cout << "=========================" << endl;
	      cout << "Vendor                     : " << Camera.DeviceVendorName.GetValue () << endl;
	      cout << "Model                      : " << Camera.DeviceModelName.GetValue () << endl;
	      cout << "Firmware version           : " << Camera.DeviceFirmwareVersion.GetValue () << endl;	    
	      cout << "DeviceID                   : " << Camera.DeviceID.GetValue () << endl;
	      cout << "DeviceUserID               : " << Camera.DeviceUserID.GetValue () << endl;
	      cout << "Image Size ( W x H )       : " << Camera.WidthMax.GetValue() << "x" <<  Camera.HeightMax.GetValue() << endl;
	      cout << "Camera Ethernet Speed      : " << Camera.GevLinkSpeed() << " Mbps" << endl;
// 	      cout << "Number of bits per pixel   : " << Camera.PixelSize() << " bits" << endl;
// 	      cout << "Pixel Color Filter         : " << Camera.PixelColorFilter() << endl;
// 	      cout << "Pixel Dynamic Range Min    : " << Camera.PixelDynamicRangeMin() << endl;
// 	      cout << "Pixel Dynamic Range Max    : " <<  Camera.PixelDynamicRangeMax() << endl;
//            double ResIPaddress = Camera.GevCurrentIPAddress();
// 	      cout << "IP Address           : " << ResIPaddress << endl;
//            double ResNetConfig =  Camera.GevCurrentIPConfiguration();
//            cout << "Network Configuration: " << ResNetConfig << endl;
              double ResFPS = Camera.ResultingFrameRateAbs.GetValue ();
	      cout << "Maximum Frame Rate   : " << ResFPS << "fps" << endl;
	      cout << "=========================" << endl << endl;
	      
	      // Open file to Log Img Time's
	      // Use QNAP
	      // sprintf (LogFileName, "/mnt/ground_truth_qnap/ImgCam%d/LogTimeCam%d.txt", CamId, CamId);
	      // Use External Disk
	      //sprintf(LogFileName, "/media/BaslerCam/ImgCam%d/LogTimeCam%d.txt",CamId,CamId);
	      // Use execution folder
	      sprintf(LogFileName, "ImgCam%d/LogTimeCam%d.txt",CamId,CamId);
	      
	      fImgTime.open(LogFileName, ios::out );
	      fImgTime << "% CamId ImageNumber UnixSec" << endl;
	      
	      // Get the image buffer size
	      const size_t ImageSize = (size_t) (Camera.PayloadSize.GetValue ());
 
	      // We won't use image buffers greater than ImageSize
	      StreamGrabber.MaxBufferSize.SetValue (ImageSize);
 
	      // We won't queue more than c_nBuffers image buffer at a time
	      StreamGrabber.MaxNumBuffer.SetValue (c_nBuffers);
 
	      // Allocate all resources for grabbing. Critical parameters like image
	      // size now must not be changed until FinishGrab() is called.
	      StreamGrabber.PrepareGrab ();
 
	      // Buffers used for grabbing must be registered at the stream grabber.
	      // The registration returns a handle to be used for queuing the buffer
	      std::vector < CGrabBuffer * >BufferList;
	    
	      for (uint32_t i = 0; i < c_nBuffers; ++i)
	      {
		CGrabBuffer * pGrabBuffer = new CGrabBuffer (ImageSize);
		pGrabBuffer->SetBufferHandle (StreamGrabber.RegisterBuffer(pGrabBuffer->GetBufferPointer(), ImageSize));
		// Put the grab buffer object into the buffer list
		BufferList.push_back (pGrabBuffer);	      
	      }
 
	      for (std::vector < CGrabBuffer * >::const_iterator x = BufferList.begin (); x != BufferList.end (); ++x)
	      {
		  // Put buffer into the grab queue for grabbing
		  StreamGrabber.QueueBuffer ((*x)->GetBufferHandle (), NULL);
	      }
 
	      Camera.AcquisitionStart.Execute ();
	    
	      cout << "Waiting for TRIGGER" << endl << endl;

// PixelConvertion
	      CPixelFormatConverterYUV422 MyColorConverterYUV422;
	      SImageFormat MyInputformat;
	      // Image width in pixels
	      MyInputformat.Width = 1278;
	      // Image height in pixels
	      MyInputformat.Height = 958;
	      // Size of one image line in bytes (= Width * bytes per pixel)
	      MyInputformat.LinePitch = 1278*2;
	      // enum of type "PixelType" declared in PixelType.h
	      MyInputformat.PixelFormat = PixelType_YUV422packed ;
	      MyColorConverterYUV422.Init(MyInputformat);
	      
	      // An RGBA image needs 4 bytes per pixel
	      int BytePerPixel = 4;
	      unsigned char *MyOutputBuffer = new unsigned char [1278*958*BytePerPixel];
	      
	      SOutputImageFormat MyOutputFormat;
	      // Size of one image line in bytes (= width * bytes per pixel)
	      MyOutputFormat.LinePitch = 1278 * BytePerPixel;
	      // enum of type "PixelType" declared in pixeltype.h
	      MyOutputFormat.PixelFormat = PixelType_RGB12packed; //PixelType_RGB16planar ;
	      
	      // USE OpenCV
	      IplImage *rawimg = cvCreateImageHeader(cvSize(1278,958), IPL_DEPTH_8U, 4);
	      //rawimg->nChannels = 3;
	      //rawimg->depth = IPL_DEPTH_8U;
	      rawimg->dataOrder = 0;
	      rawimg->origin = 0; 
	      rawimg->width = 1278;
	      rawimg->height = 958;
	      rawimg->imageSize = 1278*958*4; // 1278*958*2bytesperpixel
	      rawimg->widthStep = 1278*4; // 1278*2
	      
	      int p[3]; // JPEG Compression 
	      p[0] = CV_IMWRITE_JPEG_QUALITY;
	      p[1] = 85; // Compression Desired Quality Value
	      p[2] = 0;
	      
	      // Grab images
	      for (;;)  
	      {
		  // Wait for the grabbed image with timeout of 3 seconds
		  //if (StreamGrabber.GetWaitObject().Wait(3000))
		  //{
		  // Get the grab result from the grabber's result queue
		  GrabResult Result;
		  StreamGrabber.RetrieveResult (Result); 
		  if (Grabbed == Result.Status ())
		  {
		      clock_gettime (CLOCK_REALTIME, &ImgTime);
		      fImgTime << CamId << " " << n << " " << ImgTime.tv_sec << "." << ImgTime.tv_nsec << endl;
		      
		      // Grabbing was successful, process image
		      cout << "Cam" << CamId << "Img" << n << endl;
		      //cout << "Size: " << Result.GetSizeX() << " x "<< Result.GetSizeY() << endl;
		      //cout << Camera.AcquisitionFrameRateAbs.GetValue() << endl;
		      
                      const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer (); 
		      // SAVE YUV (YCbCr) RAW Data
	              // Use QNAP
	              //sprintf (ImgFileName, "/mnt/ground_truth_qnap/ImgCam%d/Cam%d_%06ld.jpg", CamId, CamId, n); n++;
	              // Use External Disk
	              //sprintf(ImgFileName, "/media/BaslerCam/ImgCam%d/Cam%d_%06ld.jpg",CamId,CamId,n); n++;
	              // Use execution folder
	              sprintf(ImgFileName, "ImgCam%d/Cam%d_%06ld.jpg",CamId,CamId,n); n++;
		      
// USE Color Conversion
		      MyColorConverterYUV422.Convert(
					      MyOutputBuffer, // Pointer to destination buffer
					      1278 * 958 * BytePerPixel, // Size of destination buffer in bytes
					      (void*) pImageBuffer, // void* Pointer pointing to INPUT buffer
					      1278 * 958 * 2, // Size of INPUT buffer
					      MyInputformat, // Image format of the input image
					      MyOutputFormat // Image format of the output image
					      );     
		      
// USE OpenCV
		      rawimg->imageData = (char *)MyOutputBuffer;
		      cvSaveImage (ImgFileName, rawimg, p);
		      
// 		      fstream fImgYUV;
// 		      fImgYUV.open (ImgFileName, ios::out);
// 		      fImgYUV << pImageBuffer;
// 		      fImgYUV.close ();
		      // END YUV RAW Data
		      
		      // Reuse the buffer for grabbing the next image
		      StreamGrabber.QueueBuffer (Result.Handle (), NULL);
		  }
		  else if (Failed == Result.Status ())
		  {
		      // Error Handling
		      cerr << "No image acquired!" << endl;
		      cerr << "Error code : 0x" << hex << Result.GetErrorCode () << endl;
		      cerr << "Error description : " << Result.GetErrorDescription () << endl;
		      // Reuse the buffer for grabbing the next image
		      StreamGrabber.QueueBuffer (Result.Handle (), NULL);
		  }
		
//                } // End If 3seconds timeout
//             else
//             {
//                 // Timeout
//                 cerr << "Timeout occurred!" << endl;
// 
//                 // Get the pending buffer back (It is not allowed to deregister
//                 // buffers when they are still queued)
//                 StreamGrabber.CancelGrab();
// 
//                 // Get all buffers back
//                 for (GrabResult r; StreamGrabber.RetrieveResult(r););
// 
//                 // Cancel loop
//                 break;
//             }
	      } // End for(;;)
	    
	      // Stop acquisition
	      Camera.AcquisitionStop.Execute ();

	      // clean up
	      // It is necessary to deregister buffer before freeing the memory
	      for (std::vector < CGrabBuffer * >::iterator it =  BufferList.begin (); it != BufferList.end (); it++)
	      {
		StreamGrabber.DeregisterBuffer ((*it)->GetBufferHandle ());
		delete * it;
	      }
	    
	      // Free all resources used for grabbing
	      StreamGrabber.FinishGrab ();
 
	      // Close stream grabber
	      StreamGrabber.Close ();
 
	      // Close camera
	      Camera.Close ();
	      
	      // Close file to Log Img Time's
	      fImgTime.close();
} // End if Devices
 
} // End for Devices
 
} // End Try
  
catch (GenICam::GenericException & e) 
  {
      // Error handling
      cerr << "An exception occurred!" << endl <<e.GetDescription () << endl;
      return 1;
  }
 
    // Quit application
    return 0;

}
