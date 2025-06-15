// Jusin_api_demo_editor.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Jusin_api_demo_editor.h"

#include "CMainGame.h"
#include "CScrollManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hWnd;
float g_fZoom = 5.1f;
bool bColRender = false;
bool bTileRender = false;
POINT g_ptMousePos = {};

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JUSINAPIDEMOEDITOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUSINAPIDEMOEDITOR));

    MSG msg;

    CMainGame MainGame;
    MainGame.Initialize();

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {
            MainGame.Update();
            MainGame.Late_Update();
            MainGame.Render();
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;       // 윈도우 창 생성을 필요로한 구조체 정보

    wcex.cbSize = sizeof(WNDCLASSEX);       // 자기 자신의 사이즈를 저장

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // 창 스타일 지정 : 가로 다시 그리기 | 세로 다시 그리기
    // 창의 수직 또는 수평의 크기가 변경될 경우 창을 다시 그려라하는 옵션

    wcex.lpfnWndProc = WndProc;
    // 메세지 처리기 함수 이름을 저장할 포인터

    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    // 윈도우가 특수한 목적으로 사용하는 여분의 공간, 일종의 예약 영역, 기본값 0

    wcex.hInstance = hInstance;
    // 프로그램 번호 설정

    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JUSINAPIDEMOEDITOR));
    // 윈도우 창 표현 아이콘(시작 표시줄)

    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // 마우스 커서

    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    // 배경 색상

    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDI_JUSINAPIDEMOEDITOR);
    // 창 상단에 메뉴

    wcex.lpszClassName = szWindowClass;
    // 실행 파일 이름

    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    // 윈도우 상단에 있는 작은 아이콘 모양

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    RECT rc{ 0, 0, WINCX, WINCY };

    // rc = rc + 기본 윈도우 설정 값 + 메뉴 바 사이즈 크기 고려 여부

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(szWindowClass,         // 앞서 정의한 윈도우 클래스의 전달한 클래스 이름을 넣어줌
        szTitle,                // 창 상단의 출력할 문자열
        WS_OVERLAPPEDWINDOW,    // 윈도우 창 스타일 지정(기본 값으로 사용되는 창 모양)
        CW_USEDEFAULT, 0,       // 창 출력 좌표(x,y 좌표)
        rc.right - rc.left,
        rc.bottom - rc.top,       // 창 가로, 세로 사이즈
        nullptr,        // 부모 윈도우 주소
        nullptr,        // 윈도우에서 사용할 메뉴 핸들
        hInstance,      // 윈도우를 만드는 주체
        nullptr);       // 운영체제가 특수한 목적으로 사용

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void ClientCursorLock()
{
    RECT rc{ 0, 0, WINCX, WINCY };
    POINT p1, p2;

    p1.x = rc.left;
    p1.y = rc.top;
    p2.x = rc.right;
    p2.y = rc.bottom;

    ClientToScreen(g_hWnd, &p1);
    ClientToScreen(g_hWnd, &p2);

    rc.left = p1.x;
    rc.top = p1.y - 50.f;
    rc.right = p2.x;
    rc.bottom = p2.y;

    ClipCursor(&rc);
}

void ClientCursorUnlock()
{
    ClipCursor(NULL);
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEWHEEL:
    {
        float fOldZoom = g_fZoom;

        // 스크롤 이전 커서 위치 얻기
        GetCursorPos(&g_ptMousePos);
        ScreenToClient(g_hWnd, &g_ptMousePos);  // 클라이언트 좌표로 변환

        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta > 0)
            g_fZoom += 0.1f; // 확대
        else
            g_fZoom -= 0.1f; // 축소

        if (g_fZoom < 1.0f) g_fZoom = 1.0f;
        if (g_fZoom > 5.5f) g_fZoom = 5.5f;

        InvalidateRect(hWnd, nullptr, FALSE); // 다시 그리기 요청
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
            ClientCursorUnlock();
        else
            ClientCursorLock();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
