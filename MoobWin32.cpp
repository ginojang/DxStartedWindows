#pragma comment(lib, "D2D1.lib")

#include "framework.h"
#include "MoobWin32.h"

#include <d2d1.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wincodec.h>

#include "freeImage/FreeImage.h"

//
#include "legacyCore/LegacyCore.h"
#include "legacyCore/LoopCore.h"


#define MAX_LOADSTRING 100


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND g_hWnd;
ID2D1Factory* g_pFactory = NULL;
ID2D1HwndRenderTarget* g_pRenderTarget = NULL;
IWICImagingFactory* g_pWicFactory = NULL;
IWICBitmap* g_pWicBitmap = NULL;

ID2D1Bitmap* gp_bitmap;
D2D1_RECT_F g_ScreenRectf;
//D2D1_RECT_U g_ScreenRectu;

extern LoopCore g_loopCore;

/////////////////////////////////////////////////////
//
UINT* g_pBuffers = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;

int FixedUpdateFrame = 1000 / 20;
int UpdateFrame = 1000 / 60;

//
/////////////////////////////////////////////////////


INT64 tickTimeFixedupdate = 0;
INT64 tickTimeUpdate = 0;



void DebugLog(const wchar_t* format, ...)
{
#ifdef _DEBUG
    wchar_t str[512];
    va_list args;
    va_start(args, format);
    wsprintf(str, format, args);
    OutputDebugString(str);
    va_end(args);

    OutputDebugString(L"\n");
#endif
}

int LoadImage(ID2D1RenderTarget* ap_target, const wchar_t* ap_path)
{
    // 기존에 읽은 이미지가 있으면 해당 이미지를 제거
    if (gp_bitmap != NULL)
    {
        gp_bitmap->Release();
        gp_bitmap = NULL;
    }

    // WIC관련 객체를 생성하기 위한 Factory 객체 선언
    IWICImagingFactory* p_wic_factory;

    // WIC 객체를 생성하기 위한 Factory 객체를 생성
    if (S_OK == CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wic_factory)))
    {
        // 압축된 이미지를 해제할 객체
        IWICBitmapDecoder* p_decoder;
        // 특정 그림을 선택한 객체
        IWICBitmapFrameDecode* p_frame;
        // 이미지 변환 객체
        IWICFormatConverter* p_converter;
        // 그림 파일을 읽은 결과 값 (0이면 그림 읽기 실패, 1이면 그림 읽기 성공)

        int result = 0;
        // WIC용 Factory 객체를 사용하여 이미지 압축 해제를 위한 객체를 생성
        if (S_OK == p_wic_factory->CreateDecoderFromFilename(ap_path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder)) 
        {
          
            // 파일을 구성하는 이미지 중에서 첫번째 이미지를 선택한다.
            if (S_OK == p_decoder->GetFrame(0, &p_frame))
            {
                // IWICBitmap형식의 비트맵을 ID2D1Bitmap. 형식으로 변환하기 위한 객체 생성
                if (S_OK == p_wic_factory->CreateFormatConverter(&p_converter))
                {
                    // 선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정
                    if (S_OK == p_converter->Initialize(p_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom))
                    {
                        // IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체를 생성
                        if (S_OK == ap_target->CreateBitmapFromWicBitmap(p_converter, NULL, &gp_bitmap))
                            result = 1; // 성공적으로 생성한 경우
                    }
                    // 이미지 변환 객체 제거
                    p_converter->Release();
                }
                // 그림파일에 있는 이미지를 선택하기 위해 사용한 객체 제거
                p_frame->Release();
            }
            // 압축을 해제하기 위해 생성한 객체 제거
            p_decoder->Release();
        }
        // WIC를 사용하기 위해 만들었던 Factory 객체 제거
        p_wic_factory->Release();

        // PNG 파일을 읽은 결과를 반환한다.
        return result;
    }

    return -1;
}

void DestoryRenderTarget()
{
    if(g_pRenderTarget != NULL)
    {
        g_pRenderTarget->Release();
        g_pRenderTarget = NULL;
    }
    if (g_pFactory != NULL)
    {
        g_pFactory->Release();
        g_pFactory = NULL;
    }
    /*
    if (g_pWicFactory != NULL)
    {
        g_pWicFactory->Release();
        g_pWicFactory = NULL;
    }
    if (g_pWicBitmap != NULL)
    {
        g_pWicBitmap->Release();
        g_pWicBitmap = NULL;
    }*/
    if (g_pBuffers != NULL)
    {
        delete[] g_pBuffers;
        g_pBuffers = NULL;
    }
}





