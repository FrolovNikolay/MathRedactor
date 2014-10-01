#include "EditWindow.h"
#include "SimpleSymbol.h"
#include "FractionSymbol.h"
#include <assert.h>

// класс CEditWindow
// константы

const wchar_t* CEditWindow::className = L"MathRedactorEditWindowClass";

// public методы

CEditWindow::CEditWindow() : horizontalScrollUnit( 30 ), verticalScrollUnit( 15 ), simpleSymbolHeight( 50 ), caret( this )
{
	windowHandle = 0;

	allowedSymbols = L" +-*/=~%^?><";
	
	content.push_back( CLineOfSymbols( simpleSymbolHeight ) );
	caret.MoveTo( &content[0], 0 );
}

CEditWindow::~CEditWindow()
{
}

bool CEditWindow::RegisterClass( HINSTANCE classOwnerInstance )
{
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
	DWORD style = WS_CHILD | WS_VISIBLE;

	windowHandle = ::CreateWindowEx( 0, className, 0, style, 0, 0, 0, 0, parent, 0, ownerInstance, this );

	return windowHandle;
}

void CEditWindow::Show( int nCmdShow )
{
	::ShowWindow( windowHandle, nCmdShow );
}

void CEditWindow::AddSymbol( CSymbol* symbol )
{
	content.back().Push( symbol );
	caret.Move( DRight );
	::RedrawWindow( windowHandle, 0, 0, RDW_INVALIDATE | RDW_ERASE );
}

void CEditWindow::AddSign( wchar_t sign )
{
	if( ( sign >= L'a' && sign <= L'z' ) || ( sign >= L'A' && sign <= L'Z' ) || ( sign >= L'0' && sign <= L'9' ) ||
		allowedSymbols.find( sign ) != std::wstring::npos )
	{
		AddSymbol( new CSimpleSymbol( sign ) );
	}
}

void CEditWindow::RemoveSign()
{
	if( content.size() == 0 ) {
		return;
	}
	if( content.back().Length() == 0 ) {
		if( content.size() != 1 ) {
			content.pop_back();
			caret.MoveTo( &content.back(), content.back().Length() );
		}
	} else {
		content.back().Pop();
		caret.Move( DLeft );
	}
	::RedrawWindow( windowHandle, 0, 0, RDW_INVALIDATE );
}

void CEditWindow::NewLine()
{
	content.push_back( CLineOfSymbols( simpleSymbolHeight ) );
	caret.MoveTo( &content.back(), 0 );
	::RedrawWindow( windowHandle, 0, 0, RDW_INVALIDATE );
}

void CEditWindow::ShowCaret()
{
	caret.Create();
	caret.Show();
}

void CEditWindow::HideCaret()
{
	caret.Hide();
	caret.Destroy();
}

void CEditWindow::MoveCaret( CEditWindow::TDirection direction )
{
	caret.Move( direction );
}

void CEditWindow::MoveCaretTo( int x, int y )
{
	// TODO
}

// protected методы

void CEditWindow::OnWmDestroy() {
	::PostQuitMessage( 0 );
}

