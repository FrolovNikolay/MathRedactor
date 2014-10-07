﻿// Автор: Федюнин Валерий
// Описание: классы для написания верхнего и нижнего

#pragma once

#include "Symbol.h"
#include "LineOfSymbols.h"

// символ, отвечающий за индекс (верхний или нижний)
class CIndexSymbol : public CSymbol {
public:
	// Типы индекса: верхний, нижний
	enum TIndexType {
		ITUpper, ITLower
	};

	CIndexSymbol( int simpleSymbolHeight, TIndexType type );

	// Создание независимой копии символа
	virtual CSymbol* Clone() const;

	// Высота символа
	virtual int GetHeight( int simpleSymbolHeight ) const;
	// отступ символа от базовой линии
	virtual int GetBaselineOffset( int simpleSymbolHeight ) const;

	CLineOfSymbols& GetLine();
	const CLineOfSymbols& GetLine() const;

	// Отрисовка символа в данной контексте устройства и координатах
	virtual void Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const;
	// Подсчет ширины символа в данном контексте устройства
	virtual int CalculateWidth( HDC displayHandle ) const;

private:
	// то, что записано в индексе
	CLineOfSymbols line;
	// тип индекса
	TIndexType type;
	
	// высота текста, записанного в показателе степени
	static int calculateSublineHeight( int simpleSymbolHeight );

	// соотношение высоты символа к высоте базовоого символа
	static const float heightRatio;
	// соотношение высоты текста индекса к высоте базового символа
	static const float sublineHeightRatio;
};