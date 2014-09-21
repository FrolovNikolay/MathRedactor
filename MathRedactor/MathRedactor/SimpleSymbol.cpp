#include "SimpleSymbol.h"
#include <assert.h>
#include <wingdi.h>

void CSimpleSymbol::Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const
{
	//��������� �������� �� �������� ���� ������
	int offsetTop = (realHeight - simpleSymbolHeight) / 2;

	//��������� (������� ���� ������ �� ������ symbol)
	::TextOut( displayHandle, posX, posY + offsetTop, symbol, 1 );
}

int CSimpleSymbol::CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const
{
	SIZE fontSizeStruct;
	::GetTextExtentPoint32( displayHandle, symbol, 1, &fontSizeStruct );

	return fontSizeStruct.cx;
}