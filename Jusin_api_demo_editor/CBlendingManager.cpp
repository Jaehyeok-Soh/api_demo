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

void CBlendingManager::Render(HDC _dc, const TCHAR* _path, Rect destRect, float x, float y, float cx, float cy, float _alpha)
{
	Graphics g(_dc);
	g.SetCompositingMode(CompositingModeSourceOver);

	Image image(_path);
	
	// Initialize the color matrix.
	// Notice the value 0.8 in row 4, column 4.
	ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, _alpha, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	// Create an ImageAttributes object and set its color matrix.
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault,
		ColorAdjustTypeBitmap);
	
	g.DrawImage(
		&image,
		destRect,  // Destination rectangle
		x,                             // Source rectangle X 
		y,                       // Source rectangle Y
		cx,                             // Source rectangle height
		cy,                        // Source rectangle width
		UnitPixel,
		&imageAtt);
}

void CBlendingManager::RenderBlend(HDC _dc, wstring _path, Rect destRect, float x, float y, float cx, float cy, float _alpha)
{
	Graphics g(_dc);
	g.SetCompositingMode(CompositingModeSourceOver);

	Image image(_path.c_str());
	//g.DrawImage(&image, 100, 100, 600, 600);

	// Initialize the color matrix.
	// Notice the value 0.8 in row 4, column 4.
	ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, _alpha, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	// Create an ImageAttributes object and set its color matrix.
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault,
		ColorAdjustTypeBitmap);

	g.DrawImage(
		&image,
		destRect,  // Destination rectangle
		0,                             // Source rectangle X 
		0,                       // Source rectangle Y
		cx,                             // Source rectangle height
		cy,                        // Source rectangle width
		UnitPixel,
		&imageAtt);
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