//TODO: Сделать нормальный метод
void CEditWindow::OnWmPaint( )
{
	// мы запоминаем состояние каретки до перерисовки
	bool caretShown = caret.IsShown();
	if( caretShown ) {
		caret.Hide();
	}
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
	HFONT font = ::CreateFont( simpleSymbolHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial" );
	assert( font != 0 );

	HFONT oldFont = (HFONT) ::SelectObject( displayHandle, font );

	//Настройка кисти для линии (рисование дроби)
	HPEN linePen = ::CreatePen( PS_SOLID, 1, RGB( 0, 0, 0 ) );
	HPEN oldLinePen = (HPEN) ::SelectObject( displayHandle, linePen );

	//Выравнивание для TextOut (левый верхний угол)
	::SetTextAlign( displayHandle, TA_LEFT | TA_TOP );


	for( int i = 0; i < content.size(); ++i ) {
		content[i].Draw( displayHandle, posX, posY );
		posY += content[i].GetHeight();
	}

	::SelectObject( displayHandle, oldLinePen );
	::DeleteObject( linePen );

	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	::EndPaint( windowHandle, &paintInfo );
	// если каретка показывалась до перерисовки, то отображаем ее
	if( caretShown ) {
		caret.Show();
	}
}

void CEditWindow::OnWmHScroll( WPARAM wParam, LPARAM lParam )
{
	if( lParam == 0 ) {
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof( SCROLLINFO );
		scrollInfo.fMask = SIF_ALL;
		// меняем позицию скролла
		::GetScrollInfo( windowHandle, SB_HORZ, &scrollInfo );
		switch( LOWORD( wParam ) ) {
		case SB_LINELEFT:
			scrollInfo.nPos += 1;
			break;
		case SB_LINERIGHT:
			scrollInfo.nPos -= 1;
			break;
		case SB_PAGELEFT:
			scrollInfo.nPos += scrollInfo.nPage;
			break;
		case SB_PAGERIGHT:
			scrollInfo.nPos -= scrollInfo.nPage;
			break;
		case SB_THUMBTRACK:
			scrollInfo.nPos = scrollInfo.nTrackPos;
			break;
		}
		// запоминаем и устанавливаем новую позицию скролла
		int scrollPosition = scrollInfo.nPos;
		scrollInfo.fMask = SIF_POS;
		::SetScrollInfo( windowHandle, SB_HORZ, &scrollInfo, TRUE );
		// и перестраховка, на случай того, что Window сдвинет что-то не так, как ожидалось
		::GetScrollInfo( windowHandle, SB_HORZ, &scrollInfo );
		if( scrollInfo.nPos != scrollPosition ) {
			::ScrollWindow( windowHandle, horizontalScrollUnit * ( scrollPosition - scrollInfo.nPos ), 0, 0, 0 );
		}
	}
}

void CEditWindow::OnWmVScroll( WPARAM wParam, LPARAM lParam )
{
	if( lParam == 0 ) {
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof( SCROLLINFO );
		scrollInfo.fMask = SIF_ALL;
		// меняем позицию скролла
		::GetScrollInfo( windowHandle, SB_VERT, &scrollInfo );
		switch( LOWORD( wParam ) ) {
		case SB_LINEUP:
			scrollInfo.nPos += 1;
			break;
		case SB_LINEDOWN:
			scrollInfo.nPos -= 1;
			break;
		case SB_PAGEUP:
			scrollInfo.nPos += scrollInfo.nPage;
			break;
		case SB_PAGEDOWN:
			scrollInfo.nPos -= scrollInfo.nPage;
			break;
		case SB_THUMBTRACK:
			scrollInfo.nPos = scrollInfo.nTrackPos;
			break;
		}
		// запоминаем и устанавливаем новую позицию скролла
		int scrollPosition = scrollInfo.nPos;
		scrollInfo.fMask = SIF_POS;
		::SetScrollInfo( windowHandle, SB_VERT, &scrollInfo, TRUE );
		// и перестраховка, на случай того, что Window сдвинет что-то не так, как ожидалось
		::GetScrollInfo( windowHandle, SB_VERT, &scrollInfo );
		if( scrollInfo.nPos != scrollPosition ) {
			::ScrollWindow( windowHandle, 0, verticalScrollUnit * ( scrollPosition - scrollInfo.nPos ), 0, 0 );
		}
	}
}

void CEditWindow::OnWmSize( LPARAM lParam ) 
{
	int width = LOWORD( lParam );
	int hieght = HIWORD( lParam );

	RECT editClientRect, parentClientRect;
	::GetClientRect( windowHandle, &editClientRect );
	::GetClientRect( ::GetParent( windowHandle ), &parentClientRect );

	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof( SCROLLINFO );
	scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
	scrollInfo.nMin = 0;

	// горизонтальный скролл
	scrollInfo.nMax = editClientRect.right / horizontalScrollUnit;
	scrollInfo.nPage = editClientRect.right / horizontalScrollUnit;
	::SetScrollInfo( windowHandle, SB_HORZ, &scrollInfo, TRUE );

	// вертиклаьный скролл
	scrollInfo.nMax = editClientRect.bottom / verticalScrollUnit;
	scrollInfo.nPage = editClientRect.bottom / verticalScrollUnit;
	::SetScrollInfo( windowHandle, SB_VERT, &scrollInfo, TRUE );
}

// private методы

// процедура обрабатывающая сообщения для окна редактора
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
		window->OnWmPaint();
		break;
	case WM_HSCROLL:
		window->OnWmHScroll( wParam, lParam );
		break;
	case WM_VSCROLL:
		window->OnWmVScroll( wParam, lParam );
		break;
	case WM_SIZE:
		window->OnWmSize( lParam );
		break;
	}

	return ::DefWindowProc( windowHandle, message, wParam, lParam );
}

