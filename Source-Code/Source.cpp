#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "basewin.h"
#include "Snake.h"

Snake s;

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class MainWindow : public BaseWindow<MainWindow>
{
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pBrush;


    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();
    void drawSnake(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush, std::pair<std::pair<int,int>, std::pair<int,int>> x);
    void drawApple(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush, std::pair<std::pair<int, int>, std::pair<int, int>> Snake);
    void drawArea(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush);

public:

    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

void MainWindow::drawSnake(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush, std::pair<std::pair<int, int>, std::pair<int, int>> Snake) {

    D2D1_SIZE_F size = pRT->GetSize();
    const float w = size.width;
    const float h = size.height;

    D2D1_RECT_F Rect = D2D1::RectF(
        (w < h ? (w * Snake.first.first) / 25 : ((w - h) / 2) + ((h * Snake.first.first) / 25)),
        (h < w ? (h * Snake.first.second) / 25 : ((h - w) / 2) + ((w * Snake.first.second) / 25)),
        (w < h ? (w * Snake.second.first) / 25 : ((w - h) / 2) + ((h * Snake.second.first) / 25)),
        (h < w ? (h * Snake.second.second) / 25 : ((h - w) / 2) + ((w * Snake.second.second) / 25))
    );

    const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Green);
    HRESULT hr = pRT->CreateSolidColorBrush(color, &pBrush);

    pRT->FillRectangle(&Rect, pBrush);
    pRT->DrawRectangle(&Rect, pBrush, 1.0f);
}

void MainWindow::drawApple(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush, std::pair<std::pair<int, int>, std::pair<int, int>> Snake) {

    D2D1_SIZE_F size = pRT->GetSize();
    const float w = size.width;
    const float h = size.height;

    D2D1_RECT_F Rect = D2D1::RectF(
        (w < h ? (w * Snake.first.first) / 25 : ((w - h) / 2) + ((h * Snake.first.first) / 25)),
        (h < w ? (h * Snake.first.second) / 25 : ((h - w) / 2) + ((w * Snake.first.second) / 25)),
        (w < h ? (w * Snake.second.first) / 25 : ((w - h) / 2) + ((h * Snake.second.first) / 25)),
        (h < w ? (h * Snake.second.second) / 25 : ((h - w) / 2) + ((w * Snake.second.second) / 25))
    );

    const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Red);
    HRESULT hr = pRT->CreateSolidColorBrush(color, &pBrush);

    pRT->FillRectangle(&Rect, pBrush);
    pRT->DrawRectangle(&Rect, pBrush, 1.0f);
}

void MainWindow::drawArea(ID2D1RenderTarget* pRT, ID2D1SolidColorBrush* pBrush) {

    D2D1_SIZE_F size = pRT->GetSize();
    const float w = size.width;
    const float h = size.height;

    D2D1_RECT_F Rect = D2D1::RectF(
        (w < h ? 0.f : ((w - h) / 2)),
        (h < w ? 0.f : ((h - w) / 2)),
        (w < h ? w : h + ((w - h) / 2)),
        (h < w ? h : w + ((h - w) / 2))
    );

    const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Black);
    HRESULT hr = pRT->CreateSolidColorBrush(color, &pBrush);

    pRT->FillRectangle(&Rect, pBrush);
    pRT->DrawRectangle(&Rect, pBrush, 1.0f);
}

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.f));

        drawArea(pRenderTarget, pBrush);

        drawApple(pRenderTarget, pBrush, s.getA());

        for (int i = 0; i < s.sizeS(); i++) {
            drawSnake(pRenderTarget, pBrush, s.getS(i));
        }

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    MainWindow win;

    if (!win.Create(L"Snake", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    wchar_t msg[32];
    switch (uMsg)
    {
    case WM_CREATE:

        s.INIT();

        SetTimer(m_hwnd, 1, 250, (TIMERPROC)NULL);

        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:

        s.bDeath();
        s.sDeath();
        if (s.death == true) {
            s.INIT();
        }

        s.CheckApple();

        s.move();

        OnPaint();

        return 0;

    case WM_TIMER:
        switch (wParam) {
        case 1:
            InvalidateRect(m_hwnd, NULL, FALSE);
            SetTimer(m_hwnd, 1, 150, (TIMERPROC)NULL);
            break;

            return 0;
        }

    case WM_SIZE:
        Resize();
        break;

    case WM_KEYDOWN:
    { switch (wParam) {
    case VK_LEFT:
        s.setD(-1, 0);
        break;
    case VK_UP:
        s.setD(0, -1);
        break;
    case VK_RIGHT:
        s.setD(1, 0);
        break;
    case VK_DOWN:
        s.setD(0, 1);
        break;
    }}
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}