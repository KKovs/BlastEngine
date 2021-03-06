#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

#include "Graphics.h"
#include <Windows.h>
#include "Controller.h"
#include "Platform.h"
#include "Scene.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define windowName L"Blast Engine"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Controller::renderWindowMessageProcessor;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(0);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"be1";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr,
			L"Call to RegisterClassEx failed!",
			L"Error",
			0);

		return 1;
	}

	RECT windowRect = { 200, 100, 1024, 768 };
	UINT windowStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRect(&windowRect, windowStyle, false);

	HWND hWnd = CreateWindow(
		wcex.lpszClassName,
		windowName,
		windowStyle,
		windowRect.left, windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		MessageBox(nullptr,
			L"Call to CreateWindow failed!",
			L"Error",
			0);

		return 1;
	}

	Platform::toggleSystemConsole();
	
	DragAcceptFiles(hWnd, true);

	if (!Graphics::initializeGraphics(hWnd))
	{
		MessageBox(0, L"Direct3D Initialization failed.",
			L"Error", MB_OK);
		return 0;
	}

	//Scene(R"(D:\COMMON\pracownia\VisualStudio\BlastEngine\1.scene)");
	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			if (GetActiveWindow() == hWnd)
			{
				Controller::processRealtimeInput();
			}
			Graphics::drawScene();
		}

	}

	Graphics::shutdownGraphics();
	return static_cast<int>(msg.wParam);
}
