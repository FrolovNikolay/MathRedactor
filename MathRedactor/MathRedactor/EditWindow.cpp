#include "EditWindow.h"
#include "SimpleSymbol.h"
#include "FractionSymbol.h"
#include <assert.h>

// класс CEditWindow
// public методы

CEditWindow::CEditWindow()
{
	windowHandle = 0;
	lineHeight = 200;

	//!!!Это извращение, но это для теста!!!
	CLineOfSymbols line;
	line.Push( new CSimpleSymbol( L'A' ) );
	line.Push( new CSimpleSymbol( L'+' ) );
	line.Push( new CSimpleSymbol( L'5' ) );
	line.Push( new CSimpleSymbol( L'-' ) );
	CFractionSymbol* fsp = new CFractionSymbol( );
	fsp->GetUpperLine( ).Push( new CSimpleSymbol( L'3' ) );
	fsp->GetUpperLine( ).Push( new CSimpleSymbol( L'+' ) );
	fsp->GetUpperLine( ).Push( new CSimpleSymbol( L'4' ) );
	fsp->GetLowerLine( ).Push( new CSimpleSymbol( L'5' ) );
	CFractionSymbol* fsp2 = new CFractionSymbol( );
	fsp2->GetUpperLine().Push( fsp );
	fsp2->GetLowerLine().Push( new CSimpleSymbol( L'1' ) );
	CFractionSymbol* fsp3 = new CFractionSymbol();
	fsp3->GetUpperLine().Push( fsp->Clone() );
	fsp3->GetLowerLine().Push( fsp2->Clone() );
	line.Push( fsp3 );
	content.push_back( line );
}

CEditWindow::~CEditWindow()
{
}

bool CEditWindow::RegisterClass( HINSTANCE classOwnerInstance )
{
	const wchar_t* className = L"MathRedactorEditWindowClass";

	WNDCLASSEX classInfo;
	::ZeroMemory( &classInfo, sizeof( WNDCLASSEX ) );
	classInfo.cbSize = sizeof( WNDCLASSEX );
	classInfo.hInstance = classOwnerInstance;
	classInfo.lpszClassName = className;
	classInfo.style = CS_HREDRAW | CS_VREDRAW ;
	classInfo.lpfnWndProc = &CEditWindow::windowProcedure;

	return ( ::RegisterClassEx( &classInfo ) != 0 );
}

HWND CEditWindow::Create( HWND parent, HINSTANCE ownerInstance )
{
	const wchar_t* className = L"MathRedactorEditWindowClass";

	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER;

	windowHandle = ::CreateWindowEx( 0, className, 0, style, 0, 0, 0, 0, parent, 0, ownerInstance, this );

	return windowHandle;
}

void CEditWindow::Show( int nCmdShow )
{
	::ShowWindow( windowHandle, nCmdShow );
}

// protected методы

void CEditWindow::OnWmDestroy() {
	::PostQuitMessage( 0 );
}

//TODO: Сделать нормальный метод
void CEditWindow::OnWmPaint( )
{
	PAINTSTRUCT paintInfo;
	HDC displayHandle = ::BeginPaint( windowHandle, &paintInfo );
	assert( displayHandle != 0 );

	int posX = 0;
	int posY = 0;

	//Получаем размеры клиентского окна
	RECT clientRect;
	::GetClientRect( windowHandle, &clientRect );

	//Фон
	HBRUSH bgBrush = ::CreateSolidBrush( RGB( 255, 255, 255 ) );
	HBRUSH oldBgBrush = (HBRUSH) ::SelectObject( displayHandle, bgBrush );
	::Rectangle( displayHandle, 0, 0, clientRect.right, clientRect.bottom );
	::SelectObject( displayHandle, oldBgBrush );
	::DeleteObject( bgBrush );

	//!!!Сделать настройку шрифта отдельно
	HFONT font = ::CreateFont( lineHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial" );
	assert( font != 0 );

	HFONT oldFont = (HFONT) ::SelectObject( displayHandle, font );

	//Настройка кисти для линии (рисование дроби)
	HPEN linePen = ::CreatePen( PS_SOLID, 1, RGB( 0, 0, 0 ) );
	HPEN oldLinePen = (HPEN) ::SelectObject( displayHandle, linePen );



	for( int i = 0; i < content.size( ); ++i ) {
		content[i].Draw( displayHandle, posX, posY, lineHeight );
		posY += lineHeight;
	}

	::SelectObject( displayHandle, oldLinePen );
	::DeleteObject( linePen );

	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	::EndPaint( windowHandle, &paintInfo );
}

// private методы

// процедура обрабатывающая сообщения для главного окна
LRESULT __stdcall CEditWindow::windowProcedure( HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_NCCREATE ) {
		::SetWindowLong( windowHandle, GWL_USERDATA, reinterpret_cast<LONG>( reinterpret_cast<CREATESTRUCT*>( lParam )->lpCreateParams ) );
	}

	CEditWindow* window = reinterpret_cast<CEditWindow*>( ::GetWindowLong( windowHandle, GWL_USERDATA ) );

	switch( message ) {
	case WM_CLOSE:
		window->OnWmDestroy();
		break;
	case WM_PAINT:
		window->OnWmPaint( );
		break;
	}

	return ::DefWindowProc( windowHandle, message, wParam, lParam );
}