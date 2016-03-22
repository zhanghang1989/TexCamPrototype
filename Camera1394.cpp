// Camera1394.cpp : Defines the entry point for the console application.
// Originally written by Hang Zhang
// Update on 5/7/2014

#include "stdafx.h"
#include "xmv\xmv.h"
#include <Windows.h>
#include <1394Camera.h>
#include "stdafx.h"
#using <System.dll>
#include <stdlib.h>
#include <direct.h>

using namespace System::IO::Ports;
using namespace System::Threading;
using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
#pragma comment(lib, "1394camerad.lib")
//#pragma comment(lib, "MSCOREE.lib")

#define call_by_mfc
#define delta 0.1*step

int instance = 0;
int step = 600;
int shutter = 2048;

template <typename T>
void save_image(xmv::Matrix<T> &imagematrix, String ^path);
void save_image(byte* imgbuf, int width, int height, String ^filename);
void save_image(C1394Camera &theCamera, int width, int height, String ^filename);
char* TCHAR2char(TCHAR* tchStr);

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef call_by_mfc
	char *MaterialName = TCHAR2char(argv[1]);
	instance = atoi(TCHAR2char(argv[2]));
	step = atoi(TCHAR2char(argv[3]));
	shutter = atoi(TCHAR2char(argv[4]));
	char *PointIndex = TCHAR2char(argv[5]);
	String ^dir = Application::StartupPath + "\\" 
		+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName);
	char* sdir = (char*)(void*)Marshal::StringToHGlobalAnsi(dir);
	int index_dir = _mkdir(sdir);

	if (index_dir == 0)
	{
		ofstream outf("dir.txt", ios::app);
		outf << MaterialName << endl;
		outf.close();
	}

	String ^dir2 = Application::StartupPath + "\\" 
		+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + "\\"
		+ instance.ToString();
	char* sdir2 = (char*)(void*)Marshal::StringToHGlobalAnsi(dir2);

	int index_dir2 = _mkdir(sdir2);

	if (index_dir2 == 0)
	{
		ofstream outf("dir2.txt", ios::app);
		outf << MaterialName << "\\" << instance << endl;
		outf.close();
	}

#endif // call_by_mfc

	C1394Camera theCamera;

	unsigned long size;
	int index;
	int img_num = 0;
	// check the link
	if (theCamera.CheckLink() == CAM_ERROR)
	{
		Console::Write("No camera detected, check the link please!\n");
		return -1;
	}

	theCamera.RefreshCameraList();
	theCamera.SelectCamera(0);
	// init camera
	index = theCamera.InitCamera(true);
	
	printf("Camera defaults to %d,%d,%d\n",
		theCamera.GetVideoFormat(),
		theCamera.GetVideoMode(),
		theCamera.GetVideoFrameRate());

	C1394CameraControl ControlShutter(&theCamera, FEATURE_SHUTTER);
	ControlShutter.SetValue(shutter);
	
	C1394CameraControl WhiteBalance(&theCamera, FEATURE_WHITE_BALANCE);
	WhiteBalance.SetValue(104, 220);

	// start image acquisition
	index = theCamera.StartImageAcquisition();
	
	SerialPort^ serialPort = gcnew SerialPort(); //(L"COM3",9600,Parity::None,1,StopBits::One,Handshake::None);
	/// initiliaze the port
	serialPort->PortName = "COM3";
	serialPort->BaudRate = 9600;
	serialPort->Parity = Parity::None;
	serialPort->DataBits = 8;
	serialPort->StopBits = StopBits::One;
	serialPort->Handshake = Handshake::None;
	serialPort->WriteTimeout = 1000;
	serialPort->ReadTimeout = 5000;
	if (!serialPort->IsOpen)   /// open the port
		serialPort->Open();

	// set motors' speed to be 900
	String^ Command = gcnew String("F,C,S1M900,S2M900,R");
	// send out the command throug the port 

	serialPort->WriteLine(Command);
	char nchar;
	do{
		try
		{
			nchar = serialPort->ReadChar();
		}
		catch (TimeoutException ^) {}
	} while (nchar != '^');

	Console::Write("Setup speed successfully!\n");

	// move motors 3(x) and 4(y) back to zero position, assume movement
	Command = gcnew String("F,C,IA1M-0,IA2M-0,R");
	serialPort->WriteLine(Command);
	do{
		try
		{
			nchar = serialPort->ReadChar();
		}
		catch (TimeoutException ^) {}
	} while (nchar != '^');

	Console::Write("Setup origin successfully!\n");
	//system("pause");

	// taking measurement along y-axis, 5 spots
	for (int i = -2*step; i <= 2 * step; i += step)  ///the frequency is 10Hz
	{
		//for (int j = -2 * step; j <= 2 * step; j += step)
		int j = 0;

		img_num++;
		Command = gcnew String("F,C,IA1M" + i.ToString() + ",IA2M" + j.ToString() + ",R");
		serialPort->WriteLine(Command);
		do{
			try
			{
				nchar = serialPort->ReadChar();
			}
			catch (TimeoutException ^) {}
		} while (nchar != '^');
		Sleep(1000);
		theCamera.AcquireImage();
		theCamera.GetRawData(&size);
#ifdef call_by_mfc
		save_image(theCamera, 320, 240, Application::StartupPath + "\\" 
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + "\\" 
			+ instance.ToString() + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + ".i" 
			+ instance.ToString() + ".p"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)PointIndex) + ".s" 
			+ shutter.ToString() + "." + img_num.ToString() + ".bmp");
