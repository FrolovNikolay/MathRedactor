//Автор: Давлетшин Азат
//Описание: Здесь представлен класс строки, который хранит в себе массив символов.

#pragma once

#include <Windows.h>
#include <vector>
#include "Symbol.h"

//Класс представляет собой строку символов. Умеет отрисовывать себя в hdc.
class CLineOfSymbols {
public:
	CLineOfSymbols( int _simpleSymbolHeight );
	CLineOfSymbols( const CLineOfSymbols& src );
	~CLineOfSymbols( );

	int GetHeight( ) const { return height; }
	int GetSimpleSymbolHeight( ) const { return simpleSymbolHeight; }

	//Нарисовать строку в Device Context, начиная с позиции (startX, startY) - левый верхний угол строки.
	void Draw( HDC displayHandle, int posX, int posY ) const;
	//Рассчитать ширину строки (линейно по количеству символов в строке, включая символы в сложных символах)
	int CalculateWidth( HDC displayHandle ) const;

	//Добавить символ в конец строки (память, переданная по указателю, будет освобождена в деструкторе класса CLineOfSymbols)
	void Push( CSymbol* symbol );
	void Pop() { delete arrayOfSymbolPtrs.back(); arrayOfSymbolPtrs.pop_back(); }
	int Length( ) const { return static_cast<int>(arrayOfSymbolPtrs.size( )); }

	const CSymbol* operator[] ( int index ) const { return arrayOfSymbolPtrs[index]; }
	CSymbol* operator[] ( int index ) { return arrayOfSymbolPtrs[index]; }

private:
	std::vector<CSymbol*> arrayOfSymbolPtrs;
	//Высота строки (может меняться в процессе ввода)
	int height;
	//Сдвиг базовой линии относительно верха строки. Служит для позиционирования символов (верхняя точка простых символов).
	int baselineOffset;
	//Размер (высота) базового символа. Меняется только специальным методом.
	int simpleSymbolHeight;
};