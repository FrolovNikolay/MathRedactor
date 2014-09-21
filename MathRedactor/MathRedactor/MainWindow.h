// Автор: Федюнин Валерий
// Описание: класс главного окна приложения

#pragma once

#include <Windows.h>
#include <memory>

#include "EditWindow.h"

// Класса, соответствующий главному окну приложения
class CMainWindow {
public:
	CMainWindow();
	~CMainWindow();

	// регистрирует класс окна, 
	static bool RegisterClass( HINSTANCE );
	
	// Создает окно
	bool Create( LPCWSTR windowName, HINSTANCE ownerInstance, int witdh = 800, int height = 600 );
	// Отображает окно
	void Show( int );
	
protected:
	// метод, вызываемый при получении окном сообщения WM_DESTROY
	void OnWmDestroy();
	// метод, создающий дочерние окна, вызывается при сообщени WM_CREATE
	void OnWmCreate( HWND );
	// меняет размеры дочерних окон при изменении размеров родительского
	void OnWmSize();

private:
	// хэндл окна, которому соответствует этот объект класса.
	HWND windowHandle;
	// дочернее окно, в котором отображабтся и редактируются формулы
	CEditWindow *editWindow;
	// хэндл дочернего окна-редактора
	HWND editHandle;

	static LRESULT __stdcall windowProcedure( HWND, UINT, WPARAM, LPARAM );
};