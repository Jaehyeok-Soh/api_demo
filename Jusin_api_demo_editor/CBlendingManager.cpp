#include "pch.h"
#include "CBlendingManager.h"
#include "CBmpManager.h"
CBlendingManager* CBlendingManager::m_pInstance = nullptr;
ULONG_PTR CBlendingManager::g_GdiToken;

CBlendingManager::CBlendingManager()
{
}

CBlendingManager::~CBlendingManager()
{
	Release();
}

void CBlendingManager::Initialize()
{
	InitGDIPlus();
}

void CBlendingManager::Render(HDC _dc, wstring _path, float x, float y, float cx, float cy)
{
	Graphics g(_dc);
	g.SetCompositingMode(CompositingModeSourceOver);

	Image image(L"../Image/ApiDemo/Client/endofgame/image1.png");
	//g.DrawImage(&image, 100, 100, 600, 600);
	
	// Initialize the color matrix.
	// Notice the value 0.8 in row 4, column 4.
	ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.8f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	// Create an ImageAttributes object and set its color matrix.
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault,
		ColorAdjustTypeBitmap);
	
	g.DrawImage(
		&image,
		Rect(x, y, cx, cy),  // Destination rectangle
		0,                             // Source rectangle X 
		0,                             // Source rectangle Y
		cx,                        // Source rectangle width
		cy,                       // Source rectangle height
		UnitPixel,
		&imageAtt);


	//// Create a Bitmap object and load it with the texture image.
	//Bitmap bitmap(L"Texture1.jpg");
	//Pen pen(Color(255, 0, 0, 0), 25);
	//// Initialize the color matrix.
	//// Notice the value 0.8 in row 4, column 4.
	//ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	//						   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	//						   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	//						   0.0f, 0.0f, 0.0f, 0.8f, 0.0f,
	//						   0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	//// Create an ImageAttributes object and set its color matrix.
	//ImageAttributes imageAtt;
	//imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault,
	//	ColorAdjustTypeBitmap);
	//// First draw a wide black line.
	//g.DrawLine(&pen, Point(10, 35), Point(200, 35));
	//// Now draw the semitransparent bitmap image.
	//INT iWidth = bitmap.GetWidth();
	//INT iHeight = bitmap.GetHeight();
	//g.DrawImage(
	//	&bitmap,
	//	Rect(30, 0, iWidth, iHeight),  // Destination rectangle
	//	0,                             // Source rectangle X 
	//	0,                             // Source rectangle Y
	//	iWidth,                        // Source rectangle width
	//	iHeight,                       // Source rectangle height
	//	UnitPixel,
	//	&imageAtt);
}

void CBlendingManager::Release()
{
	ShutDownGDIPlus();
}

void CBlendingManager::InitGDIPlus()
{
	GdiplusStartupInput gdiSI;
	GdiplusStartup(&g_GdiToken, &gdiSI, nullptr);
}

void CBlendingManager::ShutDownGDIPlus()
{
	GdiplusShutdown(g_GdiToken);
}
