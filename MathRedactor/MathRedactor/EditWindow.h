// Автор: Федюнин Валерий
// Описание: класс окна, в котором отрисовываются и изменяются формулы

#pragma once

#include <Windows.h>

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

protected:
	// метод, вызываемый при получении окном сообщения WM_DESTROY
	void OnWmDestroy();

private:
	// хэндл окна, которому соответствует этот объект класса.
	HWND windowHandle;

	static LRESULT __stdcall windowProcedure( HWND, UINT, WPARAM, LPARAM );
};