//Автор: Давлетшин Азат

#pragma once

#include <Windows.h>

class CLineOfSymbols;

//Абстрактный класс символа (простого и сложного). Умеет рисовать себя в Device Context
//Нельзя использовать, не добавив к строке
class CSymbol {
public:
	virtual ~CSymbol( ) {}

	//Получить высоту символа
	virtual int GetHeight( int simpleSymbolHeight ) const = 0;
	//Получить сдвиг базовой линии
	virtual int GetBaselineOffset( int simpleSymbolHeight ) const = 0;

	//Создание копии (deep) объекта (паттерн Prototype)
	virtual CSymbol* Clone( ) const = 0;

	//Нарисовать символ в displayHandle, начиная с позиции (startX, startY) - левый верхний угол. Используется
	//текущие FONT и PEN
	virtual void Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const = 0;
	//Рассчитать ширину символа
	virtual int CalculateWidth( HDC displayHandle ) const = 0;
};