#include "RootSymbol.h"
#include <assert.h>
#include <wingdi.h>
#include <Windows.h>

const wchar_t CRootSymbol::symbol[] = { L'\u221A' };

CRootSymbol::CRootSymbol( int simpleSymbolHeight ) :
	exponentLine( getExponentHeight( simpleSymbolHeight ) ),
	radicandLine( simpleSymbolHeight )
{

}

void CRootSymbol::Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const
{
	int rootHeight = getRootHeight( simpleSymbolHeight );
	//������ ����������
	int exponentY = posY - ( ( rootHeight - simpleSymbolHeight ) +
		( exponentLine.GetHeight() - getExponentVerticalOffset( rootHeight ) ) );
	exponentLine.Draw( displayHandle, posX, exponentY );
	int exponentWidth = exponentLine.CalculateWidth( displayHandle );

	//������ ���� �����
	//������������� ����� ��� �����
	HFONT oldFont = ( HFONT )::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof( LOGFONT ), &fontInfo );
	fontInfo.lfHeight = getRootHeight( simpleSymbolHeight );
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = ( HFONT )::SelectObject( displayHandle, font );

	SIZE fontSizeStruct;
	::GetTextExtentPoint32( displayHandle, symbol, 1, &fontSizeStruct );

	TEXTMETRIC textMetric;
	::GetTextMetrics( displayHandle, &textMetric );

	//������ ������� �����
	int rootWidth = fontSizeStruct.cx;

	//��������� ���������� ��� ����� � ������ �
	int rootX = posX + exponentWidth + getExponentOffset( rootWidth );
	rootX = max( posX, rootX );
	int rootY = posY - ( rootHeight - simpleSymbolHeight );
	::TextOut( displayHandle, rootX, rootY, symbol, 1 );

	//���������� ������ �����, ������� ���������
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	//������ ����������� ���������
	int radicandX = rootX + rootWidth;
	int radicandY = posY;
	radicandLine.Draw( displayHandle, radicandX, radicandY );
	int radicandWidth = radicandLine.CalculateWidth( displayHandle );

	//������ �����
	int strokeStartX = radicandX;
	int strokeStartY = rootY + textMetric.tmInternalLeading / 2;
	::MoveToEx( displayHandle, strokeStartX, strokeStartY, NULL );
	::LineTo( displayHandle, strokeStartX + radicandWidth, strokeStartY );

	//��������� ����������
	x = posX;
	y = exponentY;
	width = exponentWidth + rootWidth + getExponentOffset( rootWidth ) + radicandWidth;
	height = getRootHeight( simpleSymbolHeight ) + 
		( exponentLine.GetHeight( ) - getExponentVerticalOffset( rootHeight ) );

}

int CRootSymbol::CalculateWidth( HDC displayHandle ) const
{
	int exponentWidth = exponentLine.CalculateWidth( displayHandle );
	int radicandWidth = radicandLine.CalculateWidth( displayHandle );

	HFONT oldFont = ( HFONT )::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof( LOGFONT ), &fontInfo );
	fontInfo.lfHeight = getRootHeight( fontInfo.lfHeight );
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = ( HFONT )::SelectObject( displayHandle, font );

	SIZE fontSizeStruct;
	::GetTextExtentPoint32( displayHandle, symbol, 1, &fontSizeStruct );

	//������ ������� �����
	int rootWidth = fontSizeStruct.cx;

	//���������� ������ �����, ������� ���������
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	width = exponentWidth + rootWidth + getExponentOffset( rootWidth ) + radicandWidth;

	return width;
}

int CRootSymbol::GetHeight( int simpleSymbolHeight ) const
{
	int rootHeight = getRootHeight( simpleSymbolHeight );
	height = getRootHeight( simpleSymbolHeight ) +
		( exponentLine.GetHeight() - getExponentVerticalOffset( rootHeight ) );
	return height;
}

int CRootSymbol::GetBaselineOffset( int simpleSymbolHeight ) const
{
	int rootHeight = getRootHeight( simpleSymbolHeight );
	return ( exponentLine.GetHeight() - getExponentVerticalOffset( rootHeight ) ) +
		( rootHeight - simpleSymbolHeight );
}