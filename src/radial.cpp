#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iostream>
using namespace Gdiplus;

VOID OnPaint(HDC hdc)
{
  // Create a path that consists of a single ellipse.
  Graphics graphics(hdc);
  Point points[] = {Point(100, 0), 
                    Point(200, 200), 
                    Point(0, 200)};

  // No GraphicsPath object is created. The PathGradient
  // brush is constructed directly from the array of points.
  PathGradientBrush pthGrBrush(points, 3);
  
  Color presetColors[] = {
     Color(255, 0, 128, 0),    // Dark green
     Color(255, 0, 255, 255),  // Aqua
     Color(255, 0, 0, 255)};   // Blue
  
  REAL interpPositions[] = {
     0.0f,   // Dark green is at the boundary of the triangle.
     0.25f,  // Aqua is 25 percent of the way from the boundary
             // to the center point.
     1.0f};  // Blue is at the center point.
                  
  // Explicitly set center color for safety.
  //int count = 3;
  //pthGrBrush.SetCenterColor(Color(255, 0, 0, 255));
  //pthGrBrush.SetSurroundColors(presetColors, &count);
  
  pthGrBrush.SetInterpolationColors(presetColors, interpPositions, 3);
  
  // Fill a rectangle that is larger than the triangle
  // specified in the Point array. The portion of the
  // rectangle outside the triangle will not be painted.
  graphics.FillRectangle(&pthGrBrush, 0, 0, 200, 200);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   HWND                hWnd;
   MSG                 msg;
   WNDCLASS            wndClass;
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
   hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("Getting Started"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      CW_USEDEFAULT,            // initial x position
      CW_USEDEFAULT,            // initial y position
      CW_USEDEFAULT,            // initial x size
      CW_USEDEFAULT,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
      
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
} // WndProc
