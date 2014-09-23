#include "MainWindow.h"

// класс CMainWindow
// константы

const wchar_t* CMainWindow::className = L"MathRedactorWindowClass";

// public методы 

CMainWindow::CMainWindow() 
{
	windowHandle = 0;
	editWindow = 0;
}

CMainWindow::~CMainWindow()
{
	if( editWindow != 0 ) {
		delete( editWindow );
	}
}

bool CMainWindow::RegisterClass( HINSTANCE classOwnerInstance )
{
	WNDCLASSEX classInfo;
	::ZeroMemory( &classInfo, sizeof( WNDCLASSEX ) );
	classInfo.cbSize = sizeof( WNDCLASSEX );
	classInfo.hInstance = classOwnerInstance;
	classInfo.lpszClassName = className;
	classInfo.style = CS_HREDRAW | CS_VREDRAW;
	classInfo.lpfnWndProc = &CMainWindow::windowProcedure;

	return ( ::RegisterClassEx( &classInfo ) != 0 );
}

bool CMainWindow::Create( LPCWSTR windowName, HINSTANCE ownerInstance, int width, int height )
{	
	DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	return( ::CreateWindowEx( 0, className, windowName, style, 0, 0, width, height, 0, 0, ownerInstance, this ) != 0 );
}

void CMainWindow::Show( int nCmdShow ) 
{
	::ShowWindow( windowHandle, nCmdShow );
}

// protected методы

void CMainWindow::OnWmDestroy()
{
	::PostQuitMessage( 0 );
}

void CMainWindow::OnWmCreate( HWND _windowHandle )
{
	windowHandle = _windowHandle;
	bool reg = CEditWindow::RegisterClass( ::GetModuleHandle( 0 ) );

	editWindow = new CEditWindow();

	editHandle = editWindow->Create( windowHandle, ::GetModuleHandle( 0 ) );
}

void CMainWindow::OnWmSize()
{
	RECT clientRect;
	::GetClientRect( windowHandle, &clientRect );

	::SetWindowPos( editHandle, 0, 0, 0, clientRect.right, clientRect.bottom, 0 );
}

void CMainWindow::OnWmChar( WPARAM code )
{
	editWindow->AddSign( code );
}

// private методы

// процедура обрабатывающая сообщения для главного окна
LRESULT __stdcall CMainWindow::windowProcedure( HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_NCCREATE ) {
		::SetWindowLong( windowHandle, GWL_USERDATA, reinterpret_cast<LONG>( reinterpret_cast<CREATESTRUCT*>( lParam )->lpCreateParams ) );
	}

	CMainWindow* window = reinterpret_cast<CMainWindow*>( ::GetWindowLong( windowHandle, GWL_USERDATA ) );	

	switch( message ) {
	case WM_CLOSE:
		window->OnWmDestroy();
		break;
	case WM_CREATE:
		window->OnWmCreate( windowHandle );
		break;
	case WM_SIZE:
		window->OnWmSize();
		break;
	case WM_CHAR:
		window->OnWmChar( wParam );
		break;
	}

	return ::DefWindowProc( windowHandle, message, wParam, lParam );
}