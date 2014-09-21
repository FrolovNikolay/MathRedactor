//Автор: Давлетшин Азат

#pragma once

#include <Windows.h>
#include <vector>
#include "Symbol.h"

//Класс представляет собой строку символов. Умеет отрисовывать себя в hdc.
class CLineOfSymbols {
public:
	CLineOfSymbols() { hasFraction = false; }
	CLineOfSymbols( const CLineOfSymbols& src );
	~CLineOfSymbols();

	//Нарисовать строку в Device Context, начиная с позиции (startX, startY) - левый верхний угол строки.
	//realHeight - реальная высота строки (она не вылезет за пределы этого значения)
	void Draw( HDC displayHandle, int posX, int posY, int realHeight ) const;
	//Рассчитать ширину строки, соответствующую реальной высоте (реальная высота != высота простого символа)
	int CalculateWidth( HDC displayHandle, int realHeight ) const;

	//Добавить символ в конец строки (память, переданная по указателю, будет освобождена в деструкторе класса CLineOfSymbols)
	void Push( CSymbol* symbol );

	//TODO: Определить эти методы
	const CSymbol* operator[] ( int index ) const;
	CSymbol* operator[] ( int index );

private:
	std::vector<CSymbol*> arrayOfSymbolPtrs;
	//Флаг наличия дроби (от этого зависит simpleSymbolHeight)
	bool hasFraction;

};