void Start()
{
    DebugLog(L">>>>>>>>>   Win32 Started.. ");

 //   LoadImage(g_pRenderTarget, L"test.png");

    
    if (S_OK == CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pWicFactory)))
    {
        if (S_OK == g_pWicFactory->CreateBitmap(SCREEN_WIDTH, SCREEN_HEIGHT, GUID_WICPixelFormat32bppPRGBA, WICBitmapCacheOnDemand, &g_pWicBitmap))
        {
            g_pRenderTarget->CreateBitmapFromWicBitmap(g_pWicBitmap, &gp_bitmap);
        }
    }

    if (gp_bitmap)
    {
        D2D1_PIXEL_FORMAT pixelFormat = gp_bitmap->GetPixelFormat();
        D2D1_SIZE_U size = gp_bitmap->GetPixelSize();
        D2D1_SIZE_U pixel_size = gp_bitmap->GetPixelSize();

        //if (pixelFormat.format == DXGI_FORMAT_B8G8R8A8_UNORM)

        g_pBuffers = new UINT[pixel_size.width * pixel_size.height];
    }
    

    ///////////////////////////////////////////////////////////
    //
    g_loopCore.Start(g_pBuffers, SCREEN_WIDTH, SCREEN_HEIGHT);
    //
    ///////////////////////////////////////////////////////////
}

void FixedUpdate()
{
    float timer = GetTickCount64() / 1000;

    ///////////////////////////////////////////////////////////
    //
    g_loopCore.FixedUpdate(timer);
    //
    ///////////////////////////////////////////////////////////
}


void UpdateBuffer()
{
    ///////////////////////////////////////////////////////////
    //
    g_loopCore.Update();
    //
    ///////////////////////////////////////////////////////////

    /*
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        unsigned char* color = (unsigned char*)(g_pBuffers + i);

        *(color + 0) = 10;     //r
        *(color + 1) = 10;       //g
        *(color + 2) = 50;     //b
        *(color + 3) = 255;
    }*/
}


void Update()
{
    UpdateBuffer();

    g_pRenderTarget->BeginDraw();
    if(gp_bitmap != NULL)
    {
        //
        D2D1_RECT_U rect;
        gp_bitmap->CopyFromMemory(NULL, g_pBuffers, SCREEN_WIDTH * sizeof(UINT));
        g_pRenderTarget->DrawBitmap(gp_bitmap, &g_ScreenRectf);
        
    }
    else
        g_pRenderTarget->Clear(D2D1::ColorF(0.0f, 20.0f, 0.0f));

    g_pRenderTarget->EndDraw();
}

void Close()
{
    ///////////////////////////////////////////////////////////
    //
    g_loopCore.Close();
    //
    ///////////////////////////////////////////////////////////

    DebugLog(L">>>>>>>>>> Win32 Closed.. ");
    DestoryRenderTarget();
}

void Loop()
{
    INT64 current = GetTickCount64();

    if (current - tickTimeFixedupdate >= FixedUpdateFrame)
    {
        FixedUpdateFrame = current;
        FixedUpdate();
    }

    if (current - tickTimeUpdate >= UpdateFrame)
    {
        tickTimeUpdate = current;
        Update();
    }

    Sleep(0);
}


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
    LoadStringW(hInstance, IDC_MOOBWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    //
    g_ScreenRectf.left = 0;
    g_ScreenRectf.top = 0;
    g_ScreenRectf.right = SCREEN_WIDTH;
    g_ScreenRectf.bottom = SCREEN_HEIGHT;

 
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    //


    if (S_OK == D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pFactory))
    {
        if (S_OK == g_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(g_hWnd, D2D1::SizeU(g_ScreenRectf.right, g_ScreenRectf.bottom)), &g_pRenderTarget))
        {

        }
        else
            return -1;
    }
    else
    {
        return -1;
    }


    //
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOOBWIN32));
    MSG msg;

    Start();

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Loop();
    }

    Close();
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOOBWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MOOBWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   MoveWindow(g_hWnd, 100, 100, g_ScreenRectf.right + 10, g_ScreenRectf.bottom + 10, TRUE);
   UpdateWindow(g_hWnd);

   //
   return TRUE;
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
            if(g_pRenderTarget)
                Update();

            return 0;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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


