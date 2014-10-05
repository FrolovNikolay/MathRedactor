// Автор: Фролов Николай

// Описание: класс-механизм, отвечающий за выделение частей текста.

#pragma once

#include <vector>
#include "LineOfSymbols.h"

class CItemSelector {
public:
	CItemSelector( const std::vector<CLineOfSymbols>& _content );
	~CItemSelector();

	// Установка начальной позиции курсора
	void SetStartPosition( int x, int y );
	// Установка текущей позиции курсора
	void SetCurrentPosition( int x, int y );
	// Проверить наличие выделения
	bool HasSelection() const;
	// Выделить символы, в соответствии с заданными в классе координатами
	void MakeSelection( HDC displayHandle, int width, int height ) const;
	// Отменить выделение
	void ResetSelection();

private:
	// содержимое окна
	const std::vector<CLineOfSymbols>& content;
	// координаты начала выделения
	int startX, startY;
	// координаты конца выделения
	int currentX, currentY;
	// попадает ли старт выделения в одну из основных линий
	bool isLineMain;
	// если линия не одна из основных, то сюда попадет подлиния
	const CLineOfSymbols* baseLine;

	CItemSelector();

	void getItemInfo( HDC displayHandle, int x, int y, int& line, int& symbolIdx ) const;

	void drawSelection( HDC displayHandle, int width, int height, int leftTopX, int leftTopY, int firstLineHeight,
		int rightBotX, int RightBotY, int lastLineHeight ) const;
};