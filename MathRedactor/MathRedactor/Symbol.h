//Автор: Давлетшин Азат

#pragma once

#include <Windows.h>

//Абстрактный класс символа (простого и сложного). Умеет рисовать себя в Device Context
class CSymbol {
public:
	virtual ~CSymbol() {}

	//Создание копии (deep) объекта
	virtual CSymbol* Clone() const = 0;

	//Нарисовать символ в displayHandle, начиная с позиции (startX, startY) - левый верхний угол. Используется
	//текущие FONT и PEN. realHeight - реальная высота строки (включая сложные символы). simpleSymbolHeight - высота
	//простого символа (цифры, буквы).
	virtual void Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const = 0;
	//Рассчитать ширину символа, соответствующую высоте простого символа
	virtual int CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const = 0;
};