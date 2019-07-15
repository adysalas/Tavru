/************************************************************
 * Software for Image Acquisition with External Trigger     *
 * Basler Cameras                                           *
 * Author Tiago Santos tsantos@lsa.isep.ipp.pt              *
 * Date 2012/02/04                                          *
 ************************************************************/  

// include files to use the PYLON API
#include <pylon/PylonIncludes.h>
#include <stdlib.h>

// Use PYLON related namespaces
using namespace GenICam;
using namespace Pylon;
 
// settings to use Basler GigE cameras
#include <pylon/gige/BaslerGigECamera.h>
typedef Pylon::CBaslerGigECamera Camera_t;
using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;

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
static const uint32_t c_nBuffers = 100; 

int main (int argc, char *argv[]) 
{
  
/* always check */ 
int NumberImg = argc - 1;
if (argc != 2)
  cerr << "ERROR - Usage: " << argv[0] << " CamId" << endl;
  
int CamId = atoi (argv[1]);
int i = 0, n = 0;
char LogFileName[1024], ImgFileName[1024];

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


      cout << "CamiD = "<< CamId << endl;
    
      // Exit application if the specific transport layer is not available
      if (!pTl)  
      {
	cerr << "Failed to create transport layer!" << endl;
	return 1;
      }
 
      // Create an empty GigE device info object.
      CBaslerGigEDeviceInfo bdi( pTl->CreateDeviceInfo() );
      if ( CamId == 0 )
	{
	  // Initialize the device info object with the known IP address.
	  //bdi.SetIpAddress("182.152.34.13");
	  bdi.SetIpAddress("172.45.100.0");
	}
      if ( CamId == 1 )
	{
	  // Initialize the device info object with the known IP address.
	  //bdi.SetIpAddress("182.152.34.13");
	  bdi.SetIpAddress("172.45.100.1");
	}
      if ( CamId == 2 )
	{
	  // Initialize the device info object with the known IP address.
	  //bdi.SetIpAddress("182.152.34.13");
	  bdi.SetIpAddress("172.45.100.2");
	}
      else if ( CamId == 3 )
	{
	  // Initialize the device info object with the known IP address.
	  //bdi.SetIpAddress("182.152.34.14");
	  bdi.SetIpAddress("192.168.35.12");
	}
      else
	cout << "Couldn't open any camera!!!" << endl;
      
      // Create the camera device object.
      Camera_t Camera( pTl->CreateDevice( bdi ) );
      // Open the camera.
      Camera.Open();
   
      // Get the first stream grabber object of the selected camera
      Camera_t::StreamGrabber_t StreamGrabber (Camera.GetStreamGrabber (0));
 
      // Open the stream grabber
      StreamGrabber.Open ();
 
      // Get camera device information
      cout << "Camera Device Information" << endl;
      cout << "==================================================================" << endl;
      cout << "Vendor                                     : " << Camera.DeviceVendorName.GetValue () << endl;    
      cout << "Model                                      : " << Camera.DeviceModelName.GetValue () << endl;
      cout << "Firmware version                           : " << Camera.DeviceFirmwareVersion.GetValue () << endl;
      cout << "DeviceID                                   : " << Camera.DeviceID.GetValue () << endl;
      cout << "DeviceUserID                               : " << Camera.DeviceUserID.GetValue () << endl;
      cout << "Image Size ( W x H )                       : " << Camera.Width.GetValue () << "x" << Camera.Height.GetValue () << endl;
      cout << "Camera Ethernet Speed                      : " << Camera.GevLinkSpeed () << " Mbps" << endl;
      double ResFPS = Camera.ResultingFrameRateAbs.GetValue ();    
      cout << "Maximum Frame Rate for current definitions : " << ResFPS << "fps" << endl;
      cout << "==================================================================" << endl << endl;
	      
      sprintf(LogFileName, "LogTimeCam%d.txt",CamId);
      cout << "Log timestamp in: " << LogFileName << endl << endl;
	      
      fImgTime.open(LogFileName, ios::out );
      fImgTime << "% CamId ImageNumber CameraTimestamp(Ticks 1Tick = 90ns) CameraFrameCounter CameraTriggerCounter" << endl;

      // Enable timestamp chunks
      Camera.ChunkSelector.SetValue(ChunkSelector_Timestamp);
      Camera.ChunkEnable.SetValue(true);
	      
      // Enable frame counter chunks
      Camera.ChunkSelector.SetValue(ChunkSelector_Framecounter);
      Camera.ChunkEnable.SetValue(true);
      
      // Enable trigger Input Counter
      Camera.ChunkSelector.SetValue( ChunkSelector_Triggerinputcounter );
      Camera.ChunkEnable.SetValue(true);
      
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

      // **** Set PixelConvertion - based on ColorConversion.pdf application note
      CPixelFormatConverterYUV422 MyColorConverterYUV422;
      SImageFormat MyInputformat;
    
      // Image width in pixels
      MyInputformat.Width = Camera.Width.GetValue ();
    
      // Image height in pixels
      MyInputformat.Height = Camera.Height.GetValue ();
    
      // Size of one image line in bytes (= Width * bytes per pixel)
      MyInputformat.LinePitch = Camera.Width.GetValue () * 2;
    
      // enum of type "PixelType" declared in PixelType.h
      MyInputformat.PixelFormat = PixelType_YUV422packed;
      MyColorConverterYUV422.Init (MyInputformat);
     
      // An RGBA image needs 4 bytes per pixel
      int BytePerPixel = 4;
    
      unsigned char *MyOutputBuffer =  new unsigned char[Camera.Width.GetValue() * Camera.Height.GetValue() * BytePerPixel];
      SOutputImageFormat MyOutputFormat;
    
      // Size of one image line in bytes (= width * bytes per pixel)
      MyOutputFormat.LinePitch = Camera.Width.GetValue () * BytePerPixel;
    
      // enum of type "PixelType" declared in pixeltype.h
      MyOutputFormat.PixelFormat = PixelType_RGB12packed;	//PixelType_RGB16planar ;
    
      // Set opencv to save image in jpeg format
      IplImage * rawimg = cvCreateImageHeader (cvSize(Camera.Width.GetValue (), Camera.Height.GetValue ()), IPL_DEPTH_8U, 4);    
      rawimg->dataOrder = 0;
      rawimg->origin = 0;
      rawimg->width = Camera.Width.GetValue();
      rawimg->height = Camera.Height.GetValue();
      rawimg->imageSize = Camera.Width.GetValue() * Camera.Height.GetValue() * 4;	// 1278*958*2bytesperpixel
      rawimg->widthStep = Camera.Width.GetValue() * 4;	// 1278*2
    
      int p[3];	// JPEG Compression 
      p[0] = CV_IMWRITE_JPEG_QUALITY;
      p[1] = 85; // Compression Desired Quality Value
      p[2] = 0;
      // **** END PixelConvertion
      
      Camera.AcquisitionStart.Execute();
      cout << "Waiting for TRIGGER" << endl << endl;
	      
      // Grab images
      for (;;)  
      {
	  // Get the grab result from the grabber's result queue
	  GrabResult Result;
	  StreamGrabber.RetrieveResult (Result); 
	  if (Grabbed == Result.Status ())
	  {
	    // Check if a buffer containing chunk data has been received
	    if (PayloadType_ChunkData != Result.GetPayloadType())
	    {
	      cerr << "Unexpected payload type received" << endl;
	      return 1;
	    }
	    // Create ChunkParser
	    IChunkParser* pChunkParser = Camera.CreateChunkParser();
	    if (NULL == pChunkParser)
	    {
	      cerr << "There is no chunk parser available for this device type!" << endl;
	      return 1;
	    }
	    // Attach image buffer with chunk data to the parser. The parser extracts
	    // the included data from the chunk.
	    pChunkParser->AttachBuffer((unsigned char*)Result.Buffer(), Result.GetPayloadSize());
	    
	    fImgTime << CamId << " " << n;
	    // Access the timestamp chunk data
	    if (IsReadable(Camera.ChunkTimestamp))
	      fImgTime << " " << Camera.ChunkTimestamp.GetValue();
	    else
	      fImgTime << " 0";
	    if (IsReadable(Camera.ChunkFramecounter))
	      fImgTime << " " << Camera.ChunkFramecounter.GetValue();
	    else
	      fImgTime << " 0";
	    if (IsReadable(Camera.ChunkTriggerinputcounter))
	      fImgTime << " " << Camera.ChunkTriggerinputcounter.GetValue() << endl;
	    else
	      fImgTime << " 0";
		    
	    // Detach buffer from adapter.
	    // The buffer should always be detached before reusing the buffer for grabbing!
	    // After detaching the buffer, the chunk data is no longer accessible.
	    pChunkParser->DetachBuffer();
		      
	    // Grabbing was successful, process image
	    cout << "Cam" << CamId << "Img" << n << endl;
		      
            const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer (); 
            sprintf(ImgFileName, "Cam%d_%06ld.jpg",CamId,n); n++;
		      
	    // Apply Color Conversion - based on ColorConversion.pdf application note
	    MyColorConverterYUV422.Convert(MyOutputBuffer,	// Pointer to destination buffer
					 Camera.Width.GetValue() * Camera.Height.GetValue() * BytePerPixel,	// Size of destination buffer in bytes
					 (void *) pImageBuffer,	// void* Pointer pointing to INPUT buffer
					 Camera.Width.GetValue () * Camera.Height.GetValue () * 2,	// Size of INPUT buffer
					 MyInputformat,	// Image format of the input image
					 MyOutputFormat	); // Image format of the output image
	   
	   // Apply OpenCV to save image
	   rawimg->imageData = (char *) MyOutputBuffer;
	   cvSaveImage (ImgFileName, rawimg, p);			 

           // Destroy the chunk parser
           Camera.DestroyChunkParser(pChunkParser);
		      
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
