#ifndef HIKCAMERA_H
#define HIKCAMERA_H

#include <stdio.h>
#include <string>
#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "MvCameraControl.h"

extern unsigned int        g_nPayloadSize = 1440 * 1080;                                                    // cache size
extern unsigned char*      pData = (unsigned char *)malloc(sizeof(unsigned char) * (g_nPayloadSize));        // cache address of picture data

using namespace std;
using namespace cv;

void* CreateCamera(string cameraname);
void GrabCamera(void* handle);
Mat Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData);
Mat GetMatFrame(void* handle);
void StopCamera(void* handle);
void CloseCamera(void* handle);
void SetExposureAuto(void* handle, bool isauto);
void SetExposure(void* handle, float exposure);
void SetFramerate(void* handle, float framerate);

#endif // HIKCAMERA_H
