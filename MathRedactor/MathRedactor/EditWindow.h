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

	// регистрирует класс окна
	static bool RegisterClass( HINSTANCE );

	// Создает окно
	HWND Create( HWND, HINSTANCE );
	// Отображается окно
	void Show( int );
	
	// Добавляет символ к последней строке и посылает окну уведомление о том, что надо перерисоваться
	// TODO: добавить фокус строки и позицию для вставки символа
	void AddSign( wchar_t );

protected:
	// метод, вызываемый при получении окном сообщения WM_DESTROY
	void OnWmDestroy();
	// метод, вызываемый при получении окном сообщения WM_PAINT
	void OnWmPaint( );

private:
	// хэндл окна, которому соответствует этот объект класса.
	HWND windowHandle;
	// имя класса окна
	static const wchar_t* className;

	//Содержимое редактора (массив строк)
	std::vector<CLineOfSymbols> content;

	//TODO: Сделать нормальные настройки
	int lineHeight;

	static LRESULT __stdcall windowProcedure( HWND, UINT, WPARAM, LPARAM );
};