#else
		save_image(theCamera, 320, 240, Application::StartupPath + "\\" + img_num.ToString() + ".bmp");
#endif // call_by_mfc

		// plus delta
		int id = i + delta;
		Command = gcnew String("F,C,IA1M" + id.ToString() + ",IA2M" + j.ToString() + ",R");
		serialPort->WriteLine(Command);
		do{
			try
			{
				nchar = serialPort->ReadChar();
			}
			catch (TimeoutException ^) {}
		} while (nchar != '^');
		Sleep(1000);
		theCamera.AcquireImage();
		theCamera.GetRawData(&size);
#ifdef call_by_mfc
		save_image(theCamera, 320, 240, Application::StartupPath + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + "\\"
			+ instance.ToString() + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + ".i"
			+ instance.ToString() + ".p"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)PointIndex) + ".s"
			+ shutter.ToString() + "." + img_num.ToString() + ".d.bmp");
#else
		save_image(theCamera, 320, 240, Application::StartupPath + "\\" + img_num.ToString() + ".bmp");
#endif // call_by_mfc
	}


	// taking measurement along x-axis, 2 spots
	for (int j = - 2*step; j <= 2*step; j += 4*step)
	{
		int i = 0;
		img_num++;
		Command = gcnew String("F,C,IA1M" + i.ToString() + ",IA2M" + j.ToString() + ",R");
		serialPort->WriteLine(Command);
		do{
			try
			{
				nchar = serialPort->ReadChar();
			}
			catch (TimeoutException ^) {}
		} while (nchar != '^');
		//Sleep(1000);
		theCamera.AcquireImage();
		theCamera.GetRawData(&size);
#ifdef call_by_mfc
		save_image(theCamera, 320, 240, Application::StartupPath + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + "\\"
			+ instance.ToString() + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + ".i"
			+ instance.ToString() + ".p"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)PointIndex) + ".s"
			+ shutter.ToString() + "." + img_num.ToString() + ".bmp");
#else
		save_image(theCamera, 320, 240, Application::StartupPath + "\\" + img_num.ToString() + ".bmp");
#endif // 

		// plus delta
		int jd = j + delta;
		Command = gcnew String("F,C,IA1M" + i.ToString() + ",IA2M" + jd.ToString() + ",R");
		serialPort->WriteLine(Command);
		do{
			try
			{
				nchar = serialPort->ReadChar();
			}
			catch (TimeoutException ^) {}
		} while (nchar != '^');
		//Sleep(1000);
		theCamera.AcquireImage();
		theCamera.GetRawData(&size);
