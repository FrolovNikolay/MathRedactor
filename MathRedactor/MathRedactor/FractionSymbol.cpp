#include "FractionSymbol.h"
#include <assert.h>
#include <wingdi.h>
#include <Windows.h>

CFractionSymbol::CFractionSymbol( int simpleSymbolHeight ) :
upperLine( simpleSymbolHeight ),
lowerLine( simpleSymbolHeight )
{

}

void CFractionSymbol::Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const
{
	//Рисуем разделитель
	int strokeStartY = posY + simpleSymbolHeight / 2;
	::MoveToEx( displayHandle, posX, strokeStartY, NULL );
	int upperWidth = upperLine.CalculateWidth( displayHandle );
	int lowerWidth = lowerLine.CalculateWidth( displayHandle );
	int maxWidth = max( upperWidth, lowerWidth );
	int strokeWidth = maxWidth + 2 * getStrokeAddition( );
	::LineTo( displayHandle, posX + strokeWidth, strokeStartY );

	posX += getStrokeAddition( );

	//Рисуем верхнюю линию
	int upperStartX = posX + (maxWidth - upperWidth) / 2;
	int upperStartY = strokeStartY - getUpperOffset( ) - upperLine.GetHeight( );
	upperLine.Draw( displayHandle, upperStartX, upperStartY );

	//Рисуем нижнюю линию
	int lowerStartX = posX + (maxWidth - lowerWidth) / 2;
	int lowerStartY = strokeStartY + getStrokeHeight( ) + getLowerOffset( );
	lowerLine.Draw( displayHandle, lowerStartX, lowerStartY );

}

int CFractionSymbol::CalculateWidth( HDC displayHandle ) const
{
	int upperWidth = upperLine.CalculateWidth( displayHandle );
	int lowerWidth = lowerLine.CalculateWidth( displayHandle );

	return max( upperWidth, lowerWidth ) + 2 * getStrokeAddition( );
}

int CFractionSymbol::GetHeight( int simpleSymbolHeight ) const
{
	return upperLine.GetHeight( ) + lowerLine.GetHeight( ) + getUpperOffset( ) +
		getLowerOffset( ) + getStrokeHeight( );
}

int CFractionSymbol::GetBaselineOffset( int simpleSymbolHeight ) const
{
	return upperLine.GetHeight( ) + getUpperOffset( ) - simpleSymbolHeight / 2;
}