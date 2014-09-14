#include "MainWindow.h"

// класс CMainWindow
// public методы 

CMainWindow::CMainWindow() 
{
	windowHandle = NULL;
}

CMainWindow::~CMainWindow()
{
}

bool CMainWindow::RegisterClass( HINSTANCE classOwnerInstance )
{
	const wchar_t* className = L"MathRedactorWindowClass";

	WNDCLASSEX classInfo;
	::ZeroMemory( &classInfo, sizeof( WNDCLASSEX ) );
	classInfo.cbSize = sizeof( WNDCLASSEX );
	classInfo.hInstance = classOwnerInstance;
	classInfo.lpszClassName = className;
	classInfo.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	classInfo.lpfnWndProc = &CMainWindow::windowProcedure;

	if( ::RegisterClassEx( &classInfo ) == 0 ) {
		return false;
	}

	return true;
}

bool CMainWindow::Create( LPCWSTR windowName, HINSTANCE ownerInstance )
{
	const wchar_t* className = L"MathRedactorWindowClass";

	DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL;
	windowHandle = ::CreateWindowEx( 0, className, windowName, style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, ownerInstance, this );

	if( windowHandle == NULL ) {
		return false;
	}

	return true;
}

void CMainWindow::Show( int nCmdShow ) 
{
	::ShowWindow( windowHandle, nCmdShow );
}

// protected методы

void CMainWindow::OnWmDestroy() {
	::PostQuitMessage( 0 );
}

// private методы

// процедура обабатывающая сообщения для главного окна
LRESULT __stdcall CMainWindow::windowProcedure( HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_NCCREATE ) {
		::SetWindowLong( windowHandle, GWL_USERDATA, reinterpret_cast<LONG>( reinterpret_cast<CREATESTRUCT*>( lParam )->lpCreateParams ) );
	}

	CMainWindow* window = reinterpret_cast<CMainWindow*>( ::GetWindowLong( windowHandle, GWL_USERDATA ) );	

	switch( message ) {
	case WM_CLOSE:
		window->OnWmDestroy();
	}

	return ::DefWindowProc( windowHandle, message, wParam, lParam );
}