// Автор: Федюнин Валерий
// Описание: класс окна, в котором отрисовываются и изменяются формулы

#pragma once

#include <Windows.h>
#include <vector>
#include "LineOfSymbols.h"

class CEditWindow {
public:
	CEditWindow();
	~CEditWindow();

	//Временно
	int GetSimpleSymbolHeight() const { return simpleSymbolHeight; }

	// регистрирует класс окна
	static bool RegisterClass( HINSTANCE );

	// Создает окно
	HWND Create( HWND, HINSTANCE );
	// Отображается окно
	void Show( int );
	
	// Добавляет символ иуведомляет окно о том, что надо перерисоваться
	void AddSymbol( CSymbol* );
	// Добавляет знак с клавиатуры
	void AddSign( wchar_t );
	// Удаляет символ (например, при нажатии Backspace)
	void RemoveSign();
	// Переводит строчку
	void NewLine();

	// Отображает каретку
	void ShowCaret();
	// Скрывает каретку
	void HideCaret();

protected:
	// метод, вызываемый при получении окном сообщения WM_DESTROY
	void OnWmDestroy();
	// метод, вызываемый при получении окном сообщения WM_PAINT
	void OnWmPaint();
	// скроллирование окон
	void OnWmHScroll( WPARAM, LPARAM );
	void OnWmVScroll( WPARAM, LPARAM );
	// обработка изменения размера окна
	// пересчитывает некоторые свойства скроллов
	void OnWmSize( LPARAM );

private:
	// хэндл окна, которому соответствует этот объект класса.
	HWND windowHandle;
	// имя класса окна
	static const wchar_t* className;

	//Содержимое редактора (массив строк)
	std::vector<CLineOfSymbols> content;
	
	// Строка со спец символами, допуститмымми в нашем редакторе
	std::wstring allowedSymbols;

	//TODO: Сделать нормальные настройки
	int simpleSymbolHeight;

	// для скроллирования
	const int horizontalScrollUnit;
	const int verticalScrollUnit;

	static LRESULT __stdcall windowProcedure( HWND, UINT, WPARAM, LPARAM );
	
	// Класс каретки для этого типа окна
	class CCaret {
	public:
		CCaret( CEditWindow* );

		bool IsShown() const;

		void Create();
		void Destroy();

		// показывает/скрывает каретку
		void Show();
		void Hide();

		enum TDirection {
			DUp, DDown, DLeft, DRight
		};
		
		// сдвигает каретку на единицу в данном направлении
		void Move( TDirection );
		// сдвигает каретку в определенную позицию
		void MoveTo( CLineOfSymbols*, int );

	private:
		// окно, которому принадлежит каретка
		CEditWindow* window;
		// текущий размер каретки
		int width;
		int height;
		// положение каретки
		// линия, в которой находится каретка
		CLineOfSymbols* line;
		// индекс символа, перед которым стоит каретка
		// если равен длине строки - каретка стоит в конце строки
		int index;
		// отображается ли каретка в данный момент
		bool shown;

		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
	};

	// каретка
	CCaret caret;
};