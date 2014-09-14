// Автор: Федюнин Валерий
// Описание: класс главного окна приложения

#include <Windows.h>

// Класса, соответствующий главному окну приложения
class CMainWindow {
public:
	CMainWindow();
	~CMainWindow();

	// регистрирует класс окна, 
	static bool RegisterClass( HINSTANCE );
	
	// Создает окно
	bool Create( LPCWSTR, HINSTANCE );
	// Отображает окно
	void Show( int );
	
protected:
	// метод, вызываемый при получении окном сообщения WM_DESTROY
	void OnWmDestroy();

private:
	// хэнджл окна, которому соответствуует этот объект класса.
	HWND windowHandle;

	static LRESULT __stdcall windowProcedure( HWND, UINT, WPARAM, LPARAM );
};