#ifdef call_by_mfc
		save_image(theCamera, 320, 240, Application::StartupPath + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + "\\"
			+ instance.ToString() + "\\"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)MaterialName) + ".i"
			+ instance.ToString() + ".p"
			+ System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)PointIndex) + ".s"
			+ shutter.ToString() + "." + img_num.ToString() + ".d.bmp");
#else
		save_image(theCamera, 320, 240, Application::StartupPath + "\\" + img_num.ToString() + ".bmp");
#endif // call_by_mfc
	}


	// Index back to the origin
	Command = gcnew String("F,C,IA1M0,IA2M0,R");
	serialPort->WriteLine(Command);
	do{
		try
		{
			nchar = serialPort->ReadChar();
		}
		catch (TimeoutException ^) {}
	} while (nchar != '^');

	// Quit online mode
	//Sleep(1000);
	Command = gcnew String("Q,R");
	serialPort->WriteLine(Command);
	do{
		try
		{
			nchar = serialPort->ReadChar();
		}
		catch (TimeoutException ^) {}
	} while (nchar != '^');

	if (serialPort->IsOpen)  // close the port
		serialPort->Close();

	// stop capturing
	if (theCamera.IsAcquiring())
	{
		theCamera.StopImageAcquisition();
	}
	return 0;
}

void save_image(C1394Camera &theCamera, int width, int height, String ^filename)
{
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(width, height, PixelFormat::Format24bppRgb);
	BitmapData ^ bmpData = bmp->LockBits(System::Drawing::Rectangle::Rectangle(0, 0, bmp->Width, bmp->Height),
		ImageLockMode::ReadWrite, PixelFormat::Format24bppRgb);
	byte * bytes = (byte*)(bmpData->Scan0.ToInt32());

	theCamera.getDIB(bytes, bmp->Width*bmp->Height * 3);
	bmp->UnlockBits(bmpData);
	bmp->Save(filename, ImageFormat::Bmp);
}

void save_image(byte* imgbuf, int width, int height, String ^filename)
{
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(width, height, PixelFormat::Format24bppRgb);
	BitmapData ^ bmpData = bmp->LockBits(System::Drawing::Rectangle::Rectangle(0, 0, bmp->Width, bmp->Height),
		ImageLockMode::ReadWrite, PixelFormat::Format24bppRgb);
	byte * bytes = (byte*) (bmpData->Scan0.ToInt32());
	memcpy(bytes, imgbuf, width*height * 3);
	bmp->UnlockBits(bmpData);
	bmp->Save(filename, ImageFormat::Bmp);
}

template <typename T>
void save_image(xmv::Matrix<T> &imagematrix, String ^filename)
{
	System::Drawing::Bitmap ^bmp = gcnew System::Drawing::Bitmap(imagematrix.Col() / 3, imagematrix.Row(), PixelFormat::Format24bppRgb);
	//cout << "width = " << bmp->Width << "height = " << bmp->Height << endl;
	BitmapData ^ bmpData = bmp->LockBits(System::Drawing::Rectangle::Rectangle(0, 0, bmp->Width, bmp->Height),
		ImageLockMode::ReadWrite, PixelFormat::Format24bppRgb);

	byte * bytes = (byte*) (bmpData->Scan0.ToInt32());
	memcpy(bytes, imagematrix.Buffer(), imagematrix.Row() * imagematrix.Col());

	bmp->UnlockBits(bmpData);
	bmp->Save(filename, ImageFormat::Bmp);
}

char* TCHAR2char(TCHAR* tchStr)
{
	int iLen = 2 * wcslen(tchStr);
	char* chRtn = new char[iLen + 1];
	wcstombs(chRtn, tchStr, iLen + 1);
	return chRtn;
}
