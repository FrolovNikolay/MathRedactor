// Автор: Николай Фролов

#include "ItemSelector.h"

CItemSelector::CItemSelector( const std::vector<CLineOfSymbols>& _content ) :
	content( _content ),
	startX( -1 ),
	startY( -1 ),
	currentX( -1 ),
	currentY( -1 ),
	isLineMain( false )
{
}

CItemSelector::~CItemSelector()
{
}

bool CItemSelector::HasSelection() const
{
	if( currentX == -1 && currentY == -1 ) {
		return false;
	} else {
		return true;
	}
}

void CItemSelector::ResetSelection()
{
	startX = -1;
	startY = -1;
	currentX = -1;
	currentY = -1;
	isLineMain = false;
}

void CItemSelector::SetStartPosition( int x, int y )
{
	startX = x;
	startY = y;
	isLineMain = true;
}

void CItemSelector::SetCurrentPosition( int x, int y )
{
	currentX = x;
	currentY = y;
}

void CItemSelector::MakeSelection( HDC displayHandle, int width, int height ) const
{

	// делим на два случая: выделение внутри одной из основных строк, либо мы выделяем внутри сложного символа
	if( isLineMain ) {
		// Определяем положение символов во внутренней структуре
		int startLine = 0;
		int firstMarkedSymbol = 0;
		int endLine = 0;
		int lastMarkedSymbol = 0;
		getItemInfo( displayHandle, startX, startY, startLine, firstMarkedSymbol );
		getItemInfo( displayHandle, currentX, currentY, endLine, lastMarkedSymbol );

		// определяем начало и конец в нашем выделении
		if( startLine > endLine ) {
			std::swap( startLine, endLine );
			std::swap( firstMarkedSymbol, lastMarkedSymbol );
		} else if( startLine == endLine && firstMarkedSymbol > lastMarkedSymbol ) {
			std::swap( startLine, endLine );
			std::swap( firstMarkedSymbol, lastMarkedSymbol );
		}

		// Под выделение ничего не попало
		if( startLine == content.size() ) {
			return;
		}

		// координаты левого верхнего угла первого символа
		int leftTopX = 0;
		int leftTopY = 0;
		int firstLineHeight = content[startLine].GetHeight();
		for( int i = 0; i < startLine; ++i ) {
			leftTopY += content[i].GetHeight();
		}
		for( int i = 0; i < firstMarkedSymbol; ++i ) {
			leftTopX += content[startLine][i]->GetWidth();
		}

		// если конец выделения ушел за конец текста, то выделяем по последний символ включительно
		if( endLine == content.size() ) {
			endLine = content.size() - 1;
			lastMarkedSymbol = content[endLine].Length() - 1;
		}

		// координаты правого нижнего угла последнего символа
		int rightBotX = 0;
		int rightBotY = 0;
		int lastLineHeight = content[endLine].GetHeight();
		for( int i = 0; i <= endLine; ++i) {
			rightBotY += content[i].GetHeight();
		}
		for( int i = 0; i <= lastMarkedSymbol; ++i ) {
			rightBotX += content[endLine][i]->GetWidth();
		}

		drawSelection( displayHandle, width, height, leftTopX, leftTopY, firstLineHeight, rightBotX,
			rightBotY, lastLineHeight );
	} else {
		// TODO: случай выделения во внутренней строке(например в сумме)
	}
}

// находим положение символа, соответствующего данным координатам, во внутренней структуре
void CItemSelector::getItemInfo( HDC displayHandle, int x, int y, int& lineIdx, int& symbolIdx ) const
{
	int currentY = 0;
	for( lineIdx = 0; lineIdx < content.size(); ++lineIdx ) {
		currentY += content[lineIdx].GetHeight();
		if( currentY > y ) {
			break;
		}
	}
	if( content.size() == lineIdx ) {
		return;
	}
	int currentX = 0;
	for( symbolIdx = 0; symbolIdx < content[lineIdx].Length(); ++symbolIdx ) {
		currentX += content[lineIdx][symbolIdx]->GetWidth();
		if( currentX > x ) {
			break;
		} 
	}
	if( symbolIdx == content[lineIdx].Length() ) {
		--symbolIdx;
	}
}

// прорисовка выделения для имеющихся в классе start/current позиций
void CItemSelector::drawSelection( HDC displayHandle, int width, int height, int leftTopX, int leftTopY,
		int firstLineHeight, int rightBotX, int rightBotY, int lastLineHeight ) const
{
	RECT windowInfo;
	HBRUSH selectionBrush = ::CreateSolidBrush( RGB( 100, 100, 255 ) );
	HBRUSH lastBrush = static_cast<HBRUSH>( ::SelectObject( displayHandle, selectionBrush ) );
	HPEN newPen = CreatePen( PS_SOLID, 1, RGB( 100, 100, 255 ) );
	HPEN oldPen = static_cast<HPEN>( ::SelectObject( displayHandle, newPen ) );

	// вся прорисовка включает в себя три прямоугольника, зависящих от координат первого и последнего символа в выделении
	::Rectangle( displayHandle, leftTopX, leftTopY, rightBotX, rightBotY );
	::Rectangle( displayHandle, rightBotX, leftTopY, width, rightBotY - lastLineHeight );
	::Rectangle( displayHandle, 0, leftTopY + firstLineHeight, leftTopX, rightBotY );

	::SelectObject( displayHandle, lastBrush );
	::SelectObject( displayHandle, oldPen );
	::DeleteObject( newPen );
	::DeleteObject( selectionBrush );
}