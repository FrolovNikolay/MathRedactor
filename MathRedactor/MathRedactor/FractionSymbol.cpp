#include "FractionSymbol.h"
#include <assert.h>
#include <wingdi.h>

void CFractionSymbol::Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const
{
	//Рисуем разделитель
	int separatorYOffset = posY + (realHeight / 2);
	::MoveToEx( displayHandle, posX, posY + separatorYOffset, NULL );
	int upperWidth = upperLine.CalculateWidth( displayHandle, simpleSymbolHeight );
	int lowerWidth = lowerLine.CalculateWidth( displayHandle, simpleSymbolHeight );
	int maxWidth = max( upperWidth, lowerWidth );
	::LineTo( displayHandle, posX + maxWidth, posY + separatorYOffset );

	//Рисуем верхнюю линию
	int upperXOffset = (maxWidth - upperWidth) / 2;
	upperLine.Draw( displayHandle, posX + upperXOffset, posY, simpleSymbolHeight );

	//Рисуем нижнюю линию
	int lowerXOffset = (maxWidth - lowerWidth) / 2;
	int lowerYOffset = simpleSymbolHeight + 3;
	lowerLine.Draw( displayHandle, posX + lowerXOffset, posY + lowerYOffset, simpleSymbolHeight );

}

int CFractionSymbol::CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const
{
	int upperWidth = upperLine.CalculateWidth( displayHandle, simpleSymbolHeight );
	int lowerWidth = lowerLine.CalculateWidth( displayHandle, simpleSymbolHeight );

	return max( upperWidth, lowerWidth );
}