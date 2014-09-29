#include "SimpleSymbol.h"
#include <assert.h>
#include <wingdi.h>

void CSimpleSymbol::Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const
{
	//Отрисовка (выводим один символ из строки symbol)
	::TextOut( displayHandle, posX, posY, symbol, 1 );
}

int CSimpleSymbol::CalculateWidth( HDC displayHandle ) const
{
	SIZE fontSizeStruct;
	::GetTextExtentPoint32( displayHandle, symbol, 1, &fontSizeStruct );
	return fontSizeStruct.cx + SideOffset;
}

int CSimpleSymbol::GetHeight( int simpleSymbolHeight ) const
{
	return simpleSymbolHeight;
}