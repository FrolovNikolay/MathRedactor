#include "SigmaSymbol.h"
#include <assert.h>
#include <wingdi.h>
#include <Windows.h>

const wchar_t CSigmaSymbol::symbol[] = { L'\u2211', L'\0' };

CSigmaSymbol::CSigmaSymbol( int simpleSymbolHeight ) :
upperLine( calculateSublineHeight( simpleSymbolHeight ) ),
lowerLine( calculateSublineHeight( simpleSymbolHeight ) )
{

}

void CSigmaSymbol::GetSubstrings( std::vector<const CLineOfSymbols*>& substrings ) const
{
	substrings.push_back( &upperLine );
	substrings.push_back( &lowerLine );
}

void CSigmaSymbol::GetSubstrings( std::vector<CLineOfSymbols*>& substrings )
{
	substrings.push_back( &upperLine );
	substrings.push_back( &lowerLine );
}

void CSigmaSymbol::Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const
{
	int upperWidth = upperLine.CalculateWidth( displayHandle );
	int lowerWidth = lowerLine.CalculateWidth( displayHandle );
	width = max( upperWidth, lowerWidth );
	
	//������������� ����� ��� �����
	HFONT oldFont = ( HFONT )::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof( LOGFONT ), &fontInfo );
	fontInfo.lfHeight = getSigmaHeight( simpleSymbolHeight );
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = ( HFONT )::SelectObject( displayHandle, font );

	SIZE fontSizeStruct;
	::GetTextExtentPoint32( displayHandle, symbol, 1, &fontSizeStruct );

	//������ ������� �����
	int sigmaWidth = fontSizeStruct.cx;
	width = max( width, sigmaWidth );

	//��������� ���������� ��� ����� � ������ �
	int sigmaX = ( width - sigmaWidth ) / 2 + posX;
	int sigmaY = posY - ( getSigmaHeight( simpleSymbolHeight ) - simpleSymbolHeight ) / 2;
	sigmaY -= getSublinesOffset();
	::TextOut( displayHandle, sigmaX, sigmaY, symbol, 1 );

	//���������� ������ �����, ������� ���������
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	//������ ������� ������
	int upperX = posX + ( width - upperWidth ) / 2;
	int upperY = sigmaY - upperLine.GetHeight();
	upperLine.Draw( displayHandle, upperX, upperY );

	//������ ������ ������
	int lowerX = posX + ( width - lowerWidth ) / 2;
	int lowerY = sigmaY + getSigmaHeight( simpleSymbolHeight ) + getSublinesOffset();
	lowerLine.Draw( displayHandle, lowerX, lowerY );

	x = posX;
	y = upperY;
	height = upperLine.GetHeight() + getSublinesOffset() + getSigmaHeight( simpleSymbolHeight ) +
		getSublinesOffset() + lowerLine.GetHeight();

}

int CSigmaSymbol::CalculateWidth( HDC displayHandle ) const
{
	int upperWidth = upperLine.CalculateWidth( displayHandle );
	int lowerWidth = lowerLine.CalculateWidth( displayHandle );

	width = max( upperWidth, lowerWidth );

	//������������� ����� ��� �����
	HFONT oldFont = ( HFONT )::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof( LOGFONT ), &fontInfo );
	fontInfo.lfHeight = getSigmaHeight( fontInfo.lfHeight );
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = ( HFONT )::SelectObject( displayHandle, font );

	SIZE fontSizeStruct;
	::GetTextExtentPoint32( displayHandle, symbol, 1, &fontSizeStruct );

	//������ ������� �����
	int sigmaWidth = fontSizeStruct.cx;
	width = max( width, sigmaWidth );

	//���������� ������ �����, ������� ���������
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	return width;
}

int CSigmaSymbol::GetHeight( int simpleSymbolHeight ) const
{
	height = upperLine.GetHeight( ) + getSublinesOffset( ) + getSigmaHeight( simpleSymbolHeight ) +
		getSublinesOffset( ) + lowerLine.GetHeight( );
	return height;
}

int CSigmaSymbol::GetBaselineOffset( int simpleSymbolHeight ) const
{
	return upperLine.GetHeight() + getSublinesOffset() +
		( getSigmaHeight( simpleSymbolHeight ) - simpleSymbolHeight ) / 2;
}