//Автор: Давлетшин Азат

#pragma once

#include "Symbol.h"

//Простой символ (буква, цифра или простой оператор). Нет конструктора по-умолчанию.
class CSimpleSymbol : public CSymbol {
public:
	CSimpleSymbol( wchar_t _symbol ) { symbol[0] = _symbol; }

	//Создание копии (deep) объекта
	virtual CSymbol* Clone() const { return new CSimpleSymbol( *this ); }

	wchar_t GetSymbol() const { return symbol[0]; }
	void SetSymbol( wchar_t _symbol ) { symbol[0] = _symbol;  }

	//Описание этих методов находится в Symbol.h
	virtual void Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const;
	virtual int CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const;

private:
	//Имя символа (передается в TextOut).
	wchar_t symbol[1];
};