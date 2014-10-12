﻿//Автор: Давлетшин Азат

#pragma once

#include <Windows.h>
#include <vector>

class CLineOfSymbols;

//Абстрактный класс символа (простого и сложного). Умеет рисовать себя в Device Context
//Нельзя использовать, не добавив к строке
class CSymbol {
public:
	CSymbol() : x( -1 ), y( -1 ), width( -1 ), height( -1 ) {}
	virtual ~CSymbol( ) {}
	
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetHeight() const { return height; }
	virtual int GetHeight( int simpleSymbolHeight ) const = 0;
	int GetWidth() const { return width; }
	//Получить сдвиг базовой линии
	virtual int GetBaselineOffset( int simpleSymbolHeight ) const = 0;
	//Получить массив указателей на подстроки (может быть пустым). Освобождать память нельзя 
	virtual void GetSubstrings( std::vector<const CLineOfSymbols*>& substrings ) const = 0;
	virtual void GetSubstrings( std::vector<CLineOfSymbols*>& substrings ) = 0;

	//Создание копии (deep) объекта (паттерн Prototype)
	virtual CSymbol* Clone( ) const = 0;

	//Нарисовать символ в displayHandle, начиная с позиции (startX, startY) - левый верхний угол. Используется
	//текущие FONT и PEN
	virtual void Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const = 0;
	//Рассчитать ширину символа
	virtual int CalculateWidth( HDC displayHandle ) const = 0;
protected:
	//Левый верхний угол, ширина, высота (обновляется при перерисовке)
	//Служебная инфа, поэтому mutable (символ сам от этого не меняется)
	mutable int x;
	mutable int y;
	mutable int width;
	mutable int height;
};