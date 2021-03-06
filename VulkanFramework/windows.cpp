#include <Windows.h>
#include <chrono>
#include <string>

#ifndef NDEBUG
#define CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include "system.h"
#include "time.h"
#include "myvulkan.h"

void UpdateClientRect(const HWND& hwnd)
{
  POINT p;
  p.x = winAPI.screen_client.left;
  p.y = winAPI.screen_client.top;
  ClientToScreen(hwnd, &p);
  winAPI.screen_client.left = p.x;
  winAPI.screen_client.top = p.y;
  p.x = winAPI.screen_client.right;
  p.y = winAPI.screen_client.bottom;
  ClientToScreen(hwnd, &p);
  winAPI.screen_client.right = p.x;
  winAPI.screen_client.bottom = p.y;

  winAPI.m_width = winAPI.screen_client.right - winAPI.screen_client.left;
  winAPI.m_height = winAPI.screen_client.bottom - winAPI.screen_client.top;
}

LRESULT CALLBACK MainWindowCallback(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
  switch (u_msg)
  {
  case WM_SETCURSOR:
    static HCURSOR normal_cursor = LoadCursor(nullptr, IDC_ARROW);
    SetCursor(normal_cursor);
    break;

  case WM_SIZE:
    GetClientRect(hwnd, &winAPI.screen_client);
    GetWindowRect(hwnd, &winAPI.screen_window);
    UpdateClientRect(hwnd);
    //graphics.ResizeScreen();
    break;

  case WM_MOVE:
    GetClientRect(hwnd, &winAPI.screen_client);
    GetWindowRect(hwnd, &winAPI.screen_window);
    UpdateClientRect(hwnd);
    break;

  case WM_CLOSE:
  case WM_DESTROY:
    engine.Quit();
    //wglMakeCurrent(winAPI.m_hdc, NULL);
    //wglDeleteContext(winAPI.m_hglrc);
    ReleaseDC(hwnd, winAPI.m_hdc);
    break;

  default:
    return DefWindowProc(hwnd, u_msg, w_param, l_param);
  }

  return 0;
}

HWND InitializeWindow(WNDCLASS& wnd_class, const HINSTANCE& h_inst)
{
  wnd_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; //window class style
  wnd_class.lpfnWndProc = MainWindowCallback;
  wnd_class.hInstance = h_inst;
  wnd_class.lpszClassName = "Framework ran with Vulkan Library";

  if (RegisterClass(&wnd_class))
    return CreateWindowEx(0,
      wnd_class.lpszClassName,
      "Vulkan Framework", //title
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT,
      winAPI.m_initial_width,
      winAPI.m_initial_height,
      0, 0, wnd_class.hInstance, 0);

  return nullptr;
}

bool Initialize()
{
  srand((unsigned)time(NULL));

#ifndef NDEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //memory leak check
#endif

  if (!vulkan.BaseInitialize()) return false;
  if (!vulkan.Initialize()) return false;

  return true;
}

void Begin()
{
  timer.Begin();
}

bool Update()
{
  if (!vulkan.Update()) return false;
  timer.Update(); // should be at last, always!

  return true;
}

void End()
{
}

void Terminate()
{
  vulkan.Terminate();
  vulkan.BaseTerminate();
}

int CALLBACK WinMain(HINSTANCE h_instance, HINSTANCE, LPSTR, int)
{
  WNDCLASS window_class = {};
  winAPI.m_window_handle = InitializeWindow(window_class, h_instance);
  winAPI.m_instance_handle = h_instance;
  if (!winAPI.m_window_handle) return 0;

  if (!Initialize()) return 0;

  MSG msg;
  while (!engine.m_quit)
  {
    engine.m_restart = false;
    Begin();
    while (!engine.m_restart)
    {
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      if (!Update()) engine.Quit();
    }
    End();
  }
  Terminate();

  return 0;
}