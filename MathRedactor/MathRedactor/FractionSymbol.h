//Автор: Давлетшин Азат

#pragma once

#include "Symbol.h"
#include "LineOfSymbols.h"

//Сложный символ "дробь". Состоит из двух строк (LineOfSymbols).
class CFractionSymbol : public CSymbol {
public:

	CFractionSymbol() {}

	//Создание копии (deep) объекта. 
	virtual CSymbol* Clone() const { return new CFractionSymbol( *this ); }

	CLineOfSymbols& GetUpperLine() { return upperLine; }
	const CLineOfSymbols& GetUpperLine() const { return upperLine; }
	CLineOfSymbols& GetLowerLine( ) { return lowerLine; }
	const CLineOfSymbols& GetLowerLine( ) const { return lowerLine; }

	//Описание этих методов находится в Symbol.h
	virtual void Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const;
	virtual int CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const;

private:
	//Верхняя и нижняя части дроби
	CLineOfSymbols upperLine;
	CLineOfSymbols lowerLine;
};