// класс CCaret

CEditWindow::CCaret::CCaret( CEditWindow* _window )
{
	window = _window;
	line = 0;
	index = 0;
	shown = false;
	height = window->simpleSymbolHeight;
	width = 2;
}

void CEditWindow::CCaret::Create()
{
	::CreateCaret( window->windowHandle, 0, width, height );
	::SetCaretBlinkTime( 300 );
}

void CEditWindow::CCaret::Show()
{
	moveToNewCoordinates();
	if( !shown ) {
		shown = ::ShowCaret( window->windowHandle );
	}
}

void CEditWindow::CCaret::Hide()
{
	if( shown ) {
		::HideCaret( window->windowHandle );
		shown = false;
	}
}

void CEditWindow::CCaret::Destroy()
{
	shown = false;
	::DestroyCaret();
}

void CEditWindow::CCaret::Move( CEditWindow::TDirection direction )
{
	switch( direction ) {
	case DUp:
		moveUp();
		break;
	case DDown:
		moveDown();
		break;
	case DLeft:
		moveLeft();
		break;
	case DRight:
		moveRight();
		break;
	}
}

void CEditWindow::CCaret::MoveTo( CLineOfSymbols* _line, int _index ) 
{
	line = _line;
	index = _index;
}

bool CEditWindow::CCaret::IsShown() const
{
	return shown;
}

// этот код вынесен в отдельные методы
// для того чтобы функция CEditWindow::CCaret::Move не получилась слишком большой

// сдвигает каретку на единицу вверх
void CEditWindow::CCaret::moveUp()
{

}

// сдвигает каретку на единицу вниз
void CEditWindow::CCaret::moveDown()
{

}

// сдвигает каретку на единицу влево
void CEditWindow::CCaret::moveLeft()
{
	if( index > 0 ) {
		--index;
		moveToNewCoordinates();
	}
}

// сдвигает каретку на единицу в право
void CEditWindow::CCaret::moveRight()
{
	if( line->Length() > index ) {
		++index;
		moveToNewCoordinates();
	}
}

void CEditWindow::CCaret::moveToNewCoordinates()
{
	int x, y;
	int newHeight = height;
	if( line->Length() == 0 ) {
		x = line->GetX();
		y = line->GetY();
		newHeight = line->GetHeight();
	} else if( index == 0 ) {
		newHeight = ( *line )[index]->GetHeight();
		x = max( 0, ( *line )[index]->GetX() );
		y = max( 0, ( *line )[index]->GetY() );
	} else {
		newHeight = ( *line )[index - 1]->GetHeight();
		x = max( 0, ( *line )[index - 1]->GetX() + ( *line )[index - 1]->GetWidth() );
		y = max( 0, ( *line )[index - 1]->GetY() );
	}
	if( newHeight > 0 && newHeight != height ) {
		changeHeight( newHeight );
	}
	::SetCaretPos( x, y );
}

void CEditWindow::CCaret::changeHeight( int newHeight )
{
	bool wasShown = shown;
	height = newHeight;
	Destroy();
	Create();
	if( wasShown ) {
		Show();
	}
}